instruction_set_test()
     0 : store.u64 6, i0, 256, i0             
     1 : store.i64 7, i0, 256, i0             
     2 : store.u64 6, i0, i0, i0              
     3 : store.i64 7, i0, i0, i0              
     4 : store.u32 4, i0, 256, i0             
     5 : store.i32 5, i0, 256, i0             
     6 : store.u32 4, i0, i0, i0              
     7 : store.i32 5, i0, i0, i0              
     8 : store.u16 2, i0, 256, i0             
     9 : store.i16 3, i0, 256, i0             
    10 : store.u16 2, i0, i0, i0              
    11 : store.i16 3, i0, i0, i0              
    12 : store.u8  0, i0, 256, i0             
    13 : store.i8  1, i0, 256, i0             
    14 : store.u8  0, i0, i0, i0              
    15 : store.i8  1, i0, i0, i0              
    16 : load.u64  i0, 6, i0                  
    17 : load.u64  i0, 6, i0, 256             
    18 : load.u64  i15, 6, i0, 256            
    19 : load.u64  i0, 6, i15, 256            
    20 : load.u64  i0, 6, i0, i0              
    21 : load.u64  i15, 6, i0, i0             
    22 : load.u64  i0, 6, i15, i0             
    23 : load.u64  i0, 6, i0, i15             
    24 : load.i64  i0, 7, i0                  
    25 : load.i64  i0, 7, i0, 256             
    26 : load.i64  i15, 7, i0, 256            
    27 : load.i64  i0, 7, i15, 256            
    28 : load.i64  i0, 7, i0, i0              
    29 : load.i64  i15, 7, i0, i0             
    30 : load.i64  i0, 7, i15, i0             
    31 : load.i64  i0, 7, i0, i15             
    32 : load.u32  i0, 4, i0                  
    33 : load.u32  i0, 4, i0, 256             
    34 : load.u32  i15, 4, i0, 256            
    35 : load.u32  i0, 4, i15, 256            
    36 : load.u32  i0, 4, i0, i0              
    37 : load.u32  i15, 4, i0, i0             
    38 : load.u32  i0, 4, i15, i0             
    39 : load.u32  i0, 4, i0, i15             
    40 : load.i32  i0, 5, i0                  
    41 : load.i32  i0, 5, i0, 256             
    42 : load.i32  i15, 5, i0, 256            
    43 : load.i32  i0, 5, i15, 256            
    44 : load.i32  i0, 5, i0, i0              
    45 : load.i32  i15, 5, i0, i0             
    46 : load.i32  i0, 5, i15, i0             
    47 : load.i32  i0, 5, i0, i15             
    48 : load.u16  i0, 2, i0                  
    49 : load.u16  i0, 2, i0, 256             
    50 : load.u16  i15, 2, i0, 256            
    51 : load.u16  i0, 2, i15, 256            
    52 : load.u16  i0, 2, i0, i0              
    53 : load.u16  i15, 2, i0, i0             
    54 : load.u16  i0, 2, i15, i0             
    55 : load.u16  i0, 2, i0, i15             
    56 : load.i16  i0, 3, i0                  
    57 : load.i16  i0, 3, i0, 256             
    58 : load.i16  i15, 3, i0, 256            
    59 : load.i16  i0, 3, i15, 256            
    60 : load.i16  i0, 3, i0, i0              
    61 : load.i16  i15, 3, i0, i0             
    62 : load.i16  i0, 3, i15, i0             
    63 : load.i16  i0, 3, i0, i15             
    64 : load.u8   i0, 0, i0                  
    65 : load.u8   i0, 0, i0, 256             
    66 : load.u8   i15, 0, i0, 256            
    67 : load.u8   i0, 0, i15, 256            
    68 : load.u8   i0, 0, i0, i0              
    69 : load.u8   i15, 0, i0, i0             
    70 : load.u8   i0, 0, i15, i0             
    71 : load.u8   i0, 0, i0, i15             
    72 : load.i8   i0, 1, i0                  
    73 : load.i8   i0, 1, i0, 256             
    74 : load.i8   i15, 1, i0, 256            
    75 : load.i8   i0, 1, i15, 256            
    76 : load.i8   i0, 1, i0, i0              
    77 : load.i8   i15, 1, i0, i0             
    78 : load.i8   i0, 1, i15, i0             
    79 : load.i8   i0, 1, i0, i15             
    80 : select    i0, 0, i0, i0              
    81 : select    i0, 1, i0, i0              
    82 : select    i0, 2, i0, i0              
    83 : select    i0, 3, i0, i0              
    84 : select    i0, 4, i0, i0              
    85 : select    i0, 5, i0, i0              
    86 : select    i0, 6, i0, i0              
    87 : select    i0, 7, i0, i0              
    88 : select    i15, 0, i0, i0             
    89 : select    i0, 0, i15, i0             
    90 : select    i0, 0, i0, i15             
    91 : arm_cinc  i0, i0, 0                  
    92 : arm_cinc  i0, i0, 1                  
    93 : arm_cinc  i0, i0, 2                  
    94 : arm_cinc  i0, i0, 3                  
    95 : arm_cinc  i0, i0, 4                  
    96 : arm_cinc  i0, i0, 5                  
    97 : arm_cinc  i0, i0, 6                  
    98 : arm_cinc  i0, i0, 7                  
    99 : arm_cinc  i15, i0, 0                 
   100 : arm_cinc  i0, i15, 0                 
   101 : arm_cneg  i0, i0, 0                  
   102 : arm_cneg  i0, i0, 1                  
   103 : arm_cneg  i0, i0, 2                  
   104 : arm_cneg  i0, i0, 3                  
   105 : arm_cneg  i0, i0, 4                  
   106 : arm_cneg  i0, i0, 5                  
   107 : arm_cneg  i0, i0, 6                  
   108 : arm_cneg  i0, i0, 7                  
   109 : arm_cneg  i15, i0, 0                 
   110 : arm_cneg  i0, i15, 0                 
   111 : sar       i0, i0, 63                 
   112 : sar       i15, i0, 63                
   113 : sar       i0, i15, 63                
   114 : sar       i0, i0, i0                 
   115 : sar       i15, i0, i0                
   116 : sar       i0, i15, i0                
   117 : sar       i0, i0, i15                
   118 : shr       i0, i0, 63                 
   119 : shr       i15, i0, 63                
   120 : shr       i0, i15, 63                
   121 : shr       i0, i0, i0                 
   122 : shr       i15, i0, i0                
   123 : shr       i0, i15, i0                
   124 : shr       i0, i0, i15                
   125 : shl       i0, i0, 63                 
   126 : shl       i0, i0, 31                 
   127 : shl       i0, i0, 15                 
   128 : shl       i0, i0, 7                  
   129 : shl       i0, i0, 1                  
   130 : shl       i0, i0, 0                  
   131 : shl       i15, i0, 63                
   132 : shl       i0, i15, 63                
   133 : shl       i0, i0, i0                 
   134 : shl       i15, i0, i0                
   135 : shl       i0, i15, i0                
   136 : shl       i0, i0, i15                
   137 : and       i0, i0, i0                 
   138 : and       i15, i0, i0                
   139 : and       i0, i15, i0                
   140 : and       i0, i0, i15                
   141 : and       i0, i0, 255                
   142 : and       i0, i0, 65280              
   143 : and       i0, i0, -72057594037927936 
   144 : and       i15, i0, 255               
   145 : and       i0, i15, 255               
   146 : or        i0, i0, i0                 
   147 : or        i15, i0, i0                
   148 : or        i0, i15, i0                
   149 : or        i0, i0, i15                
   150 : or        i0, i0, 255                
   151 : or        i0, i0, 65280              
   152 : or        i0, i0, -72057594037927936 
   153 : or        i15, i0, 255               
   154 : or        i0, i15, 255               
   155 : xor       i0, i0, i0                 
   156 : xor       i15, i0, i0                
   157 : xor       i0, i15, i0                
   158 : xor       i0, i0, i15                
   159 : xor       i0, i0, 255                
   160 : xor       i0, i0, 65280              
   161 : xor       i0, i0, -72057594037927936 
   162 : xor       i15, i0, 255               
   163 : xor       i0, i15, 255               
   164 : not       i0, i0                     
   165 : not       i15, i0                    
   166 : not       i0, i15                    
   167 : neg       i0, i0                     
   168 : neg       i15, i0                    
   169 : neg       i0, i15                    
