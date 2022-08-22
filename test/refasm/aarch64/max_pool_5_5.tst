max_pool_5_5(i0, i1, i2, i3, i4, i5, i6)
     0 : sub  sp, sp, #0xc0       ; ff 03 03 d1  
     1 : str  x18, [sp], #0x0f    ; f2 3f 00 f9  
     2 : str  x19, [sp], #0x10    ; f3 43 00 f9  
     3 : str  x20, [sp], #0x11    ; f4 47 00 f9  
     4 : str  x21, [sp], #0x12    ; f5 4b 00 f9  
     5 : str  x22, [sp], #0x13    ; f6 4f 00 f9  
     6 : str  x23, [sp], #0x14    ; f7 53 00 f9  
     7 : str  x24, [sp], #0x15    ; f8 57 00 f9  
     8 : str  x25, [sp], #0x16    ; f9 5b 00 f9  
     9 : str  x26, [sp], #0x17    ; fa 5f 00 f9  
    10 : mov  x7, #0              ; 07 00 80 d2  
    11 : mov  x7, #0              ; 07 00 80 d2  
    12 : mov  x8, #0x04           ; 88 00 80 d2  
    13 : mul  x8, x6, x8          ; c8 7c 08 9b  
    14 : mov  x9, #0              ; 09 00 80 d2  
    15 : mul  x9, x9, x2          ; 29 7d 02 9b  
    16 : mov  x10, #0             ; 0a 00 80 d2  
    17 : add  x9, x9, x10         ; 29 01 0a 8b  
    18 : mov  x10, #0x04          ; 8a 00 80 d2  
    19 : mul  x9, x9, x10         ; 29 7d 0a 9b  
    20 : mov  x10, #0             ; 0a 00 80 d2  
    21 : mul  x10, x10, x2        ; 4a 7d 02 9b  
    22 : mov  x11, #0x01          ; 2b 00 80 d2  
    23 : add  x10, x10, x11       ; 4a 01 0b 8b  
    24 : mov  x11, #0x04          ; 8b 00 80 d2  
    25 : mul  x10, x10, x11       ; 4a 7d 0b 9b  
    26 : mov  x11, #0             ; 0b 00 80 d2  
    27 : mul  x11, x11, x2        ; 6b 7d 02 9b  
    28 : mov  x12, #0x02          ; 4c 00 80 d2  
    29 : add  x11, x11, x12       ; 6b 01 0c 8b  
    30 : mov  x12, #0x04          ; 8c 00 80 d2  
    31 : mul  x11, x11, x12       ; 6b 7d 0c 9b  
    32 : mov  x12, #0             ; 0c 00 80 d2  
    33 : mul  x12, x12, x2        ; 8c 7d 02 9b  
    34 : mov  x13, #0x03          ; 6d 00 80 d2  
    35 : add  x12, x12, x13       ; 8c 01 0d 8b  
    36 : mov  x13, #0x04          ; 8d 00 80 d2  
    37 : mul  x12, x12, x13       ; 8c 7d 0d 9b  
    38 : mov  x13, #0             ; 0d 00 80 d2  
    39 : mul  x13, x13, x2        ; ad 7d 02 9b  
    40 : mov  x14, #0x04          ; 8e 00 80 d2  
    41 : add  x13, x13, x14       ; ad 01 0e 8b  
    42 : mov  x14, #0x04          ; 8e 00 80 d2  
    43 : mul  x13, x13, x14       ; ad 7d 0e 9b  
    44 : mov  x14, #0x01          ; 2e 00 80 d2  
    45 : mul  x14, x14, x2        ; ce 7d 02 9b  
    46 : mov  x15, #0             ; 0f 00 80 d2  
    47 : add  x14, x14, x15       ; ce 01 0f 8b  
    48 : mov  x15, #0x04          ; 8f 00 80 d2  
    49 : mul  x14, x14, x15       ; ce 7d 0f 9b  
    50 : mov  x15, #0x01          ; 2f 00 80 d2  
    51 : mul  x15, x15, x2        ; ef 7d 02 9b  
    52 : mov  x16, #0x01          ; 30 00 80 d2  
    53 : add  x15, x15, x16       ; ef 01 10 8b  
    54 : mov  x16, #0x04          ; 90 00 80 d2  
    55 : mul  x15, x15, x16       ; ef 7d 10 9b  
    56 : mov  x16, #0x01          ; 30 00 80 d2  
    57 : mul  x16, x16, x2        ; 10 7e 02 9b  
    58 : mov  x17, #0x02          ; 51 00 80 d2  
    59 : add  x16, x16, x17       ; 10 02 11 8b  
    60 : mov  x17, #0x04          ; 91 00 80 d2  
    61 : mul  x16, x16, x17       ; 10 7e 11 9b  
    62 : mov  x17, #0x01          ; 31 00 80 d2  
    63 : mul  x17, x17, x2        ; 31 7e 02 9b  
    64 : mov  x18, #0x03          ; 72 00 80 d2  
    65 : add  x17, x17, x18       ; 31 02 12 8b  
    66 : mov  x18, #0x04          ; 92 00 80 d2  
    67 : mul  x17, x17, x18       ; 31 7e 12 9b  
    68 : mov  x18, #0x01          ; 32 00 80 d2  
    69 : mul  x18, x18, x2        ; 52 7e 02 9b  
    70 : mov  x19, #0x04          ; 93 00 80 d2  
    71 : add  x18, x18, x19       ; 52 02 13 8b  
    72 : mov  x19, #0x04          ; 93 00 80 d2  
    73 : mul  x18, x18, x19       ; 52 7e 13 9b  
    74 : mov  x19, #0x02          ; 53 00 80 d2  
    75 : mul  x19, x19, x2        ; 73 7e 02 9b  
    76 : mov  x20, #0             ; 14 00 80 d2  
    77 : add  x19, x19, x20       ; 73 02 14 8b  
    78 : mov  x20, #0x04          ; 94 00 80 d2  
    79 : mul  x26, x19, x20       ; 7a 7e 14 9b  
    80 : str  x26, [sp], #0x0e    ; fa 3b 00 f9  
    81 : mov  x20, #0x02          ; 54 00 80 d2  
    82 : mul  x20, x20, x2        ; 94 7e 02 9b  
    83 : mov  x21, #0x01          ; 35 00 80 d2  
    84 : add  x20, x20, x21       ; 94 02 15 8b  
    85 : mov  x21, #0x04          ; 95 00 80 d2  
    86 : mul  x26, x20, x21       ; 9a 7e 15 9b  
    87 : str  x26, [sp], #0x0d    ; fa 37 00 f9  
    88 : mov  x21, #0x02          ; 55 00 80 d2  
    89 : mul  x21, x21, x2        ; b5 7e 02 9b  
    90 : mov  x22, #0x02          ; 56 00 80 d2  
    91 : add  x21, x21, x22       ; b5 02 16 8b  
    92 : mov  x22, #0x04          ; 96 00 80 d2  
    93 : mul  x26, x21, x22       ; ba 7e 16 9b  
    94 : str  x26, [sp], #0x0c    ; fa 33 00 f9  
    95 : mov  x22, #0x02          ; 56 00 80 d2  
    96 : mul  x22, x22, x2        ; d6 7e 02 9b  
    97 : mov  x23, #0x03          ; 77 00 80 d2  
    98 : add  x22, x22, x23       ; d6 02 17 8b  
    99 : mov  x23, #0x04          ; 97 00 80 d2  
   100 : mul  x26, x22, x23       ; da 7e 17 9b  
   101 : str  x26, [sp], #0x0b    ; fa 2f 00 f9  
   102 : mov  x23, #0x02          ; 57 00 80 d2  
   103 : mul  x23, x23, x2        ; f7 7e 02 9b  
   104 : mov  x24, #0x04          ; 98 00 80 d2  
   105 : add  x23, x23, x24       ; f7 02 18 8b  
   106 : mov  x24, #0x04          ; 98 00 80 d2  
   107 : mul  x26, x23, x24       ; fa 7e 18 9b  
   108 : str  x26, [sp], #0x0a    ; fa 2b 00 f9  
   109 : mov  x24, #0x03          ; 78 00 80 d2  
   110 : mul  x24, x24, x2        ; 18 7f 02 9b  
   111 : mov  x25, #0             ; 19 00 80 d2  
   112 : add  x24, x24, x25       ; 18 03 19 8b  
   113 : mov  x25, #0x04          ; 99 00 80 d2  
   114 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   115 : str  x26, [sp], #0       ; fa 03 00 f9  
   116 : mov  x25, #0x03          ; 79 00 80 d2  
   117 : mul  x25, x25, x2        ; 39 7f 02 9b  
   118 : mov  x24, #0x01          ; 38 00 80 d2  
   119 : add  x24, x25, x24       ; 38 03 18 8b  
   120 : mov  x25, #0x04          ; 99 00 80 d2  
   121 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   122 : str  x26, [sp], #0x01    ; fa 07 00 f9  
   123 : mov  x25, #0x03          ; 79 00 80 d2  
   124 : mul  x25, x25, x2        ; 39 7f 02 9b  
   125 : mov  x24, #0x02          ; 58 00 80 d2  
   126 : add  x24, x25, x24       ; 38 03 18 8b  
   127 : mov  x25, #0x04          ; 99 00 80 d2  
   128 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   129 : str  x26, [sp], #0x02    ; fa 0b 00 f9  
   130 : mov  x25, #0x03          ; 79 00 80 d2  
   131 : mul  x25, x25, x2        ; 39 7f 02 9b  
   132 : mov  x24, #0x03          ; 78 00 80 d2  
   133 : add  x24, x25, x24       ; 38 03 18 8b  
   134 : mov  x25, #0x04          ; 99 00 80 d2  
   135 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   136 : str  x26, [sp], #0x03    ; fa 0f 00 f9  
   137 : mov  x25, #0x03          ; 79 00 80 d2  
   138 : mul  x25, x25, x2        ; 39 7f 02 9b  
   139 : mov  x24, #0x04          ; 98 00 80 d2  
   140 : add  x24, x25, x24       ; 38 03 18 8b  
   141 : mov  x25, #0x04          ; 99 00 80 d2  
   142 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   143 : str  x26, [sp], #0x04    ; fa 13 00 f9  
   144 : mov  x25, #0x04          ; 99 00 80 d2  
   145 : mul  x25, x25, x2        ; 39 7f 02 9b  
   146 : mov  x24, #0             ; 18 00 80 d2  
   147 : add  x24, x25, x24       ; 38 03 18 8b  
   148 : mov  x25, #0x04          ; 99 00 80 d2  
   149 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   150 : str  x26, [sp], #0x05    ; fa 17 00 f9  
   151 : mov  x25, #0x04          ; 99 00 80 d2  
   152 : mul  x25, x25, x2        ; 39 7f 02 9b  
   153 : mov  x24, #0x01          ; 38 00 80 d2  
   154 : add  x24, x25, x24       ; 38 03 18 8b  
   155 : mov  x25, #0x04          ; 99 00 80 d2  
   156 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   157 : str  x26, [sp], #0x06    ; fa 1b 00 f9  
   158 : mov  x25, #0x04          ; 99 00 80 d2  
   159 : mul  x25, x25, x2        ; 39 7f 02 9b  
   160 : mov  x24, #0x02          ; 58 00 80 d2  
   161 : add  x24, x25, x24       ; 38 03 18 8b  
   162 : mov  x25, #0x04          ; 99 00 80 d2  
   163 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   164 : str  x26, [sp], #0x07    ; fa 1f 00 f9  
   165 : mov  x25, #0x04          ; 99 00 80 d2  
   166 : mul  x25, x25, x2        ; 39 7f 02 9b  
   167 : mov  x24, #0x03          ; 78 00 80 d2  
   168 : add  x24, x25, x24       ; 38 03 18 8b  
   169 : mov  x25, #0x04          ; 99 00 80 d2  
   170 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   171 : str  x26, [sp], #0x08    ; fa 23 00 f9  
   172 : mov  x25, #0x04          ; 99 00 80 d2  
   173 : mul  x25, x25, x2        ; 39 7f 02 9b  
   174 : mov  x24, #0x04          ; 98 00 80 d2  
   175 : add  x24, x25, x24       ; 38 03 18 8b  
   176 : mov  x25, #0x04          ; 99 00 80 d2  
   177 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   178 : str  x26, [sp], #0x09    ; fa 27 00 f9  
   179 : cmp  x7, x3              ; ff 00 03 eb  
   180 : b.ge [283]               ; ea 0c 00 54  
   181 : mov  x25, #0             ; 19 00 80 d2  
   182 : mul  x24, x7, x6         ; f8 7c 06 9b  
   183 : mul  x24, x24, x5        ; 18 7f 05 9b  
   184 : mov  x23, #0x04          ; 97 00 80 d2  
   185 : mul  x23, x24, x23       ; 17 7f 17 9b  
   186 : add  x23, x4, x23        ; 97 00 17 8b  
   187 : mul  x24, x7, x2         ; f8 7c 02 9b  
   188 : mul  x24, x24, x1        ; 18 7f 01 9b  
   189 : mov  x22, #0x04          ; 96 00 80 d2  
   190 : mul  x22, x24, x22       ; 16 7f 16 9b  
   191 : add  x22, x0, x22        ; 16 00 16 8b  
   192 : cmp  x25, x5             ; 3f 03 05 eb  
   193 : b.ge [281]               ; 0a 0b 00 54  
   194 : mov  x24, #0             ; 18 00 80 d2  
   195 : mov  x21, #0x10          ; 15 02 80 d2  
   196 : cmp  x24, x8             ; 1f 03 08 eb  
   197 : b.ge [273]               ; 8a 09 00 54  
   198 : cmp  x21, x8             ; bf 02 08 eb  
   199 : b.le [203]               ; 8d 00 00 54  
   200 : sub  x20, x21, x8        ; b4 02 08 cb  
   201 : sub  x24, x24, x20       ; 18 03 14 cb  
   202 : sub  x21, x21, x20       ; b5 02 14 cb  
   203 : add  x20, x22, x24       ; d4 02 18 8b  
   204 : add  x19, x23, x24       ; f3 02 18 8b  
   205 : ldr  v0.4s, x20, x9      ; 80 6a e9 3c  
   206 : ldr  v1.4s, x20, x10     ; 81 6a ea 3c  
   207 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   208 : ldr  v1.4s, x20, x11     ; 81 6a eb 3c  
   209 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   210 : ldr  v1.4s, x20, x12     ; 81 6a ec 3c  
   211 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   212 : ldr  v1.4s, x20, x13     ; 81 6a ed 3c  
   213 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   214 : ldr  v1.4s, x20, x14     ; 81 6a ee 3c  
   215 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   216 : ldr  v1.4s, x20, x15     ; 81 6a ef 3c  
   217 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   218 : ldr  v1.4s, x20, x16     ; 81 6a f0 3c  
   219 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   220 : ldr  v1.4s, x20, x17     ; 81 6a f1 3c  
   221 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   222 : ldr  v1.4s, x20, x18     ; 81 6a f2 3c  
   223 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   224 : ldr  x26, [sp], #0x0e    ; fa 3b 40 f9  
   225 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   226 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   227 : ldr  x26, [sp], #0x0d    ; fa 37 40 f9  
   228 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   229 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   230 : ldr  x26, [sp], #0x0c    ; fa 33 40 f9  
   231 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   232 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   233 : ldr  x26, [sp], #0x0b    ; fa 2f 40 f9  
   234 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   235 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   236 : ldr  x26, [sp], #0x0a    ; fa 2b 40 f9  
   237 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   238 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   239 : ldr  x26, [sp], #0       ; fa 03 40 f9  
   240 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   241 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   242 : ldr  x26, [sp], #0x01    ; fa 07 40 f9  
   243 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   244 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   245 : ldr  x26, [sp], #0x02    ; fa 0b 40 f9  
   246 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   247 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   248 : ldr  x26, [sp], #0x03    ; fa 0f 40 f9  
   249 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   250 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   251 : ldr  x26, [sp], #0x04    ; fa 13 40 f9  
   252 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   253 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   254 : ldr  x26, [sp], #0x05    ; fa 17 40 f9  
   255 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   256 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   257 : ldr  x26, [sp], #0x06    ; fa 1b 40 f9  
   258 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   259 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   260 : ldr  x26, [sp], #0x07    ; fa 1f 40 f9  
   261 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   262 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   263 : ldr  x26, [sp], #0x08    ; fa 23 40 f9  
   264 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   265 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   266 : ldr  x26, [sp], #0x09    ; fa 27 40 f9  
   267 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   268 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   269 : str  v0.4s, x19, #0      ; 60 02 80 3d  
   270 : mov  x24, x21            ; f8 03 15 aa  
   271 : add  x21, x21, #0x10     ; b5 42 00 91  
   272 : b    [196]               ; b4 ff ff 17  
   273 : add  x25, x25, #0x01     ; 39 07 00 91  
   274 : mov  x19, #0x04          ; 93 00 80 d2  
   275 : mul  x19, x6, x19        ; d3 7c 13 9b  
   276 : add  x23, x23, x19       ; f7 02 13 8b  
   277 : mov  x19, #0x04          ; 93 00 80 d2  
   278 : mul  x19, x2, x19        ; 53 7c 13 9b  
   279 : add  x22, x22, x19       ; d6 02 13 8b  
   280 : b    [192]               ; a8 ff ff 17  
   281 : add  x7, x7, #0x01       ; e7 04 00 91  
   282 : b    [179]               ; 99 ff ff 17  
   283 : mov  x0, #0              ; 00 00 80 d2  
   284 : ldr  x18, [sp], #0x0f    ; f2 3f 40 f9  
   285 : ldr  x19, [sp], #0x10    ; f3 43 40 f9  
   286 : ldr  x20, [sp], #0x11    ; f4 47 40 f9  
   287 : ldr  x21, [sp], #0x12    ; f5 4b 40 f9  
   288 : ldr  x22, [sp], #0x13    ; f6 4f 40 f9  
   289 : ldr  x23, [sp], #0x14    ; f7 53 40 f9  
   290 : ldr  x24, [sp], #0x15    ; f8 57 40 f9  
   291 : ldr  x25, [sp], #0x16    ; f9 5b 40 f9  
   292 : ldr  x26, [sp], #0x17    ; fa 5f 40 f9  
   293 : add  sp, sp, #0xc0       ; ff 03 03 91  
   294 : ret  x30                 ; c0 03 5f d6  
