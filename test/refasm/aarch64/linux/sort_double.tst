sort_double(i0, i1)
     0 : sub  sp, sp, #0x1e0        ; ff 83 07 d1  
     1 : str  x0, [sp], #0x35       ; e0 d7 00 f9  
     2 : str  x25, [sp], #0x36      ; f9 db 00 f9  
     3 : str  x26, [sp], #0x37      ; fa df 00 f9  
     4 : str  x27, [sp], #0x38      ; fb e3 00 f9  
     5 : stp  x29, x30, sp, #0x3a   ; fd 7b 1d a9  
     6 : mov  x29, sp               ; fd 03 1f aa  
     7 : lsl  x26, x1, #0x3d, #0x3c ; 3a f0 7d d3  
     8 : str  x26, [sp], #0x34      ; fa d3 00 f9  
     9 : ldr  x26, [sp], #0x34      ; fa d3 40 f9  
    10 : sub  x27, x26, #0x08       ; 5b 23 00 d1  
    11 : str  x27, [sp], #0x33      ; fb cf 00 f9  
    12 : eor  x26, x26, x26         ; 5a 03 1a ca  
    13 : str  x26, [sp], #0x32      ; fa cb 00 f9  
    14 : ldr  x27, [sp], #0x33      ; fb cf 40 f9  
    15 : ldr  x26, [sp], #0x32      ; fa cb 40 f9  
    16 : cmp  x26, x27              ; 5f 03 1b eb  
    17 : b.ge [84]                  ; 6a 08 00 54  
    18 : ldr  x26, [sp], #0x32      ; fa cb 40 f9  
    19 : mov  x25, x26              ; f9 03 1a aa  
    20 : add  x3, x25, #0x08        ; 23 23 00 91  
    21 : ldr  x26, [sp], #0x34      ; fa d3 40 f9  
    22 : cmp  x3, x26               ; 7f 00 1a eb  
    23 : b.ge [67]                  ; 8a 05 00 54  
    24 : ldr  x26, [sp], #0x35      ; fa d7 40 f9  
    25 : ldr  x2, [x26], x3         ; 42 6b 63 f8  
    26 : ldr  x26, [sp], #0x35      ; fa d7 40 f9  
    27 : ldr  x1, [x26], x25        ; 41 6b 79 f8  
    28 : mov  x0, #0xc390           ; 00 72 98 d2  
    29 : movk x0, #0xbbf5, #0x01    ; a0 7e b7 f2  
    30 : movk x0, #0xaaaa, #0x02    ; 40 55 d5 f2  
    31 : stp  x0, x1, sp, #0        ; e0 07 00 a9  
    32 : stp  x2, x3, sp, #0x02     ; e2 0f 01 a9  
    33 : stp  x4, x5, sp, #0x04     ; e4 17 02 a9  
    34 : stp  x6, x7, sp, #0x06     ; e6 1f 03 a9  
    35 : stp  x8, x9, sp, #0x08     ; e8 27 04 a9  
    36 : stp  x10, x11, sp, #0x0a   ; ea 2f 05 a9  
    37 : stp  x12, x13, sp, #0x0c   ; ec 37 06 a9  
    38 : stp  x14, x15, sp, #0x0e   ; ee 3f 07 a9  
    39 : stp  x16, x17, sp, #0x10   ; f0 47 08 a9  
    40 : mov  x0, x2                ; e0 03 02 aa  
    41 : ldr  x9, [sp], #0          ; e9 03 40 f9  
    42 : add  x10, sp, #0x90        ; ea 43 02 91  
    43 : st1  v0.2d, x10            ; 40 2d 9f 4c  
    44 : st1  v4.2d, x10            ; 44 2d 9f 4c  
    45 : st1  v8.2d, x10            ; 48 2d 9f 4c  
    46 : st1  v12.2d, x10           ; 4c 2d 9f 4c  
    47 : blr  [x9]                  ; 20 01 3f d6  
    48 : add  x10, sp, #0x90        ; ea 43 02 91  
    49 : ld1  v0.2d, x10            ; 40 2d df 4c  
    50 : ld1  v4.2d, x10            ; 44 2d df 4c  
    51 : ld1  v8.2d, x10            ; 48 2d df 4c  
    52 : ld1  v12.2d, x10           ; 4c 2d df 4c  
    53 : ldr  x1, [sp], #0x01       ; e1 07 40 f9  
    54 : ldp  x2, x3, sp, #0x02     ; e2 0f 41 a9  
    55 : ldp  x4, x5, sp, #0x04     ; e4 17 42 a9  
    56 : ldp  x6, x7, sp, #0x06     ; e6 1f 43 a9  
    57 : ldp  x8, x9, sp, #0x08     ; e8 27 44 a9  
    58 : ldp  x10, x11, sp, #0x0a   ; ea 2f 45 a9  
    59 : ldp  x12, x13, sp, #0x0c   ; ec 37 46 a9  
    60 : ldp  x14, x15, sp, #0x0e   ; ee 3f 47 a9  
    61 : ldp  x16, x17, sp, #0x10   ; f0 47 48 a9  
    62 : cmp  x0, #0                ; 1f 00 00 f1  
    63 : b.eq [65]                  ; 40 00 00 54  
    64 : mov  x25, x3               ; f9 03 03 aa  
    65 : add  x3, x3, #0x08         ; 63 20 00 91  
    66 : b    [21]                  ; d3 ff ff 17  
    67 : ldr  x26, [sp], #0x32      ; fa cb 40 f9  
    68 : cmp  x25, x26              ; 3f 03 1a eb  
    69 : b.eq [80]                  ; 60 01 00 54  
    70 : ldr  x26, [sp], #0x35      ; fa d7 40 f9  
    71 : ldr  x27, [sp], #0x32      ; fb cb 40 f9  
    72 : ldr  x0, [x26], x27        ; 40 6b 7b f8  
    73 : ldr  x26, [sp], #0x35      ; fa d7 40 f9  
    74 : ldr  x1, [x26], x25        ; 41 6b 79 f8  
    75 : ldr  x26, [sp], #0x35      ; fa d7 40 f9  
    76 : str  #0x01, x0, x1a, x19   ; 40 6b 39 f8  
    77 : ldr  x26, [sp], #0x35      ; fa d7 40 f9  
    78 : ldr  x27, [sp], #0x32      ; fb cb 40 f9  
    79 : str  #0x01, x1, x1a, x1b   ; 41 6b 3b f8  
    80 : ldr  x26, [sp], #0x32      ; fa cb 40 f9  
    81 : add  x26, x26, #0x08       ; 5a 23 00 91  
    82 : str  x26, [sp], #0x32      ; fa cb 00 f9  
    83 : b    [14]                  ; bb ff ff 17  
    84 : ldp  x29, x30, sp, #0x3a   ; fd 7b 5d a9  
    85 : ldr  x25, [sp], #0x36      ; f9 db 40 f9  
    86 : ldr  x26, [sp], #0x37      ; fa df 40 f9  
    87 : ldr  x27, [sp], #0x38      ; fb e3 40 f9  
    88 : add  sp, sp, #0x1e0        ; ff 83 07 91  
    89 : ret  x30                   ; c0 03 5f d6  
