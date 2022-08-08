exp_f32(i0, i1, i2)
     0 : sub    sp, sp, #0x110             ; ff 43 04 d1  
     1 : str    v29.16b, [sp], #0x0f       ; fd 3f 80 3d  
     2 : str    v30.16b, [sp], #0x10       ; fe 43 80 3d  
     3 : mov    x3, #0xc0a5                ; a3 14 98 d2  
     4 : movk   x3, #0xc2b0, #0x01         ; 03 56 b8 f2  
     5 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
     6 : str    v29.4s, [sp], #0x0e        ; fd 3b 80 3d  
     7 : mov    x3, #0xc0a5                ; a3 14 98 d2  
     8 : movk   x3, #0x42b0, #0x01         ; 03 56 a8 f2  
     9 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    10 : str    v29.4s, [sp], #0x0d        ; fd 37 80 3d  
    11 : mov    x3, #0                     ; 03 00 80 d2  
    12 : movk   x3, #0x3f00, #0x01         ; 03 e0 a7 f2  
    13 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    14 : str    v29.4s, [sp], #0x0c        ; fd 33 80 3d  
    15 : mov    x3, #0                     ; 03 00 80 d2  
    16 : movk   x3, #0x3f80, #0x01         ; 03 f0 a7 f2  
    17 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    18 : str    v29.4s, [sp], #0x0b        ; fd 2f 80 3d  
    19 : mov    x3, #0xaa3b                ; 63 47 95 d2  
    20 : movk   x3, #0x3fb8, #0x01         ; 03 f7 a7 f2  
    21 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    22 : str    v29.4s, [sp], #0x01        ; fd 07 80 3d  
    23 : mov    x3, #0x8000                ; 03 00 90 d2  
    24 : movk   x3, #0xbf31, #0x01         ; 23 e6 b7 f2  
    25 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    26 : str    v29.4s, [sp], #0x02        ; fd 0b 80 3d  
    27 : mov    x3, #0x8083                ; 63 10 90 d2  
    28 : movk   x3, #0x395e, #0x01         ; c3 2b a7 f2  
    29 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    30 : str    v29.4s, [sp], #0x03        ; fd 0f 80 3d  
    31 : mov    x3, #0x6967                ; e3 2c 8d d2  
    32 : movk   x3, #0x3950, #0x01         ; 03 2a a7 f2  
    33 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    34 : str    v29.4s, [sp], #0x04        ; fd 13 80 3d  
    35 : mov    x3, #0x43ce                ; c3 79 88 d2  
    36 : movk   x3, #0x3ab7, #0x01         ; e3 56 a7 f2  
    37 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    38 : str    v29.4s, [sp], #0x05        ; fd 17 80 3d  
    39 : mov    x3, #0x8908                ; 03 21 91 d2  
    40 : movk   x3, #0x3c08, #0x01         ; 03 81 a7 f2  
    41 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    42 : str    v29.4s, [sp], #0x06        ; fd 1b 80 3d  
    43 : mov    x3, #0xa9c1                ; 23 38 95 d2  
    44 : movk   x3, #0x3d2a, #0x01         ; 43 a5 a7 f2  
    45 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    46 : str    v29.4s, [sp], #0x07        ; fd 1f 80 3d  
    47 : mov    x3, #0xaaaa                ; 43 55 95 d2  
    48 : movk   x3, #0x3e2a, #0x01         ; 43 c5 a7 f2  
    49 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    50 : str    v29.4s, [sp], #0x08        ; fd 23 80 3d  
    51 : mov    x3, #0                     ; 03 00 80 d2  
    52 : movk   x3, #0x3f00, #0x01         ; 03 e0 a7 f2  
    53 : dup    v29.4s, x3                 ; 7d 0c 04 4e  
    54 : str    v29.4s, [sp], #0x09        ; fd 27 80 3d  
    55 : movi   v29.4s, #0x1f, #0x03       ; fd 07 03 4f  
    56 : str    v29.4s, [sp], #0x0a        ; fd 2b 80 3d  
    57 : mov    x3, #0                     ; 03 00 80 d2  
    58 : cmp    x2, #0                     ; 5f 00 00 f1  
    59 : b.le   [124]                      ; 2d 08 00 54  
    60 : ldr    v3.4s, x1, x3              ; 23 68 e3 3c  
    61 : ldr    v29.4s, [sp], #0x0d        ; fd 37 c0 3d  
    62 : fmin   v3.4s, v3.4s, v29.4s       ; 63 f4 bd 4e  
    63 : ldr    v29.4s, [sp], #0x0e        ; fd 3b c0 3d  
    64 : fmax   v3.4s, v3.4s, v29.4s       ; 63 f4 3d 4e  
    65 : mov    v3.4s, v3.4s               ; 63 1c a3 4e  
    66 : ldr    v29.4s, [sp], #0x0c        ; fd 33 c0 3d  
    67 : ldr    v30.4s, [sp], #0x01        ; fe 07 c0 3d  
    68 : mov    v2.4s, v29.4s              ; a2 1f bd 4e  
    69 : fmla   v2.4s, v3.4s, v30.4s       ; 62 cc 3e 4e  
    70 : fcvtms v2.4s, v2.4s               ; 42 b8 21 4e  
    71 : scvtf  v1.4s, v2.4s               ; 41 d8 21 4e  
    72 : mov    v1.4s, v1.4s               ; 21 1c a1 4e  
    73 : ldr    v29.4s, [sp], #0x0a        ; fd 2b c0 3d  
    74 : add    v2.4s, v2.4s, v29.4s       ; 42 84 bd 4e  
    75 : shl    v2.4s, v2.4s, #0x06, #0x07 ; 42 54 37 4f  
    76 : ldr    v29.4s, [sp], #0x02        ; fd 0b c0 3d  
    77 : fmla   v3.4s, v1.4s, v29.4s       ; 23 cc 3d 4e  
    78 : mov    v3.4s, v3.4s               ; 63 1c a3 4e  
    79 : ldr    v29.4s, [sp], #0x03        ; fd 0f c0 3d  
    80 : fmla   v3.4s, v1.4s, v29.4s       ; 23 cc 3d 4e  
    81 : mov    v3.4s, v3.4s               ; 63 1c a3 4e  
    82 : fmul   v1.4s, v3.4s, v3.4s        ; 61 dc 23 6e  
    83 : ldr    v30.4s, [sp], #0x04        ; fe 13 c0 3d  
    84 : ldr    v29.4s, [sp], #0x05        ; fd 17 c0 3d  
    85 : mov    v0.4s, v29.4s              ; a0 1f bd 4e  
    86 : fmla   v0.4s, v3.4s, v30.4s       ; 60 cc 3e 4e  
    87 : ldr    v29.4s, [sp], #0x06        ; fd 1b c0 3d  
    88 : str    v1.4s, [sp], #0            ; e1 03 80 3d  
    89 : mov    v1.4s, v0.4s               ; 01 1c a0 4e  
    90 : mov    v0.4s, v29.4s              ; a0 1f bd 4e  
    91 : fmla   v0.4s, v1.4s, v3.4s        ; 20 cc 23 4e  
    92 : ldr    v1.4s, [sp], #0            ; e1 03 c0 3d  
    93 : mov    v0.4s, v0.4s               ; 00 1c a0 4e  
    94 : ldr    v29.4s, [sp], #0x07        ; fd 1f c0 3d  
    95 : str    v1.4s, [sp], #0            ; e1 03 80 3d  
    96 : mov    v1.4s, v0.4s               ; 01 1c a0 4e  
    97 : mov    v0.4s, v29.4s              ; a0 1f bd 4e  
    98 : fmla   v0.4s, v1.4s, v3.4s        ; 20 cc 23 4e  
    99 : ldr    v1.4s, [sp], #0            ; e1 03 c0 3d  
   100 : mov    v0.4s, v0.4s               ; 00 1c a0 4e  
   101 : ldr    v29.4s, [sp], #0x08        ; fd 23 c0 3d  
   102 : str    v1.4s, [sp], #0            ; e1 03 80 3d  
   103 : mov    v1.4s, v0.4s               ; 01 1c a0 4e  
   104 : mov    v0.4s, v29.4s              ; a0 1f bd 4e  
   105 : fmla   v0.4s, v1.4s, v3.4s        ; 20 cc 23 4e  
   106 : ldr    v1.4s, [sp], #0            ; e1 03 c0 3d  
   107 : mov    v0.4s, v0.4s               ; 00 1c a0 4e  
   108 : ldr    v29.4s, [sp], #0x09        ; fd 27 c0 3d  
   109 : str    v1.4s, [sp], #0            ; e1 03 80 3d  
   110 : mov    v1.4s, v0.4s               ; 01 1c a0 4e  
   111 : mov    v0.4s, v29.4s              ; a0 1f bd 4e  
   112 : fmla   v0.4s, v1.4s, v3.4s        ; 20 cc 23 4e  
   113 : ldr    v1.4s, [sp], #0            ; e1 03 c0 3d  
   114 : mov    v0.4s, v0.4s               ; 00 1c a0 4e  
   115 : fmla   v3.4s, v0.4s, v1.4s        ; 03 cc 21 4e  
   116 : mov    v3.4s, v3.4s               ; 63 1c a3 4e  
   117 : ldr    v29.4s, [sp], #0x0b        ; fd 2f c0 3d  
   118 : fadd   v0.4s, v3.4s, v29.4s       ; 60 d4 3d 4e  
   119 : fmul   v0.4s, v0.4s, v2.4s        ; 00 dc 22 6e  
   120 : str    v0.4s, x0, x3              ; 00 68 a3 3c  
   121 : add    x3, x3, #0x10              ; 63 40 00 91  
   122 : sub    x2, x2, #0x04              ; 42 10 00 d1  
   123 : b      [58]                       ; bf ff ff 17  
   124 : ldr    v29.16b, [sp], #0x0f       ; fd 3f c0 3d  
   125 : ldr    v30.16b, [sp], #0x10       ; fe 43 c0 3d  
   126 : add    sp, sp, #0x110             ; ff 43 04 91  
   127 : ret    x30                        ; c0 03 5f d6  
