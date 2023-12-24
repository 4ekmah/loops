snake(i0, i1, i2)
     0 : sub  sp, sp, #0x200         ; ff 03 08 d1  
     1 : str  x0, [sp], #0x39        ; e0 e7 00 f9  
     2 : str  x1, [sp], #0x38        ; e1 e3 00 f9  
     3 : str  x2, [sp], #0x37        ; e2 df 00 f9  
     4 : str  x25, [sp], #0x3a       ; f9 eb 00 f9  
     5 : str  x26, [sp], #0x3b       ; fa ef 00 f9  
     6 : str  x27, [sp], #0x3c       ; fb f3 00 f9  
     7 : stp  x29, x30, sp, #0x3e    ; fd 7b 1f a9  
     8 : mov  x29, sp                ; fd 03 1f aa  
     9 : ldr  x26, [sp], #0x38       ; fa e3 40 f9  
    10 : ldr  x27, [sp], #0x37       ; fb df 40 f9  
    11 : add  x3, x26, x27           ; 43 03 1b 8b  
    12 : sub  x26, x3, #0x01         ; 7a 04 00 d1  
    13 : str  x26, [sp], #0x36       ; fa db 00 f9  
    14 : eor  x26, x26, x26          ; 5a 03 1a ca  
    15 : str  x26, [sp], #0x35       ; fa d7 00 f9  
    16 : mov  x26, #0x01             ; 3a 00 80 d2  
    17 : str  x26, [sp], #0x32       ; fa cb 00 f9  
    18 : ldr  x26, [sp], #0x32       ; fa cb 40 f9  
    19 : neg  x27, x26               ; fb 03 1a cb  
    20 : str  x27, [sp], #0x33       ; fb cf 00 f9  
    21 : eor  x26, x26, x26          ; 5a 03 1a ca  
    22 : str  x26, [sp], #0x34       ; fa d3 00 f9  
    23 : ldr  x27, [sp], #0x36       ; fb db 40 f9  
    24 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    25 : cmp  x26, x27               ; 5f 03 1b eb  
    26 : b.ge [125]                  ; 6a 0c 00 54  
    27 : eor  x25, x25, x25          ; 39 03 19 ca  
    28 : eor  x3, x3, x3             ; 63 00 03 ca  
    29 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    30 : and  x2, x26, #0x01, #0, #0 ; 42 03 40 92  
    31 : cmp  x2, #0                 ; 5f 00 00 f1  
    32 : b.eq [45]                   ; a0 01 00 54  
    33 : ldr  x26, [sp], #0x37       ; fa df 40 f9  
    34 : sub  x2, x26, #0x01         ; 42 07 00 d1  
    35 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    36 : cmp  x2, x26                ; 5f 00 1a eb  
    37 : csel x25, x2, x26, #0x0b    ; 59 b0 9a 9a  
    38 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    39 : sub  x1, x26, x2            ; 41 03 02 cb  
    40 : eor  x0, x0, x0             ; 00 00 00 ca  
    41 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    42 : cmp  x26, x2                ; 5f 03 02 eb  
    43 : csel x3, x1, x0, #0x0c      ; 23 c0 80 9a  
    44 : b    [56]                   ; 0c 00 00 14  
    45 : ldr  x26, [sp], #0x38       ; fa e3 40 f9  
    46 : sub  x0, x26, #0x01         ; 40 07 00 d1  
    47 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    48 : sub  x1, x26, x0            ; 41 03 00 cb  
    49 : eor  x2, x2, x2             ; 42 00 02 ca  
    50 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    51 : cmp  x26, x0                ; 5f 03 00 eb  
    52 : csel x25, x1, x2, #0x0c     ; 39 c0 82 9a  
    53 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
    54 : cmp  x0, x26                ; 1f 00 1a eb  
    55 : csel x3, x0, x26, #0x0b     ; 03 b0 9a 9a  
    56 : cmp  x25, #0                ; 3f 03 00 f1  
    57 : b.lt [115]                  ; 4b 07 00 54  
    58 : ldr  x26, [sp], #0x37       ; fa df 40 f9  
    59 : cmp  x25, x26               ; 3f 03 1a eb  
    60 : b.ge [115]                  ; ea 06 00 54  
    61 : cmp  x3, #0                 ; 7f 00 00 f1  
    62 : b.lt [115]                  ; ab 06 00 54  
    63 : ldr  x26, [sp], #0x38       ; fa e3 40 f9  
    64 : cmp  x3, x26                ; 7f 00 1a eb  
    65 : b.ge [115]                  ; 4a 06 00 54  
    66 : mov  x0, #0xab7c            ; 80 6f 95 d2  
    67 : movk x0, #0xbbf5, #0x01     ; a0 7e b7 f2  
    68 : movk x0, #0xaaaa, #0x02     ; 40 55 d5 f2  
    69 : stp  x0, x1, sp, #0         ; e0 07 00 a9  
    70 : stp  x2, x3, sp, #0x02      ; e2 0f 01 a9  
    71 : stp  x4, x5, sp, #0x04      ; e4 17 02 a9  
    72 : stp  x6, x7, sp, #0x06      ; e6 1f 03 a9  
    73 : stp  x8, x9, sp, #0x08      ; e8 27 04 a9  
    74 : stp  x10, x11, sp, #0x0a    ; ea 2f 05 a9  
    75 : stp  x12, x13, sp, #0x0c    ; ec 37 06 a9  
    76 : stp  x14, x15, sp, #0x0e    ; ee 3f 07 a9  
    77 : stp  x16, x17, sp, #0x10    ; f0 47 08 a9  
    78 : mov  x0, x25                ; e0 03 19 aa  
    79 : mov  x1, x3                 ; e1 03 03 aa  
    80 : ldr  x9, [sp], #0           ; e9 03 40 f9  
    81 : add  x10, sp, #0x90         ; ea 43 02 91  
    82 : st1  v0.2d, x10             ; 40 2d 9f 4c  
    83 : st1  v4.2d, x10             ; 44 2d 9f 4c  
    84 : st1  v8.2d, x10             ; 48 2d 9f 4c  
    85 : st1  v12.2d, x10            ; 4c 2d 9f 4c  
    86 : blr  [x9]                   ; 20 01 3f d6  
    87 : add  x10, sp, #0x90         ; ea 43 02 91  
    88 : ld1  v0.2d, x10             ; 40 2d df 4c  
    89 : ld1  v4.2d, x10             ; 44 2d df 4c  
    90 : ld1  v8.2d, x10             ; 48 2d df 4c  
    91 : ld1  v12.2d, x10            ; 4c 2d df 4c  
    92 : ldp  x0, x1, sp, #0         ; e0 07 40 a9  
    93 : ldp  x2, x3, sp, #0x02      ; e2 0f 41 a9  
    94 : ldp  x4, x5, sp, #0x04      ; e4 17 42 a9  
    95 : ldp  x6, x7, sp, #0x06      ; e6 1f 43 a9  
    96 : ldp  x8, x9, sp, #0x08      ; e8 27 44 a9  
    97 : ldp  x10, x11, sp, #0x0a    ; ea 2f 45 a9  
    98 : ldp  x12, x13, sp, #0x0c    ; ec 37 46 a9  
    99 : ldp  x14, x15, sp, #0x0e    ; ee 3f 47 a9  
   100 : ldp  x16, x17, sp, #0x10    ; f0 47 48 a9  
   101 : ldr  x26, [sp], #0x37       ; fa df 40 f9  
   102 : mul  x0, x3, x26            ; 60 7c 1a 9b  
   103 : add  x0, x0, x25            ; 00 00 19 8b  
   104 : ldr  x26, [sp], #0x39       ; fa e7 40 f9  
   105 : ldr  x27, [sp], #0x35       ; fb d7 40 f9  
   106 : strb w27, x26, x0           ; 5b 6b 20 38  
   107 : ldr  x26, [sp], #0x35       ; fa d7 40 f9  
   108 : add  x26, x26, #0x01        ; 5a 07 00 91  
   109 : str  x26, [sp], #0x35       ; fa d7 00 f9  
   110 : ldr  x26, [sp], #0x32       ; fa cb 40 f9  
   111 : add  x25, x25, x26          ; 39 03 1a 8b  
   112 : ldr  x26, [sp], #0x33       ; fa cf 40 f9  
   113 : add  x3, x3, x26            ; 63 00 1a 8b  
   114 : b    [56]                   ; c6 ff ff 17  
   115 : ldr  x26, [sp], #0x32       ; fa cb 40 f9  
   116 : neg  x26, x26               ; fa 03 1a cb  
   117 : str  x26, [sp], #0x32       ; fa cb 00 f9  
   118 : ldr  x26, [sp], #0x33       ; fa cf 40 f9  
   119 : neg  x26, x26               ; fa 03 1a cb  
   120 : str  x26, [sp], #0x33       ; fa cf 00 f9  
   121 : ldr  x26, [sp], #0x34       ; fa d3 40 f9  
   122 : add  x26, x26, #0x01        ; 5a 07 00 91  
   123 : str  x26, [sp], #0x34       ; fa d3 00 f9  
   124 : b    [23]                   ; 9b ff ff 17  
   125 : ldp  x29, x30, sp, #0x3e    ; fd 7b 5f a9  
   126 : ldr  x25, [sp], #0x3a       ; f9 eb 40 f9  
   127 : ldr  x26, [sp], #0x3b       ; fa ef 40 f9  
   128 : ldr  x27, [sp], #0x3c       ; fb f3 40 f9  
   129 : add  sp, sp, #0x200         ; ff 03 08 91  
   130 : ret  x30                    ; c0 03 5f d6  
