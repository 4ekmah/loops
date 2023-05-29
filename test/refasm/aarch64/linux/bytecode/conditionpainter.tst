conditionpainter(i0)
     0 : sub        i31, i31, 64  
     1 : spill      1, i0         
     2 : spill      4, i18        
     3 : spill      5, i19        
     4 : spill      6, i20        
     5 : spill      7, i21        
     6 : mov        i20, -5       
     7 : spill      0, i20        
     8 : label 0:                 
     9 : unspill    i20, 0        
    10 : cmp        i20, 5        
    11 : jmp_gt 2                 
    12 : unspill    i20, 0        
    13 : add        i2, i20, 5    
    14 : mov        i3, 11        
    15 : mul        i2, i2, i3    
    16 : mov        i3, 8         
    17 : mul        i20, i2, i3   
    18 : spill      3, i20        
    19 : mov        i20, -5       
    20 : spill      2, i20        
    21 : label 3:                 
    22 : unspill    i20, 2        
    23 : cmp        i20, 5        
    24 : jmp_gt 5                 
    25 : unspill    i20, 2        
    26 : add        i18, i20, 3   
    27 : unspill    i20, 0        
    28 : cmp        i20, i18      
    29 : iverson_ge i18           
    30 : unspill    i20, 0        
    31 : cmp        i20, 4        
    32 : iverson_le i19           
    33 : and        i18, i18, i19 
    34 : mov        i19, -2       
    35 : unspill    i20, 2        
    36 : cmp        i20, i19      
    37 : iverson_ge i19           
    38 : and        i18, i18, i19 
    39 : unspill    i20, 2        
    40 : cmp        i20, 0        
    41 : iverson_le i19           
    42 : and        i18, i18, i19 
    43 : unspill    i20, 2        
    44 : sub        i19, i20, 1   
    45 : unspill    i20, 0        
    46 : cmp        i20, i19      
    47 : iverson_le i19           
    48 : unspill    i20, 2        
    49 : cmp        i20, 0        
    50 : iverson_ge i1            
    51 : and        i1, i19, i1   
    52 : unspill    i20, 0        
    53 : cmp        i20, 0        
    54 : iverson_le i19           
    55 : and        i1, i1, i19   
    56 : unspill    i20, 2        
    57 : mul        i19, i20, i20 
    58 : unspill    i20, 0        
    59 : mul        i0, i20, i20  
    60 : add        i0, i19, i0   
    61 : cmp        i0, 9         
    62 : iverson_le i0            
    63 : and        i0, i1, i0    
    64 : or         i0, i18, i0   
    65 : mov        i1, 2         
    66 : mov        i18, 0        
    67 : unspill    i20, 2        
    68 : cmp        i20, 2        
    69 : iverson_ge i19           
    70 : unspill    i20, 2        
    71 : cmp        i20, 4        
    72 : iverson_le i3            
    73 : and        i3, i19, i3   
    74 : unspill    i20, 0        
    75 : cmp        i20, 1        
    76 : iverson_ge i19           
    77 : unspill    i20, 0        
    78 : cmp        i20, 3        
    79 : iverson_le i2            
    80 : and        i2, i19, i2   
    81 : or         i2, i3, i2    
    82 : unspill    i20, 2        
    83 : mul        i3, i20, i20  
    84 : unspill    i20, 0        
    85 : mul        i19, i20, i20 
    86 : add        i3, i3, i19   
    87 : cmp        i3, 16        
    88 : iverson_le i3            
    89 : and        i2, i2, i3    
    90 : cmp        i2, 0         
    91 : select_ne  i1, i1, i18   
    92 : add        i0, i0, i1    
    93 : mov        i1, 2         
    94 : unspill    i20, 0        
    95 : mul        i1, i20, i1   
    96 : unspill    i20, 2        
    97 : cmp        i1, i20       
    98 : jmp_gt 8                 
    99 : unspill    i20, 2        
   100 : add        i1, i20, 3    
   101 : neg        i1, i1        
   102 : unspill    i20, 2        
   103 : add        i2, i20, 3    
   104 : mul        i1, i1, i2    
   105 : unspill    i20, 0        
   106 : cmp        i20, i1       
   107 : jmp_le 9                 
   108 : label 8:                 
   109 : mov        i1, 2         
   110 : unspill    i20, 2        
   111 : mul        i1, i20, i1   
   112 : unspill    i20, 0        
   113 : cmp        i20, i1       
   114 : jmp_gt 7                 
   115 : unspill    i20, 2        
   116 : add        i1, i20, 3    
   117 : neg        i1, i1        
   118 : unspill    i20, 2        
   119 : add        i2, i20, 3    
   120 : mul        i1, i1, i2    
   121 : unspill    i20, 0        
   122 : cmp        i20, i1       
   123 : jmp_gt 7                 
   124 : label 9:                 
   125 : unspill    i20, 2        
   126 : cmp        i20, 0        
   127 : jmp_gt 7                 
   128 : add        i0, i0, 3     
   129 : label 7:                 
   130 : unspill    i20, 1        
   131 : unspill    i21, 3        
   132 : add        i1, i20, i21  
   133 : unspill    i20, 2        
   134 : add        i2, i20, 5    
   135 : shl        i2, i2, 3     
   136 : add        i1, i1, i2    
   137 : store.i64  i1, i0        
   138 : unspill    i20, 2        
   139 : add        i20, i20, 1   
   140 : spill      2, i20        
   141 : jmp        3             
   142 : label 5:                 
   143 : unspill    i20, 0        
   144 : add        i20, i20, 1   
   145 : spill      0, i20        
   146 : jmp        0             
   147 : label 2:                 
   148 : unspill    i18, 4        
   149 : unspill    i19, 5        
   150 : unspill    i20, 6        
   151 : unspill    i21, 7        
   152 : add        i31, i31, 64  
   153 : ret                      
