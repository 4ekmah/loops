max_pool_9_9(i0, i1, i2, i3, i4, i5, i6)
     0 : sub  sp, sp, #0x280      ; ff 03 0a d1  
     1 : str  x18, [sp], #0x47    ; f2 1f 01 f9  
     2 : str  x19, [sp], #0x48    ; f3 23 01 f9  
     3 : str  x20, [sp], #0x49    ; f4 27 01 f9  
     4 : str  x21, [sp], #0x4a    ; f5 2b 01 f9  
     5 : str  x22, [sp], #0x4b    ; f6 2f 01 f9  
     6 : str  x23, [sp], #0x4c    ; f7 33 01 f9  
     7 : str  x24, [sp], #0x4d    ; f8 37 01 f9  
     8 : str  x25, [sp], #0x4e    ; f9 3b 01 f9  
     9 : str  x26, [sp], #0x4f    ; fa 3f 01 f9  
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
    44 : mov  x14, #0             ; 0e 00 80 d2  
    45 : mul  x14, x14, x2        ; ce 7d 02 9b  
    46 : mov  x15, #0x05          ; af 00 80 d2  
    47 : add  x14, x14, x15       ; ce 01 0f 8b  
    48 : mov  x15, #0x04          ; 8f 00 80 d2  
    49 : mul  x14, x14, x15       ; ce 7d 0f 9b  
    50 : mov  x15, #0             ; 0f 00 80 d2  
    51 : mul  x15, x15, x2        ; ef 7d 02 9b  
    52 : mov  x16, #0x06          ; d0 00 80 d2  
    53 : add  x15, x15, x16       ; ef 01 10 8b  
    54 : mov  x16, #0x04          ; 90 00 80 d2  
    55 : mul  x15, x15, x16       ; ef 7d 10 9b  
    56 : mov  x16, #0             ; 10 00 80 d2  
    57 : mul  x16, x16, x2        ; 10 7e 02 9b  
    58 : mov  x17, #0x07          ; f1 00 80 d2  
    59 : add  x16, x16, x17       ; 10 02 11 8b  
    60 : mov  x17, #0x04          ; 91 00 80 d2  
    61 : mul  x16, x16, x17       ; 10 7e 11 9b  
    62 : mov  x17, #0             ; 11 00 80 d2  
    63 : mul  x17, x17, x2        ; 31 7e 02 9b  
    64 : mov  x18, #0x08          ; 12 01 80 d2  
    65 : add  x17, x17, x18       ; 31 02 12 8b  
    66 : mov  x18, #0x04          ; 92 00 80 d2  
    67 : mul  x17, x17, x18       ; 31 7e 12 9b  
    68 : mov  x18, #0x01          ; 32 00 80 d2  
    69 : mul  x18, x18, x2        ; 52 7e 02 9b  
    70 : mov  x19, #0             ; 13 00 80 d2  
    71 : add  x18, x18, x19       ; 52 02 13 8b  
    72 : mov  x19, #0x04          ; 93 00 80 d2  
    73 : mul  x18, x18, x19       ; 52 7e 13 9b  
    74 : mov  x19, #0x01          ; 33 00 80 d2  
    75 : mul  x19, x19, x2        ; 73 7e 02 9b  
    76 : mov  x20, #0x01          ; 34 00 80 d2  
    77 : add  x19, x19, x20       ; 73 02 14 8b  
    78 : mov  x20, #0x04          ; 94 00 80 d2  
    79 : mul  x26, x19, x20       ; 7a 7e 14 9b  
    80 : str  x26, [sp], #0x46    ; fa 1b 01 f9  
    81 : mov  x20, #0x01          ; 34 00 80 d2  
    82 : mul  x20, x20, x2        ; 94 7e 02 9b  
    83 : mov  x21, #0x02          ; 55 00 80 d2  
    84 : add  x20, x20, x21       ; 94 02 15 8b  
    85 : mov  x21, #0x04          ; 95 00 80 d2  
    86 : mul  x26, x20, x21       ; 9a 7e 15 9b  
    87 : str  x26, [sp], #0x45    ; fa 17 01 f9  
    88 : mov  x21, #0x01          ; 35 00 80 d2  
    89 : mul  x21, x21, x2        ; b5 7e 02 9b  
    90 : mov  x22, #0x03          ; 76 00 80 d2  
    91 : add  x21, x21, x22       ; b5 02 16 8b  
    92 : mov  x22, #0x04          ; 96 00 80 d2  
    93 : mul  x26, x21, x22       ; ba 7e 16 9b  
    94 : str  x26, [sp], #0x44    ; fa 13 01 f9  
    95 : mov  x22, #0x01          ; 36 00 80 d2  
    96 : mul  x22, x22, x2        ; d6 7e 02 9b  
    97 : mov  x23, #0x04          ; 97 00 80 d2  
    98 : add  x22, x22, x23       ; d6 02 17 8b  
    99 : mov  x23, #0x04          ; 97 00 80 d2  
   100 : mul  x26, x22, x23       ; da 7e 17 9b  
   101 : str  x26, [sp], #0x43    ; fa 0f 01 f9  
   102 : mov  x23, #0x01          ; 37 00 80 d2  
   103 : mul  x23, x23, x2        ; f7 7e 02 9b  
   104 : mov  x24, #0x05          ; b8 00 80 d2  
   105 : add  x23, x23, x24       ; f7 02 18 8b  
   106 : mov  x24, #0x04          ; 98 00 80 d2  
   107 : mul  x26, x23, x24       ; fa 7e 18 9b  
   108 : str  x26, [sp], #0x42    ; fa 0b 01 f9  
   109 : mov  x24, #0x01          ; 38 00 80 d2  
   110 : mul  x24, x24, x2        ; 18 7f 02 9b  
   111 : mov  x25, #0x06          ; d9 00 80 d2  
   112 : add  x24, x24, x25       ; 18 03 19 8b  
   113 : mov  x25, #0x04          ; 99 00 80 d2  
   114 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   115 : str  x26, [sp], #0       ; fa 03 00 f9  
   116 : mov  x25, #0x01          ; 39 00 80 d2  
   117 : mul  x25, x25, x2        ; 39 7f 02 9b  
   118 : mov  x24, #0x07          ; f8 00 80 d2  
   119 : add  x24, x25, x24       ; 38 03 18 8b  
   120 : mov  x25, #0x04          ; 99 00 80 d2  
   121 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   122 : str  x26, [sp], #0x01    ; fa 07 00 f9  
   123 : mov  x25, #0x01          ; 39 00 80 d2  
   124 : mul  x25, x25, x2        ; 39 7f 02 9b  
   125 : mov  x24, #0x08          ; 18 01 80 d2  
   126 : add  x24, x25, x24       ; 38 03 18 8b  
   127 : mov  x25, #0x04          ; 99 00 80 d2  
   128 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   129 : str  x26, [sp], #0x02    ; fa 0b 00 f9  
   130 : mov  x25, #0x02          ; 59 00 80 d2  
   131 : mul  x25, x25, x2        ; 39 7f 02 9b  
   132 : mov  x24, #0             ; 18 00 80 d2  
   133 : add  x24, x25, x24       ; 38 03 18 8b  
   134 : mov  x25, #0x04          ; 99 00 80 d2  
   135 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   136 : str  x26, [sp], #0x03    ; fa 0f 00 f9  
   137 : mov  x25, #0x02          ; 59 00 80 d2  
   138 : mul  x25, x25, x2        ; 39 7f 02 9b  
   139 : mov  x24, #0x01          ; 38 00 80 d2  
   140 : add  x24, x25, x24       ; 38 03 18 8b  
   141 : mov  x25, #0x04          ; 99 00 80 d2  
   142 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   143 : str  x26, [sp], #0x04    ; fa 13 00 f9  
   144 : mov  x25, #0x02          ; 59 00 80 d2  
   145 : mul  x25, x25, x2        ; 39 7f 02 9b  
   146 : mov  x24, #0x02          ; 58 00 80 d2  
   147 : add  x24, x25, x24       ; 38 03 18 8b  
   148 : mov  x25, #0x04          ; 99 00 80 d2  
   149 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   150 : str  x26, [sp], #0x05    ; fa 17 00 f9  
   151 : mov  x25, #0x02          ; 59 00 80 d2  
   152 : mul  x25, x25, x2        ; 39 7f 02 9b  
   153 : mov  x24, #0x03          ; 78 00 80 d2  
   154 : add  x24, x25, x24       ; 38 03 18 8b  
   155 : mov  x25, #0x04          ; 99 00 80 d2  
   156 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   157 : str  x26, [sp], #0x06    ; fa 1b 00 f9  
   158 : mov  x25, #0x02          ; 59 00 80 d2  
   159 : mul  x25, x25, x2        ; 39 7f 02 9b  
   160 : mov  x24, #0x04          ; 98 00 80 d2  
   161 : add  x24, x25, x24       ; 38 03 18 8b  
   162 : mov  x25, #0x04          ; 99 00 80 d2  
   163 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   164 : str  x26, [sp], #0x07    ; fa 1f 00 f9  
   165 : mov  x25, #0x02          ; 59 00 80 d2  
   166 : mul  x25, x25, x2        ; 39 7f 02 9b  
   167 : mov  x24, #0x05          ; b8 00 80 d2  
   168 : add  x24, x25, x24       ; 38 03 18 8b  
   169 : mov  x25, #0x04          ; 99 00 80 d2  
   170 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   171 : str  x26, [sp], #0x08    ; fa 23 00 f9  
   172 : mov  x25, #0x02          ; 59 00 80 d2  
   173 : mul  x25, x25, x2        ; 39 7f 02 9b  
   174 : mov  x24, #0x06          ; d8 00 80 d2  
   175 : add  x24, x25, x24       ; 38 03 18 8b  
   176 : mov  x25, #0x04          ; 99 00 80 d2  
   177 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   178 : str  x26, [sp], #0x09    ; fa 27 00 f9  
   179 : mov  x25, #0x02          ; 59 00 80 d2  
   180 : mul  x25, x25, x2        ; 39 7f 02 9b  
   181 : mov  x24, #0x07          ; f8 00 80 d2  
   182 : add  x24, x25, x24       ; 38 03 18 8b  
   183 : mov  x25, #0x04          ; 99 00 80 d2  
   184 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   185 : str  x26, [sp], #0x0a    ; fa 2b 00 f9  
   186 : mov  x25, #0x02          ; 59 00 80 d2  
   187 : mul  x25, x25, x2        ; 39 7f 02 9b  
   188 : mov  x24, #0x08          ; 18 01 80 d2  
   189 : add  x24, x25, x24       ; 38 03 18 8b  
   190 : mov  x25, #0x04          ; 99 00 80 d2  
   191 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   192 : str  x26, [sp], #0x0b    ; fa 2f 00 f9  
   193 : mov  x25, #0x03          ; 79 00 80 d2  
   194 : mul  x25, x25, x2        ; 39 7f 02 9b  
   195 : mov  x24, #0             ; 18 00 80 d2  
   196 : add  x24, x25, x24       ; 38 03 18 8b  
   197 : mov  x25, #0x04          ; 99 00 80 d2  
   198 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   199 : str  x26, [sp], #0x0c    ; fa 33 00 f9  
   200 : mov  x25, #0x03          ; 79 00 80 d2  
   201 : mul  x25, x25, x2        ; 39 7f 02 9b  
   202 : mov  x24, #0x01          ; 38 00 80 d2  
   203 : add  x24, x25, x24       ; 38 03 18 8b  
   204 : mov  x25, #0x04          ; 99 00 80 d2  
   205 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   206 : str  x26, [sp], #0x0d    ; fa 37 00 f9  
   207 : mov  x25, #0x03          ; 79 00 80 d2  
   208 : mul  x25, x25, x2        ; 39 7f 02 9b  
   209 : mov  x24, #0x02          ; 58 00 80 d2  
   210 : add  x24, x25, x24       ; 38 03 18 8b  
   211 : mov  x25, #0x04          ; 99 00 80 d2  
   212 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   213 : str  x26, [sp], #0x0e    ; fa 3b 00 f9  
   214 : mov  x25, #0x03          ; 79 00 80 d2  
   215 : mul  x25, x25, x2        ; 39 7f 02 9b  
   216 : mov  x24, #0x03          ; 78 00 80 d2  
   217 : add  x24, x25, x24       ; 38 03 18 8b  
   218 : mov  x25, #0x04          ; 99 00 80 d2  
   219 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   220 : str  x26, [sp], #0x0f    ; fa 3f 00 f9  
   221 : mov  x25, #0x03          ; 79 00 80 d2  
   222 : mul  x25, x25, x2        ; 39 7f 02 9b  
   223 : mov  x24, #0x04          ; 98 00 80 d2  
   224 : add  x24, x25, x24       ; 38 03 18 8b  
   225 : mov  x25, #0x04          ; 99 00 80 d2  
   226 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   227 : str  x26, [sp], #0x10    ; fa 43 00 f9  
   228 : mov  x25, #0x03          ; 79 00 80 d2  
   229 : mul  x25, x25, x2        ; 39 7f 02 9b  
   230 : mov  x24, #0x05          ; b8 00 80 d2  
   231 : add  x24, x25, x24       ; 38 03 18 8b  
   232 : mov  x25, #0x04          ; 99 00 80 d2  
   233 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   234 : str  x26, [sp], #0x11    ; fa 47 00 f9  
   235 : mov  x25, #0x03          ; 79 00 80 d2  
   236 : mul  x25, x25, x2        ; 39 7f 02 9b  
   237 : mov  x24, #0x06          ; d8 00 80 d2  
   238 : add  x24, x25, x24       ; 38 03 18 8b  
   239 : mov  x25, #0x04          ; 99 00 80 d2  
   240 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   241 : str  x26, [sp], #0x12    ; fa 4b 00 f9  
   242 : mov  x25, #0x03          ; 79 00 80 d2  
   243 : mul  x25, x25, x2        ; 39 7f 02 9b  
   244 : mov  x24, #0x07          ; f8 00 80 d2  
   245 : add  x24, x25, x24       ; 38 03 18 8b  
   246 : mov  x25, #0x04          ; 99 00 80 d2  
   247 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   248 : str  x26, [sp], #0x13    ; fa 4f 00 f9  
   249 : mov  x25, #0x03          ; 79 00 80 d2  
   250 : mul  x25, x25, x2        ; 39 7f 02 9b  
   251 : mov  x24, #0x08          ; 18 01 80 d2  
   252 : add  x24, x25, x24       ; 38 03 18 8b  
   253 : mov  x25, #0x04          ; 99 00 80 d2  
   254 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   255 : str  x26, [sp], #0x14    ; fa 53 00 f9  
   256 : mov  x25, #0x04          ; 99 00 80 d2  
   257 : mul  x25, x25, x2        ; 39 7f 02 9b  
   258 : mov  x24, #0             ; 18 00 80 d2  
   259 : add  x24, x25, x24       ; 38 03 18 8b  
   260 : mov  x25, #0x04          ; 99 00 80 d2  
   261 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   262 : str  x26, [sp], #0x15    ; fa 57 00 f9  
   263 : mov  x25, #0x04          ; 99 00 80 d2  
   264 : mul  x25, x25, x2        ; 39 7f 02 9b  
   265 : mov  x24, #0x01          ; 38 00 80 d2  
   266 : add  x24, x25, x24       ; 38 03 18 8b  
   267 : mov  x25, #0x04          ; 99 00 80 d2  
   268 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   269 : str  x26, [sp], #0x16    ; fa 5b 00 f9  
   270 : mov  x25, #0x04          ; 99 00 80 d2  
   271 : mul  x25, x25, x2        ; 39 7f 02 9b  
   272 : mov  x24, #0x02          ; 58 00 80 d2  
   273 : add  x24, x25, x24       ; 38 03 18 8b  
   274 : mov  x25, #0x04          ; 99 00 80 d2  
   275 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   276 : str  x26, [sp], #0x17    ; fa 5f 00 f9  
   277 : mov  x25, #0x04          ; 99 00 80 d2  
   278 : mul  x25, x25, x2        ; 39 7f 02 9b  
   279 : mov  x24, #0x03          ; 78 00 80 d2  
   280 : add  x24, x25, x24       ; 38 03 18 8b  
   281 : mov  x25, #0x04          ; 99 00 80 d2  
   282 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   283 : str  x26, [sp], #0x18    ; fa 63 00 f9  
   284 : mov  x25, #0x04          ; 99 00 80 d2  
   285 : mul  x25, x25, x2        ; 39 7f 02 9b  
   286 : mov  x24, #0x04          ; 98 00 80 d2  
   287 : add  x24, x25, x24       ; 38 03 18 8b  
   288 : mov  x25, #0x04          ; 99 00 80 d2  
   289 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   290 : str  x26, [sp], #0x19    ; fa 67 00 f9  
   291 : mov  x25, #0x04          ; 99 00 80 d2  
   292 : mul  x25, x25, x2        ; 39 7f 02 9b  
   293 : mov  x24, #0x05          ; b8 00 80 d2  
   294 : add  x24, x25, x24       ; 38 03 18 8b  
   295 : mov  x25, #0x04          ; 99 00 80 d2  
   296 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   297 : str  x26, [sp], #0x1a    ; fa 6b 00 f9  
   298 : mov  x25, #0x04          ; 99 00 80 d2  
   299 : mul  x25, x25, x2        ; 39 7f 02 9b  
   300 : mov  x24, #0x06          ; d8 00 80 d2  
   301 : add  x24, x25, x24       ; 38 03 18 8b  
   302 : mov  x25, #0x04          ; 99 00 80 d2  
   303 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   304 : str  x26, [sp], #0x1b    ; fa 6f 00 f9  
   305 : mov  x25, #0x04          ; 99 00 80 d2  
   306 : mul  x25, x25, x2        ; 39 7f 02 9b  
   307 : mov  x24, #0x07          ; f8 00 80 d2  
   308 : add  x24, x25, x24       ; 38 03 18 8b  
   309 : mov  x25, #0x04          ; 99 00 80 d2  
   310 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   311 : str  x26, [sp], #0x1c    ; fa 73 00 f9  
   312 : mov  x25, #0x04          ; 99 00 80 d2  
   313 : mul  x25, x25, x2        ; 39 7f 02 9b  
   314 : mov  x24, #0x08          ; 18 01 80 d2  
   315 : add  x24, x25, x24       ; 38 03 18 8b  
   316 : mov  x25, #0x04          ; 99 00 80 d2  
   317 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   318 : str  x26, [sp], #0x1d    ; fa 77 00 f9  
   319 : mov  x25, #0x05          ; b9 00 80 d2  
   320 : mul  x25, x25, x2        ; 39 7f 02 9b  
   321 : mov  x24, #0             ; 18 00 80 d2  
   322 : add  x24, x25, x24       ; 38 03 18 8b  
   323 : mov  x25, #0x04          ; 99 00 80 d2  
   324 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   325 : str  x26, [sp], #0x1e    ; fa 7b 00 f9  
   326 : mov  x25, #0x05          ; b9 00 80 d2  
   327 : mul  x25, x25, x2        ; 39 7f 02 9b  
   328 : mov  x24, #0x01          ; 38 00 80 d2  
   329 : add  x24, x25, x24       ; 38 03 18 8b  
   330 : mov  x25, #0x04          ; 99 00 80 d2  
   331 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   332 : str  x26, [sp], #0x1f    ; fa 7f 00 f9  
   333 : mov  x25, #0x05          ; b9 00 80 d2  
   334 : mul  x25, x25, x2        ; 39 7f 02 9b  
   335 : mov  x24, #0x02          ; 58 00 80 d2  
   336 : add  x24, x25, x24       ; 38 03 18 8b  
   337 : mov  x25, #0x04          ; 99 00 80 d2  
   338 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   339 : str  x26, [sp], #0x20    ; fa 83 00 f9  
   340 : mov  x25, #0x05          ; b9 00 80 d2  
   341 : mul  x25, x25, x2        ; 39 7f 02 9b  
   342 : mov  x24, #0x03          ; 78 00 80 d2  
   343 : add  x24, x25, x24       ; 38 03 18 8b  
   344 : mov  x25, #0x04          ; 99 00 80 d2  
   345 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   346 : str  x26, [sp], #0x21    ; fa 87 00 f9  
   347 : mov  x25, #0x05          ; b9 00 80 d2  
   348 : mul  x25, x25, x2        ; 39 7f 02 9b  
   349 : mov  x24, #0x04          ; 98 00 80 d2  
   350 : add  x24, x25, x24       ; 38 03 18 8b  
   351 : mov  x25, #0x04          ; 99 00 80 d2  
   352 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   353 : str  x26, [sp], #0x22    ; fa 8b 00 f9  
   354 : mov  x25, #0x05          ; b9 00 80 d2  
   355 : mul  x25, x25, x2        ; 39 7f 02 9b  
   356 : mov  x24, #0x05          ; b8 00 80 d2  
   357 : add  x24, x25, x24       ; 38 03 18 8b  
   358 : mov  x25, #0x04          ; 99 00 80 d2  
   359 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   360 : str  x26, [sp], #0x23    ; fa 8f 00 f9  
   361 : mov  x25, #0x05          ; b9 00 80 d2  
   362 : mul  x25, x25, x2        ; 39 7f 02 9b  
   363 : mov  x24, #0x06          ; d8 00 80 d2  
   364 : add  x24, x25, x24       ; 38 03 18 8b  
   365 : mov  x25, #0x04          ; 99 00 80 d2  
   366 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   367 : str  x26, [sp], #0x24    ; fa 93 00 f9  
   368 : mov  x25, #0x05          ; b9 00 80 d2  
   369 : mul  x25, x25, x2        ; 39 7f 02 9b  
   370 : mov  x24, #0x07          ; f8 00 80 d2  
   371 : add  x24, x25, x24       ; 38 03 18 8b  
   372 : mov  x25, #0x04          ; 99 00 80 d2  
   373 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   374 : str  x26, [sp], #0x25    ; fa 97 00 f9  
   375 : mov  x25, #0x05          ; b9 00 80 d2  
   376 : mul  x25, x25, x2        ; 39 7f 02 9b  
   377 : mov  x24, #0x08          ; 18 01 80 d2  
   378 : add  x24, x25, x24       ; 38 03 18 8b  
   379 : mov  x25, #0x04          ; 99 00 80 d2  
   380 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   381 : str  x26, [sp], #0x26    ; fa 9b 00 f9  
   382 : mov  x25, #0x06          ; d9 00 80 d2  
   383 : mul  x25, x25, x2        ; 39 7f 02 9b  
   384 : mov  x24, #0             ; 18 00 80 d2  
   385 : add  x24, x25, x24       ; 38 03 18 8b  
   386 : mov  x25, #0x04          ; 99 00 80 d2  
   387 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   388 : str  x26, [sp], #0x27    ; fa 9f 00 f9  
   389 : mov  x25, #0x06          ; d9 00 80 d2  
   390 : mul  x25, x25, x2        ; 39 7f 02 9b  
   391 : mov  x24, #0x01          ; 38 00 80 d2  
   392 : add  x24, x25, x24       ; 38 03 18 8b  
   393 : mov  x25, #0x04          ; 99 00 80 d2  
   394 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   395 : str  x26, [sp], #0x28    ; fa a3 00 f9  
   396 : mov  x25, #0x06          ; d9 00 80 d2  
   397 : mul  x25, x25, x2        ; 39 7f 02 9b  
   398 : mov  x24, #0x02          ; 58 00 80 d2  
   399 : add  x24, x25, x24       ; 38 03 18 8b  
   400 : mov  x25, #0x04          ; 99 00 80 d2  
   401 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   402 : str  x26, [sp], #0x29    ; fa a7 00 f9  
   403 : mov  x25, #0x06          ; d9 00 80 d2  
   404 : mul  x25, x25, x2        ; 39 7f 02 9b  
   405 : mov  x24, #0x03          ; 78 00 80 d2  
   406 : add  x24, x25, x24       ; 38 03 18 8b  
   407 : mov  x25, #0x04          ; 99 00 80 d2  
   408 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   409 : str  x26, [sp], #0x2a    ; fa ab 00 f9  
   410 : mov  x25, #0x06          ; d9 00 80 d2  
   411 : mul  x25, x25, x2        ; 39 7f 02 9b  
   412 : mov  x24, #0x04          ; 98 00 80 d2  
   413 : add  x24, x25, x24       ; 38 03 18 8b  
   414 : mov  x25, #0x04          ; 99 00 80 d2  
   415 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   416 : str  x26, [sp], #0x2b    ; fa af 00 f9  
   417 : mov  x25, #0x06          ; d9 00 80 d2  
   418 : mul  x25, x25, x2        ; 39 7f 02 9b  
   419 : mov  x24, #0x05          ; b8 00 80 d2  
   420 : add  x24, x25, x24       ; 38 03 18 8b  
   421 : mov  x25, #0x04          ; 99 00 80 d2  
   422 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   423 : str  x26, [sp], #0x2c    ; fa b3 00 f9  
   424 : mov  x25, #0x06          ; d9 00 80 d2  
   425 : mul  x25, x25, x2        ; 39 7f 02 9b  
   426 : mov  x24, #0x06          ; d8 00 80 d2  
   427 : add  x24, x25, x24       ; 38 03 18 8b  
   428 : mov  x25, #0x04          ; 99 00 80 d2  
   429 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   430 : str  x26, [sp], #0x2d    ; fa b7 00 f9  
   431 : mov  x25, #0x06          ; d9 00 80 d2  
   432 : mul  x25, x25, x2        ; 39 7f 02 9b  
   433 : mov  x24, #0x07          ; f8 00 80 d2  
   434 : add  x24, x25, x24       ; 38 03 18 8b  
   435 : mov  x25, #0x04          ; 99 00 80 d2  
   436 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   437 : str  x26, [sp], #0x2e    ; fa bb 00 f9  
   438 : mov  x25, #0x06          ; d9 00 80 d2  
   439 : mul  x25, x25, x2        ; 39 7f 02 9b  
   440 : mov  x24, #0x08          ; 18 01 80 d2  
   441 : add  x24, x25, x24       ; 38 03 18 8b  
   442 : mov  x25, #0x04          ; 99 00 80 d2  
   443 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   444 : str  x26, [sp], #0x2f    ; fa bf 00 f9  
   445 : mov  x25, #0x07          ; f9 00 80 d2  
   446 : mul  x25, x25, x2        ; 39 7f 02 9b  
   447 : mov  x24, #0             ; 18 00 80 d2  
   448 : add  x24, x25, x24       ; 38 03 18 8b  
   449 : mov  x25, #0x04          ; 99 00 80 d2  
   450 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   451 : str  x26, [sp], #0x30    ; fa c3 00 f9  
   452 : mov  x25, #0x07          ; f9 00 80 d2  
   453 : mul  x25, x25, x2        ; 39 7f 02 9b  
   454 : mov  x24, #0x01          ; 38 00 80 d2  
   455 : add  x24, x25, x24       ; 38 03 18 8b  
   456 : mov  x25, #0x04          ; 99 00 80 d2  
   457 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   458 : str  x26, [sp], #0x31    ; fa c7 00 f9  
   459 : mov  x25, #0x07          ; f9 00 80 d2  
   460 : mul  x25, x25, x2        ; 39 7f 02 9b  
   461 : mov  x24, #0x02          ; 58 00 80 d2  
   462 : add  x24, x25, x24       ; 38 03 18 8b  
   463 : mov  x25, #0x04          ; 99 00 80 d2  
   464 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   465 : str  x26, [sp], #0x32    ; fa cb 00 f9  
   466 : mov  x25, #0x07          ; f9 00 80 d2  
   467 : mul  x25, x25, x2        ; 39 7f 02 9b  
   468 : mov  x24, #0x03          ; 78 00 80 d2  
   469 : add  x24, x25, x24       ; 38 03 18 8b  
   470 : mov  x25, #0x04          ; 99 00 80 d2  
   471 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   472 : str  x26, [sp], #0x33    ; fa cf 00 f9  
   473 : mov  x25, #0x07          ; f9 00 80 d2  
   474 : mul  x25, x25, x2        ; 39 7f 02 9b  
   475 : mov  x24, #0x04          ; 98 00 80 d2  
   476 : add  x24, x25, x24       ; 38 03 18 8b  
   477 : mov  x25, #0x04          ; 99 00 80 d2  
   478 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   479 : str  x26, [sp], #0x34    ; fa d3 00 f9  
   480 : mov  x25, #0x07          ; f9 00 80 d2  
   481 : mul  x25, x25, x2        ; 39 7f 02 9b  
   482 : mov  x24, #0x05          ; b8 00 80 d2  
   483 : add  x24, x25, x24       ; 38 03 18 8b  
   484 : mov  x25, #0x04          ; 99 00 80 d2  
   485 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   486 : str  x26, [sp], #0x35    ; fa d7 00 f9  
   487 : mov  x25, #0x07          ; f9 00 80 d2  
   488 : mul  x25, x25, x2        ; 39 7f 02 9b  
   489 : mov  x24, #0x06          ; d8 00 80 d2  
   490 : add  x24, x25, x24       ; 38 03 18 8b  
   491 : mov  x25, #0x04          ; 99 00 80 d2  
   492 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   493 : str  x26, [sp], #0x36    ; fa db 00 f9  
   494 : mov  x25, #0x07          ; f9 00 80 d2  
   495 : mul  x25, x25, x2        ; 39 7f 02 9b  
   496 : mov  x24, #0x07          ; f8 00 80 d2  
   497 : add  x24, x25, x24       ; 38 03 18 8b  
   498 : mov  x25, #0x04          ; 99 00 80 d2  
   499 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   500 : str  x26, [sp], #0x37    ; fa df 00 f9  
   501 : mov  x25, #0x07          ; f9 00 80 d2  
   502 : mul  x25, x25, x2        ; 39 7f 02 9b  
   503 : mov  x24, #0x08          ; 18 01 80 d2  
   504 : add  x24, x25, x24       ; 38 03 18 8b  
   505 : mov  x25, #0x04          ; 99 00 80 d2  
   506 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   507 : str  x26, [sp], #0x38    ; fa e3 00 f9  
   508 : mov  x25, #0x08          ; 19 01 80 d2  
   509 : mul  x25, x25, x2        ; 39 7f 02 9b  
   510 : mov  x24, #0             ; 18 00 80 d2  
   511 : add  x24, x25, x24       ; 38 03 18 8b  
   512 : mov  x25, #0x04          ; 99 00 80 d2  
   513 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   514 : str  x26, [sp], #0x39    ; fa e7 00 f9  
   515 : mov  x25, #0x08          ; 19 01 80 d2  
   516 : mul  x25, x25, x2        ; 39 7f 02 9b  
   517 : mov  x24, #0x01          ; 38 00 80 d2  
   518 : add  x24, x25, x24       ; 38 03 18 8b  
   519 : mov  x25, #0x04          ; 99 00 80 d2  
   520 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   521 : str  x26, [sp], #0x3a    ; fa eb 00 f9  
   522 : mov  x25, #0x08          ; 19 01 80 d2  
   523 : mul  x25, x25, x2        ; 39 7f 02 9b  
   524 : mov  x24, #0x02          ; 58 00 80 d2  
   525 : add  x24, x25, x24       ; 38 03 18 8b  
   526 : mov  x25, #0x04          ; 99 00 80 d2  
   527 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   528 : str  x26, [sp], #0x3b    ; fa ef 00 f9  
   529 : mov  x25, #0x08          ; 19 01 80 d2  
   530 : mul  x25, x25, x2        ; 39 7f 02 9b  
   531 : mov  x24, #0x03          ; 78 00 80 d2  
   532 : add  x24, x25, x24       ; 38 03 18 8b  
   533 : mov  x25, #0x04          ; 99 00 80 d2  
   534 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   535 : str  x26, [sp], #0x3c    ; fa f3 00 f9  
   536 : mov  x25, #0x08          ; 19 01 80 d2  
   537 : mul  x25, x25, x2        ; 39 7f 02 9b  
   538 : mov  x24, #0x04          ; 98 00 80 d2  
   539 : add  x24, x25, x24       ; 38 03 18 8b  
   540 : mov  x25, #0x04          ; 99 00 80 d2  
   541 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   542 : str  x26, [sp], #0x3d    ; fa f7 00 f9  
   543 : mov  x25, #0x08          ; 19 01 80 d2  
   544 : mul  x25, x25, x2        ; 39 7f 02 9b  
   545 : mov  x24, #0x05          ; b8 00 80 d2  
   546 : add  x24, x25, x24       ; 38 03 18 8b  
   547 : mov  x25, #0x04          ; 99 00 80 d2  
   548 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   549 : str  x26, [sp], #0x3e    ; fa fb 00 f9  
   550 : mov  x25, #0x08          ; 19 01 80 d2  
   551 : mul  x25, x25, x2        ; 39 7f 02 9b  
   552 : mov  x24, #0x06          ; d8 00 80 d2  
   553 : add  x24, x25, x24       ; 38 03 18 8b  
   554 : mov  x25, #0x04          ; 99 00 80 d2  
   555 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   556 : str  x26, [sp], #0x3f    ; fa ff 00 f9  
   557 : mov  x25, #0x08          ; 19 01 80 d2  
   558 : mul  x25, x25, x2        ; 39 7f 02 9b  
   559 : mov  x24, #0x07          ; f8 00 80 d2  
   560 : add  x24, x25, x24       ; 38 03 18 8b  
   561 : mov  x25, #0x04          ; 99 00 80 d2  
   562 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   563 : str  x26, [sp], #0x40    ; fa 03 01 f9  
   564 : mov  x25, #0x08          ; 19 01 80 d2  
   565 : mul  x25, x25, x2        ; 39 7f 02 9b  
   566 : mov  x24, #0x08          ; 18 01 80 d2  
   567 : add  x24, x25, x24       ; 38 03 18 8b  
   568 : mov  x25, #0x04          ; 99 00 80 d2  
   569 : mul  x26, x24, x25       ; 1a 7f 19 9b  
   570 : str  x26, [sp], #0x41    ; fa 07 01 f9  
   571 : cmp  x7, x3              ; ff 00 03 eb  
   572 : b.ge [843]               ; ea 21 00 54  
   573 : mov  x25, #0             ; 19 00 80 d2  
   574 : mul  x24, x7, x6         ; f8 7c 06 9b  
   575 : mul  x24, x24, x5        ; 18 7f 05 9b  
   576 : mov  x23, #0x04          ; 97 00 80 d2  
   577 : mul  x23, x24, x23       ; 17 7f 17 9b  
   578 : add  x23, x4, x23        ; 97 00 17 8b  
   579 : mul  x24, x7, x2         ; f8 7c 02 9b  
   580 : mul  x24, x24, x1        ; 18 7f 01 9b  
   581 : mov  x22, #0x04          ; 96 00 80 d2  
   582 : mul  x22, x24, x22       ; 16 7f 16 9b  
   583 : add  x22, x0, x22        ; 16 00 16 8b  
   584 : cmp  x25, x5             ; 3f 03 05 eb  
   585 : b.ge [841]               ; 0a 20 00 54  
   586 : mov  x24, #0             ; 18 00 80 d2  
   587 : mov  x21, #0x10          ; 15 02 80 d2  
   588 : cmp  x24, x8             ; 1f 03 08 eb  
   589 : b.ge [833]               ; 8a 1e 00 54  
   590 : cmp  x21, x8             ; bf 02 08 eb  
   591 : b.le [595]               ; 8d 00 00 54  
   592 : sub  x20, x21, x8        ; b4 02 08 cb  
   593 : sub  x24, x24, x20       ; 18 03 14 cb  
   594 : sub  x21, x21, x20       ; b5 02 14 cb  
   595 : add  x20, x22, x24       ; d4 02 18 8b  
   596 : add  x19, x23, x24       ; f3 02 18 8b  
   597 : ldr  v0.4s, x20, x9      ; 80 6a e9 3c  
   598 : ldr  v1.4s, x20, x10     ; 81 6a ea 3c  
   599 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   600 : ldr  v1.4s, x20, x11     ; 81 6a eb 3c  
   601 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   602 : ldr  v1.4s, x20, x12     ; 81 6a ec 3c  
   603 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   604 : ldr  v1.4s, x20, x13     ; 81 6a ed 3c  
   605 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   606 : ldr  v1.4s, x20, x14     ; 81 6a ee 3c  
   607 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   608 : ldr  v1.4s, x20, x15     ; 81 6a ef 3c  
   609 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   610 : ldr  v1.4s, x20, x16     ; 81 6a f0 3c  
   611 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   612 : ldr  v1.4s, x20, x17     ; 81 6a f1 3c  
   613 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   614 : ldr  v1.4s, x20, x18     ; 81 6a f2 3c  
   615 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   616 : ldr  x26, [sp], #0x46    ; fa 1b 41 f9  
   617 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   618 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   619 : ldr  x26, [sp], #0x45    ; fa 17 41 f9  
   620 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   621 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   622 : ldr  x26, [sp], #0x44    ; fa 13 41 f9  
   623 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   624 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   625 : ldr  x26, [sp], #0x43    ; fa 0f 41 f9  
   626 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   627 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   628 : ldr  x26, [sp], #0x42    ; fa 0b 41 f9  
   629 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   630 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   631 : ldr  x26, [sp], #0       ; fa 03 40 f9  
   632 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   633 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   634 : ldr  x26, [sp], #0x01    ; fa 07 40 f9  
   635 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   636 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   637 : ldr  x26, [sp], #0x02    ; fa 0b 40 f9  
   638 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   639 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   640 : ldr  x26, [sp], #0x03    ; fa 0f 40 f9  
   641 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   642 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   643 : ldr  x26, [sp], #0x04    ; fa 13 40 f9  
   644 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   645 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   646 : ldr  x26, [sp], #0x05    ; fa 17 40 f9  
   647 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   648 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   649 : ldr  x26, [sp], #0x06    ; fa 1b 40 f9  
   650 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   651 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   652 : ldr  x26, [sp], #0x07    ; fa 1f 40 f9  
   653 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   654 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   655 : ldr  x26, [sp], #0x08    ; fa 23 40 f9  
   656 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   657 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   658 : ldr  x26, [sp], #0x09    ; fa 27 40 f9  
   659 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   660 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   661 : ldr  x26, [sp], #0x0a    ; fa 2b 40 f9  
   662 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   663 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   664 : ldr  x26, [sp], #0x0b    ; fa 2f 40 f9  
   665 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   666 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   667 : ldr  x26, [sp], #0x0c    ; fa 33 40 f9  
   668 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   669 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   670 : ldr  x26, [sp], #0x0d    ; fa 37 40 f9  
   671 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   672 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   673 : ldr  x26, [sp], #0x0e    ; fa 3b 40 f9  
   674 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   675 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   676 : ldr  x26, [sp], #0x0f    ; fa 3f 40 f9  
   677 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   678 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   679 : ldr  x26, [sp], #0x10    ; fa 43 40 f9  
   680 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   681 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   682 : ldr  x26, [sp], #0x11    ; fa 47 40 f9  
   683 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   684 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   685 : ldr  x26, [sp], #0x12    ; fa 4b 40 f9  
   686 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   687 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   688 : ldr  x26, [sp], #0x13    ; fa 4f 40 f9  
   689 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   690 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   691 : ldr  x26, [sp], #0x14    ; fa 53 40 f9  
   692 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   693 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   694 : ldr  x26, [sp], #0x15    ; fa 57 40 f9  
   695 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   696 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   697 : ldr  x26, [sp], #0x16    ; fa 5b 40 f9  
   698 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   699 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   700 : ldr  x26, [sp], #0x17    ; fa 5f 40 f9  
   701 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   702 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   703 : ldr  x26, [sp], #0x18    ; fa 63 40 f9  
   704 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   705 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   706 : ldr  x26, [sp], #0x19    ; fa 67 40 f9  
   707 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   708 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   709 : ldr  x26, [sp], #0x1a    ; fa 6b 40 f9  
   710 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   711 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   712 : ldr  x26, [sp], #0x1b    ; fa 6f 40 f9  
   713 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   714 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   715 : ldr  x26, [sp], #0x1c    ; fa 73 40 f9  
   716 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   717 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   718 : ldr  x26, [sp], #0x1d    ; fa 77 40 f9  
   719 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   720 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   721 : ldr  x26, [sp], #0x1e    ; fa 7b 40 f9  
   722 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   723 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   724 : ldr  x26, [sp], #0x1f    ; fa 7f 40 f9  
   725 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   726 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   727 : ldr  x26, [sp], #0x20    ; fa 83 40 f9  
   728 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   729 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   730 : ldr  x26, [sp], #0x21    ; fa 87 40 f9  
   731 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   732 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   733 : ldr  x26, [sp], #0x22    ; fa 8b 40 f9  
   734 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   735 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   736 : ldr  x26, [sp], #0x23    ; fa 8f 40 f9  
   737 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   738 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   739 : ldr  x26, [sp], #0x24    ; fa 93 40 f9  
   740 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   741 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   742 : ldr  x26, [sp], #0x25    ; fa 97 40 f9  
   743 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   744 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   745 : ldr  x26, [sp], #0x26    ; fa 9b 40 f9  
   746 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   747 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   748 : ldr  x26, [sp], #0x27    ; fa 9f 40 f9  
   749 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   750 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   751 : ldr  x26, [sp], #0x28    ; fa a3 40 f9  
   752 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   753 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   754 : ldr  x26, [sp], #0x29    ; fa a7 40 f9  
   755 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   756 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   757 : ldr  x26, [sp], #0x2a    ; fa ab 40 f9  
   758 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   759 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   760 : ldr  x26, [sp], #0x2b    ; fa af 40 f9  
   761 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   762 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   763 : ldr  x26, [sp], #0x2c    ; fa b3 40 f9  
   764 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   765 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   766 : ldr  x26, [sp], #0x2d    ; fa b7 40 f9  
   767 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   768 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   769 : ldr  x26, [sp], #0x2e    ; fa bb 40 f9  
   770 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   771 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   772 : ldr  x26, [sp], #0x2f    ; fa bf 40 f9  
   773 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   774 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   775 : ldr  x26, [sp], #0x30    ; fa c3 40 f9  
   776 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   777 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   778 : ldr  x26, [sp], #0x31    ; fa c7 40 f9  
   779 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   780 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   781 : ldr  x26, [sp], #0x32    ; fa cb 40 f9  
   782 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   783 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   784 : ldr  x26, [sp], #0x33    ; fa cf 40 f9  
   785 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   786 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   787 : ldr  x26, [sp], #0x34    ; fa d3 40 f9  
   788 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   789 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   790 : ldr  x26, [sp], #0x35    ; fa d7 40 f9  
   791 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   792 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   793 : ldr  x26, [sp], #0x36    ; fa db 40 f9  
   794 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   795 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   796 : ldr  x26, [sp], #0x37    ; fa df 40 f9  
   797 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   798 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   799 : ldr  x26, [sp], #0x38    ; fa e3 40 f9  
   800 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   801 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   802 : ldr  x26, [sp], #0x39    ; fa e7 40 f9  
   803 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   804 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   805 : ldr  x26, [sp], #0x3a    ; fa eb 40 f9  
   806 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   807 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   808 : ldr  x26, [sp], #0x3b    ; fa ef 40 f9  
   809 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   810 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   811 : ldr  x26, [sp], #0x3c    ; fa f3 40 f9  
   812 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   813 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   814 : ldr  x26, [sp], #0x3d    ; fa f7 40 f9  
   815 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   816 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   817 : ldr  x26, [sp], #0x3e    ; fa fb 40 f9  
   818 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   819 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   820 : ldr  x26, [sp], #0x3f    ; fa ff 40 f9  
   821 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   822 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   823 : ldr  x26, [sp], #0x40    ; fa 03 41 f9  
   824 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   825 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   826 : ldr  x26, [sp], #0x41    ; fa 07 41 f9  
   827 : ldr  v1.4s, x20, x26     ; 81 6a fa 3c  
   828 : fmax v0.4s, v0.4s, v1.4s ; 00 f4 21 4e  
   829 : str  v0.4s, x19, #0      ; 60 02 80 3d  
   830 : mov  x24, x21            ; f8 03 15 aa  
   831 : add  x21, x21, #0x10     ; b5 42 00 91  
   832 : b    [588]               ; 0c ff ff 17  
   833 : add  x25, x25, #0x01     ; 39 07 00 91  
   834 : mov  x19, #0x04          ; 93 00 80 d2  
   835 : mul  x19, x6, x19        ; d3 7c 13 9b  
   836 : add  x23, x23, x19       ; f7 02 13 8b  
   837 : mov  x19, #0x04          ; 93 00 80 d2  
   838 : mul  x19, x2, x19        ; 53 7c 13 9b  
   839 : add  x22, x22, x19       ; d6 02 13 8b  
   840 : b    [584]               ; 00 ff ff 17  
   841 : add  x7, x7, #0x01       ; e7 04 00 91  
   842 : b    [571]               ; f1 fe ff 17  
   843 : mov  x0, #0              ; 00 00 80 d2  
   844 : ldr  x18, [sp], #0x47    ; f2 1f 41 f9  
   845 : ldr  x19, [sp], #0x48    ; f3 23 41 f9  
   846 : ldr  x20, [sp], #0x49    ; f4 27 41 f9  
   847 : ldr  x21, [sp], #0x4a    ; f5 2b 41 f9  
   848 : ldr  x22, [sp], #0x4b    ; f6 2f 41 f9  
   849 : ldr  x23, [sp], #0x4c    ; f7 33 41 f9  
   850 : ldr  x24, [sp], #0x4d    ; f8 37 41 f9  
   851 : ldr  x25, [sp], #0x4e    ; f9 3b 41 f9  
   852 : ldr  x26, [sp], #0x4f    ; fa 3f 41 f9  
   853 : add  sp, sp, #0x280      ; ff 03 0a 91  
   854 : ret  x30                 ; c0 03 5f d6  
