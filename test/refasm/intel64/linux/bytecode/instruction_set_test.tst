instruction_set_test()
     0 : mov        i0, 4563402751  
     1 : mov        i7, 4563402751  
     2 : mov        i8, 4563402751  
     3 : mov        i15, 4563402751 
     4 : mov        s32, i0         
     5 : mov        s32, i7         
     6 : mov        s32, i8         
     7 : mov        s32, i15        
     8 : mov        s32, 257        
     9 : add        i0, i0, 256     
    10 : add        i1, i1, 256     
    11 : add        i7, i7, 256     
    12 : add        i8, i8, 256     
    13 : add        i15, i15, 256   
    14 : add        i0, i0, s32     
    15 : add        i7, i7, s32     
    16 : add        i8, i8, s32     
    17 : add        i15, i15, s32   
    18 : add        s32, s32, i0    
    19 : add        s32, s32, i7    
    20 : add        s32, s32, i8    
    21 : add        s32, s32, i15   
    22 : add        s32, s32, 257   
    23 : sub        i0, i0, 256     
    24 : sub        i1, i1, 256     
    25 : sub        i7, i7, 256     
    26 : sub        i8, i8, 256     
    27 : sub        i15, i15, 256   
    28 : sub        i0, i0, s32     
    29 : sub        i7, i7, s32     
    30 : sub        i8, i8, s32     
    31 : sub        i15, i15, s32   
    32 : sub        s32, s32, i0    
    33 : sub        s32, s32, i7    
    34 : sub        s32, s32, i8    
    35 : sub        s32, s32, i15   
    36 : sub        s32, s32, 257   
    37 : mul        i0, i0, s32     
    38 : mul        i7, i7, s32     
    39 : mul        i8, i8, s32     
    40 : mul        i15, i15, s32   
    41 : div        i0, i0, s32     
    42 : neg        s32             
    43 : store.u64  i0, 256         
    44 : store.i64  i0, 256         
    45 : store.i64  i7, 256         
    46 : store.i64  i8, 256         
    47 : store.i64  i12, 256        
    48 : store.i64  i13, 256        
    49 : store.u32  i0, 256         
    50 : store.i32  i0, 256         
    51 : store.i32  i7, 256         
    52 : store.i32  i8, 256         
    53 : store.i32  i12, 256        
    54 : store.i32  i13, 256        
    55 : store.u16  i0, 256         
    56 : store.i16  i0, 256         
    57 : store.i16  i7, 256         
    58 : store.i16  i8, 256         
    59 : store.i16  i12, 256        
    60 : store.i16  i13, 256        
    61 : store.u8   i0, 255         
    62 : store.i8   i0, 255         
    63 : store.i8   i7, 255         
    64 : store.i8   i8, 255         
    65 : store.i8   i12, 255        
    66 : store.i8   i13, 255        
    67 : store.u64  i0, i0          
    68 : store.i64  i0, i0          
    69 : store.i64  i0, i7          
    70 : store.i64  i0, i8          
    71 : store.i64  i7, i0          
    72 : store.i64  i8, i0          
    73 : store.i64  i8, i8          
    74 : store.i64  i12, i0         
    75 : store.i64  i12, i7         
    76 : store.i64  i12, i8         
    77 : store.i64  i13, i0         
    78 : store.i64  i13, i7         
    79 : store.i64  i13, i8         
    80 : store.u16  i0, i0          
    81 : store.i16  i0, i0          
    82 : store.i16  i0, i7          
    83 : store.i16  i0, i8          
    84 : store.i16  i7, i0          
    85 : store.i16  i8, i0          
    86 : store.i16  i8, i8          
    87 : store.i16  i12, i0         
    88 : store.i16  i12, i7         
    89 : store.i16  i12, i8         
    90 : store.i16  i13, i0         
    91 : store.i16  i13, i7         
    92 : store.i16  i13, i8         
    93 : store.u8   i0, i0          
    94 : store.i8   i0, i0          
    95 : store.i8   i0, i7          
    96 : store.i8   i0, i8          
    97 : store.i8   i7, i0          
    98 : store.i8   i8, i0          
    99 : store.i8   i8, i8          
   100 : store.i8   i12, i0         
   101 : store.i8   i12, i7         
   102 : store.i8   i12, i8         
   103 : store.i8   i13, i0         
   104 : store.i8   i13, i7         
   105 : store.i8   i13, i8         
   106 : store.u64  i0, i0, i0      
   107 : store.i64  i0, i0, i0      
   108 : store.i64  i0, i0, i7      
   109 : store.i64  i0, i0, i8      
   110 : store.i64  i0, i7, i0      
   111 : store.i64  i0, i8, i0      
   112 : store.i64  i0, i8, i8      
   113 : store.i64  i7, i0, i0      
   114 : store.i64  i8, i0, i0      
   115 : store.i64  i8, i0, i8      
   116 : store.i64  i8, i8, i0      
   117 : store.i64  i8, i8, i8      
   118 : store.u32  i0, i0, i0      
   119 : store.i32  i0, i0, i0      
   120 : store.i32  i0, i0, i7      
   121 : store.i32  i0, i0, i8      
   122 : store.i32  i0, i7, i0      
   123 : store.i32  i0, i8, i0      
   124 : store.i32  i0, i8, i8      
   125 : store.i32  i7, i0, i0      
   126 : store.i32  i8, i0, i0      
   127 : store.i32  i8, i0, i8      
   128 : store.i32  i8, i8, i0      
   129 : store.i32  i8, i8, i8      
   130 : store.i32  i13, i0, i0     
   131 : store.i32  i13, i0, i8     
   132 : store.i32  i13, i8, i0     
   133 : store.i32  i13, i8, i8     
   134 : store.u16  i0, i0, i0      
   135 : store.i16  i0, i0, i0      
   136 : store.i16  i0, i0, i7      
   137 : store.i16  i0, i0, i8      
   138 : store.i16  i0, i7, i0      
   139 : store.i16  i0, i8, i0      
   140 : store.i16  i0, i8, i8      
   141 : store.i16  i7, i0, i0      
   142 : store.i16  i8, i0, i0      
   143 : store.i16  i8, i0, i8      
   144 : store.i16  i8, i8, i0      
   145 : store.i16  i8, i8, i8      
   146 : store.i16  i13, i0, i0     
   147 : store.i16  i13, i8, i0     
   148 : store.i16  i13, i0, i8     
   149 : store.i16  i13, i8, i8     
   150 : store.u8   i0, i0, i0      
   151 : store.i8   i0, i0, i0      
   152 : store.i8   i0, i0, i7      
   153 : store.i8   i0, i0, i8      
   154 : store.i8   i0, i7, i0      
   155 : store.i8   i0, i8, i0      
   156 : store.i8   i0, i8, i7      
   157 : store.i8   i0, i8, i8      
   158 : store.i8   i7, i0, i0      
   159 : store.i8   i8, i0, i0      
   160 : store.i8   i8, i0, i7      
   161 : store.i8   i8, i0, i8      
   162 : store.i8   i8, i8, i0      
   163 : store.i8   i8, i8, i8      
   164 : store.i8   i8, i8, i7      
   165 : store.i8   i13, i0, i0     
   166 : store.i8   i13, i0, i8     
   167 : store.i8   i13, i8, i0     
   168 : store.i8   i13, i8, i8     
   169 : store.u64  i0, 256, i0     
   170 : store.i64  i0, 256, i0     
   171 : store.i64  i0, 256, i7     
   172 : store.i64  i0, 256, i8     
   173 : store.i64  i7, 256, i0     
   174 : store.i64  i8, 256, i0     
   175 : store.i64  i8, 256, i8     
   176 : store.i64  i12, 256, i0    
   177 : store.i64  i12, 256, i7    
   178 : store.i64  i12, 256, i8    
   179 : store.u32  i0, 256, i0     
   180 : store.i32  i0, 256, i0     
   181 : store.i32  i0, 256, i7     
   182 : store.i32  i0, 256, i8     
   183 : store.i32  i7, 256, i0     
   184 : store.i32  i8, 256, i0     
   185 : store.i32  i8, 256, i8     
   186 : store.i32  i12, 256, i0    
   187 : store.i32  i12, 256, i8    
   188 : store.u16  i0, 256, i0     
   189 : store.i16  i0, 256, i0     
   190 : store.i16  i0, 256, i7     
   191 : store.i16  i0, 256, i8     
   192 : store.i16  i7, 256, i0     
   193 : store.i16  i8, 256, i0     
   194 : store.i16  i8, 256, i8     
   195 : store.i16  i12, 256, i0    
   196 : store.i16  i12, 256, i8    
   197 : store.u8   i0, 256, i0     
   198 : store.i8   i0, 256, i0     
   199 : store.i8   i0, 256, i7     
   200 : store.i8   i0, 256, i8     
   201 : store.i8   i7, 256, i0     
   202 : store.i8   i8, 256, i0     
   203 : store.i8   i8, 256, i8     
   204 : store.i8   i12, 256, i0    
   205 : store.i8   i12, 256, i8    
   206 : store.u64  i0, i0, 256     
   207 : store.i64  i0, i0, 256     
   208 : store.i64  i0, i7, 256     
   209 : store.i64  i0, i8, 256     
   210 : store.i64  i7, i0, 256     
   211 : store.i64  i8, i0, 256     
   212 : store.i64  i8, i8, 256     
   213 : store.i64  i13, i0, 256    
   214 : store.i64  i13, i8, 256    
   215 : store.u32  i0, i0, 256     
   216 : store.i32  i0, i0, 256     
   217 : store.i32  i0, i7, 256     
   218 : store.i32  i0, i8, 256     
   219 : store.i32  i7, i0, 256     
   220 : store.i32  i8, i0, 256     
   221 : store.i32  i8, i8, 256     
   222 : store.i32  i13, i0, 256    
   223 : store.i32  i13, i8, 256    
   224 : store.u16  i0, i0, 256     
   225 : store.i16  i0, i0, 256     
   226 : store.i16  i0, i7, 256     
   227 : store.i16  i0, i8, 256     
   228 : store.i16  i7, i0, 256     
   229 : store.i16  i8, i0, 256     
   230 : store.i16  i8, i8, 256     
   231 : store.i16  i13, i0, 256    
   232 : store.i16  i13, i8, 256    
   233 : store.u8   i0, i0, 255     
   234 : store.i8   i0, i0, 255     
   235 : store.i8   i0, i7, 255     
   236 : store.i8   i0, i8, 255     
   237 : store.i8   i7, i0, 255     
   238 : store.i8   i8, i0, 255     
   239 : store.i8   i8, i8, 255     
   240 : store.i8   i13, i0, 255    
   241 : store.i8   i13, i8, 255    
   242 : store.u64  i0, 257, 256    
   243 : store.i64  i0, 257, 256    
   244 : store.i64  i7, 257, 256    
   245 : store.i64  i8, 257, 256    
   246 : store.i64  i12, 257, 256   
   247 : store.u32  i0, 257, 256    
   248 : store.i32  i0, 257, 256    
   249 : store.i32  i7, 257, 256    
   250 : store.i32  i8, 257, 256    
   251 : store.i32  i12, 257, 256   
   252 : store.u16  i0, 257, 256    
   253 : store.i16  i0, 257, 256    
   254 : store.i16  i7, 257, 256    
   255 : store.i16  i8, 257, 256    
   256 : store.i16  i12, 257, 256   
   257 : store.u8   i0, 257, 255    
   258 : store.i8   i0, 257, 255    
   259 : store.i8   i7, 257, 255    
   260 : store.i8   i8, 257, 255    
   261 : store.i8   i12, 257, 255   
   262 : load.u64   i0, i0          
   263 : load.i64   i0, i0          
   264 : load.i64   i7, i0          
   265 : load.i64   i0, i7          
   266 : load.i64   i0, i0          
   267 : load.i64   i8, i0          
   268 : load.i64   i0, i8          
   269 : load.i64   i8, i8          
   270 : load.u32   i0, i0          
   271 : load.u32   i7, i0          
   272 : load.u32   i0, i7          
   273 : load.u32   i0, i0          
   274 : load.u32   i8, i0          
   275 : load.u32   i0, i8          
   276 : load.u32   i8, i8          
   277 : load.u16   i0, i0          
   278 : load.u16   i7, i0          
   279 : load.u16   i0, i7          
   280 : load.u16   i8, i0          
   281 : load.u16   i0, i8          
   282 : load.u16   i8, i8          
   283 : load.i16   i0, i0          
   284 : load.i16   i7, i0          
   285 : load.i16   i0, i7          
   286 : load.i16   i8, i0          
   287 : load.i16   i0, i8          
   288 : load.i16   i8, i8          
   289 : load.u8    i0, i0          
   290 : load.u8    i7, i0          
   291 : load.u8    i0, i7          
   292 : load.u8    i8, i0          
   293 : load.u8    i0, i8          
   294 : load.u8    i8, i8          
   295 : load.i8    i0, i0          
   296 : load.i8    i7, i0          
   297 : load.i8    i0, i7          
   298 : load.i8    i8, i0          
   299 : load.i8    i0, i8          
   300 : load.i8    i8, i8          
   301 : load.u64   i0, i0, 256     
   302 : load.i64   i0, i0, 256     
   303 : load.i64   i7, i0, 256     
   304 : load.i64   i0, i7, 256     
   305 : load.i64   i0, i0, 256     
   306 : load.i64   i8, i0, 256     
   307 : load.i64   i0, i8, 256     
   308 : load.i64   i8, i8, 256     
   309 : load.u32   i0, i0, 256     
   310 : load.u32   i7, i0, 256     
   311 : load.u32   i0, i7, 256     
   312 : load.u32   i8, i0, 256     
   313 : load.u32   i0, i8, 256     
   314 : load.u32   i8, i8, 256     
   315 : load.i32   i0, i0, 256     
   316 : load.i32   i7, i0, 256     
   317 : load.i32   i0, i7, 256     
   318 : load.i32   i8, i0, 256     
   319 : load.i32   i0, i8, 256     
   320 : load.i32   i8, i8, 256     
   321 : load.u16   i0, i0, 256     
   322 : load.u16   i7, i0, 256     
   323 : load.u16   i0, i7, 256     
   324 : load.u16   i8, i0, 256     
   325 : load.u16   i0, i8, 256     
   326 : load.u16   i8, i8, 256     
   327 : load.i16   i0, i0, 256     
   328 : load.i16   i7, i0, 256     
   329 : load.i16   i0, i7, 256     
   330 : load.i16   i8, i0, 256     
   331 : load.i16   i0, i8, 256     
   332 : load.i16   i8, i8, 256     
   333 : load.u8    i0, i0, 256     
   334 : load.u8    i7, i0, 256     
   335 : load.u8    i0, i7, 256     
   336 : load.u8    i8, i0, 256     
   337 : load.u8    i0, i8, 256     
   338 : load.u8    i8, i8, 256     
   339 : load.i8    i0, i0, 256     
   340 : load.i8    i7, i0, 256     
   341 : load.i8    i0, i7, 256     
   342 : load.i8    i8, i0, 256     
   343 : load.i8    i0, i8, 256     
   344 : load.i8    i8, i8, 256     
   345 : load.u64   i0, i0, i0      
   346 : load.i64   i0, i0, i0      
   347 : load.i64   i7, i0, i0      
   348 : load.i64   i0, i7, i0      
   349 : load.i64   i0, i0, i7      
   350 : load.i64   i0, i0, i0      
   351 : load.i64   i8, i0, i0      
   352 : load.i64   i0, i8, i0      
   353 : load.i64   i8, i8, i0      
   354 : load.i64   i0, i0, i8      
   355 : load.i64   i8, i0, i8      
   356 : load.i64   i0, i8, i8      
   357 : load.i64   i8, i8, i8      
   358 : load.u32   i0, i0, i0      
   359 : load.u32   i7, i0, i0      
   360 : load.u32   i0, i7, i0      
   361 : load.u32   i0, i0, i7      
   362 : load.u32   i8, i0, i0      
   363 : load.u32   i0, i8, i0      
   364 : load.u32   i8, i8, i0      
   365 : load.u32   i0, i0, i8      
   366 : load.u32   i8, i0, i8      
   367 : load.u32   i0, i8, i8      
   368 : load.u32   i8, i8, i8      
   369 : load.u16   i0, i0, i0      
   370 : load.u16   i7, i0, i0      
   371 : load.u16   i0, i7, i0      
   372 : load.u16   i0, i0, i7      
   373 : load.u16   i8, i0, i0      
   374 : load.u16   i0, i8, i0      
   375 : load.u16   i8, i8, i0      
   376 : load.u16   i0, i0, i8      
   377 : load.u16   i8, i0, i8      
   378 : load.u16   i0, i8, i8      
   379 : load.u16   i8, i8, i8      
   380 : load.i16   i0, i0, i0      
   381 : load.i16   i7, i0, i0      
   382 : load.i16   i0, i7, i0      
   383 : load.i16   i0, i0, i7      
   384 : load.i16   i8, i0, i0      
   385 : load.i16   i0, i8, i0      
   386 : load.i16   i8, i8, i0      
   387 : load.i16   i0, i0, i8      
   388 : load.i16   i8, i0, i8      
   389 : load.i16   i0, i8, i8      
   390 : load.i16   i8, i8, i8      
   391 : load.u8    i0, i0, i0      
   392 : load.u8    i7, i0, i0      
   393 : load.u8    i0, i7, i0      
   394 : load.u8    i0, i0, i7      
   395 : load.u8    i8, i0, i0      
   396 : load.u8    i0, i8, i0      
   397 : load.u8    i8, i8, i0      
   398 : load.u8    i0, i0, i8      
   399 : load.u8    i8, i0, i8      
   400 : load.u8    i0, i8, i8      
   401 : load.u8    i8, i8, i8      
   402 : load.i8    i0, i0, i0      
   403 : load.i8    i7, i0, i0      
   404 : load.i8    i0, i7, i0      
   405 : load.i8    i0, i0, i7      
   406 : load.i8    i8, i0, i0      
   407 : load.i8    i0, i8, i0      
   408 : load.i8    i8, i8, i0      
   409 : load.i8    i0, i0, i8      
   410 : load.i8    i8, i0, i8      
   411 : load.i8    i0, i8, i8      
   412 : load.i8    i8, i8, i8      
   413 : xchg       i0, i0          
   414 : xchg       i0, i7          
   415 : xchg       i0, i8          
   416 : xchg       i1, i1          
   417 : xchg       i7, i1          
   418 : xchg       i1, i7          
   419 : xchg       i1, i1          
   420 : xchg       i8, i1          
   421 : xchg       i1, i8          
   422 : xchg       i8, i8          
   423 : xchg       i1, s32         
   424 : xchg       i7, s32         
   425 : xchg       i8, s32         
   426 : xchg       i15, s32        
   427 : xchg       s32, i1         
   428 : xchg       s32, i7         
   429 : xchg       s32, i8         
   430 : xchg       s32, i15        
   431 : shl        i0, i0, 15      
   432 : shl        i7, i7, 15      
   433 : shl        i8, i8, 15      
   434 : shl        s32, s32, 15    
   435 : shl        i0, i0, i1      
   436 : shl        i7, i7, i1      
   437 : shl        i8, i8, i1      
   438 : shl        s32, s32, i1    
   439 : shr        i0, i0, 15      
   440 : shr        i7, i7, 15      
   441 : shr        i8, i8, 15      
   442 : shr        s32, s32, 15    
   443 : shr        i0, i0, i1      
   444 : shr        i7, i7, i1      
   445 : shr        i8, i8, i1      
   446 : shr        s32, s32, i1    
   447 : sar        i0, i0, 15      
   448 : sar        i7, i7, 15      
   449 : sar        i8, i8, 15      
   450 : sar        s32, s32, 15    
   451 : sar        i0, i0, i1      
   452 : sar        i7, i7, i1      
   453 : sar        i8, i8, i1      
   454 : sar        s32, s32, i1    
   455 : and        i0, i0, i0      
   456 : and        i0, i0, i7      
   457 : and        i7, i7, i0      
   458 : and        i0, i0, i8      
   459 : and        i8, i8, i0      
   460 : and        i8, i8, i8      
   461 : and        i0, i0, s32     
   462 : and        i7, i7, s32     
   463 : and        i8, i8, s32     
   464 : and        i0, i0, 256     
   465 : and        i1, i1, 256     
   466 : and        i7, i7, 256     
   467 : and        i8, i8, 256     
   468 : and        s32, s32, i0    
   469 : and        s32, s32, i7    
   470 : and        s32, s32, i8    
   471 : and        s32, s32, 257   
   472 : or         i0, i0, i0      
   473 : or         i0, i0, i7      
   474 : or         i7, i7, i0      
   475 : or         i0, i0, i8      
   476 : or         i8, i8, i0      
   477 : or         s32, s32, i0    
   478 : or         s32, s32, i8    
   479 : or         i0, i0, s32     
   480 : or         i8, i8, s32     
   481 : or         i0, i0, 256     
   482 : or         i8, i8, 256     
   483 : or         s32, s32, 257   
   484 : xor        i0, i0, i0      
   485 : xor        i0, i0, i7      
   486 : xor        i7, i7, i0      
   487 : xor        i0, i0, i8      
   488 : xor        i8, i8, i0      
   489 : xor        i8, i8, i8      
   490 : xor        i0, i0, s32     
   491 : xor        i7, i7, s32     
   492 : xor        i8, i8, s32     
   493 : xor        i0, i0, 256     
   494 : xor        i1, i1, 256     
   495 : xor        i7, i7, 256     
   496 : xor        i8, i8, 256     
   497 : xor        s32, s32, i0    
   498 : xor        s32, s32, i7    
   499 : xor        s32, s32, i8    
   500 : xor        s32, s32, 257   
   501 : select_eq  i0, i0, i0      
   502 : select_ne  i0, i0, i0      
   503 : select_gt  i0, i0, i0      
   504 : select_gt  i0, i0, i0      
   505 : select_le  i0, i0, i0      
   506 : select_ge  i0, i0, i0      
   507 : select_s   i0, i0, i0      
   508 : select_ns  i0, i0, i0      
   509 : select_eq  i7, i0, i7      
   510 : select_eq  i0, i7, i0      
   511 : select_eq  i8, i0, i8      
   512 : select_eq  i0, i8, i0      
   513 : select_eq  i8, i8, i8      
   514 : select_eq  i0, s32, i0     
   515 : select_eq  i7, s32, i7     
   516 : select_eq  i8, s32, i8     
   517 : iverson_eq i0              
   518 : iverson_ne i0              
   519 : iverson_gt i0              
   520 : iverson_gt i0              
   521 : iverson_le i0              
   522 : iverson_ge i0              
   523 : iverson_s  i0              
   524 : iverson_ns i0              
   525 : iverson_eq i7              
   526 : iverson_eq i8              
   527 : iverson_eq s32             
   528 : x86_adc    i0, i0, i0      
   529 : x86_adc    i7, i7, i0      
   530 : x86_adc    i0, i0, i7      
   531 : x86_adc    i8, i8, i0      
   532 : x86_adc    i0, i0, i8      
   533 : x86_adc    i8, i8, i8      
   534 : x86_adc    i0, i0, 256     
   535 : x86_adc    i1, i1, 256     
   536 : x86_adc    i7, i7, 256     
   537 : x86_adc    i8, i8, 256     
   538 : x86_adc    i0, i0, s32     
   539 : x86_adc    i7, i7, s32     
   540 : x86_adc    i8, i8, s32     
   541 : x86_adc    s32, s32, i0    
   542 : x86_adc    s32, s32, i7    
   543 : x86_adc    s32, s32, i8    
   544 : x86_adc    s32, s32, 256   
   545 : cmp        i0, s8191       
   546 : cmp        i7, s8191       
   547 : cmp        i8, s8191       
   548 : cmp        i15, s8191      
   549 : cmp        s8191, i0       
   550 : cmp        s8191, i7       
   551 : cmp        s8191, i8       
   552 : cmp        s8191, i15      
   553 : cmp        s8191, 34952    
   554 : call_noret [i0]()          
   555 : call_noret [i7]()          
   556 : call_noret [i8]()          
   557 : call_noret [i15]()         
   558 : call_noret [s32]()         
