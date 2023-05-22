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
    12 : mov        i2, -5        
    13 : unspill    i20, 0        
    14 : sub        i2, i20, i2   
    15 : mov        i3, 11        
    16 : mul        i2, i2, i3    
    17 : mov        i3, 8         
    18 : mul        i20, i2, i3   
    19 : spill      3, i20        
    20 : mov        i20, -5       
    21 : spill      2, i20        
    22 : label 3:                 
    23 : unspill    i20, 2        
    24 : cmp        i20, 5        
    25 : jmp_gt 5                 
    26 : unspill    i20, 2        
    27 : add        i18, i20, 3   
    28 : unspill    i20, 0        
    29 : cmp        i20, i18      
    30 : iverson_ge i18           
    31 : unspill    i20, 0        
    32 : cmp        i20, 4        
    33 : iverson_le i19           
    34 : and        i18, i18, i19 
    35 : mov        i19, -2       
    36 : unspill    i20, 2        
    37 : cmp        i20, i19      
    38 : iverson_ge i19           
    39 : and        i18, i18, i19 
    40 : unspill    i20, 2        
    41 : cmp        i20, 0        
    42 : iverson_le i19           
    43 : and        i18, i18, i19 
    44 : unspill    i20, 2        
    45 : sub        i19, i20, 1   
    46 : unspill    i20, 0        
    47 : cmp        i20, i19      
    48 : iverson_le i19           
    49 : unspill    i20, 2        
    50 : cmp        i20, 0        
    51 : iverson_ge i1            
    52 : and        i1, i19, i1   
    53 : unspill    i20, 0        
    54 : cmp        i20, 0        
    55 : iverson_le i19           
    56 : and        i1, i1, i19   
    57 : unspill    i20, 2        
    58 : mul        i19, i20, i20 
    59 : unspill    i20, 0        
    60 : mul        i0, i20, i20  
    61 : add        i0, i19, i0   
    62 : cmp        i0, 9         
    63 : iverson_le i0            
    64 : and        i0, i1, i0    
    65 : or         i0, i18, i0   
    66 : mov        i1, 2         
    67 : mov        i18, 0        
    68 : unspill    i20, 2        
    69 : cmp        i20, 2        
    70 : iverson_ge i19           
    71 : unspill    i20, 2        
    72 : cmp        i20, 4        
    73 : iverson_le i3            
    74 : and        i3, i19, i3   
    75 : unspill    i20, 0        
    76 : cmp        i20, 1        
    77 : iverson_ge i19           
    78 : unspill    i20, 0        
    79 : cmp        i20, 3        
    80 : iverson_le i2            
    81 : and        i2, i19, i2   
    82 : or         i2, i3, i2    
    83 : unspill    i20, 2        
    84 : mul        i3, i20, i20  
    85 : unspill    i20, 0        
    86 : mul        i19, i20, i20 
    87 : add        i3, i3, i19   
    88 : cmp        i3, 16        
    89 : iverson_le i3            
    90 : and        i2, i2, i3    
    91 : cmp        i2, 0         
    92 : select_ne  i1, i1, i18   
    93 : add        i0, i0, i1    
    94 : mov        i1, 2         
    95 : unspill    i20, 0        
    96 : mul        i1, i20, i1   
    97 : unspill    i20, 2        
    98 : cmp        i1, i20       
    99 : jmp_gt 8                 
   100 : unspill    i20, 2        
   101 : add        i1, i20, 3    
   102 : neg        i1, i1        
   103 : unspill    i20, 2        
   104 : add        i2, i20, 3    
   105 : mul        i1, i1, i2    
   106 : unspill    i20, 0        
   107 : cmp        i20, i1       
   108 : jmp_le 9                 
   109 : label 8:                 
   110 : mov        i1, 2         
   111 : unspill    i20, 2        
   112 : mul        i1, i20, i1   
   113 : unspill    i20, 0        
   114 : cmp        i20, i1       
   115 : jmp_gt 7                 
   116 : unspill    i20, 2        
   117 : add        i1, i20, 3    
   118 : neg        i1, i1        
   119 : unspill    i20, 2        
   120 : add        i2, i20, 3    
   121 : mul        i1, i1, i2    
   122 : unspill    i20, 0        
   123 : cmp        i20, i1       
   124 : jmp_gt 7                 
   125 : label 9:                 
   126 : unspill    i20, 2        
   127 : cmp        i20, 0        
   128 : jmp_gt 7                 
   129 : add        i0, i0, 3     
   130 : label 7:                 
   131 : unspill    i20, 1        
   132 : unspill    i21, 3        
   133 : add        i1, i20, i21  
   134 : mov        i2, -5        
   135 : unspill    i20, 2        
   136 : sub        i2, i20, i2   
   137 : shl        i2, i2, 3     
   138 : add        i1, i1, i2    
   139 : store.i64  i1, i0        
   140 : unspill    i20, 2        
   141 : add        i20, i20, 1   
   142 : spill      2, i20        
   143 : jmp        3             
   144 : label 5:                 
   145 : unspill    i20, 0        
   146 : add        i20, i20, 1   
   147 : spill      0, i20        
   148 : jmp        0             
   149 : label 2:                 
   150 : unspill    i18, 4        
   151 : unspill    i19, 5        
   152 : unspill    i20, 6        
   153 : unspill    i21, 7        
   154 : add        i31, i31, 64  
   155 : ret                      
