all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i31, i31, 64      
     1 : spill     4, i18            
     2 : spill     5, i19            
     3 : spill     6, i20            
     4 : spill     7, i21            
     5 : mov       i20, 0            
     6 : spill     2, i20            
     7 : mov       i20, 0            
     8 : spill     1, i20            
     9 : mov       i20, 0            
    10 : spill     0, i20            
    11 : mov       i19, 2            
    12 : mov       i18, 1            
    13 : cmp       i1, 1             
    14 : select    i18, 28, i19, i18 
    15 : mov       i19, 4            
    16 : cmp       i1, 3             
    17 : select    i18, 28, i19, i18 
    18 : mov       i19, 8            
    19 : cmp       i1, 5             
    20 : select    i20, 28, i19, i18 
    21 : spill     3, i20            
    22 : mov       i19, 2            
    23 : mov       i18, 1            
    24 : cmp       i3, 1             
    25 : select    i18, 28, i19, i18 
    26 : mov       i19, 4            
    27 : cmp       i3, 3             
    28 : select    i18, 28, i19, i18 
    29 : mov       i19, 8            
    30 : cmp       i3, 5             
    31 : select    i18, 28, i19, i18 
    32 : label 0:                    
    33 : unspill   i20, 2            
    34 : cmp       i20, i4           
    35 : jmp_ge 2                    
    36 : mov       i19, 0            
    37 : cmp       i1, 0             
    38 : jmp_ne 4                    
    39 : unspill   i20, 1            
    40 : load.u8   i19, i0, i20      
    41 : jmp       5                 
    42 : label 4:                    
    43 : cmp       i1, 1             
    44 : jmp_ne 7                    
    45 : unspill   i20, 1            
    46 : load.i8   i19, i0, i20      
    47 : jmp       8                 
    48 : label 7:                    
    49 : cmp       i1, 2             
    50 : jmp_ne 10                   
    51 : unspill   i20, 1            
    52 : load.u16  i19, i0, i20      
    53 : jmp       11                
    54 : label 10:                   
    55 : cmp       i1, 3             
    56 : jmp_ne 13                   
    57 : unspill   i20, 1            
    58 : load.i16  i19, i0, i20      
    59 : jmp       14                
    60 : label 13:                   
    61 : cmp       i1, 4             
    62 : jmp_ne 16                   
    63 : unspill   i20, 1            
    64 : load.u32  i19, i0, i20      
    65 : jmp       17                
    66 : label 16:                   
    67 : cmp       i1, 5             
    68 : jmp_ne 19                   
    69 : unspill   i20, 1            
    70 : load.i32  i19, i0, i20      
    71 : jmp       20                
    72 : label 19:                   
    73 : cmp       i1, 6             
    74 : jmp_ne 22                   
    75 : unspill   i20, 1            
    76 : load.u64  i19, i0, i20      
    77 : jmp       23                
    78 : label 22:                   
    79 : unspill   i20, 1            
    80 : load.i64  i19, i0, i20      
    81 : label 5:                    
    82 : label 8:                    
    83 : label 11:                   
    84 : label 14:                   
    85 : label 17:                   
    86 : label 20:                   
    87 : label 23:                   
    88 : cmp       i3, 0             
    89 : jmp_ne 25                   
    90 : unspill   i20, 0            
    91 : store.u8  i2, i20, i19      
    92 : jmp       26                
    93 : label 25:                   
    94 : cmp       i3, 1             
    95 : jmp_ne 28                   
    96 : unspill   i20, 0            
    97 : store.i8  i2, i20, i19      
    98 : jmp       29                
    99 : label 28:                   
   100 : cmp       i3, 2             
   101 : jmp_ne 31                   
   102 : unspill   i20, 0            
   103 : store.u16 i2, i20, i19      
   104 : jmp       32                
   105 : label 31:                   
   106 : cmp       i3, 3             
   107 : jmp_ne 34                   
   108 : unspill   i20, 0            
   109 : store.i16 i2, i20, i19      
   110 : jmp       35                
   111 : label 34:                   
   112 : cmp       i3, 4             
   113 : jmp_ne 37                   
   114 : unspill   i20, 0            
   115 : store.u32 i2, i20, i19      
   116 : jmp       38                
   117 : label 37:                   
   118 : cmp       i3, 5             
   119 : jmp_ne 40                   
   120 : unspill   i20, 0            
   121 : store.i32 i2, i20, i19      
   122 : jmp       41                
   123 : label 40:                   
   124 : cmp       i3, 6             
   125 : jmp_ne 43                   
   126 : unspill   i20, 0            
   127 : store.u64 i2, i20, i19      
   128 : jmp       44                
   129 : label 43:                   
   130 : unspill   i20, 0            
   131 : store.i64 i2, i20, i19      
   132 : label 26:                   
   133 : label 29:                   
   134 : label 32:                   
   135 : label 35:                   
   136 : label 38:                   
   137 : label 41:                   
   138 : label 44:                   
   139 : unspill   i20, 1            
   140 : unspill   i21, 3            
   141 : add       i20, i20, i21     
   142 : spill     1, i20            
   143 : unspill   i20, 0            
   144 : add       i20, i20, i18     
   145 : spill     0, i20            
   146 : unspill   i20, 2            
   147 : add       i20, i20, 1       
   148 : spill     2, i20            
   149 : jmp       0                 
   150 : label 2:                    
   151 : unspill   i18, 4            
   152 : unspill   i19, 5            
   153 : unspill   i20, 6            
   154 : unspill   i21, 7            
   155 : add       i31, i31, 64      
   156 : ret                         
