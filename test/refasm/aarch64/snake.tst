snake(i0, i1, i2)
     0 : sub  sp, sp, #0x200         ; ff 03 08 d1  
     1 : str  x1, [sp], #0x38        ; e1 e3 00 f9  
     2 : str  x2, [sp], #0x37        ; e2 df 00 f9  
     3 : str  x18, [sp], #0x39       ; f2 e7 00 f9  
     4 : str  x19, [sp], #0x3a       ; f3 eb 00 f9  
     5 : str  x20, [sp], #0x3b       ; f4 ef 00 f9  
     6 : str  x21, [sp], #0x3c       ; f5 f3 00 f9  
     7 : stp  x29, x30, sp, #0x3e    ; fd 7b 1f a9  
     8 : mov  x29, sp                ; fd 03 1f aa  
     9 : ldr  x20, [sp], #0x38       ; f4 e3 40 f9  
    10 : ldr  x21, [sp], #0x37       ; f5 df 40 f9  
    11 : add  x3, x20, x21           ; 83 02 15 8b  
    12 : sub  x20, x3, #0x01         ; 74 04 00 d1  
    13 : str  x20, [sp], #0x36       ; f4 db 00 f9  
    14 : mov  x20, #0                ; 14 00 80 d2  
    15 : str  x20, [sp], #0x35       ; f4 d7 00 f9  
    16 : mov  x20, #0x01             ; 34 00 80 d2  
    17 : str  x20, [sp], #0x34       ; f4 d3 00 f9  
    18 : ldr  x20, [sp], #0x34       ; f4 d3 40 f9  
    19 : neg  x21, x20               ; f5 03 14 cb  
    20 : str  x21, [sp], #0x32       ; f5 cb 00 f9  
    21 : mov  x20, #0                ; 14 00 80 d2  
    22 : str  x20, [sp], #0x33       ; f4 cf 00 f9  
    23 : ldr  x21, [sp], #0x36       ; f5 db 40 f9  
    24 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    25 : cmp  x20, x21               ; 9f 02 15 eb  
    26 : b.ge [124]                  ; 4a 0c 00 54  
    27 : mov  x19, #0                ; 13 00 80 d2  
    28 : mov  x18, #0                ; 12 00 80 d2  
    29 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    30 : and  x3, x20, #0x01, #0, #0 ; 83 02 40 92  
    31 : cmp  x3, #0                 ; 7f 00 00 f1  
    32 : b.eq [45]                   ; a0 01 00 54  
    33 : ldr  x20, [sp], #0x37       ; f4 df 40 f9  
    34 : sub  x3, x20, #0x01         ; 83 06 00 d1  
    35 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    36 : cmp  x3, x20                ; 7f 00 14 eb  
    37 : csel x19, x3, x20, #0x0b    ; 73 b0 94 9a  
    38 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    39 : sub  x2, x20, x3            ; 82 02 03 cb  
    40 : mov  x1, #0                 ; 01 00 80 d2  
    41 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    42 : cmp  x20, x3                ; 9f 02 03 eb  
    43 : csel x18, x2, x1, #0x0c     ; 52 c0 81 9a  
    44 : b    [56]                   ; 0c 00 00 14  
    45 : ldr  x20, [sp], #0x38       ; f4 e3 40 f9  
    46 : sub  x1, x20, #0x01         ; 81 06 00 d1  
    47 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    48 : sub  x2, x20, x1            ; 82 02 01 cb  
    49 : mov  x3, #0                 ; 03 00 80 d2  
    50 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    51 : cmp  x20, x1                ; 9f 02 01 eb  
    52 : csel x19, x2, x3, #0x0c     ; 53 c0 83 9a  
    53 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
    54 : cmp  x1, x20                ; 3f 00 14 eb  
    55 : csel x18, x1, x20, #0x0b    ; 32 b0 94 9a  
    56 : cmp  x19, #0                ; 7f 02 00 f1  
    57 : b.lt [114]                  ; 2b 07 00 54  
    58 : ldr  x20, [sp], #0x37       ; f4 df 40 f9  
    59 : cmp  x19, x20               ; 7f 02 14 eb  
    60 : b.ge [114]                  ; ca 06 00 54  
    61 : cmp  x18, #0                ; 5f 02 00 f1  
    62 : b.lt [114]                  ; 8b 06 00 54  
    63 : ldr  x20, [sp], #0x38       ; f4 e3 40 f9  
    64 : cmp  x18, x20               ; 5f 02 14 eb  
    65 : b.ge [114]                  ; 2a 06 00 54  
    66 : mov  x1, #0x2798            ; 01 f3 84 d2  
    67 : movk x1, #0x0a, #0x01       ; 41 01 a0 f2  
    68 : movk x1, #0x01, #0x02       ; 21 00 c0 f2  
    69 : stp  x0, x1, sp, #0         ; e0 07 00 a9  
    70 : stp  x2, x3, sp, #0x02      ; e2 0f 01 a9  
    71 : stp  x4, x5, sp, #0x04      ; e4 17 02 a9  
    72 : stp  x6, x7, sp, #0x06      ; e6 1f 03 a9  
    73 : stp  x8, x9, sp, #0x08      ; e8 27 04 a9  
    74 : stp  x10, x11, sp, #0x0a    ; ea 2f 05 a9  
    75 : stp  x12, x13, sp, #0x0c    ; ec 37 06 a9  
    76 : stp  x14, x15, sp, #0x0e    ; ee 3f 07 a9  
    77 : stp  x16, x17, sp, #0x10    ; f0 47 08 a9  
    78 : mov  x0, x19                ; e0 03 13 aa  
    79 : mov  x1, x18                ; e1 03 12 aa  
    80 : ldr  x9, [sp], #0x01        ; e9 07 40 f9  
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
   101 : ldr  x20, [sp], #0x37       ; f4 df 40 f9  
   102 : mul  x1, x18, x20           ; 41 7e 14 9b  
   103 : add  x1, x1, x19            ; 21 00 13 8b  
   104 : ldr  x20, [sp], #0x35       ; f4 d7 40 f9  
   105 : strb w20, x0, x1            ; 14 68 21 38  
   106 : ldr  x20, [sp], #0x35       ; f4 d7 40 f9  
   107 : add  x20, x20, #0x01        ; 94 06 00 91  
   108 : str  x20, [sp], #0x35       ; f4 d7 00 f9  
   109 : ldr  x20, [sp], #0x34       ; f4 d3 40 f9  
   110 : add  x19, x19, x20          ; 73 02 14 8b  
   111 : ldr  x20, [sp], #0x32       ; f4 cb 40 f9  
   112 : add  x18, x18, x20          ; 52 02 14 8b  
   113 : b    [56]                   ; c7 ff ff 17  
   114 : ldr  x20, [sp], #0x34       ; f4 d3 40 f9  
   115 : neg  x20, x20               ; f4 03 14 cb  
   116 : str  x20, [sp], #0x34       ; f4 d3 00 f9  
   117 : ldr  x20, [sp], #0x32       ; f4 cb 40 f9  
   118 : neg  x20, x20               ; f4 03 14 cb  
   119 : str  x20, [sp], #0x32       ; f4 cb 00 f9  
   120 : ldr  x20, [sp], #0x33       ; f4 cf 40 f9  
   121 : add  x20, x20, #0x01        ; 94 06 00 91  
   122 : str  x20, [sp], #0x33       ; f4 cf 00 f9  
   123 : b    [23]                   ; 9c ff ff 17  
   124 : ldp  x29, x30, sp, #0x3e    ; fd 7b 5f a9  
   125 : ldr  x18, [sp], #0x39       ; f2 e7 40 f9  
   126 : ldr  x19, [sp], #0x3a       ; f3 eb 40 f9  
   127 : ldr  x20, [sp], #0x3b       ; f4 ef 40 f9  
   128 : ldr  x21, [sp], #0x3c       ; f5 f3 40 f9  
   129 : add  sp, sp, #0x200         ; ff 03 08 91  
   130 : ret  x30                    ; c0 03 5f d6  
