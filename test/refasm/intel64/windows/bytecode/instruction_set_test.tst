instruction_set_test()
     0 : store.u64 6, i0, 256          
     1 : store.i64 7, i0, 256          
     2 : store.i64 7, i7, 256          
     3 : store.i64 7, i8, 256          
     4 : store.i64 7, i12, 256         
     5 : store.i64 7, i13, 256         
     6 : store.u32 4, i0, 256          
     7 : store.i32 5, i0, 256          
     8 : store.i32 5, i7, 256          
     9 : store.i32 5, i8, 256          
    10 : store.i32 5, i12, 256         
    11 : store.i32 5, i13, 256         
    12 : store.u16 2, i0, 256          
    13 : store.i16 3, i0, 256          
    14 : store.i16 3, i7, 256          
    15 : store.i16 3, i8, 256          
    16 : store.i16 3, i12, 256         
    17 : store.i16 3, i13, 256         
    18 : store.u8  0, i0, 255          
    19 : store.i8  1, i0, 255          
    20 : store.i8  1, i7, 255          
    21 : store.i8  1, i8, 255          
    22 : store.i8  1, i12, 255         
    23 : store.i8  1, i13, 255         
    24 : store.u64 6, i0, i0           
    25 : store.i64 7, i0, i0           
    26 : store.i64 7, i7, i0           
    27 : store.i64 7, i0, i7           
    28 : store.i64 7, i8, i0           
    29 : store.i64 7, i0, i8           
    30 : store.i64 7, i8, i8           
    31 : store.i64 7, i12, i0          
    32 : store.i64 7, i13, i0          
    33 : store.i64 7, i12, i8          
    34 : store.i64 7, i13, i8          
    35 : store.u16 2, i0, i0           
    36 : store.i16 3, i0, i0           
    37 : store.i16 3, i7, i0           
    38 : store.i16 3, i0, i7           
    39 : store.i16 3, i0, i0           
    40 : store.i16 3, i8, i0           
    41 : store.i16 3, i0, i8           
    42 : store.i16 3, i8, i8           
    43 : store.i16 3, i8, i0           
    44 : store.i16 3, i12, i0          
    45 : store.i16 3, i12, i7          
    46 : store.i16 3, i13, i0          
    47 : store.i16 3, i13, i7          
    48 : store.i16 3, i13, i0          
    49 : store.i16 3, i8, i8           
    50 : store.i16 3, i12, i8          
    51 : store.i16 3, i13, i8          
    52 : store.u8  0, i0, i0           
    53 : store.i8  1, i0, i0           
    54 : store.i8  1, i7, i0           
    55 : store.i8  1, i0, i7           
    56 : store.i8  1, i0, i0           
    57 : store.i8  1, i8, i0           
    58 : store.i8  1, i0, i8           
    59 : store.i8  1, i8, i8           
    60 : store.i8  1, i12, i0          
    61 : store.i8  1, i12, i7          
    62 : store.i8  1, i12, i8          
    63 : store.i8  1, i13, i0          
    64 : store.i8  1, i13, i7          
    65 : store.i8  1, i13, i8          
    66 : store.u64 6, i0, i0, i0       
    67 : store.i64 7, i0, i0, i0       
    68 : store.i64 7, i7, i0, i0       
    69 : store.i64 7, i0, i7, i0       
    70 : store.i64 7, i0, i0, i7       
    71 : store.i64 7, i0, i0, i0       
    72 : store.i64 7, i8, i0, i0       
    73 : store.i64 7, i0, i8, i0       
    74 : store.i64 7, i8, i8, i0       
    75 : store.i64 7, i0, i0, i8       
    76 : store.i64 7, i8, i0, i8       
    77 : store.i64 7, i0, i8, i8       
    78 : store.i64 7, i8, i8, i8       
    79 : store.u32 4, i0, i0, i0       
    80 : store.i32 5, i0, i0, i0       
    81 : store.i32 5, i7, i0, i0       
    82 : store.i32 5, i0, i7, i0       
    83 : store.i32 5, i0, i0, i7       
    84 : store.i32 5, i0, i0, i0       
    85 : store.i32 5, i8, i0, i0       
    86 : store.i32 5, i0, i8, i0       
    87 : store.i32 5, i8, i8, i0       
    88 : store.i32 5, i0, i0, i8       
    89 : store.i32 5, i8, i0, i8       
    90 : store.i32 5, i0, i8, i8       
    91 : store.i32 5, i8, i8, i8       
    92 : store.i32 5, i13, i0, i0      
    93 : store.i32 5, i13, i8, i0      
    94 : store.i32 5, i13, i0, i8      
    95 : store.i32 5, i13, i8, i8      
    96 : store.u16 2, i0, i0, i0       
    97 : store.i16 3, i0, i0, i0       
    98 : store.i16 3, i7, i0, i0       
    99 : store.i16 3, i0, i7, i0       
   100 : store.i16 3, i0, i0, i7       
   101 : store.i16 3, i0, i0, i0       
   102 : store.i16 3, i8, i0, i0       
   103 : store.i16 3, i0, i8, i0       
   104 : store.i16 3, i8, i8, i0       
   105 : store.i16 3, i0, i0, i8       
   106 : store.i16 3, i8, i0, i8       
   107 : store.i16 3, i0, i8, i8       
   108 : store.i16 3, i8, i8, i8       
   109 : store.i16 3, i13, i0, i0      
   110 : store.i16 3, i13, i8, i0      
   111 : store.i16 3, i13, i0, i8      
   112 : store.i16 3, i13, i8, i8      
   113 : store.u8  0, i0, i0, i0       
   114 : store.i8  1, i0, i0, i0       
   115 : store.i8  1, i7, i0, i0       
   116 : store.i8  1, i0, i7, i0       
   117 : store.i8  1, i0, i0, i7       
   118 : store.i8  1, i0, i0, i0       
   119 : store.i8  1, i8, i0, i0       
   120 : store.i8  1, i0, i8, i0       
   121 : store.i8  1, i8, i8, i0       
   122 : store.i8  1, i0, i0, i8       
   123 : store.i8  1, i8, i0, i8       
   124 : store.i8  1, i0, i8, i8       
   125 : store.i8  1, i8, i8, i8       
   126 : store.i8  1, i13, i0, i0      
   127 : store.i8  1, i13, i8, i0      
   128 : store.i8  1, i13, i0, i8      
   129 : store.i8  1, i13, i8, i8      
   130 : store.i8  1, i0, i0, i7       
   131 : store.i8  1, i8, i0, i7       
   132 : store.i8  1, i0, i8, i7       
   133 : store.i8  1, i8, i8, i7       
   134 : store.u64 6, i0, 256, i0      
   135 : store.i64 7, i0, 256, i0      
   136 : store.i64 7, i7, 256, i0      
   137 : store.i64 7, i0, 256, i7      
   138 : store.i64 7, i0, 256, i0      
   139 : store.i64 7, i8, 256, i0      
   140 : store.i64 7, i0, 256, i8      
   141 : store.i64 7, i8, 256, i8      
   142 : store.i64 7, i12, 256, i0     
   143 : store.i64 7, i12, 256, i7     
   144 : store.i64 7, i12, 256, i8     
   145 : store.u32 4, i0, 256, i0      
   146 : store.i32 5, i0, 256, i0      
   147 : store.i32 5, i7, 256, i0      
   148 : store.i32 5, i0, 256, i7      
   149 : store.i32 5, i0, 256, i0      
   150 : store.i32 5, i8, 256, i0      
   151 : store.i32 5, i0, 256, i8      
   152 : store.i32 5, i8, 256, i8      
   153 : store.i32 5, i12, 256, i0     
   154 : store.i32 5, i12, 256, i8     
   155 : store.u16 2, i0, 256, i0      
   156 : store.i16 3, i0, 256, i0      
   157 : store.i16 3, i7, 256, i0      
   158 : store.i16 3, i0, 256, i7      
   159 : store.i16 3, i0, 256, i0      
   160 : store.i16 3, i8, 256, i0      
   161 : store.i16 3, i0, 256, i8      
   162 : store.i16 3, i8, 256, i8      
   163 : store.i16 3, i12, 256, i0     
   164 : store.i16 3, i12, 256, i8     
   165 : store.u8  0, i0, 256, i0      
   166 : store.i8  1, i0, 256, i0      
   167 : store.i8  1, i7, 256, i0      
   168 : store.i8  1, i0, 256, i7      
   169 : store.i8  1, i0, 256, i0      
   170 : store.i8  1, i8, 256, i0      
   171 : store.i8  1, i0, 256, i8      
   172 : store.i8  1, i8, 256, i8      
   173 : store.i8  1, i12, 256, i0     
   174 : store.i8  1, i12, 256, i8     
   175 : store.u64 6, i0, i0, 256      
   176 : store.i64 7, i0, i0, 256      
   177 : store.i64 7, i7, i0, 256      
   178 : store.i64 7, i0, i7, 256      
   179 : store.i64 7, i0, i0, 256      
   180 : store.i64 7, i8, i0, 256      
   181 : store.i64 7, i0, i8, 256      
   182 : store.i64 7, i8, i8, 256      
   183 : store.i64 7, i13, i0, 256     
   184 : store.i64 7, i13, i8, 256     
   185 : store.u32 4, i0, i0, 256      
   186 : store.i32 5, i0, i0, 256      
   187 : store.i32 5, i7, i0, 256      
   188 : store.i32 5, i0, i7, 256      
   189 : store.i32 5, i0, i0, 256      
   190 : store.i32 5, i8, i0, 256      
   191 : store.i32 5, i0, i8, 256      
   192 : store.i32 5, i8, i8, 256      
   193 : store.i32 5, i13, i0, 256     
   194 : store.i32 5, i13, i8, 256     
   195 : store.u16 2, i0, i0, 256      
   196 : store.i16 3, i0, i0, 256      
   197 : store.i16 3, i7, i0, 256      
   198 : store.i16 3, i0, i7, 256      
   199 : store.i16 3, i0, i0, 256      
   200 : store.i16 3, i8, i0, 256      
   201 : store.i16 3, i0, i8, 256      
   202 : store.i16 3, i8, i8, 256      
   203 : store.i16 3, i13, i0, 256     
   204 : store.i16 3, i13, i8, 256     
   205 : store.u8  0, i0, i0, 255      
   206 : store.i8  1, i0, i0, 255      
   207 : store.i8  1, i7, i0, 255      
   208 : store.i8  1, i0, i7, 255      
   209 : store.i8  1, i0, i0, 255      
   210 : store.i8  1, i8, i0, 255      
   211 : store.i8  1, i0, i8, 255      
   212 : store.i8  1, i8, i8, 255      
   213 : store.i8  1, i13, i0, 255     
   214 : store.i8  1, i13, i8, 255     
   215 : store.u64 6, i0, 257, 256     
   216 : store.i64 7, i0, 257, 256     
   217 : store.i64 7, i7, 257, 256     
   218 : store.i64 7, i0, 257, 256     
   219 : store.i64 7, i8, 257, 256     
   220 : store.i64 7, i12, 257, 256    
   221 : store.u32 4, i0, 257, 256     
   222 : store.i32 5, i0, 257, 256     
   223 : store.i32 5, i7, 257, 256     
   224 : store.i32 5, i8, 257, 256     
   225 : store.i32 5, i12, 257, 256    
   226 : store.u16 2, i0, 257, 256     
   227 : store.i16 3, i0, 257, 256     
   228 : store.i16 3, i7, 257, 256     
   229 : store.i16 3, i8, 257, 256     
   230 : store.i16 3, i12, 257, 256    
   231 : store.u8  0, i0, 257, 255     
   232 : store.i8  1, i0, 257, 255     
   233 : store.i8  1, i7, 257, 255     
   234 : store.i8  1, i8, 257, 255     
   235 : store.i8  1, i12, 257, 255    
   236 : load.u64  i0, 6, i0           
   237 : load.i64  i0, 7, i0           
   238 : load.i64  i7, 7, i0           
   239 : load.i64  i0, 7, i7           
   240 : load.i64  i0, 7, i0           
   241 : load.i64  i8, 7, i0           
   242 : load.i64  i0, 7, i8           
   243 : load.i64  i8, 7, i8           
   244 : load.u32  i0, 4, i0           
   245 : load.u32  i7, 4, i0           
   246 : load.u32  i0, 4, i7           
   247 : load.u32  i0, 4, i0           
   248 : load.u32  i8, 4, i0           
   249 : load.u32  i0, 4, i8           
   250 : load.u32  i8, 4, i8           
   251 : load.u16  i0, 2, i0           
   252 : load.u16  i7, 2, i0           
   253 : load.u16  i0, 2, i7           
   254 : load.u16  i8, 2, i0           
   255 : load.u16  i0, 2, i8           
   256 : load.u16  i8, 2, i8           
   257 : load.i16  i0, 3, i0           
   258 : load.i16  i7, 3, i0           
   259 : load.i16  i0, 3, i7           
   260 : load.i16  i8, 3, i0           
   261 : load.i16  i0, 3, i8           
   262 : load.i16  i8, 3, i8           
   263 : load.u8   i0, 0, i0           
   264 : load.u8   i7, 0, i0           
   265 : load.u8   i0, 0, i7           
   266 : load.u8   i8, 0, i0           
   267 : load.u8   i0, 0, i8           
   268 : load.u8   i8, 0, i8           
   269 : load.i8   i0, 1, i0           
   270 : load.i8   i7, 1, i0           
   271 : load.i8   i0, 1, i7           
   272 : load.i8   i8, 1, i0           
   273 : load.i8   i0, 1, i8           
   274 : load.i8   i8, 1, i8           
   275 : load.u64  i0, 6, i0, 256      
   276 : load.i64  i0, 7, i0, 256      
   277 : load.i64  i7, 7, i0, 256      
   278 : load.i64  i0, 7, i7, 256      
   279 : load.i64  i0, 7, i0, 256      
   280 : load.i64  i8, 7, i0, 256      
   281 : load.i64  i0, 7, i8, 256      
   282 : load.i64  i8, 7, i8, 256      
   283 : load.u32  i0, 4, i0, 256      
   284 : load.u32  i7, 4, i0, 256      
   285 : load.u32  i0, 4, i7, 256      
   286 : load.u32  i8, 4, i0, 256      
   287 : load.u32  i0, 4, i8, 256      
   288 : load.u32  i8, 4, i8, 256      
   289 : load.i32  i0, 5, i0, 256      
   290 : load.i32  i7, 5, i0, 256      
   291 : load.i32  i0, 5, i7, 256      
   292 : load.i32  i8, 5, i0, 256      
   293 : load.i32  i0, 5, i8, 256      
   294 : load.i32  i8, 5, i8, 256      
   295 : load.u16  i0, 2, i0, 256      
   296 : load.u16  i7, 2, i0, 256      
   297 : load.u16  i0, 2, i7, 256      
   298 : load.u16  i8, 2, i0, 256      
   299 : load.u16  i0, 2, i8, 256      
   300 : load.u16  i8, 2, i8, 256      
   301 : load.i16  i0, 3, i0, 256      
   302 : load.i16  i7, 3, i0, 256      
   303 : load.i16  i0, 3, i7, 256      
   304 : load.i16  i8, 3, i0, 256      
   305 : load.i16  i0, 3, i8, 256      
   306 : load.i16  i8, 3, i8, 256      
   307 : load.u8   i0, 0, i0, 256      
   308 : load.u8   i7, 0, i0, 256      
   309 : load.u8   i0, 0, i7, 256      
   310 : load.u8   i8, 0, i0, 256      
   311 : load.u8   i0, 0, i8, 256      
   312 : load.u8   i8, 0, i8, 256      
   313 : load.i8   i0, 1, i0, 256      
   314 : load.i8   i7, 1, i0, 256      
   315 : load.i8   i0, 1, i7, 256      
   316 : load.i8   i8, 1, i0, 256      
   317 : load.i8   i0, 1, i8, 256      
   318 : load.i8   i8, 1, i8, 256      
   319 : load.u64  i0, 6, i0, i0       
   320 : load.i64  i0, 7, i0, i0       
   321 : load.i64  i7, 7, i0, i0       
   322 : load.i64  i0, 7, i7, i0       
   323 : load.i64  i0, 7, i0, i7       
   324 : load.i64  i0, 7, i0, i0       
   325 : load.i64  i8, 7, i0, i0       
   326 : load.i64  i0, 7, i8, i0       
   327 : load.i64  i8, 7, i8, i0       
   328 : load.i64  i0, 7, i0, i8       
   329 : load.i64  i8, 7, i0, i8       
   330 : load.i64  i0, 7, i8, i8       
   331 : load.i64  i8, 7, i8, i8       
   332 : load.u32  i0, 4, i0, i0       
   333 : load.u32  i7, 4, i0, i0       
   334 : load.u32  i0, 4, i7, i0       
   335 : load.u32  i0, 4, i0, i7       
   336 : load.u32  i8, 4, i0, i0       
   337 : load.u32  i0, 4, i8, i0       
   338 : load.u32  i8, 4, i8, i0       
   339 : load.u32  i0, 4, i0, i8       
   340 : load.u32  i8, 4, i0, i8       
   341 : load.u32  i0, 4, i8, i8       
   342 : load.u32  i8, 4, i8, i8       
   343 : load.u16  i0, 2, i0, i0       
   344 : load.u16  i7, 2, i0, i0       
   345 : load.u16  i0, 2, i7, i0       
   346 : load.u16  i0, 2, i0, i7       
   347 : load.u16  i8, 2, i0, i0       
   348 : load.u16  i0, 2, i8, i0       
   349 : load.u16  i8, 2, i8, i0       
   350 : load.u16  i0, 2, i0, i8       
   351 : load.u16  i8, 2, i0, i8       
   352 : load.u16  i0, 2, i8, i8       
   353 : load.u16  i8, 2, i8, i8       
   354 : load.i16  i0, 3, i0, i0       
   355 : load.i16  i7, 3, i0, i0       
   356 : load.i16  i0, 3, i7, i0       
   357 : load.i16  i0, 3, i0, i7       
   358 : load.i16  i8, 3, i0, i0       
   359 : load.i16  i0, 3, i8, i0       
   360 : load.i16  i8, 3, i8, i0       
   361 : load.i16  i0, 3, i0, i8       
   362 : load.i16  i8, 3, i0, i8       
   363 : load.i16  i0, 3, i8, i8       
   364 : load.i16  i8, 3, i8, i8       
   365 : load.u8   i0, 0, i0, i0       
   366 : load.u8   i7, 0, i0, i0       
   367 : load.u8   i0, 0, i7, i0       
   368 : load.u8   i0, 0, i0, i7       
   369 : load.u8   i8, 0, i0, i0       
   370 : load.u8   i0, 0, i8, i0       
   371 : load.u8   i8, 0, i8, i0       
   372 : load.u8   i0, 0, i0, i8       
   373 : load.u8   i8, 0, i0, i8       
   374 : load.u8   i0, 0, i8, i8       
   375 : load.u8   i8, 0, i8, i8       
   376 : load.i8   i0, 1, i0, i0       
   377 : load.i8   i7, 1, i0, i0       
   378 : load.i8   i0, 1, i7, i0       
   379 : load.i8   i0, 1, i0, i7       
   380 : load.i8   i8, 1, i0, i0       
   381 : load.i8   i0, 1, i8, i0       
   382 : load.i8   i8, 1, i8, i0       
   383 : load.i8   i0, 1, i0, i8       
   384 : load.i8   i8, 1, i0, i8       
   385 : load.i8   i0, 1, i8, i8       
   386 : load.i8   i8, 1, i8, i8       
   387 : xchg      i0, i0              
   388 : xchg      i0, i7              
   389 : xchg      i0, i8              
   390 : xchg      i1, i1              
   391 : xchg      i7, i1              
   392 : xchg      i1, i7              
   393 : xchg      i1, i1              
   394 : xchg      i8, i1              
   395 : xchg      i1, i8              
   396 : xchg      i8, i8              
   397 : xchg      i1, s32             
   398 : xchg      i7, s32             
   399 : xchg      i8, s32             
   400 : xchg      i15, s32            
   401 : xchg      s32, i1             
   402 : xchg      s32, i7             
   403 : xchg      s32, i8             
   404 : xchg      s32, i15            
   405 : shl       i0, i0, 15          
   406 : shl       i7, i7, 15          
   407 : shl       i8, i8, 15          
   408 : shl       s32, s32, 15        
   409 : shl       i0, i0, i1          
   410 : shl       i7, i7, i1          
   411 : shl       i8, i8, i1          
   412 : shl       s32, s32, i1        
   413 : shr       i0, i0, 15          
   414 : shr       i7, i7, 15          
   415 : shr       i8, i8, 15          
   416 : shr       s32, s32, 15        
   417 : shr       i0, i0, i1          
   418 : shr       i7, i7, i1          
   419 : shr       i8, i8, i1          
   420 : shr       s32, s32, i1        
   421 : sar       i0, i0, 15          
   422 : sar       i7, i7, 15          
   423 : sar       i8, i8, 15          
   424 : sar       s32, s32, 15        
   425 : sar       i0, i0, i1          
   426 : sar       i7, i7, i1          
   427 : sar       i8, i8, i1          
   428 : sar       s32, s32, i1        
   429 : and       i0, i0, i0          
   430 : and       i0, i0, i7          
   431 : and       i7, i7, i0          
   432 : and       i0, i0, i8          
   433 : and       i8, i8, i0          
   434 : and       i8, i8, i8          
   435 : and       i0, i0, s32         
   436 : and       i7, i7, s32         
   437 : and       i8, i8, s32         
   438 : and       i0, i0, 256         
   439 : and       i1, i1, 256         
   440 : and       i7, i7, 256         
   441 : and       i8, i8, 256         
   442 : and       s32, s32, i0        
   443 : and       s32, s32, i7        
   444 : and       s32, s32, i8        
   445 : and       s32, s32, 257       
   446 : or        i0, i0, i0          
   447 : or        i0, i0, i7          
   448 : or        i7, i7, i0          
   449 : or        i0, i0, i8          
   450 : or        i8, i8, i0          
   451 : or        s32, s32, i0        
   452 : or        s32, s32, i8        
   453 : or        i0, i0, s32         
   454 : or        i8, i8, s32         
   455 : or        i0, i0, 256         
   456 : or        i8, i8, 256         
   457 : or        s32, s32, 257       
   458 : xor       i0, i0, i0          
   459 : xor       i0, i0, i7          
   460 : xor       i7, i7, i0          
   461 : xor       i0, i0, i8          
   462 : xor       i8, i8, i0          
   463 : xor       i8, i8, i8          
   464 : xor       i0, i0, s32         
   465 : xor       i7, i7, s32         
   466 : xor       i8, i8, s32         
   467 : xor       i0, i0, 256         
   468 : xor       i1, i1, 256         
   469 : xor       i7, i7, 256         
   470 : xor       i8, i8, 256         
   471 : xor       s32, s32, i0        
   472 : xor       s32, s32, i7        
   473 : xor       s32, s32, i8        
   474 : xor       s32, s32, 257       
   475 : select    i0, 0, i0, i0       
   476 : select    i0, 1, i0, i0       
   477 : select    i0, 2, i0, i0       
   478 : select    i0, 3, i0, i0       
   479 : select    i0, 5, i0, i0       
   480 : select    i0, 7, i0, i0       
   481 : select    i0, 8, i0, i0       
   482 : select    i0, 9, i0, i0       
   483 : select    i7, 0, i0, i7       
   484 : select    i0, 0, i7, i0       
   485 : select    i8, 0, i0, i8       
   486 : select    i0, 0, i8, i0       
   487 : select    i8, 0, i8, i8       
   488 : select    i0, 0, s32, i0      
   489 : select    i7, 0, s32, i7      
   490 : select    i8, 0, s32, i8      
   491 : x86_adc   i0, i0, i0          
   492 : x86_adc   i7, i7, i0          
   493 : x86_adc   i0, i0, i7          
   494 : x86_adc   i8, i8, i0          
   495 : x86_adc   i0, i0, i8          
   496 : x86_adc   i8, i8, i8          
   497 : x86_adc   i0, i0, 256         
   498 : x86_adc   i1, i1, 256         
   499 : x86_adc   i7, i7, 256         
   500 : x86_adc   i8, i8, 256         
   501 : x86_adc   i0, i0, s32         
   502 : x86_adc   i7, i7, s32         
   503 : x86_adc   i8, i8, s32         
   504 : x86_adc   s32, s32, i0        
   505 : x86_adc   s32, s32, i7        
   506 : x86_adc   s32, s32, i8        
   507 : x86_adc   s32, s32, 256       
   508 : cmp       i0, s8191           
   509 : cmp       i7, s8191           
   510 : cmp       i8, s8191           
   511 : cmp       i15, s8191          
   512 : cmp       s8191, i0           
   513 : cmp       s8191, i7           
   514 : cmp       s8191, i8           
   515 : cmp       s8191, i15          
   516 : cmp       s8191, 34952        
   517 : add       i0, i0, s8191       
   518 : add       i7, i7, s8191       
   519 : add       i8, i8, s8191       
   520 : add       i15, i15, s8191     
   521 : add       s8191, s8191, i0    
   522 : add       s8191, s8191, i7    
   523 : add       s8191, s8191, i8    
   524 : add       s8191, s8191, i15   
   525 : add       s8191, s8191, 34952 
