triangle_types(i0, i1, i2)
     0 : sub       i31, i31, 16 
     1 : spill     0, i18       
     2 : spill     1, i19       
     3 : cmp       i0, 0        
     4 : jmp_gt    1            
     5 : mov       i0, 0        
     6 : jmp       51           
     7 : jmp       50           
     8 : label 1:               
     9 : cmp       i1, 0        
    10 : jmp_gt    4            
    11 : mov       i0, 0        
    12 : jmp       51           
    13 : jmp       49           
    14 : label 4:               
    15 : cmp       i2, 0        
    16 : jmp_gt    8            
    17 : mov       i0, 0        
    18 : jmp       51           
    19 : jmp       48           
    20 : label 8:               
    21 : add       i3, i0, i1   
    22 : add       i18, i0, i2  
    23 : add       i19, i1, i2  
    24 : cmp       i0, i19      
    25 : jmp_le    10           
    26 : mov       i0, 0        
    27 : jmp       51           
    28 : jmp       47           
    29 : label 10:              
    30 : cmp       i1, i18      
    31 : jmp_le    13           
    32 : mov       i0, 0        
    33 : jmp       51           
    34 : jmp       46           
    35 : label 13:              
    36 : cmp       i2, i3       
    37 : jmp_le    17           
    38 : mov       i0, 0        
    39 : jmp       51           
    40 : jmp       45           
    41 : label 17:              
    42 : cmp       i0, i1       
    43 : jmp_ne    22           
    44 : cmp       i1, i2       
    45 : jmp_ne    19           
    46 : mov       i0, 2        
    47 : jmp       51           
    48 : jmp       20           
    49 : label 19:              
    50 : mov       i0, 3        
    51 : jmp       51           
    52 : label 20:              
    53 : jmp       28           
    54 : label 22:              
    55 : cmp       i0, i2       
    56 : jmp_ne    25           
    57 : mov       i0, 3        
    58 : jmp       51           
    59 : jmp       27           
    60 : label 25:              
    61 : cmp       i1, i2       
    62 : jmp_ne    26           
    63 : mov       i0, 3        
    64 : jmp       51           
    65 : label 26:              
    66 : label 27:              
    67 : label 28:              
    68 : mul       i0, i0, i0   
    69 : mul       i1, i1, i1   
    70 : mul       i2, i2, i2   
    71 : add       i3, i0, i1   
    72 : add       i18, i0, i2  
    73 : add       i19, i1, i2  
    74 : cmp       i0, i19      
    75 : jmp_ne    30           
    76 : mov       i0, 1        
    77 : jmp       51           
    78 : jmp       36           
    79 : label 30:              
    80 : cmp       i1, i18      
    81 : jmp_ne    33           
    82 : mov       i0, 1        
    83 : jmp       51           
    84 : jmp       35           
    85 : label 33:              
    86 : cmp       i2, i3       
    87 : jmp_ne    34           
    88 : mov       i0, 1        
    89 : jmp       51           
    90 : label 34:              
    91 : label 35:              
    92 : label 36:              
    93 : sub       i0, i19, i0  
    94 : sub       i1, i18, i1  
    95 : sub       i2, i3, i2   
    96 : cmp       i0, 0        
    97 : jmp_ge    38           
    98 : mov       i0, 5        
    99 : jmp       51           
   100 : jmp       44           
   101 : label 38:              
   102 : cmp       i1, 0        
   103 : jmp_ge    41           
   104 : mov       i0, 5        
   105 : jmp       51           
   106 : jmp       43           
   107 : label 41:              
   108 : cmp       i2, 0        
   109 : jmp_ge    42           
   110 : mov       i0, 5        
   111 : jmp       51           
   112 : label 42:              
   113 : label 43:              
   114 : label 44:              
   115 : label 45:              
   116 : label 46:              
   117 : label 47:              
   118 : label 48:              
   119 : label 49:              
   120 : label 50:              
   121 : mov       i0, 4        
   122 : label 51:              
   123 : unspill   i18, 0       
   124 : unspill   i19, 1       
   125 : add       i31, i31, 16 
   126 : ret                    
