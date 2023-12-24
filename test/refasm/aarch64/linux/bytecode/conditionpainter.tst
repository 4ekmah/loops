conditionpainter(i0)
     0 : sub        i31, i31, 64  
     1 : spill      1, i0         
     2 : spill      5, i25        
     3 : spill      6, i26        
     4 : spill      7, i27        
     5 : mov        i26, -5       
     6 : spill      0, i26        
     7 : label 0:                 
     8 : unspill    i26, 0        
     9 : cmp        i26, 5        
    10 : jmp_gt 2                 
    11 : unspill    i26, 0        
    12 : add        i2, i26, 5    
    13 : mov        i3, 11        
    14 : mul        i2, i2, i3    
    15 : mov        i3, 8         
    16 : mul        i26, i2, i3   
    17 : spill      3, i26        
    18 : mov        i26, -5       
    19 : spill      2, i26        
    20 : label 3:                 
    21 : unspill    i26, 2        
    22 : cmp        i26, 5        
    23 : jmp_gt 5                 
    24 : unspill    i26, 2        
    25 : add        i25, i26, 3   
    26 : unspill    i26, 0        
    27 : cmp        i26, i25      
    28 : iverson_ge i25           
    29 : unspill    i26, 0        
    30 : cmp        i26, 4        
    31 : iverson_le i1            
    32 : and        i1, i25, i1   
    33 : mov        i25, -2       
    34 : unspill    i26, 2        
    35 : cmp        i26, i25      
    36 : iverson_ge i25           
    37 : and        i1, i1, i25   
    38 : unspill    i26, 2        
    39 : cmp        i26, 0        
    40 : iverson_le i25           
    41 : and        i1, i1, i25   
    42 : unspill    i26, 2        
    43 : sub        i25, i26, 1   
    44 : unspill    i26, 0        
    45 : cmp        i26, i25      
    46 : iverson_le i25           
    47 : unspill    i26, 2        
    48 : cmp        i26, 0        
    49 : iverson_ge i0            
    50 : and        i0, i25, i0   
    51 : unspill    i26, 0        
    52 : cmp        i26, 0        
    53 : iverson_le i25           
    54 : and        i0, i0, i25   
    55 : unspill    i26, 2        
    56 : mul        i25, i26, i26 
    57 : unspill    i26, 0        
    58 : mul        i3, i26, i26  
    59 : add        i3, i25, i3   
    60 : cmp        i3, 9         
    61 : iverson_le i3            
    62 : and        i0, i0, i3    
    63 : or         i26, i1, i0   
    64 : spill      4, i26        
    65 : mov        i1, 2         
    66 : mov        i3, 0         
    67 : unspill    i26, 2        
    68 : cmp        i26, 2        
    69 : iverson_ge i25           
    70 : unspill    i26, 2        
    71 : cmp        i26, 4        
    72 : iverson_le i2            
    73 : and        i2, i25, i2   
    74 : unspill    i26, 0        
    75 : cmp        i26, 1        
    76 : iverson_ge i25           
    77 : unspill    i26, 0        
    78 : cmp        i26, 3        
    79 : iverson_le i0            
    80 : and        i0, i25, i0   
    81 : or         i0, i2, i0    
    82 : unspill    i26, 2        
    83 : mul        i2, i26, i26  
    84 : unspill    i26, 0        
    85 : mul        i25, i26, i26 
    86 : add        i2, i2, i25   
    87 : cmp        i2, 16        
    88 : iverson_le i2            
    89 : and        i0, i0, i2    
    90 : cmp        i0, 0         
    91 : select_ne  i0, i1, i3    
    92 : unspill    i26, 4        
    93 : add        i0, i26, i0   
    94 : mov        i1, 2         
    95 : unspill    i26, 0        
    96 : mul        i1, i26, i1   
    97 : unspill    i26, 2        
    98 : cmp        i1, i26       
    99 : jmp_gt 8                 
   100 : unspill    i26, 2        
   101 : add        i1, i26, 3    
   102 : neg        i1, i1        
   103 : unspill    i26, 2        
   104 : add        i2, i26, 3    
   105 : mul        i1, i1, i2    
   106 : unspill    i26, 0        
   107 : cmp        i26, i1       
   108 : jmp_le 9                 
   109 : label 8:                 
   110 : mov        i1, 2         
   111 : unspill    i26, 2        
   112 : mul        i1, i26, i1   
   113 : unspill    i26, 0        
   114 : cmp        i26, i1       
   115 : jmp_gt 7                 
   116 : unspill    i26, 2        
   117 : add        i1, i26, 3    
   118 : neg        i1, i1        
   119 : unspill    i26, 2        
   120 : add        i2, i26, 3    
   121 : mul        i1, i1, i2    
   122 : unspill    i26, 0        
   123 : cmp        i26, i1       
   124 : jmp_gt 7                 
   125 : label 9:                 
   126 : unspill    i26, 2        
   127 : cmp        i26, 0        
   128 : jmp_gt 7                 
   129 : add        i0, i0, 3     
   130 : label 7:                 
   131 : unspill    i26, 1        
   132 : unspill    i27, 3        
   133 : add        i1, i26, i27  
   134 : unspill    i26, 2        
   135 : add        i2, i26, 5    
   136 : shl        i2, i2, 3     
   137 : add        i1, i1, i2    
   138 : store.i64  i1, i0        
   139 : unspill    i26, 2        
   140 : add        i26, i26, 1   
   141 : spill      2, i26        
   142 : jmp        3             
   143 : label 5:                 
   144 : unspill    i26, 0        
   145 : add        i26, i26, 1   
   146 : spill      0, i26        
   147 : jmp        0             
   148 : label 2:                 
   149 : unspill    i25, 5        
   150 : unspill    i26, 6        
   151 : unspill    i27, 7        
   152 : add        i31, i31, 64  
   153 : ret                      
