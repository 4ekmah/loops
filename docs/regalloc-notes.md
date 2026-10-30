# Register allocator: design notes

Working notes on the block-hierarchical register allocator (the "P2" design) in `src/reg_allocator.cpp`.
They cover the parts that are not obvious from the code: the vocabulary, the invariants that are easy to
break, where the design comes from, and what is left to do. Kept in the repo on purpose: the allocator is a
long-running effort and the rationale outlives any single working session.

## The design in one paragraph

Blocks (the function root and every `while` loop, from `BasicBlocksTree`) are allocated **independently and
innermost-first**. Each child loop is scanned on a fresh register pool (a copy of `m_poolBase`, so parameters
and callee-saved registers stay reserved), then the enclosing block scans what is left. A register that
crosses a loop boundary is therefore not one allocation but several, one per block it lives in, and the
blocks are reconciled afterwards: `insertSpillInstructions` walks the CFG edges (`buildCFG`) and inserts the
transfers (register-to-register moves, spills, reloads) that make the neighbouring allocations agree. Every
edge's copy goes at the destination(the loop back-edge copy at the endwhile instead); a branch that jumps to a
merge is reconciled by the same destination copy, which its jump target label precedes, so it flows through.
(There used to be landing-pad edge-splitting here for branch-to-merge edges; it was removed as always
redundant — see "Boundary desync after removing landing pads" under Future work.)

This is, in essence, **Callahan–Koblenz hierarchical allocation** with a linear scan inside each block
instead of a graph coloring. See "Related work" below.

## Vocabulary and invariants

**Live-range splitting.** `makeBlockSplits` cuts a register's live interval at every loop boundary it crosses;
the fragments are *splits*, one per block the register lives in. Boundaries are the loop
entry (`start_pos`, lowered to `LABEL continue`, so the cut runs once on entry rather than per iteration) and
the position right after the loop exit (`end_pos + 1`, after `LABEL break`, so it is on every exit path,
including the zero-iteration one). Cutting at `end_pos` instead would miss the zero-iteration path.

The splitting is **block-independent**: it depends only on the raw live intervals and the loop boundaries, so
it is computed once per basket in `assignRegisters` (`collectBoundaries` + `makeBlockSplits`, stored in
`block_splits`), and each block merely *claims* the splits it owns. Ownership is the deepest block whose extent
contains the split.

**`LiveInterval::hasSuccessor`.** A split ending at a boundary `b` has two different upper bounds, and this
flag separates them:

* its **renaming window** is `[start, b)` — from `b` on, the value is the next piece's business, and the
  boundary transfer at `b` reads the old location;
* its **reservation window** is `[start, b]` — the physical register must not be handed out to anyone before
  the transfer at `b` has read it. `end = b` achieves exactly this: the expire test in `linearScanBlock` is
  `end <= start_of_next`, so a value defined at `b-1` cannot steal the register early, while the successor
  split (which starts at `b`) frees it exactly when it is scanned — and may even reuse the same register,
  making the boundary transfer a no-op.

`RegisterReassignment` cannot express this: it is a strictly tiling map "op -> location" (contiguous, no
overlaps — asserted when the pieces are assembled), whereas the two windows above overlap by one op.

**`m_pieces` vs `m_reg_reassignment`.** Same data, different invariants, and the difference is load-bearing:

* `m_pieces[basket][idx]` is a *builder*: an unordered bag of `(lo, hi, loc)` that the scan appends to and
  **patches retroactively** (when a victim's register is stolen for a spill, its currently active piece is
  rewritten in place). Under bottom-up scanning, pieces of one register arrive out of position order — a
  piece from a loop body is emitted before the enclosing block's piece that precedes it in the program.
* `m_reg_reassignment[basket][idx]` is the *finished value*: sorted, contiguous, consumed by `getAt(opnum)`
  and by the emitter. It is assembled from `m_pieces` after the whole recursion (sort by start, assert the
  tiling), and never appended to incrementally.

**Pool rollback (a recurring source of bugs).** A child block is scanned on a fresh pool and the parent's pool
is restored afterwards — everything the child's pool learned is discarded, while the values it placed live on
in the result. Anything accumulated in `RegisterPool` must therefore be carried back explicitly:
`mergeUsedCallee` (or loop-only callee-saved registers are never saved in the prologue — a Release-only
segfault, fixed in `008a764`) and `mergeSpillPlaceholders` (or registers the child handed out look free to the
spill-placeholder pool, `2e0e83a`). **When adding new state to `RegisterPool`, ask whether it must survive the
child rollback.**

**Spill-slot pinning.** All spilled pieces of one register share one slot (`m_spillSlotOf`), and a piece whose
position-adjacent neighbour (in this block or in an already-scanned child) holds the value in a slot is kept
in that same slot rather than pulled into a register. Without this, a live-through value drifts between
register and memory across a boundary, the boundary transfer spills the wrong content, and a value gets
clobbered (this was the maxpool `Cis` over-write bug; fixed in `199df99`). See "Future work" — this is only
the spilled half of a more general property.

## Related work

* **Callahan & Koblenz, "Register allocation via hierarchical graph coloring", PLDI'91.** The closest
  ancestor of this design: a *tile tree* over the control structure, each tile allocated independently,
  spill/copy code placed at tile boundaries. Tile boundaries are implicit split points for everything live
  across them — exactly our loop-boundary cut.
* **Wimmer & Mössenböck, "Optimized interval splitting in a linear scan register allocator", VEE'05**
  (HotSpot C1). Splitting inside a linear scan; the fragments are *split children*. Also: move split
  positions out of loops so the copy is not executed per iteration.
* **Cooper & Simpson, "Live range splitting in a graph coloring register allocator", CC'98.** Splitting as an
  alternative to spilling, driven by register pressure rather than by program structure.
* **Cooper, Dasgupta & Eckhardt, "Revisiting graph coloring register allocation", LCPC'05.** The only
  published empirical evaluation of Callahan–Koblenz (both allocators implemented in LLVM, SPECint2000):
  CK runs **6.1% faster** than Chaitin–Briggs on average and executes **20.5% fewer** dynamic spill
  operations; the control-flow overhead of tiling is small (+5.8% static branches but only +1.4% dynamic —
  boundary blocks are cold). **But** on `epic` it *lost* 29% on spills and 10.4% on runtime: in a triply
  nested loop, a live-through global was placed in a register by the outer loop, in memory by the middle one
  and in a register again by the inner one, so boundary transfers dominated everything. Their conclusion —
  "it would have been better to keep the value in the same location across more than one tile boundary" — is
  the single most relevant sentence in the literature for this allocator. See below.

## Known perf characteristics

Measured against the pre-P2 baseline (best-of-3, Release): no global regression — roughly +8% on the smallest
kernel (maxpool 3x3, C=832, 7x7) to -11% (i.e. a win) on the heavy one (13x13, C=512), the rest within noise.
Mechanically, P2 adds **integer spills inside loop bodies** (3x3: `rsp` references 185 -> 261, +41%; the
prologue actually got lighter; zero vector spills): independent per-block scans hold long-lived GP values
across loops worse than a whole-function scan does. On large spatial kernels this amortizes; on small ones it
does not.

Beware when measuring: kernel speed swings by 7-18% from code/data placement alone, so a single A/B run
proves nothing — always best-of-N, and preferably with the layout perturbed.

## Future work

**Boundary location agreement (the main open direction).** The in-loop GP spills above are the `epic`
pathology in miniature: a value live through a loop nest drifts between register and slot from block to block,
and every drift costs a boundary transfer. The fix is not smarter per-block scans but making neighbouring
blocks *agree* on where a through-value lives — what Callahan–Koblenz call **preferencing**: a child reports
where it placed the values live across its boundaries, and the parent prefers the same location (and avoids
the physical registers the child occupies in its interior when routing a value across it).

The `199df99` slot pinning is the spilled half of exactly this property; the register half does not exist yet.
A stub for the child-to-parent report (`BlockAllocResult` with `interiorUsed` / `entryLoc` / `exitLoc`) was
written and then removed as dead code in `6e585d0` — the idea was not rejected, only unimplemented. Recover it
from history rather than reinventing it.

**Naming.** The fragments of a live interval are now *splits* (`makeBlockSplits`, `block_splits`), but the
allocated `(lo, hi, loc)` subintervals in `m_pieces` still carry the home-grown word *piece*. Worth renaming
them too (`Placement`?), with `m_pieces` becoming an explicit builder type: it is a bag that is appended to
and patched in place, and only `finalize()` turns it into the tiling `RegisterReassignment` expects.

**`m_snippet_caused_spills` is not per-basket.** It is a single scalar: `LivenessAnalysisAlgo` sums
`spillSpaceNeeded(op, basket) * basketElemX[basket]` across baskets into one number(the max over ops). The
register spills, by contrast, are laid out per basket via `basket_offset`. The snippet region tolerates this
today because it sits at the bottom of the frame at offset 0 and each snippet addresses it directly, and the
cycle-breaking scratch that now shares that region only ever holds one register at a time. But there is no
per-basket accounting for a snippet that needs scratch in both baskets at once — if that ever matters, the
region size and the offsets a snippet uses would need to be split per basket, the way register spills already
are.

**Boundary desync after removing landing pads (a deliberately opened hole).** Reconciliation now handles
every CFG edge the same way: the copy `getAt(u) -> getAt(v)` is placed at the destination `v`(the loop
back-edge at `u` instead). A branch that jumps to a merge is reconciled by that same destination copy -- its
jump target(the endif/else out-label) is emitted *before* the copy in the lowered code(`endif` -> `LABEL`,
then the copy, then the `whilecstart` `LABEL`), so the jumping path flows through it exactly like the
fall-through. The old landing-pad machinery(split the branch edge, redirect the jump to a `LABEL`+copies+`JMP`
block) was removed as always redundant: it only ever duplicated the destination copy(verified on the sole pad
in the test corpus, `snake`, and by `DBG_VERIFY` reporting zero unreconciled edges after removal).

Why "always redundant" holds today: a live-through register occupies **one** location across all its
subintervals -- slot-pinning(`m_spillSlotOf`, `199df99`) if spilled, best-reference glue(`linearScanBlock`) if
register-resident. So every predecessor of a merge delivers the register in the *same* place, and one
destination copy serves them all. Attempts to force disagreement(spill in only one IF branch; nested loops
with `BREAK_(2)` and `BREAK_(1)` into one exit) all failed: the register stayed uniform(`[.,.)=s0` or
`[.,.)=r1` across every subinterval).

The hole: if the glue **fails** -- a through-register landing in *different* registers on two edges into one
merge -- the single destination copy can reconcile only one of them; the other arrives in the wrong register.
Concretely: `BREAK_(2)` from an inner loop(register in the inner subinterval's location) and `BREAK_(1)` from
the outer loop(the outer subinterval's location) target the same post-loop merge; if those two locations
differ, the two break edges need *different* copies at one destination, and only one is emitted. This does NOT
need a jump/pad to fix -- the divergence is a register-vs-register mismatch, curable with a couple of `mov`s;
the right fix is to keep the through-register's location consistent across the boundary(make glue not fail, or
emit the compensating `mov` on the diverging edge) rather than to split the edge. It is left open on purpose:
the upcoming reallocation rework is expected to remove the divergence at its source. Until then, no test
exercises it(glue never failed in any construction tried), but a kernel that made two differently-located
break edges converge could miscompile silently.

**Move condition lowering out of `code_collecting`, so the reg-alloc IR carries no jumps.** The IR the
allocator sees is a hybrid: structural control flow is still annotations(`whilecstart`/`whilecend`/`ifcstart`/
`ifcend`/`else`/`endif`/`break`/`continue`, turned into jumps only by `Cf2jumps`, which runs *after* reg-alloc),
BUT the *conditions* of every `IF_`/`WHILE_` are already lowered to `OP_JCC`(and, for short-circuit `||`/`&&`,
`OP_JCC` chains with intermediate labels) way back in `code_collecting.cpp`(~:486, :565). So `OP_JCC` is present
and abundant at allocation time; only plain `OP_JMP` is essentially absent(structural jumps come later). This is
safe for correctness — an `OP_JCC` has no register operands(the preceding `cmp` carries the uses), so
`LivenessAnalysis` ignores it(it falls through the `default` case adding no def/use), and the branch structure
liveness needs comes from the bbt annotations, not the jump. It is also safe for speed: all condition jumps are
*forward*, so the CFG stays reducible(the only back-edge is the structural loop back-edge). Arbitrary backward
`goto` would make the liveness dataflow blow up(the reason the frontend never emits one).

The proposal: **rework `code_collecting` so it does NOT emit `OP_JCC` for conditions** — keep the condition as
structure(the `ifcstart`/`ifcend` bracket already delimits the condition region; a compound condition would need
its short-circuit shape carried as annotation, e.g. a boolean-expression tree), and let `Cf2jumps` materialize
*all* jumps, conditions included, the way it already materializes continue/break/loop back-edges. Payoff: the
reg-alloc IR becomes uniformly structured and jump-free, so `buildCFG` can derive every edge from the
annotations alone and drop its `OP_JCC`/`OP_JMP` special cases(the `OP_JMP` case is already near-dead at
reg-alloc). The hard part is entirely in the frontend: re-expressing short-circuit `||`/`&&`(today a
`cmp` + `OP_JCC`-chain, see `triangle_types`) as pure structure and moving that expansion into `Cf2jumps`. This
is a cleanliness/invariant change, not a correctness or perf one — `OP_JCC` does not bother the allocator today;
the value is a total "structured, forward-only, reducible" invariant with no jump special-casing downstream. A
lighter half-measure(make `buildCFG` derive branch edges from the bbt and ignore `OP_JCC`, without touching the
frontend) hits the same wall: the annotations do not yet fully encode a compound condition's shape.

**About data structures pipeline in RegisterAllocator**
On other hand, pipeline of algorithm stages and data transformations is provided in process function and, sufficiently
decrease complexity of class. So, I think we have to keep it ideology active.
