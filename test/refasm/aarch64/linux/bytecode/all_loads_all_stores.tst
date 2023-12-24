all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i31, i31, 64  
     1 : spill     3, i4         
     2 : spill     5, i25        
     3 : spill     6, i26        
     4 : spill     7, i27        
     5 : mov       i26, 0        
     6 : spill     2, i26        
     7 : mov       i26, 0        
     8 : spill     0, i26        
     9 : mov       i26, 0        
    10 : spill     1, i26        
    11 : mov       i25, 2        
    12 : mov       i4, 1         
    13 : cmp       i1, 1         
    14 : select_gt i4, i25, i4   
    15 : mov       i25, 4        
    16 : cmp       i1, 3         
    17 : select_gt i4, i25, i4   
    18 : mov       i25, 8        
    19 : cmp       i1, 5         
    20 : select_gt i26, i25, i4  
    21 : spill     4, i26        
    22 : mov       i25, 2        
    23 : mov       i4, 1         
    24 : cmp       i3, 1         
    25 : select_gt i4, i25, i4   
    26 : mov       i25, 4        
    27 : cmp       i3, 3         
    28 : select_gt i4, i25, i4   
    29 : mov       i25, 8        
    30 : cmp       i3, 5         
    31 : select_gt i4, i25, i4   
    32 : label 0:                
    33 : unspill   i27, 3        
    34 : unspill   i26, 2        
    35 : cmp       i26, i27      
    36 : jmp_ge 2                
    37 : mov       i25, 0        
    38 : cmp       i1, 0         
    39 : jmp_ne 4                
    40 : unspill   i26, 0        
    41 : load.u8   i25, i0, i26  
    42 : jmp       5             
    43 : label 4:                
    44 : cmp       i1, 1         
    45 : jmp_ne 7                
    46 : unspill   i26, 0        
    47 : load.i8   i25, i0, i26  
    48 : jmp       8             
    49 : label 7:                
    50 : cmp       i1, 2         
    51 : jmp_ne 10               
    52 : unspill   i26, 0        
    53 : load.u16  i25, i0, i26  
    54 : jmp       11            
    55 : label 10:               
    56 : cmp       i1, 3         
    57 : jmp_ne 13               
    58 : unspill   i26, 0        
    59 : load.i16  i25, i0, i26  
    60 : jmp       14            
    61 : label 13:               
    62 : cmp       i1, 4         
    63 : jmp_ne 16               
    64 : unspill   i26, 0        
    65 : load.u32  i25, i0, i26  
    66 : jmp       17            
    67 : label 16:               
    68 : cmp       i1, 5         
    69 : jmp_ne 19               
    70 : unspill   i26, 0        
    71 : load.i32  i25, i0, i26  
    72 : jmp       20            
    73 : label 19:               
    74 : cmp       i1, 6         
    75 : jmp_ne 22               
    76 : unspill   i26, 0        
    77 : load.u64  i25, i0, i26  
    78 : jmp       23            
    79 : label 22:               
    80 : unspill   i26, 0        
    81 : load.i64  i25, i0, i26  
    82 : label 5:                
    83 : label 8:                
    84 : label 11:               
    85 : label 14:               
    86 : label 17:               
    87 : label 20:               
    88 : label 23:               
    89 : cmp       i3, 0         
    90 : jmp_ne 25               
    91 : unspill   i26, 1        
    92 : store.u8  i2, i26, i25  
    93 : jmp       26            
    94 : label 25:               
    95 : cmp       i3, 1         
    96 : jmp_ne 28               
    97 : unspill   i26, 1        
    98 : store.i8  i2, i26, i25  
    99 : jmp       29            
   100 : label 28:               
   101 : cmp       i3, 2         
   102 : jmp_ne 31               
   103 : unspill   i26, 1        
   104 : store.u16 i2, i26, i25  
   105 : jmp       32            
   106 : label 31:               
   107 : cmp       i3, 3         
   108 : jmp_ne 34               
   109 : unspill   i26, 1        
   110 : store.i16 i2, i26, i25  
   111 : jmp       35            
   112 : label 34:               
   113 : cmp       i3, 4         
   114 : jmp_ne 37               
   115 : unspill   i26, 1        
   116 : store.u32 i2, i26, i25  
   117 : jmp       38            
   118 : label 37:               
   119 : cmp       i3, 5         
   120 : jmp_ne 40               
   121 : unspill   i26, 1        
   122 : store.i32 i2, i26, i25  
   123 : jmp       41            
   124 : label 40:               
   125 : cmp       i3, 6         
   126 : jmp_ne 43               
   127 : unspill   i26, 1        
   128 : store.u64 i2, i26, i25  
   129 : jmp       44            
   130 : label 43:               
   131 : unspill   i26, 1        
   132 : store.i64 i2, i26, i25  
   133 : label 26:               
   134 : label 29:               
   135 : label 32:               
   136 : label 35:               
   137 : label 38:               
   138 : label 41:               
   139 : label 44:               
   140 : unspill   i26, 0        
   141 : unspill   i27, 4        
   142 : add       i26, i26, i27 
   143 : spill     0, i26        
   144 : unspill   i26, 1        
   145 : add       i26, i26, i4  
   146 : spill     1, i26        
   147 : unspill   i26, 2        
   148 : add       i26, i26, 1   
   149 : spill     2, i26        
   150 : jmp       0             
   151 : label 2:                
   152 : unspill   i25, 5        
   153 : unspill   i26, 6        
   154 : unspill   i27, 7        
   155 : add       i31, i31, 64  
   156 : ret                     
