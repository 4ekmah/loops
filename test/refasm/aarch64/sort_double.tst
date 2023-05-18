sort_double(i0, i1)
     0 : sub  sp, sp, #0x1e0        ; ff 83 07 d1  
     1 : str  x18, [sp], #0x35      ; f2 d7 00 f9  
     2 : str  x19, [sp], #0x36      ; f3 db 00 f9  
     3 : str  x20, [sp], #0x37      ; f4 df 00 f9  
     4 : str  x21, [sp], #0x38      ; f5 e3 00 f9  
     5 : stp  x29, x30, sp, #0x3a   ; fd 7b 1d a9  
     6 : mov  x29, sp               ; fd 03 1f aa  
     7 : lsl  x20, x1, #0x3d, #0x3c ; 34 f0 7d d3  
     8 : str  x20, [sp], #0x34      ; f4 d3 00 f9  
     9 : ldr  x20, [sp], #0x34      ; f4 d3 40 f9  
    10 : sub  x21, x20, #0x08       ; 95 22 00 d1  
    11 : str  x21, [sp], #0x33      ; f5 cf 00 f9  
    12 : mov  x20, #0               ; 14 00 80 d2  
    13 : str  x20, [sp], #0x32      ; f4 cb 00 f9  
    14 : ldr  x21, [sp], #0x33      ; f5 cf 40 f9  
    15 : ldr  x20, [sp], #0x32      ; f4 cb 40 f9  
    16 : cmp  x20, x21              ; 9f 02 15 eb  
    17 : b.ge [80]                  ; ea 07 00 54  
    18 : ldr  x20, [sp], #0x32      ; f4 cb 40 f9  
    19 : add  x18, x20, #0x08       ; 92 22 00 91  
    20 : mov  x19, x18              ; f3 03 12 aa  
    21 : ldr  x20, [sp], #0x34      ; f4 d3 40 f9  
    22 : cmp  x19, x20              ; 7f 02 14 eb  
    23 : b.ge [67]                  ; 8a 05 00 54  
    24 : ldr  x3, [x0], x19         ; 03 68 73 f8  
    25 : ldr  x2, [x0], x18         ; 02 68 72 f8  
    26 : mov  x1, #0x2974           ; 81 2e 85 d2  
    27 : movk x1, #0x0a, #0x01      ; 41 01 a0 f2  
    28 : movk x1, #0x01, #0x02      ; 21 00 c0 f2  
    29 : stp  x0, x1, sp, #0        ; e0 07 00 a9  
    30 : stp  x2, x3, sp, #0x02     ; e2 0f 01 a9  
    31 : stp  x4, x5, sp, #0x04     ; e4 17 02 a9  
    32 : stp  x6, x7, sp, #0x06     ; e6 1f 03 a9  
    33 : stp  x8, x9, sp, #0x08     ; e8 27 04 a9  
    34 : stp  x10, x11, sp, #0x0a   ; ea 2f 05 a9  
    35 : stp  x12, x13, sp, #0x0c   ; ec 37 06 a9  
    36 : stp  x14, x15, sp, #0x0e   ; ee 3f 07 a9  
    37 : stp  x16, x17, sp, #0x10   ; f0 47 08 a9  
    38 : mov  x0, x3                ; e0 03 03 aa  
    39 : mov  x1, x2                ; e1 03 02 aa  
    40 : ldr  x9, [sp], #0x01       ; e9 07 40 f9  
    41 : add  x10, sp, #0x90        ; ea 43 02 91  
    42 : st1  v0.2d, x10            ; 40 2d 9f 4c  
    43 : st1  v4.2d, x10            ; 44 2d 9f 4c  
    44 : st1  v8.2d, x10            ; 48 2d 9f 4c  
    45 : st1  v12.2d, x10           ; 4c 2d 9f 4c  
    46 : blr  [x9]                  ; 20 01 3f d6  
    47 : add  x10, sp, #0x90        ; ea 43 02 91  
    48 : ld1  v0.2d, x10            ; 40 2d df 4c  
    49 : ld1  v4.2d, x10            ; 44 2d df 4c  
    50 : ld1  v8.2d, x10            ; 48 2d df 4c  
    51 : ld1  v12.2d, x10           ; 4c 2d df 4c  
    52 : mov  x1, x0                ; e1 03 00 aa  
    53 : ldr  x0, [sp], #0          ; e0 03 40 f9  
    54 : ldp  x2, x3, sp, #0x02     ; e2 0f 41 a9  
    55 : ldp  x4, x5, sp, #0x04     ; e4 17 42 a9  
    56 : ldp  x6, x7, sp, #0x06     ; e6 1f 43 a9  
    57 : ldp  x8, x9, sp, #0x08     ; e8 27 44 a9  
    58 : ldp  x10, x11, sp, #0x0a   ; ea 2f 45 a9  
    59 : ldp  x12, x13, sp, #0x0c   ; ec 37 46 a9  
    60 : ldp  x14, x15, sp, #0x0e   ; ee 3f 47 a9  
    61 : ldp  x16, x17, sp, #0x10   ; f0 47 48 a9  
    62 : cmp  x1, #0                ; 3f 00 00 f1  
    63 : b.eq [65]                  ; 40 00 00 54  
    64 : mov  x18, x19              ; f2 03 13 aa  
    65 : add  x19, x19, #0x08       ; 73 22 00 91  
    66 : b    [21]                  ; d3 ff ff 17  
    67 : ldr  x20, [sp], #0x32      ; f4 cb 40 f9  
    68 : cmp  x18, x20              ; 5f 02 14 eb  
    69 : b.eq [76]                  ; e0 00 00 54  
    70 : ldr  x20, [sp], #0x32      ; f4 cb 40 f9  
    71 : ldr  x1, [x0], x20         ; 01 68 74 f8  
    72 : ldr  x2, [x0], x18         ; 02 68 72 f8  
    73 : str  #0x01, x1, x0, x12    ; 01 68 32 f8  
    74 : ldr  x20, [sp], #0x32      ; f4 cb 40 f9  
    75 : str  #0x01, x2, x0, x14    ; 02 68 34 f8  
    76 : ldr  x20, [sp], #0x32      ; f4 cb 40 f9  
    77 : add  x20, x20, #0x08       ; 94 22 00 91  
    78 : str  x20, [sp], #0x32      ; f4 cb 00 f9  
    79 : b    [14]                  ; bf ff ff 17  
    80 : ldp  x29, x30, sp, #0x3a   ; fd 7b 5d a9  
    81 : ldr  x18, [sp], #0x35      ; f2 d7 40 f9  
    82 : ldr  x19, [sp], #0x36      ; f3 db 40 f9  
    83 : ldr  x20, [sp], #0x37      ; f4 df 40 f9  
    84 : ldr  x21, [sp], #0x38      ; f5 e3 40 f9  
    85 : add  sp, sp, #0x1e0        ; ff 83 07 91  
    86 : ret  x30                   ; c0 03 5f d6  
