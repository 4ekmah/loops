instruction_set_test()
     0 : mov            i0, -1                     
     1 : mov            i15, -1                    
     2 : mov            i0, -65536                 
     3 : store.u64      6, i0, 256, i0             
     4 : store.i64      7, i0, 256, i0             
     5 : store.u64      6, i0, i0, i0              
     6 : store.i64      7, i0, i0, i0              
     7 : store.u32      4, i0, 256, i0             
     8 : store.i32      5, i0, 256, i0             
     9 : store.u32      4, i0, i0, i0              
    10 : store.i32      5, i0, i0, i0              
    11 : store.u16      2, i0, 256, i0             
    12 : store.i16      3, i0, 256, i0             
    13 : store.u16      2, i0, i0, i0              
    14 : store.i16      3, i0, i0, i0              
    15 : store.u8       0, i0, 256, i0             
    16 : store.i8       1, i0, 256, i0             
    17 : store.u8       0, i0, i0, i0              
    18 : store.i8       1, i0, i0, i0              
    19 : load.u64       i0, 6, i0                  
    20 : load.u64       i0, 6, i0, 256             
    21 : load.u64       i15, 6, i0, 256            
    22 : load.u64       i0, 6, i15, 256            
    23 : load.u64       i0, 6, i0, i0              
    24 : load.u64       i15, 6, i0, i0             
    25 : load.u64       i0, 6, i15, i0             
    26 : load.u64       i0, 6, i0, i15             
    27 : load.i64       i0, 7, i0                  
    28 : load.i64       i0, 7, i0, 256             
    29 : load.i64       i15, 7, i0, 256            
    30 : load.i64       i0, 7, i15, 256            
    31 : load.i64       i0, 7, i0, i0              
    32 : load.i64       i15, 7, i0, i0             
    33 : load.i64       i0, 7, i15, i0             
    34 : load.i64       i0, 7, i0, i15             
    35 : load.u32       i0, 4, i0                  
    36 : load.u32       i0, 4, i0, 256             
    37 : load.u32       i15, 4, i0, 256            
    38 : load.u32       i0, 4, i15, 256            
    39 : load.u32       i0, 4, i0, i0              
    40 : load.u32       i15, 4, i0, i0             
    41 : load.u32       i0, 4, i15, i0             
    42 : load.u32       i0, 4, i0, i15             
    43 : load.i32       i0, 5, i0                  
    44 : load.i32       i0, 5, i0, 256             
    45 : load.i32       i15, 5, i0, 256            
    46 : load.i32       i0, 5, i15, 256            
    47 : load.i32       i0, 5, i0, i0              
    48 : load.i32       i15, 5, i0, i0             
    49 : load.i32       i0, 5, i15, i0             
    50 : load.i32       i0, 5, i0, i15             
    51 : load.u16       i0, 2, i0                  
    52 : load.u16       i0, 2, i0, 256             
    53 : load.u16       i15, 2, i0, 256            
    54 : load.u16       i0, 2, i15, 256            
    55 : load.u16       i0, 2, i0, i0              
    56 : load.u16       i15, 2, i0, i0             
    57 : load.u16       i0, 2, i15, i0             
    58 : load.u16       i0, 2, i0, i15             
    59 : load.i16       i0, 3, i0                  
    60 : load.i16       i0, 3, i0, 256             
    61 : load.i16       i15, 3, i0, 256            
    62 : load.i16       i0, 3, i15, 256            
    63 : load.i16       i0, 3, i0, i0              
    64 : load.i16       i15, 3, i0, i0             
    65 : load.i16       i0, 3, i15, i0             
    66 : load.i16       i0, 3, i0, i15             
    67 : load.u8        i0, 0, i0                  
    68 : load.u8        i0, 0, i0, 256             
    69 : load.u8        i15, 0, i0, 256            
    70 : load.u8        i0, 0, i15, 256            
    71 : load.u8        i0, 0, i0, i0              
    72 : load.u8        i15, 0, i0, i0             
    73 : load.u8        i0, 0, i15, i0             
    74 : load.u8        i0, 0, i0, i15             
    75 : load.i8        i0, 1, i0                  
    76 : load.i8        i0, 1, i0, 256             
    77 : load.i8        i15, 1, i0, 256            
    78 : load.i8        i0, 1, i15, 256            
    79 : load.i8        i0, 1, i0, i0              
    80 : load.i8        i15, 1, i0, i0             
    81 : load.i8        i0, 1, i15, i0             
    82 : load.i8        i0, 1, i0, i15             
    83 : select         i0, 0, i0, i0              
    84 : select         i0, 1, i0, i0              
    85 : select         i0, 2, i0, i0              
    86 : select         i0, 3, i0, i0              
    87 : select         i0, 4, i0, i0              
    88 : select         i0, 5, i0, i0              
    89 : select         i0, 6, i0, i0              
    90 : select         i0, 7, i0, i0              
    91 : select         i15, 0, i0, i0             
    92 : select         i0, 0, i15, i0             
    93 : select         i0, 0, i0, i15             
    94 : arm_cinc       i0, i0, 0                  
    95 : arm_cinc       i0, i0, 1                  
    96 : arm_cinc       i0, i0, 2                  
    97 : arm_cinc       i0, i0, 3                  
    98 : arm_cinc       i0, i0, 4                  
    99 : arm_cinc       i0, i0, 5                  
   100 : arm_cinc       i0, i0, 6                  
   101 : arm_cinc       i0, i0, 7                  
   102 : arm_cinc       i15, i0, 0                 
   103 : arm_cinc       i0, i15, 0                 
   104 : arm_cneg       i0, i0, 0                  
   105 : arm_cneg       i0, i0, 1                  
   106 : arm_cneg       i0, i0, 2                  
   107 : arm_cneg       i0, i0, 3                  
   108 : arm_cneg       i0, i0, 4                  
   109 : arm_cneg       i0, i0, 5                  
   110 : arm_cneg       i0, i0, 6                  
   111 : arm_cneg       i0, i0, 7                  
   112 : arm_cneg       i15, i0, 0                 
   113 : arm_cneg       i0, i15, 0                 
   114 : sar            i0, i0, 63                 
   115 : sar            i15, i0, 63                
   116 : sar            i0, i15, 63                
   117 : sar            i0, i0, i0                 
   118 : sar            i15, i0, i0                
   119 : sar            i0, i15, i0                
   120 : sar            i0, i0, i15                
   121 : shr            i0, i0, 63                 
   122 : shr            i15, i0, 63                
   123 : shr            i0, i15, 63                
   124 : shr            i0, i0, i0                 
   125 : shr            i15, i0, i0                
   126 : shr            i0, i15, i0                
   127 : shr            i0, i0, i15                
   128 : shl            i0, i0, 63                 
   129 : shl            i0, i0, 31                 
   130 : shl            i0, i0, 15                 
   131 : shl            i0, i0, 7                  
   132 : shl            i0, i0, 1                  
   133 : shl            i0, i0, 0                  
   134 : shl            i15, i0, 63                
   135 : shl            i0, i15, 63                
   136 : shl            i0, i0, i0                 
   137 : shl            i15, i0, i0                
   138 : shl            i0, i15, i0                
   139 : shl            i0, i0, i15                
   140 : and            i0, i0, i0                 
   141 : and            i15, i0, i0                
   142 : and            i0, i15, i0                
   143 : and            i0, i0, i15                
   144 : and            i0, i0, 255                
   145 : and            i0, i0, 65280              
   146 : and            i0, i0, -72057594037927936 
   147 : and            i15, i0, 255               
   148 : and            i0, i15, 255               
   149 : or             i0, i0, i0                 
   150 : or             i15, i0, i0                
   151 : or             i0, i15, i0                
   152 : or             i0, i0, i15                
   153 : or             i0, i0, 255                
   154 : or             i0, i0, 65280              
   155 : or             i0, i0, -72057594037927936 
   156 : or             i15, i0, 255               
   157 : or             i0, i15, 255               
   158 : xor            i0, i0, i0                 
   159 : xor            i15, i0, i0                
   160 : xor            i0, i15, i0                
   161 : xor            i0, i0, i15                
   162 : xor            i0, i0, 255                
   163 : xor            i0, i0, 65280              
   164 : xor            i0, i0, -72057594037927936 
   165 : xor            i15, i0, 255               
   166 : xor            i0, i15, 255               
   167 : not            i0, i0                     
   168 : not            i15, i0                    
   169 : not            i0, i15                    
   170 : neg            i0, i0                     
   171 : neg            i15, i0                    
   172 : neg            i0, i15                    
   173 : vld.u64        v0, i0                     
   174 : vld.u64        v0, i0, 32784              
   175 : vld.u64        v31, i0                    
   176 : vld.u64        v0, i15                    
   177 : vld.u64        v0, i0, i0                 
   178 : vld.u64        v31, i0, i0                
   179 : vld.u64        v0, i15, i0                
   180 : vld.u64        v0, i0, i15                
   181 : vst.u64        i0, v0                     
   182 : vst.u64        i0, 32784, v0              
   183 : vst.u64        i0, v31                    
   184 : vst.u64        i15, v0                    
   185 : vst.u64        i0, i0, v0                 
   186 : vst.u64        i0, i0, v31                
   187 : vst.u64        i15, i0, v0                
   188 : vst.u64        i0, i15, v0                
   189 : add.u8         v0, v0, v0                 
   190 : add.u8         v31, v0, v0                
   191 : add.u8         v0, v31, v0                
   192 : add.u8         v0, v0, v31                
   193 : add.u16        v0, v0, v0                 
   194 : add.u16        v31, v0, v0                
   195 : add.u16        v0, v31, v0                
   196 : add.u16        v0, v0, v31                
   197 : add.u32        v0, v0, v0                 
   198 : add.u32        v31, v0, v0                
   199 : add.u32        v0, v31, v0                
   200 : add.u32        v0, v0, v31                
   201 : add.u64        v0, v0, v0                 
   202 : add.u64        v31, v0, v0                
   203 : add.u64        v0, v31, v0                
   204 : add.u64        v0, v0, v31                
   205 : add.fp16       v0, v0, v0                 
   206 : add.fp16       v31, v0, v0                
   207 : add.fp16       v0, v31, v0                
   208 : add.fp16       v0, v0, v31                
   209 : add.fp32       v0, v0, v0                 
   210 : add.fp32       v31, v0, v0                
   211 : add.fp32       v0, v31, v0                
   212 : add.fp32       v0, v0, v31                
   213 : add.fp64       v0, v0, v0                 
   214 : add.fp64       v31, v0, v0                
   215 : add.fp64       v0, v31, v0                
   216 : add.fp64       v0, v0, v31                
   217 : sub.u8         v0, v0, v0                 
   218 : sub.u8         v31, v0, v0                
   219 : sub.u8         v0, v31, v0                
   220 : sub.u8         v0, v0, v31                
   221 : sub.u16        v0, v0, v0                 
   222 : sub.u16        v31, v0, v0                
   223 : sub.u16        v0, v31, v0                
   224 : sub.u16        v0, v0, v31                
   225 : sub.u32        v0, v0, v0                 
   226 : sub.u32        v31, v0, v0                
   227 : sub.u32        v0, v31, v0                
   228 : sub.u32        v0, v0, v31                
   229 : sub.u64        v0, v0, v0                 
   230 : sub.u64        v31, v0, v0                
   231 : sub.u64        v0, v31, v0                
   232 : sub.u64        v0, v0, v31                
   233 : sub.fp16       v0, v0, v0                 
   234 : sub.fp16       v31, v0, v0                
   235 : sub.fp16       v0, v31, v0                
   236 : sub.fp16       v0, v0, v31                
   237 : sub.fp32       v0, v0, v0                 
   238 : sub.fp32       v31, v0, v0                
   239 : sub.fp32       v0, v31, v0                
   240 : sub.fp32       v0, v0, v31                
   241 : sub.fp64       v0, v0, v0                 
   242 : sub.fp64       v31, v0, v0                
   243 : sub.fp64       v0, v31, v0                
   244 : sub.fp64       v0, v0, v31                
   245 : mul.u8         v0, v0, v0                 
   246 : mul.u8         v31, v0, v0                
   247 : mul.u8         v0, v31, v0                
   248 : mul.u8         v0, v0, v31                
   249 : mul.u16        v0, v0, v0                 
   250 : mul.u16        v31, v0, v0                
   251 : mul.u16        v0, v31, v0                
   252 : mul.u16        v0, v0, v31                
   253 : mul.u32        v0, v0, v0                 
   254 : mul.u32        v31, v0, v0                
   255 : mul.u32        v0, v31, v0                
   256 : mul.u32        v0, v0, v31                
   257 : mul.fp16       v0, v0, v0                 
   258 : mul.fp16       v31, v0, v0                
   259 : mul.fp16       v0, v31, v0                
   260 : mul.fp16       v0, v0, v31                
   261 : mul.fp32       v0, v0, v0                 
   262 : mul.fp32       v31, v0, v0                
   263 : mul.fp32       v0, v31, v0                
   264 : mul.fp32       v0, v0, v31                
   265 : mul.fp64       v0, v0, v0                 
   266 : mul.fp64       v31, v0, v0                
   267 : mul.fp64       v0, v31, v0                
   268 : mul.fp64       v0, v0, v31                
   269 : mul.fp32       v0, v0, v0                 
   270 : mul.fp32       v31, v0, v0                
   271 : mul.fp32       v0, v31, v0                
   272 : mul.fp32       v0, v0, v31                
   273 : mul.fp64       v0, v0, v0                 
   274 : mul.fp64       v31, v0, v0                
   275 : mul.fp64       v0, v31, v0                
   276 : mul.fp64       v0, v0, v31                
   277 : mul.fp16       v0, v0, v0                 
   278 : mul.fp16       v31, v0, v0                
   279 : mul.fp16       v0, v31, v0                
   280 : mul.fp16       v0, v0, v31                
   281 : sal.u8         v0, v0, 1                  
   282 : sal.u8         v31, v0, 1                 
   283 : sal.u8         v0, v31, 1                 
   284 : sal.u8         v0, v0, 7                  
   285 : sal.u16        v0, v0, 1                  
   286 : sal.u16        v31, v0, 1                 
   287 : sal.u16        v0, v31, 1                 
   288 : sal.u16        v0, v0, 15                 
   289 : sal.u32        v0, v0, 1                  
   290 : sal.u32        v31, v0, 1                 
   291 : sal.u32        v0, v31, 1                 
   292 : sal.u32        v0, v0, 31                 
   293 : sal.u64        v0, v0, 1                  
   294 : sal.u64        v31, v0, 1                 
   295 : sal.u64        v0, v31, 1                 
   296 : sal.u64        v0, v0, 63                 
   297 : sal.u8         v0, v0, v0                 
   298 : sal.u8         v31, v0, v0                
   299 : sal.u8         v0, v31, v0                
   300 : sal.u8         v0, v0, v31                
   301 : sal.u16        v0, v0, v0                 
   302 : sal.u16        v31, v0, v0                
   303 : sal.u16        v0, v31, v0                
   304 : sal.u16        v0, v0, v31                
   305 : sal.u32        v0, v0, v0                 
   306 : sal.u32        v31, v0, v0                
   307 : sal.u32        v0, v31, v0                
   308 : sal.u32        v0, v0, v31                
   309 : sal.u64        v0, v0, v0                 
   310 : sal.u64        v31, v0, v0                
   311 : sal.u64        v0, v31, v0                
   312 : sal.u64        v0, v0, v31                
   313 : sal.i8         v0, v0, v0                 
   314 : sal.i8         v31, v0, v0                
   315 : sal.i8         v0, v31, v0                
   316 : sal.i8         v0, v0, v31                
   317 : sal.i16        v0, v0, v0                 
   318 : sal.i16        v31, v0, v0                
   319 : sal.i16        v0, v31, v0                
   320 : sal.i16        v0, v0, v31                
   321 : sal.i32        v0, v0, v0                 
   322 : sal.i32        v31, v0, v0                
   323 : sal.i32        v0, v31, v0                
   324 : sal.i32        v0, v0, v31                
   325 : sal.i64        v0, v0, v0                 
   326 : sal.i64        v31, v0, v0                
   327 : sal.i64        v0, v31, v0                
   328 : sal.i64        v0, v0, v31                
   329 : sar.u8         v0, v0, 1                  
   330 : sar.u8         v31, v0, 1                 
   331 : sar.u8         v0, v31, 1                 
   332 : sar.u8         v0, v0, 7                  
   333 : sar.u16        v0, v0, 1                  
   334 : sar.u16        v31, v0, 1                 
   335 : sar.u16        v0, v31, 1                 
   336 : sar.u16        v0, v0, 15                 
   337 : sar.u32        v0, v0, 1                  
   338 : sar.u32        v31, v0, 1                 
   339 : sar.u32        v0, v31, 1                 
   340 : sar.u32        v0, v0, 31                 
   341 : sar.u64        v0, v0, 1                  
   342 : sar.u64        v31, v0, 1                 
   343 : sar.u64        v0, v31, 1                 
   344 : sar.u64        v0, v0, 63                 
   345 : sar.i8         v0, v0, 1                  
   346 : sar.i8         v31, v0, 1                 
   347 : sar.i8         v0, v31, 1                 
   348 : sar.i8         v0, v0, 7                  
   349 : sar.i16        v0, v0, 1                  
   350 : sar.i16        v31, v0, 1                 
   351 : sar.i16        v0, v31, 1                 
   352 : sar.i16        v0, v0, 15                 
   353 : sar.i32        v0, v0, 1                  
   354 : sar.i32        v31, v0, 1                 
   355 : sar.i32        v0, v31, 1                 
   356 : sar.i32        v0, v0, 31                 
   357 : sar.i64        v0, v0, 1                  
   358 : sar.i64        v31, v0, 1                 
   359 : sar.i64        v0, v31, 1                 
   360 : sar.i64        v0, v0, 63                 
   361 : and            v0, v0, v0                 
   362 : and            v31, v0, v0                
   363 : and            v0, v31, v0                
   364 : and            v0, v0, v31                
   365 : or             v0, v0, v0                 
   366 : or             v31, v0, v0                
   367 : or             v0, v31, v0                
   368 : or             v0, v0, v31                
   369 : xor            v0, v0, v0                 
   370 : xor            v31, v0, v0                
   371 : xor            v0, v31, v0                
   372 : xor            v0, v0, v31                
   373 : not            v0, v0                     
   374 : not            v31, v0                    
   375 : not            v0, v31                    
   376 : mov            v0, 0                      
   377 : mov            v31, 0                     
   378 : mov            v0, 255                    
   379 : mov            v0, 0                      
   380 : mov            v31, 0                     
   381 : mov            v0, 255                    
   382 : mov            v0, 0                      
   383 : mov            v31, 0                     
   384 : mov            v0, 255                    
   385 : mov            v0, -1                     
   386 : mov            v31, -1                    
   387 : mov            v0, -256                   
   388 : mov            v0, -1                     
   389 : mov            v31, -1                    
   390 : mov            v0, -256                   
   391 : min.fp16       v0, v0, v0                 
   392 : min.fp16       v31, v0, v0                
   393 : min.fp16       v0, v31, v0                
   394 : min.fp16       v0, v0, v31                
   395 : min.fp32       v0, v0, v0                 
   396 : min.fp32       v31, v0, v0                
   397 : min.fp32       v0, v31, v0                
   398 : min.fp32       v0, v0, v31                
   399 : min.fp64       v0, v0, v0                 
   400 : min.fp64       v31, v0, v0                
   401 : min.fp64       v0, v31, v0                
   402 : min.fp64       v0, v0, v31                
   403 : max.fp16       v0, v0, v0                 
   404 : max.fp16       v31, v0, v0                
   405 : max.fp16       v0, v31, v0                
   406 : max.fp16       v0, v0, v31                
   407 : max.fp32       v0, v0, v0                 
   408 : max.fp32       v31, v0, v0                
   409 : max.fp32       v0, v31, v0                
   410 : max.fp32       v0, v0, v31                
   411 : max.fp64       v0, v0, v0                 
   412 : max.fp64       v31, v0, v0                
   413 : max.fp64       v0, v31, v0                
   414 : max.fp64       v0, v0, v31                
   415 : fma.fp16       v0, v0, v0, v0             
   416 : fma.fp16       v31, v31, v0, v0           
   417 : fma.fp16       v0, v0, v31, v0            
   418 : fma.fp16       v0, v0, v0, v31            
   419 : fma.fp32       v0, v0, v0, v0             
   420 : fma.fp32       v31, v31, v0, v0           
   421 : fma.fp32       v0, v0, v31, v0            
   422 : fma.fp32       v0, v0, v0, v31            
   423 : fma.fp64       v0, v0, v0, v0             
   424 : fma.fp64       v31, v31, v0, v0           
   425 : fma.fp64       v0, v0, v31, v0            
   426 : fma.fp64       v0, v0, v0, v31            
   427 : trunc.fp16_i16 v0, v0                     
   428 : trunc.fp16_i16 v31, v0                    
   429 : trunc.fp16_i16 v0, v31                    
   430 : trunc.fp32_i32 v0, v0                     
   431 : trunc.fp32_i32 v31, v0                    
   432 : trunc.fp32_i32 v0, v31                    
   433 : trunc.fp64_i64 v0, v0                     
   434 : trunc.fp64_i64 v31, v0                    
   435 : trunc.fp64_i64 v0, v31                    
   436 : trunc.fp16_u16 v0, v0                     
   437 : trunc.fp16_u16 v31, v0                    
   438 : trunc.fp16_u16 v0, v31                    
   439 : trunc.fp32_u32 v0, v0                     
   440 : trunc.fp32_u32 v31, v0                    
   441 : trunc.fp32_u32 v0, v31                    
   442 : trunc.fp64_u64 v0, v0                     
   443 : trunc.fp64_u64 v31, v0                    
   444 : trunc.fp64_u64 v0, v31                    
   445 : floor.fp16_i16 v0, v0                     
   446 : floor.fp16_i16 v31, v0                    
   447 : floor.fp16_i16 v0, v31                    
   448 : floor.fp32_i32 v0, v0                     
   449 : floor.fp32_i32 v31, v0                    
   450 : floor.fp32_i32 v0, v31                    
   451 : floor.fp64_i64 v0, v0                     
   452 : floor.fp64_i64 v31, v0                    
   453 : floor.fp64_i64 v0, v31                    
   454 : floor.fp16_u16 v0, v0                     
   455 : floor.fp16_u16 v31, v0                    
   456 : floor.fp16_u16 v0, v31                    
   457 : floor.fp32_u32 v0, v0                     
   458 : floor.fp32_u32 v31, v0                    
   459 : floor.fp32_u32 v0, v31                    
   460 : floor.fp64_u64 v0, v0                     
   461 : floor.fp64_u64 v31, v0                    
   462 : floor.fp64_u64 v0, v31                    
   463 : cast.i16_fp16  v0, v0                     
   464 : cast.i16_fp16  v31, v0                    
   465 : cast.i16_fp16  v0, v31                    
   466 : cast.i32_fp32  v0, v0                     
   467 : cast.i32_fp32  v31, v0                    
   468 : cast.i32_fp32  v0, v31                    
   469 : cast.i64_fp64  v0, v0                     
   470 : cast.i64_fp64  v31, v0                    
   471 : cast.i64_fp64  v0, v31                    
   472 : cast.u16_fp16  v0, v0                     
   473 : cast.u16_fp16  v31, v0                    
   474 : cast.u16_fp16  v0, v31                    
   475 : cast.u32_fp32  v0, v0                     
   476 : cast.u32_fp32  v31, v0                    
   477 : cast.u32_fp32  v0, v31                    
   478 : cast.u64_fp64  v0, v0                     
   479 : cast.u64_fp64  v31, v0                    
   480 : cast.u64_fp64  v0, v31                    
   481 : gt.u16         v0, v0, v0                 
   482 : gt.u16         v31, v0, v0                
   483 : gt.u16         v0, v31, v0                
   484 : gt.u16         v0, v0, v31                
   485 : gt.u32         v0, v0, v0                 
   486 : gt.u32         v31, v0, v0                
   487 : gt.u32         v0, v31, v0                
   488 : gt.u32         v0, v0, v31                
   489 : gt.u64         v0, v0, v0                 
   490 : gt.u64         v31, v0, v0                
   491 : gt.u64         v0, v31, v0                
   492 : gt.u64         v0, v0, v31                
   493 : ge.u16         v0, v0, v0                 
   494 : ge.u16         v31, v0, v0                
   495 : ge.u16         v0, v31, v0                
   496 : ge.u16         v0, v0, v31                
   497 : ge.u32         v0, v0, v0                 
   498 : ge.u32         v31, v0, v0                
   499 : ge.u32         v0, v31, v0                
   500 : ge.u32         v0, v0, v31                
   501 : ge.u64         v0, v0, v0                 
   502 : ge.u64         v31, v0, v0                
   503 : ge.u64         v0, v31, v0                
   504 : ge.u64         v0, v0, v31                
   505 : eq.u16         v0, v0, v0                 
   506 : eq.u16         v31, v0, v0                
   507 : eq.u16         v0, v31, v0                
   508 : eq.u16         v0, v0, v31                
   509 : eq.u32         v0, v0, v0                 
   510 : eq.u32         v31, v0, v0                
   511 : eq.u32         v0, v31, v0                
   512 : eq.u32         v0, v0, v31                
   513 : eq.u64         v0, v0, v0                 
   514 : eq.u64         v31, v0, v0                
   515 : eq.u64         v0, v31, v0                
   516 : eq.u64         v0, v0, v31                
   517 : neg.i8         v0, v0                     
   518 : neg.i8         v31, v0                    
   519 : neg.i8         v0, v31                    
   520 : neg.i16        v0, v0                     
   521 : neg.i16        v31, v0                    
   522 : neg.i16        v0, v31                    
   523 : neg.i32        v0, v0                     
   524 : neg.i32        v31, v0                    
   525 : neg.i32        v0, v31                    
   526 : neg.i64        v0, v0                     
   527 : neg.i64        v31, v0                    
   528 : neg.i64        v0, v31                    
   529 : neg.fp16       v0, v0                     
   530 : neg.fp16       v31, v0                    
   531 : neg.fp16       v0, v31                    
   532 : neg.fp32       v0, v0                     
   533 : neg.fp32       v31, v0                    
   534 : neg.fp32       v0, v31                    
   535 : neg.fp64       v0, v0                     
   536 : neg.fp64       v31, v0                    
   537 : neg.fp64       v0, v31                    
   538 : arm_movk       i0, 0, 16                  
   539 : arm_movk       i15, 0, 16                 
   540 : arm_movk       i0, 65535, 16              
   541 : arm_movk       i0, 0, 32                  
   542 : arm_movk       i15, 0, 32                 
   543 : arm_movk       i0, 65535, 32              
   544 : arm_movk       i0, 0, 48                  
   545 : arm_movk       i15, 0, 48                 
   546 : arm_movk       i0, 65535, 48              
   547 : broadcast.i8   v0, i0                     
   548 : broadcast.i8   v31, i0                    
   549 : broadcast.i8   v0, i15                    
   550 : broadcast.i16  v0, i0                     
   551 : broadcast.i16  v31, i0                    
   552 : broadcast.i16  v0, i15                    
   553 : broadcast.i32  v0, i0                     
   554 : broadcast.i32  v31, i0                    
   555 : broadcast.i32  v0, i15                    
   556 : broadcast.i64  v0, i0                     
   557 : broadcast.i64  v31, i0                    
   558 : broadcast.i64  v0, i15                    
   559 : vld_lane.i8    v0, 0, i0                  
   560 : vld_lane.i8    v31, 0, i0                 
   561 : vld_lane.i8    v0, 15, i0                 
   562 : vld_lane.i8    v0, 0, i15                 
   563 : vld_lane.i16   v0, 0, i0                  
   564 : vld_lane.i16   v31, 0, i0                 
   565 : vld_lane.i16   v0, 7, i0                  
   566 : vld_lane.i16   v0, 0, i15                 
   567 : vld_lane.i32   v0, 0, i0                  
   568 : vld_lane.i32   v31, 0, i0                 
   569 : vld_lane.i32   v0, 3, i0                  
   570 : vld_lane.i32   v0, 0, i15                 
   571 : vld_lane.i64   v0, 0, i0                  
   572 : vld_lane.i64   v31, 0, i0                 
   573 : vld_lane.i64   v0, 1, i0                  
   574 : vld_lane.i64   v0, 0, i15                 
   575 : vst_lane.i8    i0, v0, 0                  
   576 : vst_lane.i8    i0, v31, 0                 
   577 : vst_lane.i8    i0, v0, 15                 
   578 : vst_lane.i8    i15, v0, 0                 
   579 : vst_lane.i16   i0, v0, 0                  
   580 : vst_lane.i16   i0, v31, 0                 
   581 : vst_lane.i16   i0, v0, 7                  
   582 : vst_lane.i16   i15, v0, 0                 
   583 : vst_lane.i32   i0, v0, 0                  
   584 : vst_lane.i32   i0, v31, 0                 
   585 : vst_lane.i32   i0, v0, 3                  
   586 : vst_lane.i32   i15, v0, 0                 
   587 : vst_lane.i64   i0, v0, 0                  
   588 : vst_lane.i64   i0, v31, 0                 
   589 : vst_lane.i64   i0, v0, 1                  
   590 : vst_lane.i64   i15, v0, 0                 
   591 : ext.i8         v0, v0, v0, 0              
   592 : ext.i8         v31, v0, v0, 0             
   593 : ext.i8         v0, v31, v0, 0             
   594 : ext.i8         v0, v0, v31, 0             
   595 : ext.i8         v0, v0, v0, 15             
   596 : ext.i16        v0, v0, v0, 0              
   597 : ext.i16        v31, v0, v0, 0             
   598 : ext.i16        v0, v31, v0, 0             
   599 : ext.i16        v0, v0, v31, 0             
   600 : ext.i16        v0, v0, v0, 7              
   601 : ext.i32        v0, v0, v0, 0              
   602 : ext.i32        v31, v0, v0, 0             
   603 : ext.i32        v0, v31, v0, 0             
   604 : ext.i32        v0, v0, v31, 0             
   605 : ext.i32        v0, v0, v0, 3              
   606 : ext.i64        v0, v0, v0, 0              
   607 : ext.i64        v31, v0, v0, 0             
   608 : ext.i64        v0, v31, v0, 0             
   609 : ext.i64        v0, v0, v31, 0             
   610 : ext.i64        v0, v0, v0, 1              
   611 : fma.fp16       v0, v0, v0, v0, 0          
   612 : fma.fp16       v31, v31, v0, v0, 0        
   613 : fma.fp16       v0, v0, v31, v0, 0         
   614 : fma.fp16       v0, v0, v0, v15, 0         
   615 : fma.fp16       v0, v0, v0, v0, 7          
   616 : fma.fp32       v0, v0, v0, v0, 0          
   617 : fma.fp32       v31, v31, v0, v0, 0        
   618 : fma.fp32       v0, v0, v31, v0, 0         
   619 : fma.fp32       v0, v0, v0, v31, 0         
   620 : fma.fp32       v0, v0, v0, v0, 3          
   621 : fma.fp64       v0, v0, v0, v0, 0          
   622 : fma.fp64       v31, v31, v0, v0, 0        
   623 : fma.fp64       v0, v0, v31, v0, 0         
   624 : fma.fp64       v0, v0, v0, v31, 0         
   625 : fma.fp64       v0, v0, v0, v0, 1          
   626 : gt.u8          v0, v0, v0                 
   627 : gt.u8          v31, v0, v0                
   628 : gt.u8          v0, v31, v0                
   629 : gt.u8          v0, v0, v31                
   630 : gt.u16         v0, v0, v0                 
   631 : gt.u16         v31, v0, v0                
   632 : gt.u16         v0, v31, v0                
   633 : gt.u16         v0, v0, v31                
   634 : gt.u32         v0, v0, v0                 
   635 : gt.u32         v31, v0, v0                
   636 : gt.u32         v0, v31, v0                
   637 : gt.u32         v0, v0, v31                
   638 : gt.u64         v0, v0, v0                 
   639 : gt.u64         v31, v0, v0                
   640 : gt.u64         v0, v31, v0                
   641 : gt.u64         v0, v0, v31                
   642 : ge.u8          v0, v0, v0                 
   643 : ge.u8          v31, v0, v0                
   644 : ge.u8          v0, v31, v0                
   645 : ge.u8          v0, v0, v31                
   646 : ge.u16         v0, v0, v0                 
   647 : ge.u16         v31, v0, v0                
   648 : ge.u16         v0, v31, v0                
   649 : ge.u16         v0, v0, v31                
   650 : ge.u32         v0, v0, v0                 
   651 : ge.u32         v31, v0, v0                
   652 : ge.u32         v0, v31, v0                
   653 : ge.u32         v0, v0, v31                
   654 : ge.u64         v0, v0, v0                 
   655 : ge.u64         v31, v0, v0                
   656 : ge.u64         v0, v31, v0                
   657 : ge.u64         v0, v0, v31                
   658 : eq.u8          v0, v0, v0                 
   659 : eq.u8          v31, v0, v0                
   660 : eq.u8          v0, v31, v0                
   661 : eq.u8          v0, v0, v31                
   662 : eq.u16         v0, v0, v0                 
   663 : eq.u16         v31, v0, v0                
   664 : eq.u16         v0, v31, v0                
   665 : eq.u16         v0, v0, v31                
   666 : eq.u32         v0, v0, v0                 
   667 : eq.u32         v31, v0, v0                
   668 : eq.u32         v0, v31, v0                
   669 : eq.u32         v0, v0, v31                
   670 : eq.u64         v0, v0, v0                 
   671 : eq.u64         v31, v0, v0                
   672 : eq.u64         v0, v31, v0                
   673 : eq.u64         v0, v0, v31                
   674 : ge.u8          v0, v0, v0                 
   675 : ge.u8          v31, v0, v0                
   676 : ge.u8          v0, v31, v0                
   677 : ge.u8          v0, v0, v31                
   678 : ge.u16         v0, v0, v0                 
   679 : ge.u16         v31, v0, v0                
   680 : ge.u16         v0, v31, v0                
   681 : ge.u16         v0, v0, v31                
   682 : ge.u32         v0, v0, v0                 
   683 : ge.u32         v31, v0, v0                
   684 : ge.u32         v0, v31, v0                
   685 : ge.u32         v0, v0, v31                
   686 : ge.u64         v0, v0, v0                 
   687 : ge.u64         v31, v0, v0                
   688 : ge.u64         v0, v31, v0                
   689 : ge.u64         v0, v0, v31                
   690 : gt.u8          v0, v0, v0                 
   691 : gt.u8          v31, v0, v0                
   692 : gt.u8          v0, v31, v0                
   693 : gt.u8          v0, v0, v31                
   694 : gt.u16         v0, v0, v0                 
   695 : gt.u16         v31, v0, v0                
   696 : gt.u16         v0, v31, v0                
   697 : gt.u16         v0, v0, v31                
   698 : gt.u32         v0, v0, v0                 
   699 : gt.u32         v31, v0, v0                
   700 : gt.u32         v0, v31, v0                
   701 : gt.u32         v0, v0, v31                
   702 : gt.u64         v0, v0, v0                 
   703 : gt.u64         v31, v0, v0                
   704 : gt.u64         v0, v31, v0                
   705 : gt.u64         v0, v0, v31                
