big_immediates(i0, i1)
     0 : cmp           i1, 0         
     1 : jmp_ne 1                    
     2 : mov           i2, 65535     
     3 : store.u64     i0, i2        
     4 : add           i0, i0, 8     
     5 : mov           i2, 0         
     6 : arm_movk      i2, 1, 16     
     7 : store.u64     i0, i2        
     8 : add           i0, i0, 8     
     9 : mov           i2, -32768    
    10 : store.i64     i0, i2        
    11 : add           i0, i0, 8     
    12 : mov           i2, -32769    
    13 : store.i64     i0, i2        
    14 : add           i0, i0, 8     
    15 : mov           i2, 23007     
    16 : arm_movk      i2, 24375, 16 
    17 : store.u64     i0, i2        
    18 : add           i0, i0, 8     
    19 : mov           i2, 22377     
    20 : arm_movk      i2, 35604, 16 
    21 : arm_movk      i2, 48906, 32 
    22 : arm_movk      i2, 16389, 48 
    23 : store.u64     i0, i2        
    24 : add           i0, i0, 8     
    25 : jmp           2             
    26 : label 1:                    
    27 : cmp           i1, 1         
    28 : jmp_ne 4                    
    29 : mov           v0, 0         
    30 : vst.u8        i0, v0        
    31 : add           i0, i0, 16    
    32 : mov           v0, 255       
    33 : vst.u8        i0, v0        
    34 : add           i0, i0, 16    
    35 : jmp           5             
    36 : label 4:                    
    37 : cmp           i1, 2         
    38 : jmp_ne 7                    
    39 : mov           v0, 128       
    40 : vst.i8        i0, v0        
    41 : add           i0, i0, 16    
    42 : jmp           8             
    43 : label 7:                    
    44 : cmp           i1, 3         
    45 : jmp_ne 10                   
    46 : mov           v0, 0         
    47 : vst.u16       i0, v0        
    48 : add           i0, i0, 16    
    49 : mov           v0, 255       
    50 : vst.u16       i0, v0        
    51 : add           i0, i0, 16    
    52 : mov           i2, 256       
    53 : broadcast.u16 v0, i2        
    54 : vst.u16       i0, v0        
    55 : add           i0, i0, 16    
    56 : jmp           11            
    57 : label 10:                   
    58 : cmp           i1, 4         
    59 : jmp_ne 13                   
    60 : mov           i2, 65408     
    61 : broadcast.i16 v0, i2        
    62 : vst.i16       i0, v0        
    63 : add           i0, i0, 16    
    64 : mov           i2, 65407     
    65 : broadcast.i16 v0, i2        
    66 : vst.i16       i0, v0        
    67 : add           i0, i0, 16    
    68 : jmp           14            
    69 : label 13:                   
    70 : cmp           i1, 5         
    71 : jmp_ne 16                   
    72 : mov           v0, 0         
    73 : vst.u32       i0, v0        
    74 : add           i0, i0, 16    
    75 : mov           v0, 255       
    76 : vst.u32       i0, v0        
    77 : add           i0, i0, 16    
    78 : mov           i2, 256       
    79 : broadcast.u32 v0, i2        
    80 : vst.u32       i0, v0        
    81 : add           i0, i0, 16    
    82 : jmp           17            
    83 : label 16:                   
    84 : cmp           i1, 6         
    85 : jmp_ne 19                   
    86 : mov           i2, 65408     
    87 : arm_movk      i2, 65535, 16 
    88 : broadcast.i32 v0, i2        
    89 : vst.i32       i0, v0        
    90 : add           i0, i0, 16    
    91 : mov           i2, 65407     
    92 : arm_movk      i2, 65535, 16 
    93 : broadcast.i32 v0, i2        
    94 : vst.i32       i0, v0        
    95 : add           i0, i0, 16    
    96 : jmp           20            
    97 : label 19:                   
    98 : cmp           i1, 7         
    99 : jmp_ne 22                   
   100 : mov           v0, 0         
   101 : vst.u64       i0, v0        
   102 : add           i0, i0, 16    
   103 : mov           v0, 255       
   104 : vst.u64       i0, v0        
   105 : add           i0, i0, 16    
   106 : mov           v0, 65280     
   107 : vst.u64       i0, v0        
   108 : add           i0, i0, 16    
   109 : jmp           23            
   110 : label 22:                   
   111 : cmp           i1, 8         
   112 : jmp_ne 25                   
   113 : mov           v0, -256      
   114 : vst.i64       i0, v0        
   115 : add           i0, i0, 16    
   116 : jmp           26            
   117 : label 25:                   
   118 : cmp           i1, 9         
   119 : jmp_ne 28                   
   120 : mov           i1, 256       
   121 : broadcast.u64 v0, i1        
   122 : vst.u64       i0, v0        
   123 : add           i0, i0, 16    
   124 : mov           i1, 254       
   125 : broadcast.u64 v0, i1        
   126 : vst.u64       i0, v0        
   127 : add           i0, i0, 16    
   128 : label 2:                    
   129 : label 5:                    
   130 : label 8:                    
   131 : label 11:                   
   132 : label 14:                   
   133 : label 17:                   
   134 : label 20:                   
   135 : label 23:                   
   136 : label 26:                   
   137 : label 28:                   
   138 : ret                         
