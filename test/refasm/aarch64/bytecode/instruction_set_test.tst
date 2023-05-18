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
    94 : select                   i0, 35, i0, i0             
    95 : select                   i0, 34, i0, i0             
    96 : select                   i0, 31, i0, i0             
    97 : select                   i0, 28, i0, i0             
    98 : select                   i0, 32, i0, i0             
    99 : select                   i0, 30, i0, i0             
   100 : select                   i0, 36, i0, i0             
   101 : select                   i0, 37, i0, i0             
   102 : select                   i15, 35, i0, i0            
   103 : select                   i0, 35, i15, i0            
   104 : select                   i0, 35, i0, i15            
   105 : arm_cinc                 i0, i0, 35                 
   106 : arm_cinc                 i0, i0, 34                 
   107 : arm_cinc                 i0, i0, 31                 
   108 : arm_cinc                 i0, i0, 28                 
   109 : arm_cinc                 i0, i0, 32                 
   110 : arm_cinc                 i0, i0, 30                 
   111 : arm_cinc                 i0, i0, 36                 
   112 : arm_cinc                 i0, i0, 37                 
   113 : arm_cinc                 i15, i0, 35                
   114 : arm_cinc                 i0, i15, 35                
   115 : arm_cneg                 i0, i0, 35                 
   116 : arm_cneg                 i0, i0, 34                 
   117 : arm_cneg                 i0, i0, 31                 
   118 : arm_cneg                 i0, i0, 28                 
   119 : arm_cneg                 i0, i0, 32                 
   120 : arm_cneg                 i0, i0, 30                 
   121 : arm_cneg                 i0, i0, 36                 
   122 : arm_cneg                 i0, i0, 37                 
   123 : arm_cneg                 i15, i0, 35                
   124 : arm_cneg                 i0, i15, 35                
   125 : sar                      i0, i0, 63                 
   126 : sar                      i15, i0, 63                
   127 : sar                      i0, i15, 63                
   128 : sar                      i0, i0, i0                 
   129 : sar                      i15, i0, i0                
   130 : sar                      i0, i15, i0                
   131 : sar                      i0, i0, i15                
   132 : shr                      i0, i0, 63                 
   133 : shr                      i15, i0, 63                
   134 : shr                      i0, i15, 63                
   135 : shr                      i0, i0, i0                 
   136 : shr                      i15, i0, i0                
   137 : shr                      i0, i15, i0                
   138 : shr                      i0, i0, i15                
   139 : shl                      i0, i0, 63                 
   140 : shl                      i0, i0, 31                 
   141 : shl                      i0, i0, 15                 
   142 : shl                      i0, i0, 7                  
   143 : shl                      i0, i0, 1                  
   144 : shl                      i0, i0, 0                  
   145 : shl                      i15, i0, 63                
   146 : shl                      i0, i15, 63                
   147 : shl                      i0, i0, i0                 
   148 : shl                      i15, i0, i0                
   149 : shl                      i0, i15, i0                
   150 : shl                      i0, i0, i15                
   151 : and                      i0, i0, i0                 
   152 : and                      i15, i0, i0                
   153 : and                      i0, i15, i0                
   154 : and                      i0, i0, i15                
   155 : and                      i0, i0, 255                
   156 : and                      i0, i0, 65280              
   157 : and                      i0, i0, -72057594037927936 
   158 : and                      i15, i0, 255               
   159 : and                      i0, i15, 255               
   160 : or                       i0, i0, i0                 
   161 : or                       i15, i0, i0                
   162 : or                       i0, i15, i0                
   163 : or                       i0, i0, i15                
   164 : or                       i0, i0, 255                
   165 : or                       i0, i0, 65280              
   166 : or                       i0, i0, -72057594037927936 
   167 : or                       i15, i0, 255               
   168 : or                       i0, i15, 255               
   169 : xor                      i0, i0, i0                 
   170 : xor                      i15, i0, i0                
   171 : xor                      i0, i15, i0                
   172 : xor                      i0, i0, i15                
   173 : xor                      i0, i0, 255                
   174 : xor                      i0, i0, 65280              
   175 : xor                      i0, i0, -72057594037927936 
   176 : xor                      i15, i0, 255               
   177 : xor                      i0, i15, 255               
   178 : not                      i0, i0                     
   179 : not                      i15, i0                    
   180 : not                      i0, i15                    
   181 : neg                      i0, i0                     
   182 : neg                      i15, i0                    
   183 : neg                      i0, i15                    
   184 : vld.u64                  v0, i0                     
   185 : vld.u64                  v0, i0, 32784              
   186 : vld.u64                  v31, i0                    
   187 : vld.u64                  v0, i15                    
   188 : vld.u64                  v0, i0, i0                 
   189 : vld.u64                  v31, i0, i0                
   190 : vld.u64                  v0, i15, i0                
   191 : vld.u64                  v0, i0, i15                
   192 : vst.u64                  i0, v0                     
   193 : vst.u64                  i0, 32784, v0              
   194 : vst.u64                  i0, v31                    
   195 : vst.u64                  i15, v0                    
   196 : vst.u64                  i0, i0, v0                 
   197 : vst.u64                  i0, i0, v31                
   198 : vst.u64                  i15, i0, v0                
   199 : vst.u64                  i0, i15, v0                
   200 : add.u8                   v0, v0, v0                 
   201 : add.u8                   v31, v0, v0                
   202 : add.u8                   v0, v31, v0                
   203 : add.u8                   v0, v0, v31                
   204 : add.u16                  v0, v0, v0                 
   205 : add.u16                  v31, v0, v0                
   206 : add.u16                  v0, v31, v0                
   207 : add.u16                  v0, v0, v31                
   208 : add.u32                  v0, v0, v0                 
   209 : add.u32                  v31, v0, v0                
   210 : add.u32                  v0, v31, v0                
   211 : add.u32                  v0, v0, v31                
   212 : add.u64                  v0, v0, v0                 
   213 : add.u64                  v31, v0, v0                
   214 : add.u64                  v0, v31, v0                
   215 : add.u64                  v0, v0, v31                
   216 : add.fp16                 v0, v0, v0                 
   217 : add.fp16                 v31, v0, v0                
   218 : add.fp16                 v0, v31, v0                
   219 : add.fp16                 v0, v0, v31                
   220 : add.fp32                 v0, v0, v0                 
   221 : add.fp32                 v31, v0, v0                
   222 : add.fp32                 v0, v31, v0                
   223 : add.fp32                 v0, v0, v31                
   224 : add.fp64                 v0, v0, v0                 
   225 : add.fp64                 v31, v0, v0                
   226 : add.fp64                 v0, v31, v0                
   227 : add.fp64                 v0, v0, v31                
   228 : sub.u8                   v0, v0, v0                 
   229 : sub.u8                   v31, v0, v0                
   230 : sub.u8                   v0, v31, v0                
   231 : sub.u8                   v0, v0, v31                
   232 : sub.u16                  v0, v0, v0                 
   233 : sub.u16                  v31, v0, v0                
   234 : sub.u16                  v0, v31, v0                
   235 : sub.u16                  v0, v0, v31                
   236 : sub.u32                  v0, v0, v0                 
   237 : sub.u32                  v31, v0, v0                
   238 : sub.u32                  v0, v31, v0                
   239 : sub.u32                  v0, v0, v31                
   240 : sub.u64                  v0, v0, v0                 
   241 : sub.u64                  v31, v0, v0                
   242 : sub.u64                  v0, v31, v0                
   243 : sub.u64                  v0, v0, v31                
   244 : sub.fp16                 v0, v0, v0                 
   245 : sub.fp16                 v31, v0, v0                
   246 : sub.fp16                 v0, v31, v0                
   247 : sub.fp16                 v0, v0, v31                
   248 : sub.fp32                 v0, v0, v0                 
   249 : sub.fp32                 v31, v0, v0                
   250 : sub.fp32                 v0, v31, v0                
   251 : sub.fp32                 v0, v0, v31                
   252 : sub.fp64                 v0, v0, v0                 
   253 : sub.fp64                 v31, v0, v0                
   254 : sub.fp64                 v0, v31, v0                
   255 : sub.fp64                 v0, v0, v31                
   256 : mul.u8                   v0, v0, v0                 
   257 : mul.u8                   v31, v0, v0                
   258 : mul.u8                   v0, v31, v0                
   259 : mul.u8                   v0, v0, v31                
   260 : mul.u16                  v0, v0, v0                 
   261 : mul.u16                  v31, v0, v0                
   262 : mul.u16                  v0, v31, v0                
   263 : mul.u16                  v0, v0, v31                
   264 : mul.u32                  v0, v0, v0                 
   265 : mul.u32                  v31, v0, v0                
   266 : mul.u32                  v0, v31, v0                
   267 : mul.u32                  v0, v0, v31                
   268 : mul.fp16                 v0, v0, v0                 
   269 : mul.fp16                 v31, v0, v0                
   270 : mul.fp16                 v0, v31, v0                
   271 : mul.fp16                 v0, v0, v31                
   272 : mul.fp32                 v0, v0, v0                 
   273 : mul.fp32                 v31, v0, v0                
   274 : mul.fp32                 v0, v31, v0                
   275 : mul.fp32                 v0, v0, v31                
   276 : mul.fp64                 v0, v0, v0                 
   277 : mul.fp64                 v31, v0, v0                
   278 : mul.fp64                 v0, v31, v0                
   279 : mul.fp64                 v0, v0, v31                
   280 : mul.fp32                 v0, v0, v0                 
   281 : mul.fp32                 v31, v0, v0                
   282 : mul.fp32                 v0, v31, v0                
   283 : mul.fp32                 v0, v0, v31                
   284 : mul.fp64                 v0, v0, v0                 
   285 : mul.fp64                 v31, v0, v0                
   286 : mul.fp64                 v0, v31, v0                
   287 : mul.fp64                 v0, v0, v31                
   288 : mul.fp16                 v0, v0, v0                 
   289 : mul.fp16                 v31, v0, v0                
   290 : mul.fp16                 v0, v31, v0                
   291 : mul.fp16                 v0, v0, v31                
   292 : sal.u8                   v0, v0, 1                  
   293 : sal.u8                   v31, v0, 1                 
   294 : sal.u8                   v0, v31, 1                 
   295 : sal.u8                   v0, v0, 7                  
   296 : sal.u16                  v0, v0, 1                  
   297 : sal.u16                  v31, v0, 1                 
   298 : sal.u16                  v0, v31, 1                 
   299 : sal.u16                  v0, v0, 15                 
   300 : sal.u32                  v0, v0, 1                  
   301 : sal.u32                  v31, v0, 1                 
   302 : sal.u32                  v0, v31, 1                 
   303 : sal.u32                  v0, v0, 31                 
   304 : sal.u64                  v0, v0, 1                  
   305 : sal.u64                  v31, v0, 1                 
   306 : sal.u64                  v0, v31, 1                 
   307 : sal.u64                  v0, v0, 63                 
   308 : sal.u8                   v0, v0, v0                 
   309 : sal.u8                   v31, v0, v0                
   310 : sal.u8                   v0, v31, v0                
   311 : sal.u8                   v0, v0, v31                
   312 : sal.u16                  v0, v0, v0                 
   313 : sal.u16                  v31, v0, v0                
   314 : sal.u16                  v0, v31, v0                
   315 : sal.u16                  v0, v0, v31                
   316 : sal.u32                  v0, v0, v0                 
   317 : sal.u32                  v31, v0, v0                
   318 : sal.u32                  v0, v31, v0                
   319 : sal.u32                  v0, v0, v31                
   320 : sal.u64                  v0, v0, v0                 
   321 : sal.u64                  v31, v0, v0                
   322 : sal.u64                  v0, v31, v0                
   323 : sal.u64                  v0, v0, v31                
   324 : sal.i8                   v0, v0, v0                 
   325 : sal.i8                   v31, v0, v0                
   326 : sal.i8                   v0, v31, v0                
   327 : sal.i8                   v0, v0, v31                
   328 : sal.i16                  v0, v0, v0                 
   329 : sal.i16                  v31, v0, v0                
   330 : sal.i16                  v0, v31, v0                
   331 : sal.i16                  v0, v0, v31                
   332 : sal.i32                  v0, v0, v0                 
   333 : sal.i32                  v31, v0, v0                
   334 : sal.i32                  v0, v31, v0                
   335 : sal.i32                  v0, v0, v31                
   336 : sal.i64                  v0, v0, v0                 
   337 : sal.i64                  v31, v0, v0                
   338 : sal.i64                  v0, v31, v0                
   339 : sal.i64                  v0, v0, v31                
   340 : sar.u8                   v0, v0, 1                  
   341 : sar.u8                   v31, v0, 1                 
   342 : sar.u8                   v0, v31, 1                 
   343 : sar.u8                   v0, v0, 7                  
   344 : sar.u16                  v0, v0, 1                  
   345 : sar.u16                  v31, v0, 1                 
   346 : sar.u16                  v0, v31, 1                 
   347 : sar.u16                  v0, v0, 15                 
   348 : sar.u32                  v0, v0, 1                  
   349 : sar.u32                  v31, v0, 1                 
   350 : sar.u32                  v0, v31, 1                 
   351 : sar.u32                  v0, v0, 31                 
   352 : sar.u64                  v0, v0, 1                  
   353 : sar.u64                  v31, v0, 1                 
   354 : sar.u64                  v0, v31, 1                 
   355 : sar.u64                  v0, v0, 63                 
   356 : sar.i8                   v0, v0, 1                  
   357 : sar.i8                   v31, v0, 1                 
   358 : sar.i8                   v0, v31, 1                 
   359 : sar.i8                   v0, v0, 7                  
   360 : sar.i16                  v0, v0, 1                  
   361 : sar.i16                  v31, v0, 1                 
   362 : sar.i16                  v0, v31, 1                 
   363 : sar.i16                  v0, v0, 15                 
   364 : sar.i32                  v0, v0, 1                  
   365 : sar.i32                  v31, v0, 1                 
   366 : sar.i32                  v0, v31, 1                 
   367 : sar.i32                  v0, v0, 31                 
   368 : sar.i64                  v0, v0, 1                  
   369 : sar.i64                  v31, v0, 1                 
   370 : sar.i64                  v0, v31, 1                 
   371 : sar.i64                  v0, v0, 63                 
   372 : and                      v0, v0, v0                 
   373 : and                      v31, v0, v0                
   374 : and                      v0, v31, v0                
   375 : and                      v0, v0, v31                
   376 : or                       v0, v0, v0                 
   377 : or                       v31, v0, v0                
   378 : or                       v0, v31, v0                
   379 : or                       v0, v0, v31                
   380 : xor                      v0, v0, v0                 
   381 : xor                      v31, v0, v0                
   382 : xor                      v0, v31, v0                
   383 : xor                      v0, v0, v31                
   384 : not                      v0, v0                     
   385 : not                      v31, v0                    
   386 : not                      v0, v31                    
   387 : mov                      v0, 0                      
   388 : mov                      v31, 0                     
   389 : mov                      v0, 255                    
   390 : mov                      v0, 0                      
   391 : mov                      v31, 0                     
   392 : mov                      v0, 255                    
   393 : mov                      v0, 0                      
   394 : mov                      v31, 0                     
   395 : mov                      v0, 255                    
   396 : mov                      v0, -1                     
   397 : mov                      v31, -1                    
   398 : mov                      v0, -256                   
   399 : mov                      v0, -1                     
   400 : mov                      v31, -1                    
   401 : mov                      v0, -256                   
   402 : min.fp16                 v0, v0, v0                 
   403 : min.fp16                 v31, v0, v0                
   404 : min.fp16                 v0, v31, v0                
   405 : min.fp16                 v0, v0, v31                
   406 : min.fp32                 v0, v0, v0                 
   407 : min.fp32                 v31, v0, v0                
   408 : min.fp32                 v0, v31, v0                
   409 : min.fp32                 v0, v0, v31                
   410 : min.fp64                 v0, v0, v0                 
   411 : min.fp64                 v31, v0, v0                
   412 : min.fp64                 v0, v31, v0                
   413 : min.fp64                 v0, v0, v31                
   414 : max.fp16                 v0, v0, v0                 
   415 : max.fp16                 v31, v0, v0                
   416 : max.fp16                 v0, v31, v0                
   417 : max.fp16                 v0, v0, v31                
   418 : max.fp32                 v0, v0, v0                 
   419 : max.fp32                 v31, v0, v0                
   420 : max.fp32                 v0, v31, v0                
   421 : max.fp32                 v0, v0, v31                
   422 : max.fp64                 v0, v0, v0                 
   423 : max.fp64                 v31, v0, v0                
   424 : max.fp64                 v0, v31, v0                
   425 : max.fp64                 v0, v0, v31                
   426 : fma.fp16                 v0, v0, v0, v0             
   427 : fma.fp16                 v31, v31, v0, v0           
   428 : fma.fp16                 v0, v0, v31, v0            
   429 : fma.fp16                 v0, v0, v0, v31            
   430 : fma.fp32                 v0, v0, v0, v0             
   431 : fma.fp32                 v31, v31, v0, v0           
   432 : fma.fp32                 v0, v0, v31, v0            
   433 : fma.fp32                 v0, v0, v0, v31            
   434 : fma.fp64                 v0, v0, v0, v0             
   435 : fma.fp64                 v31, v31, v0, v0           
   436 : fma.fp64                 v0, v0, v31, v0            
   437 : fma.fp64                 v0, v0, v0, v31            
   438 : trunc.fp16_i16           v0, v0                     
   439 : trunc.fp16_i16           v31, v0                    
   440 : trunc.fp16_i16           v0, v31                    
   441 : trunc.fp32_i32           v0, v0                     
   442 : trunc.fp32_i32           v31, v0                    
   443 : trunc.fp32_i32           v0, v31                    
   444 : trunc.fp64_i64           v0, v0                     
   445 : trunc.fp64_i64           v31, v0                    
   446 : trunc.fp64_i64           v0, v31                    
   447 : trunc.fp16_u16           v0, v0                     
   448 : trunc.fp16_u16           v31, v0                    
   449 : trunc.fp16_u16           v0, v31                    
   450 : trunc.fp32_u32           v0, v0                     
   451 : trunc.fp32_u32           v31, v0                    
   452 : trunc.fp32_u32           v0, v31                    
   453 : trunc.fp64_u64           v0, v0                     
   454 : trunc.fp64_u64           v31, v0                    
   455 : trunc.fp64_u64           v0, v31                    
   456 : floor.fp16_i16           v0, v0                     
   457 : floor.fp16_i16           v31, v0                    
   458 : floor.fp16_i16           v0, v31                    
   459 : floor.fp32_i32           v0, v0                     
   460 : floor.fp32_i32           v31, v0                    
   461 : floor.fp32_i32           v0, v31                    
   462 : floor.fp64_i64           v0, v0                     
   463 : floor.fp64_i64           v31, v0                    
   464 : floor.fp64_i64           v0, v31                    
   465 : floor.fp16_u16           v0, v0                     
   466 : floor.fp16_u16           v31, v0                    
   467 : floor.fp16_u16           v0, v31                    
   468 : floor.fp32_u32           v0, v0                     
   469 : floor.fp32_u32           v31, v0                    
   470 : floor.fp32_u32           v0, v31                    
   471 : floor.fp64_u64           v0, v0                     
   472 : floor.fp64_u64           v31, v0                    
   473 : floor.fp64_u64           v0, v31                    
   474 : cast.i16_fp16            v0, v0                     
   475 : cast.i16_fp16            v31, v0                    
   476 : cast.i16_fp16            v0, v31                    
   477 : cast.i32_fp32            v0, v0                     
   478 : cast.i32_fp32            v31, v0                    
   479 : cast.i32_fp32            v0, v31                    
   480 : cast.i64_fp64            v0, v0                     
   481 : cast.i64_fp64            v31, v0                    
   482 : cast.i64_fp64            v0, v31                    
   483 : cast.u16_fp16            v0, v0                     
   484 : cast.u16_fp16            v31, v0                    
   485 : cast.u16_fp16            v0, v31                    
   486 : cast.u32_fp32            v0, v0                     
   487 : cast.u32_fp32            v31, v0                    
   488 : cast.u32_fp32            v0, v31                    
   489 : cast.u64_fp64            v0, v0                     
   490 : cast.u64_fp64            v31, v0                    
   491 : cast.u64_fp64            v0, v31                    
   492 : gt.u16                   v0, v0, v0                 
   493 : gt.u16                   v31, v0, v0                
   494 : gt.u16                   v0, v31, v0                
   495 : gt.u16                   v0, v0, v31                
   496 : gt.u32                   v0, v0, v0                 
   497 : gt.u32                   v31, v0, v0                
   498 : gt.u32                   v0, v31, v0                
   499 : gt.u32                   v0, v0, v31                
   500 : gt.u64                   v0, v0, v0                 
   501 : gt.u64                   v31, v0, v0                
   502 : gt.u64                   v0, v31, v0                
   503 : gt.u64                   v0, v0, v31                
   504 : ge.u16                   v0, v0, v0                 
   505 : ge.u16                   v31, v0, v0                
   506 : ge.u16                   v0, v31, v0                
   507 : ge.u16                   v0, v0, v31                
   508 : ge.u32                   v0, v0, v0                 
   509 : ge.u32                   v31, v0, v0                
   510 : ge.u32                   v0, v31, v0                
   511 : ge.u32                   v0, v0, v31                
   512 : ge.u64                   v0, v0, v0                 
   513 : ge.u64                   v31, v0, v0                
   514 : ge.u64                   v0, v31, v0                
   515 : ge.u64                   v0, v0, v31                
   516 : eq.u16                   v0, v0, v0                 
   517 : eq.u16                   v31, v0, v0                
   518 : eq.u16                   v0, v31, v0                
   519 : eq.u16                   v0, v0, v31                
   520 : eq.u32                   v0, v0, v0                 
   521 : eq.u32                   v31, v0, v0                
   522 : eq.u32                   v0, v31, v0                
   523 : eq.u32                   v0, v0, v31                
   524 : eq.u64                   v0, v0, v0                 
   525 : eq.u64                   v31, v0, v0                
   526 : eq.u64                   v0, v31, v0                
   527 : eq.u64                   v0, v0, v31                
   528 : neg.i8                   v0, v0                     
   529 : neg.i8                   v31, v0                    
   530 : neg.i8                   v0, v31                    
   531 : neg.i16                  v0, v0                     
   532 : neg.i16                  v31, v0                    
   533 : neg.i16                  v0, v31                    
   534 : neg.i32                  v0, v0                     
   535 : neg.i32                  v31, v0                    
   536 : neg.i32                  v0, v31                    
   537 : neg.i64                  v0, v0                     
   538 : neg.i64                  v31, v0                    
   539 : neg.i64                  v0, v31                    
   540 : neg.fp16                 v0, v0                     
   541 : neg.fp16                 v31, v0                    
   542 : neg.fp16                 v0, v31                    
   543 : neg.fp32                 v0, v0                     
   544 : neg.fp32                 v31, v0                    
   545 : neg.fp32                 v0, v31                    
   546 : neg.fp64                 v0, v0                     
   547 : neg.fp64                 v31, v0                    
   548 : neg.fp64                 v0, v31                    
   549 : arm_movk                 i0, 0, 16                  
   550 : arm_movk                 i15, 0, 16                 
   551 : arm_movk                 i0, 65535, 16              
   552 : arm_movk                 i0, 0, 32                  
   553 : arm_movk                 i15, 0, 32                 
   554 : arm_movk                 i0, 65535, 32              
   555 : arm_movk                 i0, 0, 48                  
   556 : arm_movk                 i15, 0, 48                 
   557 : arm_movk                 i0, 65535, 48              
   558 : broadcast.i8             v0, i0                     
   559 : broadcast.i8             v31, i0                    
   560 : broadcast.i8             v0, i15                    
   561 : broadcast.i16            v0, i0                     
   562 : broadcast.i16            v31, i0                    
   563 : broadcast.i16            v0, i15                    
   564 : broadcast.i32            v0, i0                     
   565 : broadcast.i32            v31, i0                    
   566 : broadcast.i32            v0, i15                    
   567 : broadcast.i64            v0, i0                     
   568 : broadcast.i64            v31, i0                    
   569 : broadcast.i64            v0, i15                    
   570 : broadcast.u8             v0, v0, 0                  
   571 : broadcast.u8             v31, v0, 0                 
   572 : broadcast.u8             v0, v31, 0                 
   573 : broadcast.u8             v0, v0, 15                 
   574 : broadcast.u16            v0, v0, 0                  
   575 : broadcast.u16            v31, v0, 0                 
   576 : broadcast.u16            v0, v31, 0                 
   577 : broadcast.u16            v0, v0, 7                  
   578 : broadcast.u32            v0, v0, 0                  
   579 : broadcast.u32            v31, v0, 0                 
   580 : broadcast.u32            v0, v31, 0                 
   581 : broadcast.u32            v0, v0, 3                  
   582 : broadcast.u64            v0, v0, 0                  
   583 : broadcast.u64            v31, v0, 0                 
   584 : broadcast.u64            v0, v31, 0                 
   585 : broadcast.u64            v0, v0, 1                  
   586 : cast.i16.from.i8.low     v0, v0                     
   587 : cast.i16.from.i8.low     v31, v0                    
   588 : cast.i16.from.i8.low     v0, v31                    
   589 : cast.i32.from.i16.low    v0, v0                     
   590 : cast.i32.from.i16.low    v31, v0                    
   591 : cast.i32.from.i16.low    v0, v31                    
   592 : cast.i64.from.i32.low    v0, v0                     
   593 : cast.i64.from.i32.low    v31, v0                    
   594 : cast.i64.from.i32.low    v0, v31                    
   595 : cast.u16.from.u8.low     v0, v0                     
   596 : cast.u16.from.u8.low     v31, v0                    
   597 : cast.u16.from.u8.low     v0, v31                    
   598 : cast.u32.from.u16.low    v0, v0                     
   599 : cast.u32.from.u16.low    v31, v0                    
   600 : cast.u32.from.u16.low    v0, v31                    
   601 : cast.u64.from.u32.low    v0, v0                     
   602 : cast.u64.from.u32.low    v31, v0                    
   603 : cast.u64.from.u32.low    v0, v31                    
   604 : cast.i16.from.i8.high    v0, v0                     
   605 : cast.i16.from.i8.high    v31, v0                    
   606 : cast.i16.from.i8.high    v0, v31                    
   607 : cast.i32.from.i16.high   v0, v0                     
   608 : cast.i32.from.i16.high   v31, v0                    
   609 : cast.i32.from.i16.high   v0, v31                    
   610 : cast.i64.from.i32.high   v0, v0                     
   611 : cast.i64.from.i32.high   v31, v0                    
   612 : cast.i64.from.i32.high   v0, v31                    
   613 : cast.u16.from.u8.high    v0, v0                     
   614 : cast.u16.from.u8.high    v31, v0                    
   615 : cast.u16.from.u8.high    v0, v31                    
   616 : cast.u32.from.u16.high   v0, v0                     
   617 : cast.u32.from.u16.high   v31, v0                    
   618 : cast.u32.from.u16.high   v0, v31                    
   619 : cast.u64.from.u32.high   v0, v0                     
   620 : cast.u64.from.u32.high   v31, v0                    
   621 : cast.u64.from.u32.high   v0, v31                    
   622 : shrink.i8.from.i16.low   v0, v0                     
   623 : shrink.i8.from.i16.low   v31, v0                    
   624 : shrink.i8.from.i16.low   v0, v31                    
   625 : shrink.i16.from.i32.low  v0, v0                     
   626 : shrink.i16.from.i32.low  v31, v0                    
   627 : shrink.i16.from.i32.low  v0, v31                    
   628 : shrink.i32.from.i64.low  v0, v0                     
   629 : shrink.i32.from.i64.low  v31, v0                    
   630 : shrink.i32.from.i64.low  v0, v31                    
   631 : shrink.i8.from.i16.high  v0, v0                     
   632 : shrink.i8.from.i16.high  v31, v0                    
   633 : shrink.i8.from.i16.high  v0, v31                    
   634 : shrink.i16.from.i32.high v0, v0                     
   635 : shrink.i16.from.i32.high v31, v0                    
   636 : shrink.i16.from.i32.high v0, v31                    
   637 : shrink.i32.from.i64.high v0, v0                     
   638 : shrink.i32.from.i64.high v31, v0                    
   639 : shrink.i32.from.i64.high v0, v31                    
   640 : reduce.max.i8            v0, v0                     
   641 : reduce.max.i8            v31, v0                    
   642 : reduce.max.i8            v0, v31                    
   643 : reduce.max.i16           v0, v0                     
   644 : reduce.max.i16           v31, v0                    
   645 : reduce.max.i16           v0, v31                    
   646 : reduce.max.i32           v0, v0                     
   647 : reduce.max.i32           v31, v0                    
   648 : reduce.max.i32           v0, v31                    
   649 : reduce.max.u8            v0, v0                     
   650 : reduce.max.u8            v31, v0                    
   651 : reduce.max.u8            v0, v31                    
   652 : reduce.max.u16           v0, v0                     
   653 : reduce.max.u16           v31, v0                    
   654 : reduce.max.u16           v0, v31                    
   655 : reduce.max.u32           v0, v0                     
   656 : reduce.max.u32           v31, v0                    
   657 : reduce.max.u32           v0, v31                    
   658 : reduce.max.fp16          v0, v0                     
   659 : reduce.max.fp16          v31, v0                    
   660 : reduce.max.fp16          v0, v31                    
   661 : reduce.max.fp32          v0, v0                     
   662 : reduce.max.fp32          v31, v0                    
   663 : reduce.max.fp32          v0, v31                    
   664 : reduce.max.i8            v0, v0                     
   665 : reduce.max.i8            v31, v0                    
   666 : reduce.max.i8            v0, v31                    
   667 : reduce.max.i16           v0, v0                     
   668 : reduce.max.i16           v31, v0                    
   669 : reduce.max.i16           v0, v31                    
   670 : reduce.max.i32           v0, v0                     
   671 : reduce.max.i32           v31, v0                    
   672 : reduce.max.i32           v0, v31                    
   673 : reduce.max.u8            v0, v0                     
   674 : reduce.max.u8            v31, v0                    
   675 : reduce.max.u8            v0, v31                    
   676 : reduce.max.u16           v0, v0                     
   677 : reduce.max.u16           v31, v0                    
   678 : reduce.max.u16           v0, v31                    
   679 : reduce.max.u32           v0, v0                     
   680 : reduce.max.u32           v31, v0                    
   681 : reduce.max.u32           v0, v31                    
   682 : reduce.max.fp16          v0, v0                     
   683 : reduce.max.fp16          v31, v0                    
   684 : reduce.max.fp16          v0, v31                    
   685 : reduce.max.fp32          v0, v0                     
   686 : reduce.max.fp32          v31, v0                    
   687 : reduce.max.fp32          v0, v31                    
   688 : vld_lane.i8              v0, 0, i0                  
   689 : vld_lane.i8              v31, 0, i0                 
   690 : vld_lane.i8              v0, 15, i0                 
   691 : vld_lane.i8              v0, 0, i15                 
   692 : vld_lane.i16             v0, 0, i0                  
   693 : vld_lane.i16             v31, 0, i0                 
   694 : vld_lane.i16             v0, 7, i0                  
   695 : vld_lane.i16             v0, 0, i15                 
   696 : vld_lane.i32             v0, 0, i0                  
   697 : vld_lane.i32             v31, 0, i0                 
   698 : vld_lane.i32             v0, 3, i0                  
   699 : vld_lane.i32             v0, 0, i15                 
   700 : vld_lane.i64             v0, 0, i0                  
   701 : vld_lane.i64             v31, 0, i0                 
   702 : vld_lane.i64             v0, 1, i0                  
   703 : vld_lane.i64             v0, 0, i15                 
   704 : vld_deinterleave2.i8     v0, v1, i0                 
   705 : vld_deinterleave2.i8     v31, v0, i0                
   706 : vld_deinterleave2.i8     v0, v1, i15                
   707 : vld_deinterleave2.i16    v0, v1, i0                 
   708 : vld_deinterleave2.i16    v31, v0, i0                
   709 : vld_deinterleave2.i16    v0, v1, i15                
   710 : vld_deinterleave2.i32    v0, v1, i0                 
   711 : vld_deinterleave2.i32    v31, v0, i0                
   712 : vld_deinterleave2.i32    v0, v1, i15                
   713 : vld_deinterleave2.i64    v0, v1, i0                 
   714 : vld_deinterleave2.i64    v31, v0, i0                
   715 : vld_deinterleave2.i64    v0, v1, i15                
   716 : vld_lane.i8              v0, i0                     
   717 : vld_lane.i8              v31, i0                    
   718 : vld_lane.i8              v0, i15                    
   719 : vld_lane.i16             v0, i0                     
   720 : vld_lane.i16             v31, i0                    
   721 : vld_lane.i16             v0, i15                    
   722 : vld_lane.i32             v0, i0                     
   723 : vld_lane.i32             v31, i0                    
   724 : vld_lane.i32             v0, i15                    
   725 : vld_lane.i64             v0, i0                     
   726 : vld_lane.i64             v31, i0                    
   727 : vld_lane.i64             v0, i15                    
   728 : vst_lane.i8              i0, v0, 0                  
   729 : vst_lane.i8              i0, v31, 0                 
   730 : vst_lane.i8              i0, v0, 15                 
   731 : vst_lane.i8              i15, v0, 0                 
   732 : vst_lane.i16             i0, v0, 0                  
   733 : vst_lane.i16             i0, v31, 0                 
   734 : vst_lane.i16             i0, v0, 7                  
   735 : vst_lane.i16             i15, v0, 0                 
   736 : vst_lane.i32             i0, v0, 0                  
   737 : vst_lane.i32             i0, v31, 0                 
   738 : vst_lane.i32             i0, v0, 3                  
   739 : vst_lane.i32             i15, v0, 0                 
   740 : vst_lane.i64             i0, v0, 0                  
   741 : vst_lane.i64             i0, v31, 0                 
   742 : vst_lane.i64             i0, v0, 1                  
   743 : vst_lane.i64             i15, v0, 0                 
   744 : vst_lane.i8              i0, v0                     
   745 : vst_lane.i8              i15, v0                    
   746 : vst_lane.i8              i0, v31                    
   747 : vst_lane.i16             i0, v0                     
   748 : vst_lane.i16             i15, v0                    
   749 : vst_lane.i16             i0, v31                    
   750 : vst_lane.i32             i0, v0                     
   751 : vst_lane.i32             i15, v0                    
   752 : vst_lane.i32             i0, v31                    
   753 : vst_lane.i64             i0, v0                     
   754 : vst_lane.i64             i15, v0                    
   755 : vst_lane.i64             i0, v31                    
   756 : ext.i8                   v0, v0, v0, 0              
   757 : ext.i8                   v31, v0, v0, 0             
   758 : ext.i8                   v0, v31, v0, 0             
   759 : ext.i8                   v0, v0, v31, 0             
   760 : ext.i8                   v0, v0, v0, 15             
   761 : ext.i16                  v0, v0, v0, 0              
   762 : ext.i16                  v31, v0, v0, 0             
   763 : ext.i16                  v0, v31, v0, 0             
   764 : ext.i16                  v0, v0, v31, 0             
   765 : ext.i16                  v0, v0, v0, 7              
   766 : ext.i32                  v0, v0, v0, 0              
   767 : ext.i32                  v31, v0, v0, 0             
   768 : ext.i32                  v0, v31, v0, 0             
   769 : ext.i32                  v0, v0, v31, 0             
   770 : ext.i32                  v0, v0, v0, 3              
   771 : ext.i64                  v0, v0, v0, 0              
   772 : ext.i64                  v31, v0, v0, 0             
   773 : ext.i64                  v0, v31, v0, 0             
   774 : ext.i64                  v0, v0, v31, 0             
   775 : ext.i64                  v0, v0, v0, 1              
   776 : fma.fp16                 v0, v0, v0, v0, 0          
   777 : fma.fp16                 v31, v31, v0, v0, 0        
   778 : fma.fp16                 v0, v0, v31, v0, 0         
   779 : fma.fp16                 v0, v0, v0, v15, 0         
   780 : fma.fp16                 v0, v0, v0, v0, 7          
   781 : fma.fp32                 v0, v0, v0, v0, 0          
   782 : fma.fp32                 v31, v31, v0, v0, 0        
   783 : fma.fp32                 v0, v0, v31, v0, 0         
   784 : fma.fp32                 v0, v0, v0, v31, 0         
   785 : fma.fp32                 v0, v0, v0, v0, 3          
   786 : fma.fp64                 v0, v0, v0, v0, 0          
   787 : fma.fp64                 v31, v31, v0, v0, 0        
   788 : fma.fp64                 v0, v0, v31, v0, 0         
   789 : fma.fp64                 v0, v0, v0, v31, 0         
   790 : fma.fp64                 v0, v0, v0, v0, 1          
   791 : gt.u8                    v0, v0, v0                 
   792 : gt.u8                    v31, v0, v0                
   793 : gt.u8                    v0, v31, v0                
   794 : gt.u8                    v0, v0, v31                
   795 : gt.u16                   v0, v0, v0                 
   796 : gt.u16                   v31, v0, v0                
   797 : gt.u16                   v0, v31, v0                
   798 : gt.u16                   v0, v0, v31                
   799 : gt.u32                   v0, v0, v0                 
   800 : gt.u32                   v31, v0, v0                
   801 : gt.u32                   v0, v31, v0                
   802 : gt.u32                   v0, v0, v31                
   803 : gt.u64                   v0, v0, v0                 
   804 : gt.u64                   v31, v0, v0                
   805 : gt.u64                   v0, v31, v0                
   806 : gt.u64                   v0, v0, v31                
   807 : ge.u8                    v0, v0, v0                 
   808 : ge.u8                    v31, v0, v0                
   809 : ge.u8                    v0, v31, v0                
   810 : ge.u8                    v0, v0, v31                
   811 : ge.u16                   v0, v0, v0                 
   812 : ge.u16                   v31, v0, v0                
   813 : ge.u16                   v0, v31, v0                
   814 : ge.u16                   v0, v0, v31                
   815 : ge.u32                   v0, v0, v0                 
   816 : ge.u32                   v31, v0, v0                
   817 : ge.u32                   v0, v31, v0                
   818 : ge.u32                   v0, v0, v31                
   819 : ge.u64                   v0, v0, v0                 
   820 : ge.u64                   v31, v0, v0                
   821 : ge.u64                   v0, v31, v0                
   822 : ge.u64                   v0, v0, v31                
   823 : eq.u8                    v0, v0, v0                 
   824 : eq.u8                    v31, v0, v0                
   825 : eq.u8                    v0, v31, v0                
   826 : eq.u8                    v0, v0, v31                
   827 : eq.u16                   v0, v0, v0                 
   828 : eq.u16                   v31, v0, v0                
   829 : eq.u16                   v0, v31, v0                
   830 : eq.u16                   v0, v0, v31                
   831 : eq.u32                   v0, v0, v0                 
   832 : eq.u32                   v31, v0, v0                
   833 : eq.u32                   v0, v31, v0                
   834 : eq.u32                   v0, v0, v31                
   835 : eq.u64                   v0, v0, v0                 
   836 : eq.u64                   v31, v0, v0                
   837 : eq.u64                   v0, v31, v0                
   838 : eq.u64                   v0, v0, v31                
   839 : ge.u8                    v0, v0, v0                 
   840 : ge.u8                    v31, v0, v0                
   841 : ge.u8                    v0, v31, v0                
   842 : ge.u8                    v0, v0, v31                
   843 : ge.u16                   v0, v0, v0                 
   844 : ge.u16                   v31, v0, v0                
   845 : ge.u16                   v0, v31, v0                
   846 : ge.u16                   v0, v0, v31                
   847 : ge.u32                   v0, v0, v0                 
   848 : ge.u32                   v31, v0, v0                
   849 : ge.u32                   v0, v31, v0                
   850 : ge.u32                   v0, v0, v31                
   851 : ge.u64                   v0, v0, v0                 
   852 : ge.u64                   v31, v0, v0                
   853 : ge.u64                   v0, v31, v0                
   854 : ge.u64                   v0, v0, v31                
   855 : gt.u8                    v0, v0, v0                 
   856 : gt.u8                    v31, v0, v0                
   857 : gt.u8                    v0, v31, v0                
   858 : gt.u8                    v0, v0, v31                
   859 : gt.u16                   v0, v0, v0                 
   860 : gt.u16                   v31, v0, v0                
   861 : gt.u16                   v0, v31, v0                
   862 : gt.u16                   v0, v0, v31                
   863 : gt.u32                   v0, v0, v0                 
   864 : gt.u32                   v31, v0, v0                
   865 : gt.u32                   v0, v31, v0                
   866 : gt.u32                   v0, v0, v31                
   867 : gt.u64                   v0, v0, v0                 
   868 : gt.u64                   v31, v0, v0                
   869 : gt.u64                   v0, v31, v0                
   870 : gt.u64                   v0, v0, v31                
   871 : getlane.u8               i0, v0, 0                  
   872 : getlane.u8               i15, v0, 0                 
   873 : getlane.u8               i0, v31, 0                 
   874 : getlane.u8               i0, v0, 15                 
   875 : getlane.u16              i0, v0, 0                  
   876 : getlane.u16              i15, v0, 0                 
   877 : getlane.u16              i0, v31, 0                 
   878 : getlane.u16              i0, v0, 7                  
   879 : getlane.u32              i0, v0, 0                  
   880 : getlane.u32              i15, v0, 0                 
   881 : getlane.u32              i0, v31, 0                 
   882 : getlane.u32              i0, v0, 3                  
   883 : getlane.u64              i0, v0, 0                  
   884 : getlane.u64              i15, v0, 0                 
   885 : getlane.u64              i0, v31, 0                 
   886 : getlane.u64              i0, v0, 1                  
   887 : setlane.u8               v0, 0, i0                  
   888 : setlane.u8               v31, 0, i0                 
   889 : setlane.u8               v0, 15, i0                 
   890 : setlane.u8               v0, 0, i15                 
   891 : setlane.u16              v0, 0, i0                  
   892 : setlane.u16              v31, 0, i0                 
   893 : setlane.u16              v0, 7, i0                  
   894 : setlane.u16              v0, 0, i15                 
   895 : setlane.u32              v0, 0, i0                  
   896 : setlane.u32              v31, 0, i0                 
   897 : setlane.u32              v0, 3, i0                  
   898 : setlane.u32              v0, 0, i15                 
   899 : setlane.u64              v0, 0, i0                  
   900 : setlane.u64              v31, 0, i0                 
   901 : setlane.u64              v0, 1, i0                  
   902 : setlane.u64              v0, 0, i15                 
   903 : setlane.u8               v0, 0, v0, 0               
   904 : setlane.u8               v31, 0, v0, 0              
   905 : setlane.u8               v0, 15, v0, 0              
   906 : setlane.u8               v0, 0, v31, 0              
   907 : setlane.u8               v0, 0, v0, 15              
   908 : setlane.u16              v0, 0, v0, 0               
   909 : setlane.u16              v31, 0, v0, 0              
   910 : setlane.u16              v0, 7, v0, 0               
   911 : setlane.u16              v0, 0, v31, 0              
   912 : setlane.u16              v0, 0, v0, 7               
   913 : setlane.u32              v0, 0, v0, 0               
   914 : setlane.u32              v31, 0, v0, 0              
   915 : setlane.u32              v0, 3, v0, 0               
   916 : setlane.u32              v0, 0, v31, 0              
   917 : setlane.u32              v0, 0, v0, 3               
   918 : setlane.u64              v0, 0, v0, 0               
   919 : setlane.u64              v31, 0, v0, 0              
   920 : setlane.u64              v0, 1, v0, 0               
   921 : setlane.u64              v0, 0, v31, 0              
   922 : setlane.u64              v0, 0, v0, 1               
   923 : select                   v0, v0, v0, v0             
   924 : select                   v31, v31, v0, v0           
   925 : select                   v0, v0, v31, v0            
   926 : select                   v0, v0, v0, v31            
   927 : call_noret               [i0]()                     
   928 : call_noret               [i15]()                    
