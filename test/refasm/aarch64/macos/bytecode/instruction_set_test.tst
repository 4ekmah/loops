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
    19 : arm_stp                  i0, 0, i0, i0              
    20 : arm_stp                  i15, 0, i0, i0             
    21 : arm_stp                  i0, 63, i0, i0             
    22 : arm_stp                  i0, -64, i0, i0            
    23 : arm_stp                  i0, 0, i15, i0             
    24 : arm_stp                  i0, 0, i0, i15             
    25 : load.u64                 i0, i0                     
    26 : load.u64                 i0, i0, 256                
    27 : load.u64                 i15, i0, 256               
    28 : load.u64                 i0, i15, 256               
    29 : load.u64                 i0, i0, i0                 
    30 : load.u64                 i15, i0, i0                
    31 : load.u64                 i0, i15, i0                
    32 : load.u64                 i0, i0, i15                
    33 : load.i64                 i0, i0                     
    34 : load.i64                 i0, i0, 256                
    35 : load.i64                 i15, i0, 256               
    36 : load.i64                 i0, i15, 256               
    37 : load.i64                 i0, i0, i0                 
    38 : load.i64                 i15, i0, i0                
    39 : load.i64                 i0, i15, i0                
    40 : load.i64                 i0, i0, i15                
    41 : load.u32                 i0, i0                     
    42 : load.u32                 i0, i0, 256                
    43 : load.u32                 i15, i0, 256               
    44 : load.u32                 i0, i15, 256               
    45 : load.u32                 i0, i0, i0                 
    46 : load.u32                 i15, i0, i0                
    47 : load.u32                 i0, i15, i0                
    48 : load.u32                 i0, i0, i15                
    49 : load.i32                 i0, i0                     
    50 : load.i32                 i0, i0, 256                
    51 : load.i32                 i15, i0, 256               
    52 : load.i32                 i0, i15, 256               
    53 : load.i32                 i0, i0, i0                 
    54 : load.i32                 i15, i0, i0                
    55 : load.i32                 i0, i15, i0                
    56 : load.i32                 i0, i0, i15                
    57 : load.u16                 i0, i0                     
    58 : load.u16                 i0, i0, 256                
    59 : load.u16                 i15, i0, 256               
    60 : load.u16                 i0, i15, 256               
    61 : load.u16                 i0, i0, i0                 
    62 : load.u16                 i15, i0, i0                
    63 : load.u16                 i0, i15, i0                
    64 : load.u16                 i0, i0, i15                
    65 : load.i16                 i0, i0                     
    66 : load.i16                 i0, i0, 256                
    67 : load.i16                 i15, i0, 256               
    68 : load.i16                 i0, i15, 256               
    69 : load.i16                 i0, i0, i0                 
    70 : load.i16                 i15, i0, i0                
    71 : load.i16                 i0, i15, i0                
    72 : load.i16                 i0, i0, i15                
    73 : load.u8                  i0, i0                     
    74 : load.u8                  i0, i0, 256                
    75 : load.u8                  i15, i0, 256               
    76 : load.u8                  i0, i15, 256               
    77 : load.u8                  i0, i0, i0                 
    78 : load.u8                  i15, i0, i0                
    79 : load.u8                  i0, i15, i0                
    80 : load.u8                  i0, i0, i15                
    81 : load.i8                  i0, i0                     
    82 : load.i8                  i0, i0, 256                
    83 : load.i8                  i15, i0, 256               
    84 : load.i8                  i0, i15, 256               
    85 : load.i8                  i0, i0, i0                 
    86 : load.i8                  i15, i0, i0                
    87 : load.i8                  i0, i15, i0                
    88 : load.i8                  i0, i0, i15                
    89 : arm_ldp                  i0, i7, i15, 0             
    90 : arm_ldp                  i7, i0, i15, 0             
    91 : arm_ldp                  i7, i15, i0, 0             
    92 : arm_ldp                  i0, i7, i15, 63            
    93 : arm_ldp                  i0, i7, i15, -64           
    94 : select_eq                i0, i0, i0                 
    95 : select_ne                i0, i0, i0                 
    96 : select_gt                i0, i0, i0                 
    97 : select_gt                i0, i0, i0                 
    98 : select_le                i0, i0, i0                 
    99 : select_ge                i0, i0, i0                 
   100 : select_s                 i0, i0, i0                 
   101 : select_ns                i0, i0, i0                 
   102 : select_eq                i15, i0, i0                
   103 : select_eq                i0, i15, i0                
   104 : select_eq                i0, i0, i15                
   105 : iverson_eq               i0                         
   106 : iverson_ne               i0                         
   107 : iverson_gt               i0                         
   108 : iverson_gt               i0                         
   109 : iverson_le               i0                         
   110 : iverson_ge               i0                         
   111 : iverson_s                i0                         
   112 : iverson_ns               i0                         
   113 : iverson_eq               i15                        
   114 : arm_cinc                 i0, i0, 36                 
   115 : arm_cinc                 i0, i0, 35                 
   116 : arm_cinc                 i0, i0, 32                 
   117 : arm_cinc                 i0, i0, 29                 
   118 : arm_cinc                 i0, i0, 33                 
   119 : arm_cinc                 i0, i0, 31                 
   120 : arm_cinc                 i0, i0, 37                 
   121 : arm_cinc                 i0, i0, 38                 
   122 : arm_cinc                 i15, i0, 36                
   123 : arm_cinc                 i0, i15, 36                
   124 : arm_cneg                 i0, i0, 36                 
   125 : arm_cneg                 i0, i0, 35                 
   126 : arm_cneg                 i0, i0, 32                 
   127 : arm_cneg                 i0, i0, 29                 
   128 : arm_cneg                 i0, i0, 33                 
   129 : arm_cneg                 i0, i0, 31                 
   130 : arm_cneg                 i0, i0, 37                 
   131 : arm_cneg                 i0, i0, 38                 
   132 : arm_cneg                 i15, i0, 36                
   133 : arm_cneg                 i0, i15, 36                
   134 : sar                      i0, i0, 63                 
   135 : sar                      i15, i0, 63                
   136 : sar                      i0, i15, 63                
   137 : sar                      i0, i0, i0                 
   138 : sar                      i15, i0, i0                
   139 : sar                      i0, i15, i0                
   140 : sar                      i0, i0, i15                
   141 : shr                      i0, i0, 63                 
   142 : shr                      i15, i0, 63                
   143 : shr                      i0, i15, 63                
   144 : shr                      i0, i0, i0                 
   145 : shr                      i15, i0, i0                
   146 : shr                      i0, i15, i0                
   147 : shr                      i0, i0, i15                
   148 : shl                      i0, i0, 63                 
   149 : shl                      i0, i0, 31                 
   150 : shl                      i0, i0, 15                 
   151 : shl                      i0, i0, 7                  
   152 : shl                      i0, i0, 1                  
   153 : shl                      i0, i0, 0                  
   154 : shl                      i15, i0, 63                
   155 : shl                      i0, i15, 63                
   156 : shl                      i0, i0, i0                 
   157 : shl                      i15, i0, i0                
   158 : shl                      i0, i15, i0                
   159 : shl                      i0, i0, i15                
   160 : and                      i0, i0, i0                 
   161 : and                      i15, i0, i0                
   162 : and                      i0, i15, i0                
   163 : and                      i0, i0, i15                
   164 : and                      i0, i0, 255                
   165 : and                      i0, i0, 65280              
   166 : and                      i0, i0, -72057594037927936 
   167 : and                      i15, i0, 255               
   168 : and                      i0, i15, 255               
   169 : or                       i0, i0, i0                 
   170 : or                       i15, i0, i0                
   171 : or                       i0, i15, i0                
   172 : or                       i0, i0, i15                
   173 : or                       i0, i0, 255                
   174 : or                       i0, i0, 65280              
   175 : or                       i0, i0, -72057594037927936 
   176 : or                       i15, i0, 255               
   177 : or                       i0, i15, 255               
   178 : xor                      i0, i0, i0                 
   179 : xor                      i15, i0, i0                
   180 : xor                      i0, i15, i0                
   181 : xor                      i0, i0, i15                
   182 : xor                      i0, i0, 255                
   183 : xor                      i0, i0, 65280              
   184 : xor                      i0, i0, -72057594037927936 
   185 : xor                      i15, i0, 255               
   186 : xor                      i0, i15, 255               
   187 : not                      i0, i0                     
   188 : not                      i15, i0                    
   189 : not                      i0, i15                    
   190 : neg                      i0, i0                     
   191 : neg                      i15, i0                    
   192 : neg                      i0, i15                    
   193 : vld.u64                  v0, i0                     
   194 : vld.u64                  v0, i0, 32784              
   195 : vld.u64                  v31, i0                    
   196 : vld.u64                  v0, i15                    
   197 : vld.u64                  v0, i0, i0                 
   198 : vld.u64                  v31, i0, i0                
   199 : vld.u64                  v0, i15, i0                
   200 : vld.u64                  v0, i0, i15                
   201 : vst.u64                  i0, v0                     
   202 : vst.u64                  i0, 32784, v0              
   203 : vst.u64                  i0, v31                    
   204 : vst.u64                  i15, v0                    
   205 : vst.u64                  i0, i0, v0                 
   206 : vst.u64                  i0, i0, v31                
   207 : vst.u64                  i15, i0, v0                
   208 : vst.u64                  i0, i15, v0                
   209 : add.u8                   v0, v0, v0                 
   210 : add.u8                   v31, v0, v0                
   211 : add.u8                   v0, v31, v0                
   212 : add.u8                   v0, v0, v31                
   213 : add.u16                  v0, v0, v0                 
   214 : add.u16                  v31, v0, v0                
   215 : add.u16                  v0, v31, v0                
   216 : add.u16                  v0, v0, v31                
   217 : add.u32                  v0, v0, v0                 
   218 : add.u32                  v31, v0, v0                
   219 : add.u32                  v0, v31, v0                
   220 : add.u32                  v0, v0, v31                
   221 : add.u64                  v0, v0, v0                 
   222 : add.u64                  v31, v0, v0                
   223 : add.u64                  v0, v31, v0                
   224 : add.u64                  v0, v0, v31                
   225 : add.fp16                 v0, v0, v0                 
   226 : add.fp16                 v31, v0, v0                
   227 : add.fp16                 v0, v31, v0                
   228 : add.fp16                 v0, v0, v31                
   229 : add.fp32                 v0, v0, v0                 
   230 : add.fp32                 v31, v0, v0                
   231 : add.fp32                 v0, v31, v0                
   232 : add.fp32                 v0, v0, v31                
   233 : add.fp64                 v0, v0, v0                 
   234 : add.fp64                 v31, v0, v0                
   235 : add.fp64                 v0, v31, v0                
   236 : add.fp64                 v0, v0, v31                
   237 : sub.u8                   v0, v0, v0                 
   238 : sub.u8                   v31, v0, v0                
   239 : sub.u8                   v0, v31, v0                
   240 : sub.u8                   v0, v0, v31                
   241 : sub.u16                  v0, v0, v0                 
   242 : sub.u16                  v31, v0, v0                
   243 : sub.u16                  v0, v31, v0                
   244 : sub.u16                  v0, v0, v31                
   245 : sub.u32                  v0, v0, v0                 
   246 : sub.u32                  v31, v0, v0                
   247 : sub.u32                  v0, v31, v0                
   248 : sub.u32                  v0, v0, v31                
   249 : sub.u64                  v0, v0, v0                 
   250 : sub.u64                  v31, v0, v0                
   251 : sub.u64                  v0, v31, v0                
   252 : sub.u64                  v0, v0, v31                
   253 : sub.fp16                 v0, v0, v0                 
   254 : sub.fp16                 v31, v0, v0                
   255 : sub.fp16                 v0, v31, v0                
   256 : sub.fp16                 v0, v0, v31                
   257 : sub.fp32                 v0, v0, v0                 
   258 : sub.fp32                 v31, v0, v0                
   259 : sub.fp32                 v0, v31, v0                
   260 : sub.fp32                 v0, v0, v31                
   261 : sub.fp64                 v0, v0, v0                 
   262 : sub.fp64                 v31, v0, v0                
   263 : sub.fp64                 v0, v31, v0                
   264 : sub.fp64                 v0, v0, v31                
   265 : mul.u8                   v0, v0, v0                 
   266 : mul.u8                   v31, v0, v0                
   267 : mul.u8                   v0, v31, v0                
   268 : mul.u8                   v0, v0, v31                
   269 : mul.u16                  v0, v0, v0                 
   270 : mul.u16                  v31, v0, v0                
   271 : mul.u16                  v0, v31, v0                
   272 : mul.u16                  v0, v0, v31                
   273 : mul.u32                  v0, v0, v0                 
   274 : mul.u32                  v31, v0, v0                
   275 : mul.u32                  v0, v31, v0                
   276 : mul.u32                  v0, v0, v31                
   277 : mul.fp16                 v0, v0, v0                 
   278 : mul.fp16                 v31, v0, v0                
   279 : mul.fp16                 v0, v31, v0                
   280 : mul.fp16                 v0, v0, v31                
   281 : mul.fp32                 v0, v0, v0                 
   282 : mul.fp32                 v31, v0, v0                
   283 : mul.fp32                 v0, v31, v0                
   284 : mul.fp32                 v0, v0, v31                
   285 : mul.fp64                 v0, v0, v0                 
   286 : mul.fp64                 v31, v0, v0                
   287 : mul.fp64                 v0, v31, v0                
   288 : mul.fp64                 v0, v0, v31                
   289 : mul.fp32                 v0, v0, v0                 
   290 : mul.fp32                 v31, v0, v0                
   291 : mul.fp32                 v0, v31, v0                
   292 : mul.fp32                 v0, v0, v31                
   293 : mul.fp64                 v0, v0, v0                 
   294 : mul.fp64                 v31, v0, v0                
   295 : mul.fp64                 v0, v31, v0                
   296 : mul.fp64                 v0, v0, v31                
   297 : mul.fp16                 v0, v0, v0                 
   298 : mul.fp16                 v31, v0, v0                
   299 : mul.fp16                 v0, v31, v0                
   300 : mul.fp16                 v0, v0, v31                
   301 : sal.u8                   v0, v0, 1                  
   302 : sal.u8                   v31, v0, 1                 
   303 : sal.u8                   v0, v31, 1                 
   304 : sal.u8                   v0, v0, 7                  
   305 : sal.u16                  v0, v0, 1                  
   306 : sal.u16                  v31, v0, 1                 
   307 : sal.u16                  v0, v31, 1                 
   308 : sal.u16                  v0, v0, 15                 
   309 : sal.u32                  v0, v0, 1                  
   310 : sal.u32                  v31, v0, 1                 
   311 : sal.u32                  v0, v31, 1                 
   312 : sal.u32                  v0, v0, 31                 
   313 : sal.u64                  v0, v0, 1                  
   314 : sal.u64                  v31, v0, 1                 
   315 : sal.u64                  v0, v31, 1                 
   316 : sal.u64                  v0, v0, 63                 
   317 : sal.u8                   v0, v0, v0                 
   318 : sal.u8                   v31, v0, v0                
   319 : sal.u8                   v0, v31, v0                
   320 : sal.u8                   v0, v0, v31                
   321 : sal.u16                  v0, v0, v0                 
   322 : sal.u16                  v31, v0, v0                
   323 : sal.u16                  v0, v31, v0                
   324 : sal.u16                  v0, v0, v31                
   325 : sal.u32                  v0, v0, v0                 
   326 : sal.u32                  v31, v0, v0                
   327 : sal.u32                  v0, v31, v0                
   328 : sal.u32                  v0, v0, v31                
   329 : sal.u64                  v0, v0, v0                 
   330 : sal.u64                  v31, v0, v0                
   331 : sal.u64                  v0, v31, v0                
   332 : sal.u64                  v0, v0, v31                
   333 : sal.i8                   v0, v0, v0                 
   334 : sal.i8                   v31, v0, v0                
   335 : sal.i8                   v0, v31, v0                
   336 : sal.i8                   v0, v0, v31                
   337 : sal.i16                  v0, v0, v0                 
   338 : sal.i16                  v31, v0, v0                
   339 : sal.i16                  v0, v31, v0                
   340 : sal.i16                  v0, v0, v31                
   341 : sal.i32                  v0, v0, v0                 
   342 : sal.i32                  v31, v0, v0                
   343 : sal.i32                  v0, v31, v0                
   344 : sal.i32                  v0, v0, v31                
   345 : sal.i64                  v0, v0, v0                 
   346 : sal.i64                  v31, v0, v0                
   347 : sal.i64                  v0, v31, v0                
   348 : sal.i64                  v0, v0, v31                
   349 : sar.u8                   v0, v0, 1                  
   350 : sar.u8                   v31, v0, 1                 
   351 : sar.u8                   v0, v31, 1                 
   352 : sar.u8                   v0, v0, 7                  
   353 : sar.u16                  v0, v0, 1                  
   354 : sar.u16                  v31, v0, 1                 
   355 : sar.u16                  v0, v31, 1                 
   356 : sar.u16                  v0, v0, 15                 
   357 : sar.u32                  v0, v0, 1                  
   358 : sar.u32                  v31, v0, 1                 
   359 : sar.u32                  v0, v31, 1                 
   360 : sar.u32                  v0, v0, 31                 
   361 : sar.u64                  v0, v0, 1                  
   362 : sar.u64                  v31, v0, 1                 
   363 : sar.u64                  v0, v31, 1                 
   364 : sar.u64                  v0, v0, 63                 
   365 : sar.i8                   v0, v0, 1                  
   366 : sar.i8                   v31, v0, 1                 
   367 : sar.i8                   v0, v31, 1                 
   368 : sar.i8                   v0, v0, 7                  
   369 : sar.i16                  v0, v0, 1                  
   370 : sar.i16                  v31, v0, 1                 
   371 : sar.i16                  v0, v31, 1                 
   372 : sar.i16                  v0, v0, 15                 
   373 : sar.i32                  v0, v0, 1                  
   374 : sar.i32                  v31, v0, 1                 
   375 : sar.i32                  v0, v31, 1                 
   376 : sar.i32                  v0, v0, 31                 
   377 : sar.i64                  v0, v0, 1                  
   378 : sar.i64                  v31, v0, 1                 
   379 : sar.i64                  v0, v31, 1                 
   380 : sar.i64                  v0, v0, 63                 
   381 : and                      v0, v0, v0                 
   382 : and                      v31, v0, v0                
   383 : and                      v0, v31, v0                
   384 : and                      v0, v0, v31                
   385 : or                       v0, v0, v0                 
   386 : or                       v31, v0, v0                
   387 : or                       v0, v31, v0                
   388 : or                       v0, v0, v31                
   389 : xor                      v0, v0, v0                 
   390 : xor                      v31, v0, v0                
   391 : xor                      v0, v31, v0                
   392 : xor                      v0, v0, v31                
   393 : not                      v0, v0                     
   394 : not                      v31, v0                    
   395 : not                      v0, v31                    
   396 : mov                      v0, 0                      
   397 : mov                      v31, 0                     
   398 : mov                      v0, 255                    
   399 : mov                      v0, 0                      
   400 : mov                      v31, 0                     
   401 : mov                      v0, 255                    
   402 : mov                      v0, 0                      
   403 : mov                      v31, 0                     
   404 : mov                      v0, 255                    
   405 : mov                      v0, -1                     
   406 : mov                      v31, -1                    
   407 : mov                      v0, -256                   
   408 : mov                      v0, -1                     
   409 : mov                      v31, -1                    
   410 : mov                      v0, -256                   
   411 : min.fp16                 v0, v0, v0                 
   412 : min.fp16                 v31, v0, v0                
   413 : min.fp16                 v0, v31, v0                
   414 : min.fp16                 v0, v0, v31                
   415 : min.fp32                 v0, v0, v0                 
   416 : min.fp32                 v31, v0, v0                
   417 : min.fp32                 v0, v31, v0                
   418 : min.fp32                 v0, v0, v31                
   419 : min.fp64                 v0, v0, v0                 
   420 : min.fp64                 v31, v0, v0                
   421 : min.fp64                 v0, v31, v0                
   422 : min.fp64                 v0, v0, v31                
   423 : max.fp16                 v0, v0, v0                 
   424 : max.fp16                 v31, v0, v0                
   425 : max.fp16                 v0, v31, v0                
   426 : max.fp16                 v0, v0, v31                
   427 : max.fp32                 v0, v0, v0                 
   428 : max.fp32                 v31, v0, v0                
   429 : max.fp32                 v0, v31, v0                
   430 : max.fp32                 v0, v0, v31                
   431 : max.fp64                 v0, v0, v0                 
   432 : max.fp64                 v31, v0, v0                
   433 : max.fp64                 v0, v31, v0                
   434 : max.fp64                 v0, v0, v31                
   435 : fma.fp16                 v0, v0, v0, v0             
   436 : fma.fp16                 v31, v31, v0, v0           
   437 : fma.fp16                 v0, v0, v31, v0            
   438 : fma.fp16                 v0, v0, v0, v31            
   439 : fma.fp32                 v0, v0, v0, v0             
   440 : fma.fp32                 v31, v31, v0, v0           
   441 : fma.fp32                 v0, v0, v31, v0            
   442 : fma.fp32                 v0, v0, v0, v31            
   443 : fma.fp64                 v0, v0, v0, v0             
   444 : fma.fp64                 v31, v31, v0, v0           
   445 : fma.fp64                 v0, v0, v31, v0            
   446 : fma.fp64                 v0, v0, v0, v31            
   447 : trunc.fp16_i16           v0, v0                     
   448 : trunc.fp16_i16           v31, v0                    
   449 : trunc.fp16_i16           v0, v31                    
   450 : trunc.fp32_i32           v0, v0                     
   451 : trunc.fp32_i32           v31, v0                    
   452 : trunc.fp32_i32           v0, v31                    
   453 : trunc.fp64_i64           v0, v0                     
   454 : trunc.fp64_i64           v31, v0                    
   455 : trunc.fp64_i64           v0, v31                    
   456 : trunc.fp16_u16           v0, v0                     
   457 : trunc.fp16_u16           v31, v0                    
   458 : trunc.fp16_u16           v0, v31                    
   459 : trunc.fp32_u32           v0, v0                     
   460 : trunc.fp32_u32           v31, v0                    
   461 : trunc.fp32_u32           v0, v31                    
   462 : trunc.fp64_u64           v0, v0                     
   463 : trunc.fp64_u64           v31, v0                    
   464 : trunc.fp64_u64           v0, v31                    
   465 : floor.fp16_i16           v0, v0                     
   466 : floor.fp16_i16           v31, v0                    
   467 : floor.fp16_i16           v0, v31                    
   468 : floor.fp32_i32           v0, v0                     
   469 : floor.fp32_i32           v31, v0                    
   470 : floor.fp32_i32           v0, v31                    
   471 : floor.fp64_i64           v0, v0                     
   472 : floor.fp64_i64           v31, v0                    
   473 : floor.fp64_i64           v0, v31                    
   474 : floor.fp16_u16           v0, v0                     
   475 : floor.fp16_u16           v31, v0                    
   476 : floor.fp16_u16           v0, v31                    
   477 : floor.fp32_u32           v0, v0                     
   478 : floor.fp32_u32           v31, v0                    
   479 : floor.fp32_u32           v0, v31                    
   480 : floor.fp64_u64           v0, v0                     
   481 : floor.fp64_u64           v31, v0                    
   482 : floor.fp64_u64           v0, v31                    
   483 : cast.i16_fp16            v0, v0                     
   484 : cast.i16_fp16            v31, v0                    
   485 : cast.i16_fp16            v0, v31                    
   486 : cast.i32_fp32            v0, v0                     
   487 : cast.i32_fp32            v31, v0                    
   488 : cast.i32_fp32            v0, v31                    
   489 : cast.i64_fp64            v0, v0                     
   490 : cast.i64_fp64            v31, v0                    
   491 : cast.i64_fp64            v0, v31                    
   492 : cast.u16_fp16            v0, v0                     
   493 : cast.u16_fp16            v31, v0                    
   494 : cast.u16_fp16            v0, v31                    
   495 : cast.u32_fp32            v0, v0                     
   496 : cast.u32_fp32            v31, v0                    
   497 : cast.u32_fp32            v0, v31                    
   498 : cast.u64_fp64            v0, v0                     
   499 : cast.u64_fp64            v31, v0                    
   500 : cast.u64_fp64            v0, v31                    
   501 : gt.u16                   v0, v0, v0                 
   502 : gt.u16                   v31, v0, v0                
   503 : gt.u16                   v0, v31, v0                
   504 : gt.u16                   v0, v0, v31                
   505 : gt.u32                   v0, v0, v0                 
   506 : gt.u32                   v31, v0, v0                
   507 : gt.u32                   v0, v31, v0                
   508 : gt.u32                   v0, v0, v31                
   509 : gt.u64                   v0, v0, v0                 
   510 : gt.u64                   v31, v0, v0                
   511 : gt.u64                   v0, v31, v0                
   512 : gt.u64                   v0, v0, v31                
   513 : ge.u16                   v0, v0, v0                 
   514 : ge.u16                   v31, v0, v0                
   515 : ge.u16                   v0, v31, v0                
   516 : ge.u16                   v0, v0, v31                
   517 : ge.u32                   v0, v0, v0                 
   518 : ge.u32                   v31, v0, v0                
   519 : ge.u32                   v0, v31, v0                
   520 : ge.u32                   v0, v0, v31                
   521 : ge.u64                   v0, v0, v0                 
   522 : ge.u64                   v31, v0, v0                
   523 : ge.u64                   v0, v31, v0                
   524 : ge.u64                   v0, v0, v31                
   525 : eq.u16                   v0, v0, v0                 
   526 : eq.u16                   v31, v0, v0                
   527 : eq.u16                   v0, v31, v0                
   528 : eq.u16                   v0, v0, v31                
   529 : eq.u32                   v0, v0, v0                 
   530 : eq.u32                   v31, v0, v0                
   531 : eq.u32                   v0, v31, v0                
   532 : eq.u32                   v0, v0, v31                
   533 : eq.u64                   v0, v0, v0                 
   534 : eq.u64                   v31, v0, v0                
   535 : eq.u64                   v0, v31, v0                
   536 : eq.u64                   v0, v0, v31                
   537 : neg.i8                   v0, v0                     
   538 : neg.i8                   v31, v0                    
   539 : neg.i8                   v0, v31                    
   540 : neg.i16                  v0, v0                     
   541 : neg.i16                  v31, v0                    
   542 : neg.i16                  v0, v31                    
   543 : neg.i32                  v0, v0                     
   544 : neg.i32                  v31, v0                    
   545 : neg.i32                  v0, v31                    
   546 : neg.i64                  v0, v0                     
   547 : neg.i64                  v31, v0                    
   548 : neg.i64                  v0, v31                    
   549 : neg.fp16                 v0, v0                     
   550 : neg.fp16                 v31, v0                    
   551 : neg.fp16                 v0, v31                    
   552 : neg.fp32                 v0, v0                     
   553 : neg.fp32                 v31, v0                    
   554 : neg.fp32                 v0, v31                    
   555 : neg.fp64                 v0, v0                     
   556 : neg.fp64                 v31, v0                    
   557 : neg.fp64                 v0, v31                    
   558 : arm_movk                 i0, 0, 16                  
   559 : arm_movk                 i15, 0, 16                 
   560 : arm_movk                 i0, 65535, 16              
   561 : arm_movk                 i0, 0, 32                  
   562 : arm_movk                 i15, 0, 32                 
   563 : arm_movk                 i0, 65535, 32              
   564 : arm_movk                 i0, 0, 48                  
   565 : arm_movk                 i15, 0, 48                 
   566 : arm_movk                 i0, 65535, 48              
   567 : broadcast.i8             v0, i0                     
   568 : broadcast.i8             v31, i0                    
   569 : broadcast.i8             v0, i15                    
   570 : broadcast.i16            v0, i0                     
   571 : broadcast.i16            v31, i0                    
   572 : broadcast.i16            v0, i15                    
   573 : broadcast.i32            v0, i0                     
   574 : broadcast.i32            v31, i0                    
   575 : broadcast.i32            v0, i15                    
   576 : broadcast.i64            v0, i0                     
   577 : broadcast.i64            v31, i0                    
   578 : broadcast.i64            v0, i15                    
   579 : broadcast.u8             v0, v0, 0                  
   580 : broadcast.u8             v31, v0, 0                 
   581 : broadcast.u8             v0, v31, 0                 
   582 : broadcast.u8             v0, v0, 15                 
   583 : broadcast.u16            v0, v0, 0                  
   584 : broadcast.u16            v31, v0, 0                 
   585 : broadcast.u16            v0, v31, 0                 
   586 : broadcast.u16            v0, v0, 7                  
   587 : broadcast.u32            v0, v0, 0                  
   588 : broadcast.u32            v31, v0, 0                 
   589 : broadcast.u32            v0, v31, 0                 
   590 : broadcast.u32            v0, v0, 3                  
   591 : broadcast.u64            v0, v0, 0                  
   592 : broadcast.u64            v31, v0, 0                 
   593 : broadcast.u64            v0, v31, 0                 
   594 : broadcast.u64            v0, v0, 1                  
   595 : cast.i16.from.i8.low     v0, v0                     
   596 : cast.i16.from.i8.low     v31, v0                    
   597 : cast.i16.from.i8.low     v0, v31                    
   598 : cast.i32.from.i16.low    v0, v0                     
   599 : cast.i32.from.i16.low    v31, v0                    
   600 : cast.i32.from.i16.low    v0, v31                    
   601 : cast.i64.from.i32.low    v0, v0                     
   602 : cast.i64.from.i32.low    v31, v0                    
   603 : cast.i64.from.i32.low    v0, v31                    
   604 : cast.u16.from.u8.low     v0, v0                     
   605 : cast.u16.from.u8.low     v31, v0                    
   606 : cast.u16.from.u8.low     v0, v31                    
   607 : cast.u32.from.u16.low    v0, v0                     
   608 : cast.u32.from.u16.low    v31, v0                    
   609 : cast.u32.from.u16.low    v0, v31                    
   610 : cast.u64.from.u32.low    v0, v0                     
   611 : cast.u64.from.u32.low    v31, v0                    
   612 : cast.u64.from.u32.low    v0, v31                    
   613 : cast.i16.from.i8.high    v0, v0                     
   614 : cast.i16.from.i8.high    v31, v0                    
   615 : cast.i16.from.i8.high    v0, v31                    
   616 : cast.i32.from.i16.high   v0, v0                     
   617 : cast.i32.from.i16.high   v31, v0                    
   618 : cast.i32.from.i16.high   v0, v31                    
   619 : cast.i64.from.i32.high   v0, v0                     
   620 : cast.i64.from.i32.high   v31, v0                    
   621 : cast.i64.from.i32.high   v0, v31                    
   622 : cast.u16.from.u8.high    v0, v0                     
   623 : cast.u16.from.u8.high    v31, v0                    
   624 : cast.u16.from.u8.high    v0, v31                    
   625 : cast.u32.from.u16.high   v0, v0                     
   626 : cast.u32.from.u16.high   v31, v0                    
   627 : cast.u32.from.u16.high   v0, v31                    
   628 : cast.u64.from.u32.high   v0, v0                     
   629 : cast.u64.from.u32.high   v31, v0                    
   630 : cast.u64.from.u32.high   v0, v31                    
   631 : shrink.i8.from.i16.low   v0, v0                     
   632 : shrink.i8.from.i16.low   v31, v0                    
   633 : shrink.i8.from.i16.low   v0, v31                    
   634 : shrink.i16.from.i32.low  v0, v0                     
   635 : shrink.i16.from.i32.low  v31, v0                    
   636 : shrink.i16.from.i32.low  v0, v31                    
   637 : shrink.i32.from.i64.low  v0, v0                     
   638 : shrink.i32.from.i64.low  v31, v0                    
   639 : shrink.i32.from.i64.low  v0, v31                    
   640 : shrink.i8.from.i16.high  v0, v0                     
   641 : shrink.i8.from.i16.high  v31, v0                    
   642 : shrink.i8.from.i16.high  v0, v31                    
   643 : shrink.i16.from.i32.high v0, v0                     
   644 : shrink.i16.from.i32.high v31, v0                    
   645 : shrink.i16.from.i32.high v0, v31                    
   646 : shrink.i32.from.i64.high v0, v0                     
   647 : shrink.i32.from.i64.high v31, v0                    
   648 : shrink.i32.from.i64.high v0, v31                    
   649 : reduce.max.i8            v0, v0                     
   650 : reduce.max.i8            v31, v0                    
   651 : reduce.max.i8            v0, v31                    
   652 : reduce.max.i16           v0, v0                     
   653 : reduce.max.i16           v31, v0                    
   654 : reduce.max.i16           v0, v31                    
   655 : reduce.max.i32           v0, v0                     
   656 : reduce.max.i32           v31, v0                    
   657 : reduce.max.i32           v0, v31                    
   658 : reduce.max.u8            v0, v0                     
   659 : reduce.max.u8            v31, v0                    
   660 : reduce.max.u8            v0, v31                    
   661 : reduce.max.u16           v0, v0                     
   662 : reduce.max.u16           v31, v0                    
   663 : reduce.max.u16           v0, v31                    
   664 : reduce.max.u32           v0, v0                     
   665 : reduce.max.u32           v31, v0                    
   666 : reduce.max.u32           v0, v31                    
   667 : reduce.max.fp16          v0, v0                     
   668 : reduce.max.fp16          v31, v0                    
   669 : reduce.max.fp16          v0, v31                    
   670 : reduce.max.fp32          v0, v0                     
   671 : reduce.max.fp32          v31, v0                    
   672 : reduce.max.fp32          v0, v31                    
   673 : reduce.max.i8            v0, v0                     
   674 : reduce.max.i8            v31, v0                    
   675 : reduce.max.i8            v0, v31                    
   676 : reduce.max.i16           v0, v0                     
   677 : reduce.max.i16           v31, v0                    
   678 : reduce.max.i16           v0, v31                    
   679 : reduce.max.i32           v0, v0                     
   680 : reduce.max.i32           v31, v0                    
   681 : reduce.max.i32           v0, v31                    
   682 : reduce.max.u8            v0, v0                     
   683 : reduce.max.u8            v31, v0                    
   684 : reduce.max.u8            v0, v31                    
   685 : reduce.max.u16           v0, v0                     
   686 : reduce.max.u16           v31, v0                    
   687 : reduce.max.u16           v0, v31                    
   688 : reduce.max.u32           v0, v0                     
   689 : reduce.max.u32           v31, v0                    
   690 : reduce.max.u32           v0, v31                    
   691 : reduce.max.fp16          v0, v0                     
   692 : reduce.max.fp16          v31, v0                    
   693 : reduce.max.fp16          v0, v31                    
   694 : reduce.max.fp32          v0, v0                     
   695 : reduce.max.fp32          v31, v0                    
   696 : reduce.max.fp32          v0, v31                    
   697 : vld_lane.i8              v0, 0, i0                  
   698 : vld_lane.i8              v31, 0, i0                 
   699 : vld_lane.i8              v0, 15, i0                 
   700 : vld_lane.i8              v0, 0, i15                 
   701 : vld_lane.i16             v0, 0, i0                  
   702 : vld_lane.i16             v31, 0, i0                 
   703 : vld_lane.i16             v0, 7, i0                  
   704 : vld_lane.i16             v0, 0, i15                 
   705 : vld_lane.i32             v0, 0, i0                  
   706 : vld_lane.i32             v31, 0, i0                 
   707 : vld_lane.i32             v0, 3, i0                  
   708 : vld_lane.i32             v0, 0, i15                 
   709 : vld_lane.i64             v0, 0, i0                  
   710 : vld_lane.i64             v31, 0, i0                 
   711 : vld_lane.i64             v0, 1, i0                  
   712 : vld_lane.i64             v0, 0, i15                 
   713 : vld_deinterleave2.i8     v0, v1, i0                 
   714 : vld_deinterleave2.i8     v31, v0, i0                
   715 : vld_deinterleave2.i8     v0, v1, i15                
   716 : vld_deinterleave2.i16    v0, v1, i0                 
   717 : vld_deinterleave2.i16    v31, v0, i0                
   718 : vld_deinterleave2.i16    v0, v1, i15                
   719 : vld_deinterleave2.i32    v0, v1, i0                 
   720 : vld_deinterleave2.i32    v31, v0, i0                
   721 : vld_deinterleave2.i32    v0, v1, i15                
   722 : vld_deinterleave2.i64    v0, v1, i0                 
   723 : vld_deinterleave2.i64    v31, v0, i0                
   724 : vld_deinterleave2.i64    v0, v1, i15                
   725 : vld_lane.i8              v0, i0                     
   726 : vld_lane.i8              v31, i0                    
   727 : vld_lane.i8              v0, i15                    
   728 : vld_lane.i16             v0, i0                     
   729 : vld_lane.i16             v31, i0                    
   730 : vld_lane.i16             v0, i15                    
   731 : vld_lane.i32             v0, i0                     
   732 : vld_lane.i32             v31, i0                    
   733 : vld_lane.i32             v0, i15                    
   734 : vld_lane.i64             v0, i0                     
   735 : vld_lane.i64             v31, i0                    
   736 : vld_lane.i64             v0, i15                    
   737 : vst_lane.i8              i0, v0, 0                  
   738 : vst_lane.i8              i0, v31, 0                 
   739 : vst_lane.i8              i0, v0, 15                 
   740 : vst_lane.i8              i15, v0, 0                 
   741 : vst_lane.i16             i0, v0, 0                  
   742 : vst_lane.i16             i0, v31, 0                 
   743 : vst_lane.i16             i0, v0, 7                  
   744 : vst_lane.i16             i15, v0, 0                 
   745 : vst_lane.i32             i0, v0, 0                  
   746 : vst_lane.i32             i0, v31, 0                 
   747 : vst_lane.i32             i0, v0, 3                  
   748 : vst_lane.i32             i15, v0, 0                 
   749 : vst_lane.i64             i0, v0, 0                  
   750 : vst_lane.i64             i0, v31, 0                 
   751 : vst_lane.i64             i0, v0, 1                  
   752 : vst_lane.i64             i15, v0, 0                 
   753 : vst_lane.i8              i0, v0                     
   754 : vst_lane.i8              i15, v0                    
   755 : vst_lane.i8              i0, v31                    
   756 : vst_lane.i16             i0, v0                     
   757 : vst_lane.i16             i15, v0                    
   758 : vst_lane.i16             i0, v31                    
   759 : vst_lane.i32             i0, v0                     
   760 : vst_lane.i32             i15, v0                    
   761 : vst_lane.i32             i0, v31                    
   762 : vst_lane.i64             i0, v0                     
   763 : vst_lane.i64             i15, v0                    
   764 : vst_lane.i64             i0, v31                    
   765 : ext.i8                   v0, v0, v0, 0              
   766 : ext.i8                   v31, v0, v0, 0             
   767 : ext.i8                   v0, v31, v0, 0             
   768 : ext.i8                   v0, v0, v31, 0             
   769 : ext.i8                   v0, v0, v0, 15             
   770 : ext.i16                  v0, v0, v0, 0              
   771 : ext.i16                  v31, v0, v0, 0             
   772 : ext.i16                  v0, v31, v0, 0             
   773 : ext.i16                  v0, v0, v31, 0             
   774 : ext.i16                  v0, v0, v0, 7              
   775 : ext.i32                  v0, v0, v0, 0              
   776 : ext.i32                  v31, v0, v0, 0             
   777 : ext.i32                  v0, v31, v0, 0             
   778 : ext.i32                  v0, v0, v31, 0             
   779 : ext.i32                  v0, v0, v0, 3              
   780 : ext.i64                  v0, v0, v0, 0              
   781 : ext.i64                  v31, v0, v0, 0             
   782 : ext.i64                  v0, v31, v0, 0             
   783 : ext.i64                  v0, v0, v31, 0             
   784 : ext.i64                  v0, v0, v0, 1              
   785 : fma.fp16                 v0, v0, v0, v0, 0          
   786 : fma.fp16                 v31, v31, v0, v0, 0        
   787 : fma.fp16                 v0, v0, v31, v0, 0         
   788 : fma.fp16                 v0, v0, v0, v15, 0         
   789 : fma.fp16                 v0, v0, v0, v0, 7          
   790 : fma.fp32                 v0, v0, v0, v0, 0          
   791 : fma.fp32                 v31, v31, v0, v0, 0        
   792 : fma.fp32                 v0, v0, v31, v0, 0         
   793 : fma.fp32                 v0, v0, v0, v31, 0         
   794 : fma.fp32                 v0, v0, v0, v0, 3          
   795 : fma.fp64                 v0, v0, v0, v0, 0          
   796 : fma.fp64                 v31, v31, v0, v0, 0        
   797 : fma.fp64                 v0, v0, v31, v0, 0         
   798 : fma.fp64                 v0, v0, v0, v31, 0         
   799 : fma.fp64                 v0, v0, v0, v0, 1          
   800 : gt.u8                    v0, v0, v0                 
   801 : gt.u8                    v31, v0, v0                
   802 : gt.u8                    v0, v31, v0                
   803 : gt.u8                    v0, v0, v31                
   804 : gt.u16                   v0, v0, v0                 
   805 : gt.u16                   v31, v0, v0                
   806 : gt.u16                   v0, v31, v0                
   807 : gt.u16                   v0, v0, v31                
   808 : gt.u32                   v0, v0, v0                 
   809 : gt.u32                   v31, v0, v0                
   810 : gt.u32                   v0, v31, v0                
   811 : gt.u32                   v0, v0, v31                
   812 : gt.u64                   v0, v0, v0                 
   813 : gt.u64                   v31, v0, v0                
   814 : gt.u64                   v0, v31, v0                
   815 : gt.u64                   v0, v0, v31                
   816 : ge.u8                    v0, v0, v0                 
   817 : ge.u8                    v31, v0, v0                
   818 : ge.u8                    v0, v31, v0                
   819 : ge.u8                    v0, v0, v31                
   820 : ge.u16                   v0, v0, v0                 
   821 : ge.u16                   v31, v0, v0                
   822 : ge.u16                   v0, v31, v0                
   823 : ge.u16                   v0, v0, v31                
   824 : ge.u32                   v0, v0, v0                 
   825 : ge.u32                   v31, v0, v0                
   826 : ge.u32                   v0, v31, v0                
   827 : ge.u32                   v0, v0, v31                
   828 : ge.u64                   v0, v0, v0                 
   829 : ge.u64                   v31, v0, v0                
   830 : ge.u64                   v0, v31, v0                
   831 : ge.u64                   v0, v0, v31                
   832 : eq.u8                    v0, v0, v0                 
   833 : eq.u8                    v31, v0, v0                
   834 : eq.u8                    v0, v31, v0                
   835 : eq.u8                    v0, v0, v31                
   836 : eq.u16                   v0, v0, v0                 
   837 : eq.u16                   v31, v0, v0                
   838 : eq.u16                   v0, v31, v0                
   839 : eq.u16                   v0, v0, v31                
   840 : eq.u32                   v0, v0, v0                 
   841 : eq.u32                   v31, v0, v0                
   842 : eq.u32                   v0, v31, v0                
   843 : eq.u32                   v0, v0, v31                
   844 : eq.u64                   v0, v0, v0                 
   845 : eq.u64                   v31, v0, v0                
   846 : eq.u64                   v0, v31, v0                
   847 : eq.u64                   v0, v0, v31                
   848 : ge.u8                    v0, v0, v0                 
   849 : ge.u8                    v31, v0, v0                
   850 : ge.u8                    v0, v31, v0                
   851 : ge.u8                    v0, v0, v31                
   852 : ge.u16                   v0, v0, v0                 
   853 : ge.u16                   v31, v0, v0                
   854 : ge.u16                   v0, v31, v0                
   855 : ge.u16                   v0, v0, v31                
   856 : ge.u32                   v0, v0, v0                 
   857 : ge.u32                   v31, v0, v0                
   858 : ge.u32                   v0, v31, v0                
   859 : ge.u32                   v0, v0, v31                
   860 : ge.u64                   v0, v0, v0                 
   861 : ge.u64                   v31, v0, v0                
   862 : ge.u64                   v0, v31, v0                
   863 : ge.u64                   v0, v0, v31                
   864 : gt.u8                    v0, v0, v0                 
   865 : gt.u8                    v31, v0, v0                
   866 : gt.u8                    v0, v31, v0                
   867 : gt.u8                    v0, v0, v31                
   868 : gt.u16                   v0, v0, v0                 
   869 : gt.u16                   v31, v0, v0                
   870 : gt.u16                   v0, v31, v0                
   871 : gt.u16                   v0, v0, v31                
   872 : gt.u32                   v0, v0, v0                 
   873 : gt.u32                   v31, v0, v0                
   874 : gt.u32                   v0, v31, v0                
   875 : gt.u32                   v0, v0, v31                
   876 : gt.u64                   v0, v0, v0                 
   877 : gt.u64                   v31, v0, v0                
   878 : gt.u64                   v0, v31, v0                
   879 : gt.u64                   v0, v0, v31                
   880 : getlane.u8               i0, v0, 0                  
   881 : getlane.u8               i15, v0, 0                 
   882 : getlane.u8               i0, v31, 0                 
   883 : getlane.u8               i0, v0, 15                 
   884 : getlane.u16              i0, v0, 0                  
   885 : getlane.u16              i15, v0, 0                 
   886 : getlane.u16              i0, v31, 0                 
   887 : getlane.u16              i0, v0, 7                  
   888 : getlane.u32              i0, v0, 0                  
   889 : getlane.u32              i15, v0, 0                 
   890 : getlane.u32              i0, v31, 0                 
   891 : getlane.u32              i0, v0, 3                  
   892 : getlane.u64              i0, v0, 0                  
   893 : getlane.u64              i15, v0, 0                 
   894 : getlane.u64              i0, v31, 0                 
   895 : getlane.u64              i0, v0, 1                  
   896 : setlane.u8               v0, 0, i0                  
   897 : setlane.u8               v31, 0, i0                 
   898 : setlane.u8               v0, 15, i0                 
   899 : setlane.u8               v0, 0, i15                 
   900 : setlane.u16              v0, 0, i0                  
   901 : setlane.u16              v31, 0, i0                 
   902 : setlane.u16              v0, 7, i0                  
   903 : setlane.u16              v0, 0, i15                 
   904 : setlane.u32              v0, 0, i0                  
   905 : setlane.u32              v31, 0, i0                 
   906 : setlane.u32              v0, 3, i0                  
   907 : setlane.u32              v0, 0, i15                 
   908 : setlane.u64              v0, 0, i0                  
   909 : setlane.u64              v31, 0, i0                 
   910 : setlane.u64              v0, 1, i0                  
   911 : setlane.u64              v0, 0, i15                 
   912 : setlane.u8               v0, 0, v0, 0               
   913 : setlane.u8               v31, 0, v0, 0              
   914 : setlane.u8               v0, 15, v0, 0              
   915 : setlane.u8               v0, 0, v31, 0              
   916 : setlane.u8               v0, 0, v0, 15              
   917 : setlane.u16              v0, 0, v0, 0               
   918 : setlane.u16              v31, 0, v0, 0              
   919 : setlane.u16              v0, 7, v0, 0               
   920 : setlane.u16              v0, 0, v31, 0              
   921 : setlane.u16              v0, 0, v0, 7               
   922 : setlane.u32              v0, 0, v0, 0               
   923 : setlane.u32              v31, 0, v0, 0              
   924 : setlane.u32              v0, 3, v0, 0               
   925 : setlane.u32              v0, 0, v31, 0              
   926 : setlane.u32              v0, 0, v0, 3               
   927 : setlane.u64              v0, 0, v0, 0               
   928 : setlane.u64              v31, 0, v0, 0              
   929 : setlane.u64              v0, 1, v0, 0               
   930 : setlane.u64              v0, 0, v31, 0              
   931 : setlane.u64              v0, 0, v0, 1               
   932 : select                   v0, v0, v0, v0             
   933 : select                   v31, v31, v0, v0           
   934 : select                   v0, v0, v31, v0            
   935 : select                   v0, v0, v0, v31            
   936 : call_noret               [i0]()                     
   937 : call_noret               [i15]()                    
