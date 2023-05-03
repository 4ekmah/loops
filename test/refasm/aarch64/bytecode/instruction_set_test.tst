instruction_set_test()
     0 : mov                      i0, -1                     
     1 : mov                      i15, -1                    
     2 : mov                      i0, -65536                 
     3 : store.u64                i0, 256, i0                
     4 : store.i64                i0, 256, i0                
     5 : store.u64                i0, i0, i0                 
     6 : store.i64                i0, i0, i0                 
     7 : store.u32                i0, 256, i0                
     8 : store.i32                i0, 256, i0                
     9 : store.u32                i0, i0, i0                 
    10 : store.i32                i0, i0, i0                 
    11 : store.u16                i0, 256, i0                
    12 : store.i16                i0, 256, i0                
    13 : store.u16                i0, i0, i0                 
    14 : store.i16                i0, i0, i0                 
    15 : store.u8                 i0, 256, i0                
    16 : store.i8                 i0, 256, i0                
    17 : store.u8                 i0, i0, i0                 
    18 : store.i8                 i0, i0, i0                 
    19 : load.u64                 i0, i0                     
    20 : load.u64                 i0, i0, 256                
    21 : load.u64                 i15, i0, 256               
    22 : load.u64                 i0, i15, 256               
    23 : load.u64                 i0, i0, i0                 
    24 : load.u64                 i15, i0, i0                
    25 : load.u64                 i0, i15, i0                
    26 : load.u64                 i0, i0, i15                
    27 : load.i64                 i0, i0                     
    28 : load.i64                 i0, i0, 256                
    29 : load.i64                 i15, i0, 256               
    30 : load.i64                 i0, i15, 256               
    31 : load.i64                 i0, i0, i0                 
    32 : load.i64                 i15, i0, i0                
    33 : load.i64                 i0, i15, i0                
    34 : load.i64                 i0, i0, i15                
    35 : load.u32                 i0, i0                     
    36 : load.u32                 i0, i0, 256                
    37 : load.u32                 i15, i0, 256               
    38 : load.u32                 i0, i15, 256               
    39 : load.u32                 i0, i0, i0                 
    40 : load.u32                 i15, i0, i0                
    41 : load.u32                 i0, i15, i0                
    42 : load.u32                 i0, i0, i15                
    43 : load.i32                 i0, i0                     
    44 : load.i32                 i0, i0, 256                
    45 : load.i32                 i15, i0, 256               
    46 : load.i32                 i0, i15, 256               
    47 : load.i32                 i0, i0, i0                 
    48 : load.i32                 i15, i0, i0                
    49 : load.i32                 i0, i15, i0                
    50 : load.i32                 i0, i0, i15                
    51 : load.u16                 i0, i0                     
    52 : load.u16                 i0, i0, 256                
    53 : load.u16                 i15, i0, 256               
    54 : load.u16                 i0, i15, 256               
    55 : load.u16                 i0, i0, i0                 
    56 : load.u16                 i15, i0, i0                
    57 : load.u16                 i0, i15, i0                
    58 : load.u16                 i0, i0, i15                
    59 : load.i16                 i0, i0                     
    60 : load.i16                 i0, i0, 256                
    61 : load.i16                 i15, i0, 256               
    62 : load.i16                 i0, i15, 256               
    63 : load.i16                 i0, i0, i0                 
    64 : load.i16                 i15, i0, i0                
    65 : load.i16                 i0, i15, i0                
    66 : load.i16                 i0, i0, i15                
    67 : load.u8                  i0, i0                     
    68 : load.u8                  i0, i0, 256                
    69 : load.u8                  i15, i0, 256               
    70 : load.u8                  i0, i15, 256               
    71 : load.u8                  i0, i0, i0                 
    72 : load.u8                  i15, i0, i0                
    73 : load.u8                  i0, i15, i0                
    74 : load.u8                  i0, i0, i15                
    75 : load.i8                  i0, i0                     
    76 : load.i8                  i0, i0, 256                
    77 : load.i8                  i15, i0, 256               
    78 : load.i8                  i0, i15, 256               
    79 : load.i8                  i0, i0, i0                 
    80 : load.i8                  i15, i0, i0                
    81 : load.i8                  i0, i15, i0                
    82 : load.i8                  i0, i0, i15                
    83 : select                   i0, 35, i0, i0             
    84 : select                   i0, 34, i0, i0             
    85 : select                   i0, 31, i0, i0             
    86 : select                   i0, 28, i0, i0             
    87 : select                   i0, 32, i0, i0             
    88 : select                   i0, 30, i0, i0             
    89 : select                   i0, 36, i0, i0             
    90 : select                   i0, 37, i0, i0             
    91 : select                   i15, 35, i0, i0            
    92 : select                   i0, 35, i15, i0            
    93 : select                   i0, 35, i0, i15            
    94 : arm_cinc                 i0, i0, 35                 
    95 : arm_cinc                 i0, i0, 34                 
    96 : arm_cinc                 i0, i0, 31                 
    97 : arm_cinc                 i0, i0, 28                 
    98 : arm_cinc                 i0, i0, 32                 
    99 : arm_cinc                 i0, i0, 30                 
   100 : arm_cinc                 i0, i0, 36                 
   101 : arm_cinc                 i0, i0, 37                 
   102 : arm_cinc                 i15, i0, 35                
   103 : arm_cinc                 i0, i15, 35                
   104 : arm_cneg                 i0, i0, 35                 
   105 : arm_cneg                 i0, i0, 34                 
   106 : arm_cneg                 i0, i0, 31                 
   107 : arm_cneg                 i0, i0, 28                 
   108 : arm_cneg                 i0, i0, 32                 
   109 : arm_cneg                 i0, i0, 30                 
   110 : arm_cneg                 i0, i0, 36                 
   111 : arm_cneg                 i0, i0, 37                 
   112 : arm_cneg                 i15, i0, 35                
   113 : arm_cneg                 i0, i15, 35                
   114 : sar                      i0, i0, 63                 
   115 : sar                      i15, i0, 63                
   116 : sar                      i0, i15, 63                
   117 : sar                      i0, i0, i0                 
   118 : sar                      i15, i0, i0                
   119 : sar                      i0, i15, i0                
   120 : sar                      i0, i0, i15                
   121 : shr                      i0, i0, 63                 
   122 : shr                      i15, i0, 63                
   123 : shr                      i0, i15, 63                
   124 : shr                      i0, i0, i0                 
   125 : shr                      i15, i0, i0                
   126 : shr                      i0, i15, i0                
   127 : shr                      i0, i0, i15                
   128 : shl                      i0, i0, 63                 
   129 : shl                      i0, i0, 31                 
   130 : shl                      i0, i0, 15                 
   131 : shl                      i0, i0, 7                  
   132 : shl                      i0, i0, 1                  
   133 : shl                      i0, i0, 0                  
   134 : shl                      i15, i0, 63                
   135 : shl                      i0, i15, 63                
   136 : shl                      i0, i0, i0                 
   137 : shl                      i15, i0, i0                
   138 : shl                      i0, i15, i0                
   139 : shl                      i0, i0, i15                
   140 : and                      i0, i0, i0                 
   141 : and                      i15, i0, i0                
   142 : and                      i0, i15, i0                
   143 : and                      i0, i0, i15                
   144 : and                      i0, i0, 255                
   145 : and                      i0, i0, 65280              
   146 : and                      i0, i0, -72057594037927936 
   147 : and                      i15, i0, 255               
   148 : and                      i0, i15, 255               
   149 : or                       i0, i0, i0                 
   150 : or                       i15, i0, i0                
   151 : or                       i0, i15, i0                
   152 : or                       i0, i0, i15                
   153 : or                       i0, i0, 255                
   154 : or                       i0, i0, 65280              
   155 : or                       i0, i0, -72057594037927936 
   156 : or                       i15, i0, 255               
   157 : or                       i0, i15, 255               
   158 : xor                      i0, i0, i0                 
   159 : xor                      i15, i0, i0                
   160 : xor                      i0, i15, i0                
   161 : xor                      i0, i0, i15                
   162 : xor                      i0, i0, 255                
   163 : xor                      i0, i0, 65280              
   164 : xor                      i0, i0, -72057594037927936 
   165 : xor                      i15, i0, 255               
   166 : xor                      i0, i15, 255               
   167 : not                      i0, i0                     
   168 : not                      i15, i0                    
   169 : not                      i0, i15                    
   170 : neg                      i0, i0                     
   171 : neg                      i15, i0                    
   172 : neg                      i0, i15                    
   173 : vld.u64                  v0, i0                     
   174 : vld.u64                  v0, i0, 32784              
   175 : vld.u64                  v31, i0                    
   176 : vld.u64                  v0, i15                    
   177 : vld.u64                  v0, i0, i0                 
   178 : vld.u64                  v31, i0, i0                
   179 : vld.u64                  v0, i15, i0                
   180 : vld.u64                  v0, i0, i15                
   181 : vst.u64                  i0, v0                     
   182 : vst.u64                  i0, 32784, v0              
   183 : vst.u64                  i0, v31                    
   184 : vst.u64                  i15, v0                    
   185 : vst.u64                  i0, i0, v0                 
   186 : vst.u64                  i0, i0, v31                
   187 : vst.u64                  i15, i0, v0                
   188 : vst.u64                  i0, i15, v0                
   189 : add.u8                   v0, v0, v0                 
   190 : add.u8                   v31, v0, v0                
   191 : add.u8                   v0, v31, v0                
   192 : add.u8                   v0, v0, v31                
   193 : add.u16                  v0, v0, v0                 
   194 : add.u16                  v31, v0, v0                
   195 : add.u16                  v0, v31, v0                
   196 : add.u16                  v0, v0, v31                
   197 : add.u32                  v0, v0, v0                 
   198 : add.u32                  v31, v0, v0                
   199 : add.u32                  v0, v31, v0                
   200 : add.u32                  v0, v0, v31                
   201 : add.u64                  v0, v0, v0                 
   202 : add.u64                  v31, v0, v0                
   203 : add.u64                  v0, v31, v0                
   204 : add.u64                  v0, v0, v31                
   205 : add.fp16                 v0, v0, v0                 
   206 : add.fp16                 v31, v0, v0                
   207 : add.fp16                 v0, v31, v0                
   208 : add.fp16                 v0, v0, v31                
   209 : add.fp32                 v0, v0, v0                 
   210 : add.fp32                 v31, v0, v0                
   211 : add.fp32                 v0, v31, v0                
   212 : add.fp32                 v0, v0, v31                
   213 : add.fp64                 v0, v0, v0                 
   214 : add.fp64                 v31, v0, v0                
   215 : add.fp64                 v0, v31, v0                
   216 : add.fp64                 v0, v0, v31                
   217 : sub.u8                   v0, v0, v0                 
   218 : sub.u8                   v31, v0, v0                
   219 : sub.u8                   v0, v31, v0                
   220 : sub.u8                   v0, v0, v31                
   221 : sub.u16                  v0, v0, v0                 
   222 : sub.u16                  v31, v0, v0                
   223 : sub.u16                  v0, v31, v0                
   224 : sub.u16                  v0, v0, v31                
   225 : sub.u32                  v0, v0, v0                 
   226 : sub.u32                  v31, v0, v0                
   227 : sub.u32                  v0, v31, v0                
   228 : sub.u32                  v0, v0, v31                
   229 : sub.u64                  v0, v0, v0                 
   230 : sub.u64                  v31, v0, v0                
   231 : sub.u64                  v0, v31, v0                
   232 : sub.u64                  v0, v0, v31                
   233 : sub.fp16                 v0, v0, v0                 
   234 : sub.fp16                 v31, v0, v0                
   235 : sub.fp16                 v0, v31, v0                
   236 : sub.fp16                 v0, v0, v31                
   237 : sub.fp32                 v0, v0, v0                 
   238 : sub.fp32                 v31, v0, v0                
   239 : sub.fp32                 v0, v31, v0                
   240 : sub.fp32                 v0, v0, v31                
   241 : sub.fp64                 v0, v0, v0                 
   242 : sub.fp64                 v31, v0, v0                
   243 : sub.fp64                 v0, v31, v0                
   244 : sub.fp64                 v0, v0, v31                
   245 : mul.u8                   v0, v0, v0                 
   246 : mul.u8                   v31, v0, v0                
   247 : mul.u8                   v0, v31, v0                
   248 : mul.u8                   v0, v0, v31                
   249 : mul.u16                  v0, v0, v0                 
   250 : mul.u16                  v31, v0, v0                
   251 : mul.u16                  v0, v31, v0                
   252 : mul.u16                  v0, v0, v31                
   253 : mul.u32                  v0, v0, v0                 
   254 : mul.u32                  v31, v0, v0                
   255 : mul.u32                  v0, v31, v0                
   256 : mul.u32                  v0, v0, v31                
   257 : mul.fp16                 v0, v0, v0                 
   258 : mul.fp16                 v31, v0, v0                
   259 : mul.fp16                 v0, v31, v0                
   260 : mul.fp16                 v0, v0, v31                
   261 : mul.fp32                 v0, v0, v0                 
   262 : mul.fp32                 v31, v0, v0                
   263 : mul.fp32                 v0, v31, v0                
   264 : mul.fp32                 v0, v0, v31                
   265 : mul.fp64                 v0, v0, v0                 
   266 : mul.fp64                 v31, v0, v0                
   267 : mul.fp64                 v0, v31, v0                
   268 : mul.fp64                 v0, v0, v31                
   269 : mul.fp32                 v0, v0, v0                 
   270 : mul.fp32                 v31, v0, v0                
   271 : mul.fp32                 v0, v31, v0                
   272 : mul.fp32                 v0, v0, v31                
   273 : mul.fp64                 v0, v0, v0                 
   274 : mul.fp64                 v31, v0, v0                
   275 : mul.fp64                 v0, v31, v0                
   276 : mul.fp64                 v0, v0, v31                
   277 : mul.fp16                 v0, v0, v0                 
   278 : mul.fp16                 v31, v0, v0                
   279 : mul.fp16                 v0, v31, v0                
   280 : mul.fp16                 v0, v0, v31                
   281 : sal.u8                   v0, v0, 1                  
   282 : sal.u8                   v31, v0, 1                 
   283 : sal.u8                   v0, v31, 1                 
   284 : sal.u8                   v0, v0, 7                  
   285 : sal.u16                  v0, v0, 1                  
   286 : sal.u16                  v31, v0, 1                 
   287 : sal.u16                  v0, v31, 1                 
   288 : sal.u16                  v0, v0, 15                 
   289 : sal.u32                  v0, v0, 1                  
   290 : sal.u32                  v31, v0, 1                 
   291 : sal.u32                  v0, v31, 1                 
   292 : sal.u32                  v0, v0, 31                 
   293 : sal.u64                  v0, v0, 1                  
   294 : sal.u64                  v31, v0, 1                 
   295 : sal.u64                  v0, v31, 1                 
   296 : sal.u64                  v0, v0, 63                 
   297 : sal.u8                   v0, v0, v0                 
   298 : sal.u8                   v31, v0, v0                
   299 : sal.u8                   v0, v31, v0                
   300 : sal.u8                   v0, v0, v31                
   301 : sal.u16                  v0, v0, v0                 
   302 : sal.u16                  v31, v0, v0                
   303 : sal.u16                  v0, v31, v0                
   304 : sal.u16                  v0, v0, v31                
   305 : sal.u32                  v0, v0, v0                 
   306 : sal.u32                  v31, v0, v0                
   307 : sal.u32                  v0, v31, v0                
   308 : sal.u32                  v0, v0, v31                
   309 : sal.u64                  v0, v0, v0                 
   310 : sal.u64                  v31, v0, v0                
   311 : sal.u64                  v0, v31, v0                
   312 : sal.u64                  v0, v0, v31                
   313 : sal.i8                   v0, v0, v0                 
   314 : sal.i8                   v31, v0, v0                
   315 : sal.i8                   v0, v31, v0                
   316 : sal.i8                   v0, v0, v31                
   317 : sal.i16                  v0, v0, v0                 
   318 : sal.i16                  v31, v0, v0                
   319 : sal.i16                  v0, v31, v0                
   320 : sal.i16                  v0, v0, v31                
   321 : sal.i32                  v0, v0, v0                 
   322 : sal.i32                  v31, v0, v0                
   323 : sal.i32                  v0, v31, v0                
   324 : sal.i32                  v0, v0, v31                
   325 : sal.i64                  v0, v0, v0                 
   326 : sal.i64                  v31, v0, v0                
   327 : sal.i64                  v0, v31, v0                
   328 : sal.i64                  v0, v0, v31                
   329 : sar.u8                   v0, v0, 1                  
   330 : sar.u8                   v31, v0, 1                 
   331 : sar.u8                   v0, v31, 1                 
   332 : sar.u8                   v0, v0, 7                  
   333 : sar.u16                  v0, v0, 1                  
   334 : sar.u16                  v31, v0, 1                 
   335 : sar.u16                  v0, v31, 1                 
   336 : sar.u16                  v0, v0, 15                 
   337 : sar.u32                  v0, v0, 1                  
   338 : sar.u32                  v31, v0, 1                 
   339 : sar.u32                  v0, v31, 1                 
   340 : sar.u32                  v0, v0, 31                 
   341 : sar.u64                  v0, v0, 1                  
   342 : sar.u64                  v31, v0, 1                 
   343 : sar.u64                  v0, v31, 1                 
   344 : sar.u64                  v0, v0, 63                 
   345 : sar.i8                   v0, v0, 1                  
   346 : sar.i8                   v31, v0, 1                 
   347 : sar.i8                   v0, v31, 1                 
   348 : sar.i8                   v0, v0, 7                  
   349 : sar.i16                  v0, v0, 1                  
   350 : sar.i16                  v31, v0, 1                 
   351 : sar.i16                  v0, v31, 1                 
   352 : sar.i16                  v0, v0, 15                 
   353 : sar.i32                  v0, v0, 1                  
   354 : sar.i32                  v31, v0, 1                 
   355 : sar.i32                  v0, v31, 1                 
   356 : sar.i32                  v0, v0, 31                 
   357 : sar.i64                  v0, v0, 1                  
   358 : sar.i64                  v31, v0, 1                 
   359 : sar.i64                  v0, v31, 1                 
   360 : sar.i64                  v0, v0, 63                 
   361 : and                      v0, v0, v0                 
   362 : and                      v31, v0, v0                
   363 : and                      v0, v31, v0                
   364 : and                      v0, v0, v31                
   365 : or                       v0, v0, v0                 
   366 : or                       v31, v0, v0                
   367 : or                       v0, v31, v0                
   368 : or                       v0, v0, v31                
   369 : xor                      v0, v0, v0                 
   370 : xor                      v31, v0, v0                
   371 : xor                      v0, v31, v0                
   372 : xor                      v0, v0, v31                
   373 : not                      v0, v0                     
   374 : not                      v31, v0                    
   375 : not                      v0, v31                    
   376 : mov                      v0, 0                      
   377 : mov                      v31, 0                     
   378 : mov                      v0, 255                    
   379 : mov                      v0, 0                      
   380 : mov                      v31, 0                     
   381 : mov                      v0, 255                    
   382 : mov                      v0, 0                      
   383 : mov                      v31, 0                     
   384 : mov                      v0, 255                    
   385 : mov                      v0, -1                     
   386 : mov                      v31, -1                    
   387 : mov                      v0, -256                   
   388 : mov                      v0, -1                     
   389 : mov                      v31, -1                    
   390 : mov                      v0, -256                   
   391 : min.fp16                 v0, v0, v0                 
   392 : min.fp16                 v31, v0, v0                
   393 : min.fp16                 v0, v31, v0                
   394 : min.fp16                 v0, v0, v31                
   395 : min.fp32                 v0, v0, v0                 
   396 : min.fp32                 v31, v0, v0                
   397 : min.fp32                 v0, v31, v0                
   398 : min.fp32                 v0, v0, v31                
   399 : min.fp64                 v0, v0, v0                 
   400 : min.fp64                 v31, v0, v0                
   401 : min.fp64                 v0, v31, v0                
   402 : min.fp64                 v0, v0, v31                
   403 : max.fp16                 v0, v0, v0                 
   404 : max.fp16                 v31, v0, v0                
   405 : max.fp16                 v0, v31, v0                
   406 : max.fp16                 v0, v0, v31                
   407 : max.fp32                 v0, v0, v0                 
   408 : max.fp32                 v31, v0, v0                
   409 : max.fp32                 v0, v31, v0                
   410 : max.fp32                 v0, v0, v31                
   411 : max.fp64                 v0, v0, v0                 
   412 : max.fp64                 v31, v0, v0                
   413 : max.fp64                 v0, v31, v0                
   414 : max.fp64                 v0, v0, v31                
   415 : fma.fp16                 v0, v0, v0, v0             
   416 : fma.fp16                 v31, v31, v0, v0           
   417 : fma.fp16                 v0, v0, v31, v0            
   418 : fma.fp16                 v0, v0, v0, v31            
   419 : fma.fp32                 v0, v0, v0, v0             
   420 : fma.fp32                 v31, v31, v0, v0           
   421 : fma.fp32                 v0, v0, v31, v0            
   422 : fma.fp32                 v0, v0, v0, v31            
   423 : fma.fp64                 v0, v0, v0, v0             
   424 : fma.fp64                 v31, v31, v0, v0           
   425 : fma.fp64                 v0, v0, v31, v0            
   426 : fma.fp64                 v0, v0, v0, v31            
   427 : trunc.fp16_i16           v0, v0                     
   428 : trunc.fp16_i16           v31, v0                    
   429 : trunc.fp16_i16           v0, v31                    
   430 : trunc.fp32_i32           v0, v0                     
   431 : trunc.fp32_i32           v31, v0                    
   432 : trunc.fp32_i32           v0, v31                    
   433 : trunc.fp64_i64           v0, v0                     
   434 : trunc.fp64_i64           v31, v0                    
   435 : trunc.fp64_i64           v0, v31                    
   436 : trunc.fp16_u16           v0, v0                     
   437 : trunc.fp16_u16           v31, v0                    
   438 : trunc.fp16_u16           v0, v31                    
   439 : trunc.fp32_u32           v0, v0                     
   440 : trunc.fp32_u32           v31, v0                    
   441 : trunc.fp32_u32           v0, v31                    
   442 : trunc.fp64_u64           v0, v0                     
   443 : trunc.fp64_u64           v31, v0                    
   444 : trunc.fp64_u64           v0, v31                    
   445 : floor.fp16_i16           v0, v0                     
   446 : floor.fp16_i16           v31, v0                    
   447 : floor.fp16_i16           v0, v31                    
   448 : floor.fp32_i32           v0, v0                     
   449 : floor.fp32_i32           v31, v0                    
   450 : floor.fp32_i32           v0, v31                    
   451 : floor.fp64_i64           v0, v0                     
   452 : floor.fp64_i64           v31, v0                    
   453 : floor.fp64_i64           v0, v31                    
   454 : floor.fp16_u16           v0, v0                     
   455 : floor.fp16_u16           v31, v0                    
   456 : floor.fp16_u16           v0, v31                    
   457 : floor.fp32_u32           v0, v0                     
   458 : floor.fp32_u32           v31, v0                    
   459 : floor.fp32_u32           v0, v31                    
   460 : floor.fp64_u64           v0, v0                     
   461 : floor.fp64_u64           v31, v0                    
   462 : floor.fp64_u64           v0, v31                    
   463 : cast.i16_fp16            v0, v0                     
   464 : cast.i16_fp16            v31, v0                    
   465 : cast.i16_fp16            v0, v31                    
   466 : cast.i32_fp32            v0, v0                     
   467 : cast.i32_fp32            v31, v0                    
   468 : cast.i32_fp32            v0, v31                    
   469 : cast.i64_fp64            v0, v0                     
   470 : cast.i64_fp64            v31, v0                    
   471 : cast.i64_fp64            v0, v31                    
   472 : cast.u16_fp16            v0, v0                     
   473 : cast.u16_fp16            v31, v0                    
   474 : cast.u16_fp16            v0, v31                    
   475 : cast.u32_fp32            v0, v0                     
   476 : cast.u32_fp32            v31, v0                    
   477 : cast.u32_fp32            v0, v31                    
   478 : cast.u64_fp64            v0, v0                     
   479 : cast.u64_fp64            v31, v0                    
   480 : cast.u64_fp64            v0, v31                    
   481 : gt.u16                   v0, v0, v0                 
   482 : gt.u16                   v31, v0, v0                
   483 : gt.u16                   v0, v31, v0                
   484 : gt.u16                   v0, v0, v31                
   485 : gt.u32                   v0, v0, v0                 
   486 : gt.u32                   v31, v0, v0                
   487 : gt.u32                   v0, v31, v0                
   488 : gt.u32                   v0, v0, v31                
   489 : gt.u64                   v0, v0, v0                 
   490 : gt.u64                   v31, v0, v0                
   491 : gt.u64                   v0, v31, v0                
   492 : gt.u64                   v0, v0, v31                
   493 : ge.u16                   v0, v0, v0                 
   494 : ge.u16                   v31, v0, v0                
   495 : ge.u16                   v0, v31, v0                
   496 : ge.u16                   v0, v0, v31                
   497 : ge.u32                   v0, v0, v0                 
   498 : ge.u32                   v31, v0, v0                
   499 : ge.u32                   v0, v31, v0                
   500 : ge.u32                   v0, v0, v31                
   501 : ge.u64                   v0, v0, v0                 
   502 : ge.u64                   v31, v0, v0                
   503 : ge.u64                   v0, v31, v0                
   504 : ge.u64                   v0, v0, v31                
   505 : eq.u16                   v0, v0, v0                 
   506 : eq.u16                   v31, v0, v0                
   507 : eq.u16                   v0, v31, v0                
   508 : eq.u16                   v0, v0, v31                
   509 : eq.u32                   v0, v0, v0                 
   510 : eq.u32                   v31, v0, v0                
   511 : eq.u32                   v0, v31, v0                
   512 : eq.u32                   v0, v0, v31                
   513 : eq.u64                   v0, v0, v0                 
   514 : eq.u64                   v31, v0, v0                
   515 : eq.u64                   v0, v31, v0                
   516 : eq.u64                   v0, v0, v31                
   517 : neg.i8                   v0, v0                     
   518 : neg.i8                   v31, v0                    
   519 : neg.i8                   v0, v31                    
   520 : neg.i16                  v0, v0                     
   521 : neg.i16                  v31, v0                    
   522 : neg.i16                  v0, v31                    
   523 : neg.i32                  v0, v0                     
   524 : neg.i32                  v31, v0                    
   525 : neg.i32                  v0, v31                    
   526 : neg.i64                  v0, v0                     
   527 : neg.i64                  v31, v0                    
   528 : neg.i64                  v0, v31                    
   529 : neg.fp16                 v0, v0                     
   530 : neg.fp16                 v31, v0                    
   531 : neg.fp16                 v0, v31                    
   532 : neg.fp32                 v0, v0                     
   533 : neg.fp32                 v31, v0                    
   534 : neg.fp32                 v0, v31                    
   535 : neg.fp64                 v0, v0                     
   536 : neg.fp64                 v31, v0                    
   537 : neg.fp64                 v0, v31                    
   538 : arm_movk                 i0, 0, 16                  
   539 : arm_movk                 i15, 0, 16                 
   540 : arm_movk                 i0, 65535, 16              
   541 : arm_movk                 i0, 0, 32                  
   542 : arm_movk                 i15, 0, 32                 
   543 : arm_movk                 i0, 65535, 32              
   544 : arm_movk                 i0, 0, 48                  
   545 : arm_movk                 i15, 0, 48                 
   546 : arm_movk                 i0, 65535, 48              
   547 : broadcast.i8             v0, i0                     
   548 : broadcast.i8             v31, i0                    
   549 : broadcast.i8             v0, i15                    
   550 : broadcast.i16            v0, i0                     
   551 : broadcast.i16            v31, i0                    
   552 : broadcast.i16            v0, i15                    
   553 : broadcast.i32            v0, i0                     
   554 : broadcast.i32            v31, i0                    
   555 : broadcast.i32            v0, i15                    
   556 : broadcast.i64            v0, i0                     
   557 : broadcast.i64            v31, i0                    
   558 : broadcast.i64            v0, i15                    
   559 : broadcast.u8             v0, v0, 0                  
   560 : broadcast.u8             v31, v0, 0                 
   561 : broadcast.u8             v0, v31, 0                 
   562 : broadcast.u8             v0, v0, 15                 
   563 : broadcast.u16            v0, v0, 0                  
   564 : broadcast.u16            v31, v0, 0                 
   565 : broadcast.u16            v0, v31, 0                 
   566 : broadcast.u16            v0, v0, 7                  
   567 : broadcast.u32            v0, v0, 0                  
   568 : broadcast.u32            v31, v0, 0                 
   569 : broadcast.u32            v0, v31, 0                 
   570 : broadcast.u32            v0, v0, 3                  
   571 : broadcast.u64            v0, v0, 0                  
   572 : broadcast.u64            v31, v0, 0                 
   573 : broadcast.u64            v0, v31, 0                 
   574 : broadcast.u64            v0, v0, 1                  
   575 : cast.i16.from.i8.low     v0, v0                     
   576 : cast.i16.from.i8.low     v31, v0                    
   577 : cast.i16.from.i8.low     v0, v31                    
   578 : cast.i32.from.i16.low    v0, v0                     
   579 : cast.i32.from.i16.low    v31, v0                    
   580 : cast.i32.from.i16.low    v0, v31                    
   581 : cast.i64.from.i32.low    v0, v0                     
   582 : cast.i64.from.i32.low    v31, v0                    
   583 : cast.i64.from.i32.low    v0, v31                    
   584 : cast.u16.from.u8.low     v0, v0                     
   585 : cast.u16.from.u8.low     v31, v0                    
   586 : cast.u16.from.u8.low     v0, v31                    
   587 : cast.u32.from.u16.low    v0, v0                     
   588 : cast.u32.from.u16.low    v31, v0                    
   589 : cast.u32.from.u16.low    v0, v31                    
   590 : cast.u64.from.u32.low    v0, v0                     
   591 : cast.u64.from.u32.low    v31, v0                    
   592 : cast.u64.from.u32.low    v0, v31                    
   593 : cast.i16.from.i8.high    v0, v0                     
   594 : cast.i16.from.i8.high    v31, v0                    
   595 : cast.i16.from.i8.high    v0, v31                    
   596 : cast.i32.from.i16.high   v0, v0                     
   597 : cast.i32.from.i16.high   v31, v0                    
   598 : cast.i32.from.i16.high   v0, v31                    
   599 : cast.i64.from.i32.high   v0, v0                     
   600 : cast.i64.from.i32.high   v31, v0                    
   601 : cast.i64.from.i32.high   v0, v31                    
   602 : cast.u16.from.u8.high    v0, v0                     
   603 : cast.u16.from.u8.high    v31, v0                    
   604 : cast.u16.from.u8.high    v0, v31                    
   605 : cast.u32.from.u16.high   v0, v0                     
   606 : cast.u32.from.u16.high   v31, v0                    
   607 : cast.u32.from.u16.high   v0, v31                    
   608 : cast.u64.from.u32.high   v0, v0                     
   609 : cast.u64.from.u32.high   v31, v0                    
   610 : cast.u64.from.u32.high   v0, v31                    
   611 : shrink.i8.from.i16.low   v0, v0                     
   612 : shrink.i8.from.i16.low   v31, v0                    
   613 : shrink.i8.from.i16.low   v0, v31                    
   614 : shrink.i16.from.i32.low  v0, v0                     
   615 : shrink.i16.from.i32.low  v31, v0                    
   616 : shrink.i16.from.i32.low  v0, v31                    
   617 : shrink.i32.from.i64.low  v0, v0                     
   618 : shrink.i32.from.i64.low  v31, v0                    
   619 : shrink.i32.from.i64.low  v0, v31                    
   620 : shrink.i8.from.i16.high  v0, v0                     
   621 : shrink.i8.from.i16.high  v31, v0                    
   622 : shrink.i8.from.i16.high  v0, v31                    
   623 : shrink.i16.from.i32.high v0, v0                     
   624 : shrink.i16.from.i32.high v31, v0                    
   625 : shrink.i16.from.i32.high v0, v31                    
   626 : shrink.i32.from.i64.high v0, v0                     
   627 : shrink.i32.from.i64.high v31, v0                    
   628 : shrink.i32.from.i64.high v0, v31                    
   629 : reduce.max.i8            v0, v0                     
   630 : reduce.max.i8            v31, v0                    
   631 : reduce.max.i8            v0, v31                    
   632 : reduce.max.i16           v0, v0                     
   633 : reduce.max.i16           v31, v0                    
   634 : reduce.max.i16           v0, v31                    
   635 : reduce.max.i32           v0, v0                     
   636 : reduce.max.i32           v31, v0                    
   637 : reduce.max.i32           v0, v31                    
   638 : reduce.max.u8            v0, v0                     
   639 : reduce.max.u8            v31, v0                    
   640 : reduce.max.u8            v0, v31                    
   641 : reduce.max.u16           v0, v0                     
   642 : reduce.max.u16           v31, v0                    
   643 : reduce.max.u16           v0, v31                    
   644 : reduce.max.u32           v0, v0                     
   645 : reduce.max.u32           v31, v0                    
   646 : reduce.max.u32           v0, v31                    
   647 : reduce.max.fp16          v0, v0                     
   648 : reduce.max.fp16          v31, v0                    
   649 : reduce.max.fp16          v0, v31                    
   650 : reduce.max.fp32          v0, v0                     
   651 : reduce.max.fp32          v31, v0                    
   652 : reduce.max.fp32          v0, v31                    
   653 : reduce.max.i8            v0, v0                     
   654 : reduce.max.i8            v31, v0                    
   655 : reduce.max.i8            v0, v31                    
   656 : reduce.max.i16           v0, v0                     
   657 : reduce.max.i16           v31, v0                    
   658 : reduce.max.i16           v0, v31                    
   659 : reduce.max.i32           v0, v0                     
   660 : reduce.max.i32           v31, v0                    
   661 : reduce.max.i32           v0, v31                    
   662 : reduce.max.u8            v0, v0                     
   663 : reduce.max.u8            v31, v0                    
   664 : reduce.max.u8            v0, v31                    
   665 : reduce.max.u16           v0, v0                     
   666 : reduce.max.u16           v31, v0                    
   667 : reduce.max.u16           v0, v31                    
   668 : reduce.max.u32           v0, v0                     
   669 : reduce.max.u32           v31, v0                    
   670 : reduce.max.u32           v0, v31                    
   671 : reduce.max.fp16          v0, v0                     
   672 : reduce.max.fp16          v31, v0                    
   673 : reduce.max.fp16          v0, v31                    
   674 : reduce.max.fp32          v0, v0                     
   675 : reduce.max.fp32          v31, v0                    
   676 : reduce.max.fp32          v0, v31                    
   677 : vld_lane.i8              v0, 0, i0                  
   678 : vld_lane.i8              v31, 0, i0                 
   679 : vld_lane.i8              v0, 15, i0                 
   680 : vld_lane.i8              v0, 0, i15                 
   681 : vld_lane.i16             v0, 0, i0                  
   682 : vld_lane.i16             v31, 0, i0                 
   683 : vld_lane.i16             v0, 7, i0                  
   684 : vld_lane.i16             v0, 0, i15                 
   685 : vld_lane.i32             v0, 0, i0                  
   686 : vld_lane.i32             v31, 0, i0                 
   687 : vld_lane.i32             v0, 3, i0                  
   688 : vld_lane.i32             v0, 0, i15                 
   689 : vld_lane.i64             v0, 0, i0                  
   690 : vld_lane.i64             v31, 0, i0                 
   691 : vld_lane.i64             v0, 1, i0                  
   692 : vld_lane.i64             v0, 0, i15                 
   693 : vld_deinterleave2.i8     v0, v1, i0                 
   694 : vld_deinterleave2.i8     v31, v0, i0                
   695 : vld_deinterleave2.i8     v0, v1, i15                
   696 : vld_deinterleave2.i16    v0, v1, i0                 
   697 : vld_deinterleave2.i16    v31, v0, i0                
   698 : vld_deinterleave2.i16    v0, v1, i15                
   699 : vld_deinterleave2.i32    v0, v1, i0                 
   700 : vld_deinterleave2.i32    v31, v0, i0                
   701 : vld_deinterleave2.i32    v0, v1, i15                
   702 : vld_deinterleave2.i64    v0, v1, i0                 
   703 : vld_deinterleave2.i64    v31, v0, i0                
   704 : vld_deinterleave2.i64    v0, v1, i15                
   705 : vst_lane.i8              i0, v0, 0                  
   706 : vst_lane.i8              i0, v31, 0                 
   707 : vst_lane.i8              i0, v0, 15                 
   708 : vst_lane.i8              i15, v0, 0                 
   709 : vst_lane.i16             i0, v0, 0                  
   710 : vst_lane.i16             i0, v31, 0                 
   711 : vst_lane.i16             i0, v0, 7                  
   712 : vst_lane.i16             i15, v0, 0                 
   713 : vst_lane.i32             i0, v0, 0                  
   714 : vst_lane.i32             i0, v31, 0                 
   715 : vst_lane.i32             i0, v0, 3                  
   716 : vst_lane.i32             i15, v0, 0                 
   717 : vst_lane.i64             i0, v0, 0                  
   718 : vst_lane.i64             i0, v31, 0                 
   719 : vst_lane.i64             i0, v0, 1                  
   720 : vst_lane.i64             i15, v0, 0                 
   721 : ext.i8                   v0, v0, v0, 0              
   722 : ext.i8                   v31, v0, v0, 0             
   723 : ext.i8                   v0, v31, v0, 0             
   724 : ext.i8                   v0, v0, v31, 0             
   725 : ext.i8                   v0, v0, v0, 15             
   726 : ext.i16                  v0, v0, v0, 0              
   727 : ext.i16                  v31, v0, v0, 0             
   728 : ext.i16                  v0, v31, v0, 0             
   729 : ext.i16                  v0, v0, v31, 0             
   730 : ext.i16                  v0, v0, v0, 7              
   731 : ext.i32                  v0, v0, v0, 0              
   732 : ext.i32                  v31, v0, v0, 0             
   733 : ext.i32                  v0, v31, v0, 0             
   734 : ext.i32                  v0, v0, v31, 0             
   735 : ext.i32                  v0, v0, v0, 3              
   736 : ext.i64                  v0, v0, v0, 0              
   737 : ext.i64                  v31, v0, v0, 0             
   738 : ext.i64                  v0, v31, v0, 0             
   739 : ext.i64                  v0, v0, v31, 0             
   740 : ext.i64                  v0, v0, v0, 1              
   741 : fma.fp16                 v0, v0, v0, v0, 0          
   742 : fma.fp16                 v31, v31, v0, v0, 0        
   743 : fma.fp16                 v0, v0, v31, v0, 0         
   744 : fma.fp16                 v0, v0, v0, v15, 0         
   745 : fma.fp16                 v0, v0, v0, v0, 7          
   746 : fma.fp32                 v0, v0, v0, v0, 0          
   747 : fma.fp32                 v31, v31, v0, v0, 0        
   748 : fma.fp32                 v0, v0, v31, v0, 0         
   749 : fma.fp32                 v0, v0, v0, v31, 0         
   750 : fma.fp32                 v0, v0, v0, v0, 3          
   751 : fma.fp64                 v0, v0, v0, v0, 0          
   752 : fma.fp64                 v31, v31, v0, v0, 0        
   753 : fma.fp64                 v0, v0, v31, v0, 0         
   754 : fma.fp64                 v0, v0, v0, v31, 0         
   755 : fma.fp64                 v0, v0, v0, v0, 1          
   756 : gt.u8                    v0, v0, v0                 
   757 : gt.u8                    v31, v0, v0                
   758 : gt.u8                    v0, v31, v0                
   759 : gt.u8                    v0, v0, v31                
   760 : gt.u16                   v0, v0, v0                 
   761 : gt.u16                   v31, v0, v0                
   762 : gt.u16                   v0, v31, v0                
   763 : gt.u16                   v0, v0, v31                
   764 : gt.u32                   v0, v0, v0                 
   765 : gt.u32                   v31, v0, v0                
   766 : gt.u32                   v0, v31, v0                
   767 : gt.u32                   v0, v0, v31                
   768 : gt.u64                   v0, v0, v0                 
   769 : gt.u64                   v31, v0, v0                
   770 : gt.u64                   v0, v31, v0                
   771 : gt.u64                   v0, v0, v31                
   772 : ge.u8                    v0, v0, v0                 
   773 : ge.u8                    v31, v0, v0                
   774 : ge.u8                    v0, v31, v0                
   775 : ge.u8                    v0, v0, v31                
   776 : ge.u16                   v0, v0, v0                 
   777 : ge.u16                   v31, v0, v0                
   778 : ge.u16                   v0, v31, v0                
   779 : ge.u16                   v0, v0, v31                
   780 : ge.u32                   v0, v0, v0                 
   781 : ge.u32                   v31, v0, v0                
   782 : ge.u32                   v0, v31, v0                
   783 : ge.u32                   v0, v0, v31                
   784 : ge.u64                   v0, v0, v0                 
   785 : ge.u64                   v31, v0, v0                
   786 : ge.u64                   v0, v31, v0                
   787 : ge.u64                   v0, v0, v31                
   788 : eq.u8                    v0, v0, v0                 
   789 : eq.u8                    v31, v0, v0                
   790 : eq.u8                    v0, v31, v0                
   791 : eq.u8                    v0, v0, v31                
   792 : eq.u16                   v0, v0, v0                 
   793 : eq.u16                   v31, v0, v0                
   794 : eq.u16                   v0, v31, v0                
   795 : eq.u16                   v0, v0, v31                
   796 : eq.u32                   v0, v0, v0                 
   797 : eq.u32                   v31, v0, v0                
   798 : eq.u32                   v0, v31, v0                
   799 : eq.u32                   v0, v0, v31                
   800 : eq.u64                   v0, v0, v0                 
   801 : eq.u64                   v31, v0, v0                
   802 : eq.u64                   v0, v31, v0                
   803 : eq.u64                   v0, v0, v31                
   804 : ge.u8                    v0, v0, v0                 
   805 : ge.u8                    v31, v0, v0                
   806 : ge.u8                    v0, v31, v0                
   807 : ge.u8                    v0, v0, v31                
   808 : ge.u16                   v0, v0, v0                 
   809 : ge.u16                   v31, v0, v0                
   810 : ge.u16                   v0, v31, v0                
   811 : ge.u16                   v0, v0, v31                
   812 : ge.u32                   v0, v0, v0                 
   813 : ge.u32                   v31, v0, v0                
   814 : ge.u32                   v0, v31, v0                
   815 : ge.u32                   v0, v0, v31                
   816 : ge.u64                   v0, v0, v0                 
   817 : ge.u64                   v31, v0, v0                
   818 : ge.u64                   v0, v31, v0                
   819 : ge.u64                   v0, v0, v31                
   820 : gt.u8                    v0, v0, v0                 
   821 : gt.u8                    v31, v0, v0                
   822 : gt.u8                    v0, v31, v0                
   823 : gt.u8                    v0, v0, v31                
   824 : gt.u16                   v0, v0, v0                 
   825 : gt.u16                   v31, v0, v0                
   826 : gt.u16                   v0, v31, v0                
   827 : gt.u16                   v0, v0, v31                
   828 : gt.u32                   v0, v0, v0                 
   829 : gt.u32                   v31, v0, v0                
   830 : gt.u32                   v0, v31, v0                
   831 : gt.u32                   v0, v0, v31                
   832 : gt.u64                   v0, v0, v0                 
   833 : gt.u64                   v31, v0, v0                
   834 : gt.u64                   v0, v31, v0                
   835 : gt.u64                   v0, v0, v31                
   836 : getlane.u8               i0, v0, 0                  
   837 : getlane.u8               i15, v0, 0                 
   838 : getlane.u8               i0, v31, 0                 
   839 : getlane.u8               i0, v0, 15                 
   840 : getlane.u16              i0, v0, 0                  
   841 : getlane.u16              i15, v0, 0                 
   842 : getlane.u16              i0, v31, 0                 
   843 : getlane.u16              i0, v0, 7                  
   844 : getlane.u32              i0, v0, 0                  
   845 : getlane.u32              i15, v0, 0                 
   846 : getlane.u32              i0, v31, 0                 
   847 : getlane.u32              i0, v0, 3                  
   848 : getlane.u64              i0, v0, 0                  
   849 : getlane.u64              i15, v0, 0                 
   850 : getlane.u64              i0, v31, 0                 
   851 : getlane.u64              i0, v0, 1                  
   852 : setlane.u8               v0, 0, i0                  
   853 : setlane.u8               v31, 0, i0                 
   854 : setlane.u8               v0, 15, i0                 
   855 : setlane.u8               v0, 0, i15                 
   856 : setlane.u16              v0, 0, i0                  
   857 : setlane.u16              v31, 0, i0                 
   858 : setlane.u16              v0, 7, i0                  
   859 : setlane.u16              v0, 0, i15                 
   860 : setlane.u32              v0, 0, i0                  
   861 : setlane.u32              v31, 0, i0                 
   862 : setlane.u32              v0, 3, i0                  
   863 : setlane.u32              v0, 0, i15                 
   864 : setlane.u64              v0, 0, i0                  
   865 : setlane.u64              v31, 0, i0                 
   866 : setlane.u64              v0, 1, i0                  
   867 : setlane.u64              v0, 0, i15                 
   868 : setlane.u8               v0, 0, v0, 0               
   869 : setlane.u8               v31, 0, v0, 0              
   870 : setlane.u8               v0, 15, v0, 0              
   871 : setlane.u8               v0, 0, v31, 0              
   872 : setlane.u8               v0, 0, v0, 15              
   873 : setlane.u16              v0, 0, v0, 0               
   874 : setlane.u16              v31, 0, v0, 0              
   875 : setlane.u16              v0, 7, v0, 0               
   876 : setlane.u16              v0, 0, v31, 0              
   877 : setlane.u16              v0, 0, v0, 7               
   878 : setlane.u32              v0, 0, v0, 0               
   879 : setlane.u32              v31, 0, v0, 0              
   880 : setlane.u32              v0, 3, v0, 0               
   881 : setlane.u32              v0, 0, v31, 0              
   882 : setlane.u32              v0, 0, v0, 3               
   883 : setlane.u64              v0, 0, v0, 0               
   884 : setlane.u64              v31, 0, v0, 0              
   885 : setlane.u64              v0, 1, v0, 0               
   886 : setlane.u64              v0, 0, v31, 0              
   887 : setlane.u64              v0, 0, v0, 1               
   888 : select                   v0, v0, v0, v0             
   889 : select                   v31, v31, v0, v0           
   890 : select                   v0, v0, v31, v0            
   891 : select                   v0, v0, v0, v31            
