exp_f32(i0, i1, i2)
     0 : sub    sp, sp, #0xb0                ; ff c3 02 d1  
     1 : str    v8.16b, [sp], #0x01          ; e8 07 80 3d  
     2 : str    v9.16b, [sp], #0x02          ; e9 0b 80 3d  
     3 : str    v10.16b, [sp], #0x03         ; ea 0f 80 3d  
     4 : str    v11.16b, [sp], #0x04         ; eb 13 80 3d  
     5 : str    v12.16b, [sp], #0x05         ; ec 17 80 3d  
     6 : str    v13.16b, [sp], #0x06         ; ed 1b 80 3d  
     7 : str    v14.16b, [sp], #0x07         ; ee 1f 80 3d  
     8 : str    v15.16b, [sp], #0x08         ; ef 23 80 3d  
     9 : str    v16.16b, [sp], #0x09         ; f0 27 80 3d  
    10 : str    v17.16b, [sp], #0x0a         ; f1 2b 80 3d  
    11 : mov    x3, #0xc0a5                  ; a3 14 98 d2  
    12 : movk   x3, #0xc2b0, #0x01           ; 03 56 b8 f2  
    13 : dup    v0.4s, x3                    ; 60 0c 04 4e  
    14 : mov    x3, #0xc0a5                  ; a3 14 98 d2  
    15 : movk   x3, #0x42b0, #0x01           ; 03 56 a8 f2  
    16 : dup    v1.4s, x3                    ; 61 0c 04 4e  
    17 : eor    x3, x3, x3                   ; 63 00 03 ca  
    18 : movk   x3, #0x3f00, #0x01           ; 03 e0 a7 f2  
    19 : dup    v2.4s, x3                    ; 62 0c 04 4e  
    20 : eor    x3, x3, x3                   ; 63 00 03 ca  
    21 : movk   x3, #0x3f80, #0x01           ; 03 f0 a7 f2  
    22 : dup    v3.4s, x3                    ; 63 0c 04 4e  
    23 : mov    x3, #0xaa3b                  ; 63 47 95 d2  
    24 : movk   x3, #0x3fb8, #0x01           ; 03 f7 a7 f2  
    25 : dup    v4.4s, x3                    ; 64 0c 04 4e  
    26 : mov    x3, #0x8000                  ; 03 00 90 d2  
    27 : movk   x3, #0xbf31, #0x01           ; 23 e6 b7 f2  
    28 : dup    v5.4s, x3                    ; 65 0c 04 4e  
    29 : mov    x3, #0x8083                  ; 63 10 90 d2  
    30 : movk   x3, #0x395e, #0x01           ; c3 2b a7 f2  
    31 : dup    v6.4s, x3                    ; 66 0c 04 4e  
    32 : mov    x3, #0x6967                  ; e3 2c 8d d2  
    33 : movk   x3, #0x3950, #0x01           ; 03 2a a7 f2  
    34 : dup    v7.4s, x3                    ; 67 0c 04 4e  
    35 : mov    x3, #0x43ce                  ; c3 79 88 d2  
    36 : movk   x3, #0x3ab7, #0x01           ; e3 56 a7 f2  
    37 : dup    v8.4s, x3                    ; 68 0c 04 4e  
    38 : mov    x3, #0x8908                  ; 03 21 91 d2  
    39 : movk   x3, #0x3c08, #0x01           ; 03 81 a7 f2  
    40 : dup    v9.4s, x3                    ; 69 0c 04 4e  
    41 : mov    x3, #0xa9c1                  ; 23 38 95 d2  
    42 : movk   x3, #0x3d2a, #0x01           ; 43 a5 a7 f2  
    43 : dup    v10.4s, x3                   ; 6a 0c 04 4e  
    44 : mov    x3, #0xaaaa                  ; 43 55 95 d2  
    45 : movk   x3, #0x3e2a, #0x01           ; 43 c5 a7 f2  
    46 : dup    v11.4s, x3                   ; 6b 0c 04 4e  
    47 : eor    x3, x3, x3                   ; 63 00 03 ca  
    48 : movk   x3, #0x3f00, #0x01           ; 03 e0 a7 f2  
    49 : dup    v12.4s, x3                   ; 6c 0c 04 4e  
    50 : movi   v13.4s, #0x1f, #0x03         ; ed 07 03 4f  
    51 : eor    x3, x3, x3                   ; 63 00 03 ca  
    52 : cmp    x2, #0                       ; 5f 00 00 f1  
    53 : b.le   [95]                         ; 4d 05 00 54  
    54 : ldr    v14.4s, x1, x3               ; 2e 68 e3 3c  
    55 : fmin   v14.4s, v14.4s, v1.4s        ; ce f5 a1 4e  
    56 : fmax   v14.4s, v14.4s, v0.4s        ; ce f5 20 4e  
    57 : mov    v15.4s, v2.4s                ; 4f 1c a2 4e  
    58 : fmla   v15.4s, v14.4s, v4.4s        ; cf cd 24 4e  
    59 : fcvtms v15.4s, v15.4s               ; ef b9 21 4e  
    60 : scvtf  v16.4s, v15.4s               ; f0 d9 21 4e  
    61 : fmla   v14.4s, v16.4s, v5.4s        ; 0e ce 25 4e  
    62 : fmla   v14.4s, v16.4s, v6.4s        ; 0e ce 26 4e  
    63 : mov    v16.4s, v8.4s                ; 10 1d a8 4e  
    64 : fmla   v16.4s, v14.4s, v7.4s        ; d0 cd 27 4e  
    65 : str    v0.4s, [sp], #0              ; e0 03 80 3d  
    66 : mov    v0.4s, v16.4s                ; 00 1e b0 4e  
    67 : mov    v16.4s, v9.4s                ; 30 1d a9 4e  
    68 : fmla   v16.4s, v0.4s, v14.4s        ; 10 cc 2e 4e  
    69 : ldr    v0.4s, [sp], #0              ; e0 03 c0 3d  
    70 : str    v0.4s, [sp], #0              ; e0 03 80 3d  
    71 : mov    v0.4s, v16.4s                ; 00 1e b0 4e  
    72 : mov    v16.4s, v10.4s               ; 50 1d aa 4e  
    73 : fmla   v16.4s, v0.4s, v14.4s        ; 10 cc 2e 4e  
    74 : ldr    v0.4s, [sp], #0              ; e0 03 c0 3d  
    75 : str    v0.4s, [sp], #0              ; e0 03 80 3d  
    76 : mov    v0.4s, v16.4s                ; 00 1e b0 4e  
    77 : mov    v16.4s, v11.4s               ; 70 1d ab 4e  
    78 : fmla   v16.4s, v0.4s, v14.4s        ; 10 cc 2e 4e  
    79 : ldr    v0.4s, [sp], #0              ; e0 03 c0 3d  
    80 : str    v0.4s, [sp], #0              ; e0 03 80 3d  
    81 : mov    v0.4s, v16.4s                ; 00 1e b0 4e  
    82 : mov    v16.4s, v12.4s               ; 90 1d ac 4e  
    83 : fmla   v16.4s, v0.4s, v14.4s        ; 10 cc 2e 4e  
    84 : ldr    v0.4s, [sp], #0              ; e0 03 c0 3d  
    85 : fmul   v17.4s, v14.4s, v14.4s       ; d1 dd 2e 6e  
    86 : fmla   v14.4s, v16.4s, v17.4s       ; 0e ce 31 4e  
    87 : fadd   v14.4s, v14.4s, v3.4s        ; ce d5 23 4e  
    88 : add    v15.4s, v15.4s, v13.4s       ; ef 85 ad 4e  
    89 : shl    v15.4s, v15.4s, #0x06, #0x07 ; ef 55 37 4f  
    90 : fmul   v14.4s, v14.4s, v15.4s       ; ce dd 2f 6e  
    91 : str    v14.4s, x0, x3               ; 0e 68 a3 3c  
    92 : add    x3, x3, #0x10                ; 63 40 00 91  
    93 : sub    x2, x2, #0x04                ; 42 10 00 d1  
    94 : b      [52]                         ; d6 ff ff 17  
    95 : ldr    v8.16b, [sp], #0x01          ; e8 07 c0 3d  
    96 : ldr    v9.16b, [sp], #0x02          ; e9 0b c0 3d  
    97 : ldr    v10.16b, [sp], #0x03         ; ea 0f c0 3d  
    98 : ldr    v11.16b, [sp], #0x04         ; eb 13 c0 3d  
    99 : ldr    v12.16b, [sp], #0x05         ; ec 17 c0 3d  
   100 : ldr    v13.16b, [sp], #0x06         ; ed 1b c0 3d  
   101 : ldr    v14.16b, [sp], #0x07         ; ee 1f c0 3d  
   102 : ldr    v15.16b, [sp], #0x08         ; ef 23 c0 3d  
   103 : ldr    v16.16b, [sp], #0x09         ; f0 27 c0 3d  
   104 : ldr    v17.16b, [sp], #0x0a         ; f1 2b c0 3d  
   105 : add    sp, sp, #0xb0                ; ff c3 02 91  
   106 : ret    x30                          ; c0 03 5f d6  
