max_pool_3_3(i0, i1, i2, i3, i4, i5, i6)
     0 : sub  sp, sp, #0x40       ; ff 03 01 d1  
     1 : str  x18, [sp], #0       ; f2 03 00 f9  
     2 : str  x19, [sp], #0x01    ; f3 07 00 f9  
     3 : str  x20, [sp], #0x02    ; f4 0b 00 f9  
     4 : str  x21, [sp], #0x03    ; f5 0f 00 f9  
     5 : str  x22, [sp], #0x04    ; f6 13 00 f9  
     6 : str  x23, [sp], #0x05    ; f7 17 00 f9  
     7 : str  x24, [sp], #0x06    ; f8 1b 00 f9  
     8 : mov  x7, #0              ; 07 00 80 d2  
     9 : mov  x7, #0              ; 07 00 80 d2  
    10 : mov  x8, #0x04           ; 88 00 80 d2  
    11 : mul  x8, x6, x8          ; c8 7c 08 9b  
    12 : mov  x9, #0              ; 09 00 80 d2  
    13 : mul  x9, x9, x2          ; 29 7d 02 9b  
    14 : mov  x10, #0             ; 0a 00 80 d2  
    15 : add  x9, x9, x10         ; 29 01 0a 8b  
    16 : mov  x10, #0x04          ; 8a 00 80 d2  
    17 : mul  x9, x9, x10         ; 29 7d 0a 9b  
    18 : mov  x10, #0             ; 0a 00 80 d2  
    19 : mul  x10, x10, x2        ; 4a 7d 02 9b  
    20 : mov  x11, #0x01          ; 2b 00 80 d2  
    21 : add  x10, x10, x11       ; 4a 01 0b 8b  
    22 : mov  x11, #0x04          ; 8b 00 80 d2  
    23 : mul  x10, x10, x11       ; 4a 7d 0b 9b  
    24 : mov  x11, #0             ; 0b 00 80 d2  
    25 : mul  x11, x11, x2        ; 6b 7d 02 9b  
    26 : mov  x12, #0x02          ; 4c 00 80 d2  
    27 : add  x11, x11, x12       ; 6b 01 0c 8b  
    28 : mov  x12, #0x04          ; 8c 00 80 d2  
    29 : mul  x11, x11, x12       ; 6b 7d 0c 9b  
    30 : mov  x12, #0x01          ; 2c 00 80 d2  
    31 : mul  x12, x12, x2        ; 8c 7d 02 9b  
    32 : mov  x13, #0             ; 0d 00 80 d2  
    33 : add  x12, x12, x13       ; 8c 01 0d 8b  
    34 : mov  x13, #0x04          ; 8d 00 80 d2  
    35 : mul  x12, x12, x13       ; 8c 7d 0d 9b  
    36 : mov  x13, #0x01          ; 2d 00 80 d2  
    37 : mul  x13, x13, x2        ; ad 7d 02 9b  
    38 : mov  x14, #0x01          ; 2e 00 80 d2  
    39 : add  x13, x13, x14       ; ad 01 0e 8b  
    40 : mov  x14, #0x04          ; 8e 00 80 d2  
    41 : mul  x13, x13, x14       ; ad 7d 0e 9b  
    42 : mov  x14, #0x01          ; 2e 00 80 d2  
    43 : mul  x14, x14, x2        ; ce 7d 02 9b  
    44 : mov  x15, #0x02          ; 4f 00 80 d2  
    45 : add  x14, x14, x15       ; ce 01 0f 8b  
    46 : mov  x15, #0x04          ; 8f 00 80 d2  
    47 : mul  x14, x14, x15       ; ce 7d 0f 9b  
    48 : mov  x15, #0x02          ; 4f 00 80 d2  
    49 : mul  x15, x15, x2        ; ef 7d 02 9b  
    50 : mov  x16, #0             ; 10 00 80 d2  
    51 : add  x15, x15, x16       ; ef 01 10 8b  
    52 : mov  x16, #0x04          ; 90 00 80 d2  
    53 : mul  x15, x15, x16       ; ef 7d 10 9b  
    54 : mov  x16, #0x02          ; 50 00 80 d2  
    55 : mul  x16, x16, x2        ; 10 7e 02 9b  
    56 : mov  x17, #0x01          ; 31 00 80 d2  
    57 : add  x16, x16, x17       ; 10 02 11 8b  
    58 : mov  x17, #0x04          ; 91 00 80 d2  
    59 : mul  x16, x16, x17       ; 10 7e 11 9b  
    60 : mov  x17, #0x02          ; 51 00 80 d2  
    61 : mul  x17, x17, x2        ; 31 7e 02 9b  
    62 : mov  x18, #0x02          ; 52 00 80 d2  
    63 : add  x17, x17, x18       ; 31 02 12 8b  
    64 : mov  x18, #0x04          ; 92 00 80 d2  
    65 : mul  x17, x17, x18       ; 31 7e 12 9b  
    66 : cmp  x7, x3              ; ff 00 03 eb  
    67 : b.ge [123]               ; 0a 07 00 54  
    68 : mov  x18, #0             ; 12 00 80 d2  
    69 : mul  x19, x7, x6         ; f3 7c 06 9b  
    70 : mul  x19, x19, x5        ; 73 7e 05 9b  
    71 : mov  x20, #0x04          ; 94 00 80 d2  
    72 : mul  x19, x19, x20       ; 73 7e 14 9b  
    73 : add  x19, x4, x19        ; 93 00 13 8b  
    74 : mul  x20, x7, x2         ; f4 7c 02 9b  
    75 : mul  x20, x20, x1        ; 94 7e 01 9b  
    76 : mov  x21, #0x04          ; 95 00 80 d2  
    77 : mul  x20, x20, x21       ; 94 7e 15 9b  
    78 : add  x20, x0, x20        ; 14 00 14 8b  
    79 : cmp  x18, x5             ; 5f 02 05 eb  
    80 : b.ge [121]               ; 2a 05 00 54  
    81 : mov  x21, #0             ; 15 00 80 d2  
    82 : mov  x22, #0x10          ; 16 02 80 d2  
    83 : cmp  x21, x8             ; bf 02 08 eb  
    84 : b.ge [113]               ; aa 03 00 54  
    85 : cmp  x22, x8             ; df 02 08 eb  
    86 : b.le [90]                ; 8d 00 00 54  
    87 : sub  x23, x22, x8        ; d7 02 08 cb  
    88 : sub  x21, x21, x23       ; b5 02 17 cb  
    89 : sub  x22, x22, x23       ; d6 02 17 cb  
    90 : add  x23, x20, x21       ; 97 02 15 8b  
    91 : add  x24, x19, x21       ; 78 02 15 8b  
    92 : ldr  v0.4s, x23, x9      ; e0 6a e9 3c  
    93 : ldr  v1.4s, x23, x10     ; e1 6a ea 3c  
    94 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
    95 : ldr  v1.4s, x23, x11     ; e1 6a eb 3c  
    96 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
    97 : ldr  v1.4s, x23, x12     ; e1 6a ec 3c  
    98 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
    99 : ldr  v1.4s, x23, x13     ; e1 6a ed 3c  
   100 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   101 : ldr  v1.4s, x23, x14     ; e1 6a ee 3c  
   102 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   103 : ldr  v1.4s, x23, x15     ; e1 6a ef 3c  
   104 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   105 : ldr  v1.4s, x23, x16     ; e1 6a f0 3c  
   106 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   107 : ldr  v1.4s, x23, x17     ; e1 6a f1 3c  
   108 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   109 : str  v0.4s, x24, #0      ; 00 03 80 3d  
   110 : mov  x21, x22            ; f5 03 16 aa  
   111 : add  x22, x22, #0x10     ; d6 42 00 91  
   112 : b    [83]                ; e3 ff ff 17  
   113 : add  x18, x18, #0x01     ; 52 06 00 91  
   114 : mov  x21, #0x04          ; 95 00 80 d2  
   115 : mul  x21, x6, x21        ; d5 7c 15 9b  
   116 : add  x19, x19, x21       ; 73 02 15 8b  
   117 : mov  x21, #0x04          ; 95 00 80 d2  
   118 : mul  x21, x2, x21        ; 55 7c 15 9b  
   119 : add  x20, x20, x21       ; 94 02 15 8b  
   120 : b    [79]                ; d7 ff ff 17  
   121 : add  x7, x7, #0x01       ; e7 04 00 91  
   122 : b    [66]                ; c8 ff ff 17  
   123 : mov  x0, #0              ; 00 00 80 d2  
   124 : ldr  x18, [sp], #0       ; f2 03 40 f9  
   125 : ldr  x19, [sp], #0x01    ; f3 07 40 f9  
   126 : ldr  x20, [sp], #0x02    ; f4 0b 40 f9  
   127 : ldr  x21, [sp], #0x03    ; f5 0f 40 f9  
   128 : ldr  x22, [sp], #0x04    ; f6 13 40 f9  
   129 : ldr  x23, [sp], #0x05    ; f7 17 40 f9  
   130 : ldr  x24, [sp], #0x06    ; f8 1b 40 f9  
   131 : add  sp, sp, #0x40       ; ff 03 01 91  
   132 : ret  x30                 ; c0 03 5f d6  
