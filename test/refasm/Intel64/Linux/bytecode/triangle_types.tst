triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 8    
     1 : spill     0, i12       
     2 : cmp       i7, 0        
     3 : jmp_gt    1            
     4 : mov       i0, 0        
     5 : jmp       51           
     6 : jmp       50           
     7 : label 1:               
     8 : cmp       i6, 0        
     9 : jmp_gt    4            
    10 : mov       i0, 0        
    11 : jmp       51           
    12 : jmp       49           
    13 : label 4:               
    14 : cmp       i2, 0        
    15 : jmp_gt    8            
    16 : mov       i0, 0        
    17 : jmp       51           
    18 : jmp       48           
    19 : label 8:               
    20 : mov       i1, i7       
    21 : add       i1, i1, i6   
    22 : mov       i0, i7       
    23 : add       i0, i0, i2   
    24 : mov       i12, i6      
    25 : add       i12, i12, i2 
    26 : cmp       i7, i12      
    27 : jmp_le    10           
    28 : mov       i0, 0        
    29 : jmp       51           
    30 : jmp       47           
    31 : label 10:              
    32 : cmp       i6, i0       
    33 : jmp_le    13           
    34 : mov       i0, 0        
    35 : jmp       51           
    36 : jmp       46           
    37 : label 13:              
    38 : cmp       i2, i1       
    39 : jmp_le    17           
    40 : mov       i0, 0        
    41 : jmp       51           
    42 : jmp       45           
    43 : label 17:              
    44 : cmp       i7, i6       
    45 : jmp_ne    22           
    46 : cmp       i6, i2       
    47 : jmp_ne    19           
    48 : mov       i0, 2        
    49 : jmp       51           
    50 : jmp       20           
    51 : label 19:              
    52 : mov       i0, 3        
    53 : jmp       51           
    54 : label 20:              
    55 : jmp       28           
    56 : label 22:              
    57 : cmp       i7, i2       
    58 : jmp_ne    25           
    59 : mov       i0, 3        
    60 : jmp       51           
    61 : jmp       27           
    62 : label 25:              
    63 : cmp       i6, i2       
    64 : jmp_ne    26           
    65 : mov       i0, 3        
    66 : jmp       51           
    67 : label 26:              
    68 : label 27:              
    69 : label 28:              
    70 : mul       i7, i7, i7   
    71 : mul       i6, i6, i6   
    72 : mul       i2, i2, i2   
    73 : mov       i1, i7       
    74 : add       i1, i1, i6   
    75 : mov       i0, i7       
    76 : add       i0, i0, i2   
    77 : mov       i12, i6      
    78 : add       i12, i12, i2 
    79 : cmp       i7, i12      
    80 : jmp_ne    30           
    81 : mov       i0, 1        
    82 : jmp       51           
    83 : jmp       36           
    84 : label 30:              
    85 : cmp       i6, i0       
    86 : jmp_ne    33           
    87 : mov       i0, 1        
    88 : jmp       51           
    89 : jmp       35           
    90 : label 33:              
    91 : cmp       i2, i1       
    92 : jmp_ne    34           
    93 : mov       i0, 1        
    94 : jmp       51           
    95 : label 34:              
    96 : label 35:              
    97 : label 36:              
    98 : sub       i12, i12, i7 
    99 : sub       i0, i0, i6   
   100 : sub       i1, i1, i2   
   101 : cmp       i12, 0       
   102 : jmp_ge    38           
   103 : mov       i0, 5        
   104 : jmp       51           
   105 : jmp       44           
   106 : label 38:              
   107 : cmp       i0, 0        
   108 : jmp_ge    41           
   109 : mov       i0, 5        
   110 : jmp       51           
   111 : jmp       43           
   112 : label 41:              
   113 : cmp       i1, 0        
   114 : jmp_ge    42           
   115 : mov       i0, 5        
   116 : jmp       51           
   117 : label 42:              
   118 : label 43:              
   119 : label 44:              
   120 : label 45:              
   121 : label 46:              
   122 : label 47:              
   123 : label 48:              
   124 : label 49:              
   125 : label 50:              
   126 : mov       i0, 4        
   127 : label 51:              
   128 : unspill   i12, 0       
   129 : add       i4, i4, 8    
   130 : ret                    
