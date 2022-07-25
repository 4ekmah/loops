instruction_set_test()
     0 : store.u64      6, i0, 256, i0             
     1 : store.i64      7, i0, 256, i0             
     2 : store.u64      6, i0, i0, i0              
     3 : store.i64      7, i0, i0, i0              
     4 : store.u32      4, i0, 256, i0             
     5 : store.i32      5, i0, 256, i0             
     6 : store.u32      4, i0, i0, i0              
     7 : store.i32      5, i0, i0, i0              
     8 : store.u16      2, i0, 256, i0             
     9 : store.i16      3, i0, 256, i0             
    10 : store.u16      2, i0, i0, i0              
    11 : store.i16      3, i0, i0, i0              
    12 : store.u8       0, i0, 256, i0             
    13 : store.i8       1, i0, 256, i0             
    14 : store.u8       0, i0, i0, i0              
    15 : store.i8       1, i0, i0, i0              
    16 : load.u64       i0, 6, i0                  
    17 : load.u64       i0, 6, i0, 256             
    18 : load.u64       i15, 6, i0, 256            
    19 : load.u64       i0, 6, i15, 256            
    20 : load.u64       i0, 6, i0, i0              
    21 : load.u64       i15, 6, i0, i0             
    22 : load.u64       i0, 6, i15, i0             
    23 : load.u64       i0, 6, i0, i15             
    24 : load.i64       i0, 7, i0                  
    25 : load.i64       i0, 7, i0, 256             
    26 : load.i64       i15, 7, i0, 256            
    27 : load.i64       i0, 7, i15, 256            
    28 : load.i64       i0, 7, i0, i0              
    29 : load.i64       i15, 7, i0, i0             
    30 : load.i64       i0, 7, i15, i0             
    31 : load.i64       i0, 7, i0, i15             
    32 : load.u32       i0, 4, i0                  
    33 : load.u32       i0, 4, i0, 256             
    34 : load.u32       i15, 4, i0, 256            
    35 : load.u32       i0, 4, i15, 256            
    36 : load.u32       i0, 4, i0, i0              
    37 : load.u32       i15, 4, i0, i0             
    38 : load.u32       i0, 4, i15, i0             
    39 : load.u32       i0, 4, i0, i15             
    40 : load.i32       i0, 5, i0                  
    41 : load.i32       i0, 5, i0, 256             
    42 : load.i32       i15, 5, i0, 256            
    43 : load.i32       i0, 5, i15, 256            
    44 : load.i32       i0, 5, i0, i0              
    45 : load.i32       i15, 5, i0, i0             
    46 : load.i32       i0, 5, i15, i0             
    47 : load.i32       i0, 5, i0, i15             
    48 : load.u16       i0, 2, i0                  
    49 : load.u16       i0, 2, i0, 256             
    50 : load.u16       i15, 2, i0, 256            
    51 : load.u16       i0, 2, i15, 256            
    52 : load.u16       i0, 2, i0, i0              
    53 : load.u16       i15, 2, i0, i0             
    54 : load.u16       i0, 2, i15, i0             
    55 : load.u16       i0, 2, i0, i15             
    56 : load.i16       i0, 3, i0                  
    57 : load.i16       i0, 3, i0, 256             
    58 : load.i16       i15, 3, i0, 256            
    59 : load.i16       i0, 3, i15, 256            
    60 : load.i16       i0, 3, i0, i0              
    61 : load.i16       i15, 3, i0, i0             
    62 : load.i16       i0, 3, i15, i0             
    63 : load.i16       i0, 3, i0, i15             
    64 : load.u8        i0, 0, i0                  
    65 : load.u8        i0, 0, i0, 256             
    66 : load.u8        i15, 0, i0, 256            
    67 : load.u8        i0, 0, i15, 256            
    68 : load.u8        i0, 0, i0, i0              
    69 : load.u8        i15, 0, i0, i0             
    70 : load.u8        i0, 0, i15, i0             
    71 : load.u8        i0, 0, i0, i15             
    72 : load.i8        i0, 1, i0                  
    73 : load.i8        i0, 1, i0, 256             
    74 : load.i8        i15, 1, i0, 256            
    75 : load.i8        i0, 1, i15, 256            
    76 : load.i8        i0, 1, i0, i0              
    77 : load.i8        i15, 1, i0, i0             
    78 : load.i8        i0, 1, i15, i0             
    79 : load.i8        i0, 1, i0, i15             
    80 : select         i0, 0, i0, i0              
    81 : select         i0, 1, i0, i0              
    82 : select         i0, 2, i0, i0              
    83 : select         i0, 3, i0, i0              
    84 : select         i0, 4, i0, i0              
    85 : select         i0, 5, i0, i0              
    86 : select         i0, 6, i0, i0              
    87 : select         i0, 7, i0, i0              
    88 : select         i15, 0, i0, i0             
    89 : select         i0, 0, i15, i0             
    90 : select         i0, 0, i0, i15             
    91 : arm_cinc       i0, i0, 0                  
    92 : arm_cinc       i0, i0, 1                  
    93 : arm_cinc       i0, i0, 2                  
    94 : arm_cinc       i0, i0, 3                  
    95 : arm_cinc       i0, i0, 4                  
    96 : arm_cinc       i0, i0, 5                  
    97 : arm_cinc       i0, i0, 6                  
    98 : arm_cinc       i0, i0, 7                  
    99 : arm_cinc       i15, i0, 0                 
   100 : arm_cinc       i0, i15, 0                 
   101 : arm_cneg       i0, i0, 0                  
   102 : arm_cneg       i0, i0, 1                  
   103 : arm_cneg       i0, i0, 2                  
   104 : arm_cneg       i0, i0, 3                  
   105 : arm_cneg       i0, i0, 4                  
   106 : arm_cneg       i0, i0, 5                  
   107 : arm_cneg       i0, i0, 6                  
   108 : arm_cneg       i0, i0, 7                  
   109 : arm_cneg       i15, i0, 0                 
   110 : arm_cneg       i0, i15, 0                 
   111 : sar            i0, i0, 63                 
   112 : sar            i15, i0, 63                
   113 : sar            i0, i15, 63                
   114 : sar            i0, i0, i0                 
   115 : sar            i15, i0, i0                
   116 : sar            i0, i15, i0                
   117 : sar            i0, i0, i15                
   118 : shr            i0, i0, 63                 
   119 : shr            i15, i0, 63                
   120 : shr            i0, i15, 63                
   121 : shr            i0, i0, i0                 
   122 : shr            i15, i0, i0                
   123 : shr            i0, i15, i0                
   124 : shr            i0, i0, i15                
   125 : shl            i0, i0, 63                 
   126 : shl            i0, i0, 31                 
   127 : shl            i0, i0, 15                 
   128 : shl            i0, i0, 7                  
   129 : shl            i0, i0, 1                  
   130 : shl            i0, i0, 0                  
   131 : shl            i15, i0, 63                
   132 : shl            i0, i15, 63                
   133 : shl            i0, i0, i0                 
   134 : shl            i15, i0, i0                
   135 : shl            i0, i15, i0                
   136 : shl            i0, i0, i15                
   137 : and            i0, i0, i0                 
   138 : and            i15, i0, i0                
   139 : and            i0, i15, i0                
   140 : and            i0, i0, i15                
   141 : and            i0, i0, 255                
   142 : and            i0, i0, 65280              
   143 : and            i0, i0, -72057594037927936 
   144 : and            i15, i0, 255               
   145 : and            i0, i15, 255               
   146 : or             i0, i0, i0                 
   147 : or             i15, i0, i0                
   148 : or             i0, i15, i0                
   149 : or             i0, i0, i15                
   150 : or             i0, i0, 255                
   151 : or             i0, i0, 65280              
   152 : or             i0, i0, -72057594037927936 
   153 : or             i15, i0, 255               
   154 : or             i0, i15, 255               
   155 : xor            i0, i0, i0                 
   156 : xor            i15, i0, i0                
   157 : xor            i0, i15, i0                
   158 : xor            i0, i0, i15                
   159 : xor            i0, i0, 255                
   160 : xor            i0, i0, 65280              
   161 : xor            i0, i0, -72057594037927936 
   162 : xor            i15, i0, 255               
   163 : xor            i0, i15, 255               
   164 : not            i0, i0                     
   165 : not            i15, i0                    
   166 : not            i0, i15                    
   167 : neg            i0, i0                     
   168 : neg            i15, i0                    
   169 : neg            i0, i15                    
   170 : vld.u64        v0, i0                     
   171 : vld.u64        v0, i0, 32784              
   172 : vld.u64        v31, i0                    
   173 : vld.u64        v0, i15                    
   174 : vld.u64        v0, i0, i0                 
   175 : vld.u64        v31, i0, i0                
   176 : vld.u64        v0, i15, i0                
   177 : vld.u64        v0, i0, i15                
   178 : vld.u64        i0, v0                     
   179 : vld.u64        i0, 32784, v0              
   180 : vld.u64        i0, v31                    
   181 : vld.u64        i15, v0                    
   182 : vld.u64        i0, i0, v0                 
   183 : vld.u64        i0, i0, v31                
   184 : vld.u64        i15, i0, v0                
   185 : vld.u64        i0, i15, v0                
   186 : add.u8         v0, v0, v0                 
   187 : add.u8         v31, v0, v0                
   188 : add.u8         v0, v31, v0                
   189 : add.u8         v0, v0, v31                
   190 : add.u16        v0, v0, v0                 
   191 : add.u16        v31, v0, v0                
   192 : add.u16        v0, v31, v0                
   193 : add.u16        v0, v0, v31                
   194 : add.u32        v0, v0, v0                 
   195 : add.u32        v31, v0, v0                
   196 : add.u32        v0, v31, v0                
   197 : add.u32        v0, v0, v31                
   198 : add.u64        v0, v0, v0                 
   199 : add.u64        v31, v0, v0                
   200 : add.u64        v0, v31, v0                
   201 : add.u64        v0, v0, v31                
   202 : add.fp16       v0, v0, v0                 
   203 : add.fp16       v31, v0, v0                
   204 : add.fp16       v0, v31, v0                
   205 : add.fp16       v0, v0, v31                
   206 : add.fp32       v0, v0, v0                 
   207 : add.fp32       v31, v0, v0                
   208 : add.fp32       v0, v31, v0                
   209 : add.fp32       v0, v0, v31                
   210 : add.fp64       v0, v0, v0                 
   211 : add.fp64       v31, v0, v0                
   212 : add.fp64       v0, v31, v0                
   213 : add.fp64       v0, v0, v31                
   214 : sub.u8         v0, v0, v0                 
   215 : sub.u8         v31, v0, v0                
   216 : sub.u8         v0, v31, v0                
   217 : sub.u8         v0, v0, v31                
   218 : sub.u16        v0, v0, v0                 
   219 : sub.u16        v31, v0, v0                
   220 : sub.u16        v0, v31, v0                
   221 : sub.u16        v0, v0, v31                
   222 : sub.u32        v0, v0, v0                 
   223 : sub.u32        v31, v0, v0                
   224 : sub.u32        v0, v31, v0                
   225 : sub.u32        v0, v0, v31                
   226 : sub.u64        v0, v0, v0                 
   227 : sub.u64        v31, v0, v0                
   228 : sub.u64        v0, v31, v0                
   229 : sub.u64        v0, v0, v31                
   230 : sub.fp16       v0, v0, v0                 
   231 : sub.fp16       v31, v0, v0                
   232 : sub.fp16       v0, v31, v0                
   233 : sub.fp16       v0, v0, v31                
   234 : sub.fp32       v0, v0, v0                 
   235 : sub.fp32       v31, v0, v0                
   236 : sub.fp32       v0, v31, v0                
   237 : sub.fp32       v0, v0, v31                
   238 : sub.fp64       v0, v0, v0                 
   239 : sub.fp64       v31, v0, v0                
   240 : sub.fp64       v0, v31, v0                
   241 : sub.fp64       v0, v0, v31                
   242 : mul.u8         v0, v0, v0                 
   243 : mul.u8         v31, v0, v0                
   244 : mul.u8         v0, v31, v0                
   245 : mul.u8         v0, v0, v31                
   246 : mul.u16        v0, v0, v0                 
   247 : mul.u16        v31, v0, v0                
   248 : mul.u16        v0, v31, v0                
   249 : mul.u16        v0, v0, v31                
   250 : mul.u32        v0, v0, v0                 
   251 : mul.u32        v31, v0, v0                
   252 : mul.u32        v0, v31, v0                
   253 : mul.u32        v0, v0, v31                
   254 : mul.fp16       v0, v0, v0                 
   255 : mul.fp16       v31, v0, v0                
   256 : mul.fp16       v0, v31, v0                
   257 : mul.fp16       v0, v0, v31                
   258 : mul.fp32       v0, v0, v0                 
   259 : mul.fp32       v31, v0, v0                
   260 : mul.fp32       v0, v31, v0                
   261 : mul.fp32       v0, v0, v31                
   262 : mul.fp64       v0, v0, v0                 
   263 : mul.fp64       v31, v0, v0                
   264 : mul.fp64       v0, v31, v0                
   265 : mul.fp64       v0, v0, v31                
   266 : mul.fp32       v0, v0, v0                 
   267 : mul.fp32       v31, v0, v0                
   268 : mul.fp32       v0, v31, v0                
   269 : mul.fp32       v0, v0, v31                
   270 : mul.fp64       v0, v0, v0                 
   271 : mul.fp64       v31, v0, v0                
   272 : mul.fp64       v0, v31, v0                
   273 : mul.fp64       v0, v0, v31                
   274 : mul.fp16       v0, v0, v0                 
   275 : mul.fp16       v31, v0, v0                
   276 : mul.fp16       v0, v31, v0                
   277 : mul.fp16       v0, v0, v31                
   278 : sal.u8         v0, v0, 1                  
   279 : sal.u8         v31, v0, 1                 
   280 : sal.u8         v0, v31, 1                 
   281 : sal.u8         v0, v0, 7                  
   282 : sal.u16        v0, v0, 1                  
   283 : sal.u16        v31, v0, 1                 
   284 : sal.u16        v0, v31, 1                 
   285 : sal.u16        v0, v0, 15                 
   286 : sal.u32        v0, v0, 1                  
   287 : sal.u32        v31, v0, 1                 
   288 : sal.u32        v0, v31, 1                 
   289 : sal.u32        v0, v0, 31                 
   290 : sal.u64        v0, v0, 1                  
   291 : sal.u64        v31, v0, 1                 
   292 : sal.u64        v0, v31, 1                 
   293 : sal.u64        v0, v0, 63                 
   294 : sal.u8         v0, v0, v0                 
   295 : sal.u8         v31, v0, v0                
   296 : sal.u8         v0, v31, v0                
   297 : sal.u8         v0, v0, v31                
   298 : sal.u16        v0, v0, v0                 
   299 : sal.u16        v31, v0, v0                
   300 : sal.u16        v0, v31, v0                
   301 : sal.u16        v0, v0, v31                
   302 : sal.u32        v0, v0, v0                 
   303 : sal.u32        v31, v0, v0                
   304 : sal.u32        v0, v31, v0                
   305 : sal.u32        v0, v0, v31                
   306 : sal.u64        v0, v0, v0                 
   307 : sal.u64        v31, v0, v0                
   308 : sal.u64        v0, v31, v0                
   309 : sal.u64        v0, v0, v31                
   310 : sal.i8         v0, v0, v0                 
   311 : sal.i8         v31, v0, v0                
   312 : sal.i8         v0, v31, v0                
   313 : sal.i8         v0, v0, v31                
   314 : sal.i16        v0, v0, v0                 
   315 : sal.i16        v31, v0, v0                
   316 : sal.i16        v0, v31, v0                
   317 : sal.i16        v0, v0, v31                
   318 : sal.i32        v0, v0, v0                 
   319 : sal.i32        v31, v0, v0                
   320 : sal.i32        v0, v31, v0                
   321 : sal.i32        v0, v0, v31                
   322 : sal.i64        v0, v0, v0                 
   323 : sal.i64        v31, v0, v0                
   324 : sal.i64        v0, v31, v0                
   325 : sal.i64        v0, v0, v31                
   326 : sar.u8         v0, v0, 1                  
   327 : sar.u8         v31, v0, 1                 
   328 : sar.u8         v0, v31, 1                 
   329 : sar.u8         v0, v0, 7                  
   330 : sar.u16        v0, v0, 1                  
   331 : sar.u16        v31, v0, 1                 
   332 : sar.u16        v0, v31, 1                 
   333 : sar.u16        v0, v0, 15                 
   334 : sar.u32        v0, v0, 1                  
   335 : sar.u32        v31, v0, 1                 
   336 : sar.u32        v0, v31, 1                 
   337 : sar.u32        v0, v0, 31                 
   338 : sar.u64        v0, v0, 1                  
   339 : sar.u64        v31, v0, 1                 
   340 : sar.u64        v0, v31, 1                 
   341 : sar.u64        v0, v0, 63                 
   342 : sar.i8         v0, v0, 1                  
   343 : sar.i8         v31, v0, 1                 
   344 : sar.i8         v0, v31, 1                 
   345 : sar.i8         v0, v0, 7                  
   346 : sar.i16        v0, v0, 1                  
   347 : sar.i16        v31, v0, 1                 
   348 : sar.i16        v0, v31, 1                 
   349 : sar.i16        v0, v0, 15                 
   350 : sar.i32        v0, v0, 1                  
   351 : sar.i32        v31, v0, 1                 
   352 : sar.i32        v0, v31, 1                 
   353 : sar.i32        v0, v0, 31                 
   354 : sar.i64        v0, v0, 1                  
   355 : sar.i64        v31, v0, 1                 
   356 : sar.i64        v0, v31, 1                 
   357 : sar.i64        v0, v0, 63                 
   358 : and            v0, v0, v0                 
   359 : and            v31, v0, v0                
   360 : and            v0, v31, v0                
   361 : and            v0, v0, v31                
   362 : or             v0, v0, v0                 
   363 : or             v31, v0, v0                
   364 : or             v0, v31, v0                
   365 : or             v0, v0, v31                
   366 : xor            v0, v0, v0                 
   367 : xor            v31, v0, v0                
   368 : xor            v0, v31, v0                
   369 : xor            v0, v0, v31                
   370 : not            v0, v0                     
   371 : not            v31, v0                    
   372 : not            v0, v31                    
   373 : mov            v0, 0                      
   374 : mov            v31, 0                     
   375 : mov            v0, 255                    
   376 : mov            v0, 0                      
   377 : mov            v31, 0                     
   378 : mov            v0, 255                    
   379 : mov            v0, 0                      
   380 : mov            v31, 0                     
   381 : mov            v0, 255                    
   382 : min.fp16       v0, v0, v0                 
   383 : min.fp16       v31, v0, v0                
   384 : min.fp16       v0, v31, v0                
   385 : min.fp16       v0, v0, v31                
   386 : min.fp32       v0, v0, v0                 
   387 : min.fp32       v31, v0, v0                
   388 : min.fp32       v0, v31, v0                
   389 : min.fp32       v0, v0, v31                
   390 : min.fp64       v0, v0, v0                 
   391 : min.fp64       v31, v0, v0                
   392 : min.fp64       v0, v31, v0                
   393 : min.fp64       v0, v0, v31                
   394 : max.fp16       v0, v0, v0                 
   395 : max.fp16       v31, v0, v0                
   396 : max.fp16       v0, v31, v0                
   397 : max.fp16       v0, v0, v31                
   398 : max.fp32       v0, v0, v0                 
   399 : max.fp32       v31, v0, v0                
   400 : max.fp32       v0, v31, v0                
   401 : max.fp32       v0, v0, v31                
   402 : max.fp64       v0, v0, v0                 
   403 : max.fp64       v31, v0, v0                
   404 : max.fp64       v0, v31, v0                
   405 : max.fp64       v0, v0, v31                
   406 : mla.fp16       v0, v0, v0                 
   407 : mla.fp16       v31, v0, v0                
   408 : mla.fp16       v0, v31, v0                
   409 : mla.fp16       v0, v0, v31                
   410 : mla.fp32       v0, v0, v0                 
   411 : mla.fp32       v31, v0, v0                
   412 : mla.fp32       v0, v31, v0                
   413 : mla.fp32       v0, v0, v31                
   414 : mla.fp64       v0, v0, v0                 
   415 : mla.fp64       v31, v0, v0                
   416 : mla.fp64       v0, v31, v0                
   417 : mla.fp64       v0, v0, v31                
   418 : cvttz.fp16_i16 v0, v0                     
   419 : cvttz.fp16_i16 v31, v0                    
   420 : cvttz.fp16_i16 v0, v31                    
   421 : cvttz.fp32_i32 v0, v0                     
   422 : cvttz.fp32_i32 v31, v0                    
   423 : cvttz.fp32_i32 v0, v31                    
   424 : cvttz.fp64_i64 v0, v0                     
   425 : cvttz.fp64_i64 v31, v0                    
   426 : cvttz.fp64_i64 v0, v31                    
   427 : cvttz.fp16_u16 v0, v0                     
   428 : cvttz.fp16_u16 v31, v0                    
   429 : cvttz.fp16_u16 v0, v31                    
   430 : cvttz.fp32_u32 v0, v0                     
   431 : cvttz.fp32_u32 v31, v0                    
   432 : cvttz.fp32_u32 v0, v31                    
   433 : cvttz.fp64_u64 v0, v0                     
   434 : cvttz.fp64_u64 v31, v0                    
   435 : cvttz.fp64_u64 v0, v31                    
   436 : cvttz.i16_fp16 v0, v0                     
   437 : cvttz.i16_fp16 v31, v0                    
   438 : cvttz.i16_fp16 v0, v31                    
   439 : cvttz.i32_fp32 v0, v0                     
   440 : cvttz.i32_fp32 v31, v0                    
   441 : cvttz.i32_fp32 v0, v31                    
   442 : cvttz.i64_fp64 v0, v0                     
   443 : cvttz.i64_fp64 v31, v0                    
   444 : cvttz.i64_fp64 v0, v31                    
   445 : cvttz.u16_fp16 v0, v0                     
   446 : cvttz.u16_fp16 v31, v0                    
   447 : cvttz.u16_fp16 v0, v31                    
   448 : cvttz.u32_fp32 v0, v0                     
   449 : cvttz.u32_fp32 v31, v0                    
   450 : cvttz.u32_fp32 v0, v31                    
   451 : cvttz.u64_fp64 v0, v0                     
   452 : cvttz.u64_fp64 v31, v0                    
   453 : cvttz.u64_fp64 v0, v31                    
   454 : gt.u16         v0, v0, v0                 
   455 : gt.u16         v31, v0, v0                
   456 : gt.u16         v0, v31, v0                
   457 : gt.u16         v0, v0, v31                
   458 : gt.u32         v0, v0, v0                 
   459 : gt.u32         v31, v0, v0                
   460 : gt.u32         v0, v31, v0                
   461 : gt.u32         v0, v0, v31                
   462 : gt.u64         v0, v0, v0                 
   463 : gt.u64         v31, v0, v0                
   464 : gt.u64         v0, v31, v0                
   465 : gt.u64         v0, v0, v31                
   466 : ge.u16         v0, v0, v0                 
   467 : ge.u16         v31, v0, v0                
   468 : ge.u16         v0, v31, v0                
   469 : ge.u16         v0, v0, v31                
   470 : ge.u32         v0, v0, v0                 
   471 : ge.u32         v31, v0, v0                
   472 : ge.u32         v0, v31, v0                
   473 : ge.u32         v0, v0, v31                
   474 : ge.u64         v0, v0, v0                 
   475 : ge.u64         v31, v0, v0                
   476 : ge.u64         v0, v31, v0                
   477 : ge.u64         v0, v0, v31                
   478 : eq.u16         v0, v0, v0                 
   479 : eq.u16         v31, v0, v0                
   480 : eq.u16         v0, v31, v0                
   481 : eq.u16         v0, v0, v31                
   482 : eq.u32         v0, v0, v0                 
   483 : eq.u32         v31, v0, v0                
   484 : eq.u32         v0, v31, v0                
   485 : eq.u32         v0, v0, v31                
   486 : eq.u64         v0, v0, v0                 
   487 : eq.u64         v31, v0, v0                
   488 : eq.u64         v0, v31, v0                
   489 : eq.u64         v0, v0, v31                
   490 : neg.i8         v0, v0                     
   491 : neg.i8         v31, v0                    
   492 : neg.i8         v0, v31                    
   493 : neg.i16        v0, v0                     
   494 : neg.i16        v31, v0                    
   495 : neg.i16        v0, v31                    
   496 : neg.i32        v0, v0                     
   497 : neg.i32        v31, v0                    
   498 : neg.i32        v0, v31                    
   499 : neg.i64        v0, v0                     
   500 : neg.i64        v31, v0                    
   501 : neg.i64        v0, v31                    
   502 : neg.fp16       v0, v0                     
   503 : neg.fp16       v31, v0                    
   504 : neg.fp16       v0, v31                    
   505 : neg.fp32       v0, v0                     
   506 : neg.fp32       v31, v0                    
   507 : neg.fp32       v0, v31                    
   508 : neg.fp64       v0, v0                     
   509 : neg.fp64       v31, v0                    
   510 : neg.fp64       v0, v31                    
