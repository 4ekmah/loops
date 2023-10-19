conditionpainter(i0)
     0 : sub        i4, i4, 56   
     1 : spill      1, i1        
     2 : spill      4, i12       
     3 : spill      5, i13       
     4 : mov        i13, -5      
     5 : spill      0, i13       
     6 : label 0:                
     7 : cmp        s0, 5        
     8 : jmp_gt 2                
     9 : unspill    i13, 0       
    10 : mov        i8, i13      
    11 : add        i8, i8, 5    
    12 : mul        i8, i8, 11   
    13 : mov        i13, i8      
    14 : mul        i13, i13, 8  
    15 : spill      3, i13       
    16 : mov        i13, -5      
    17 : spill      2, i13       
    18 : label 3:                
    19 : cmp        s2, 5        
    20 : jmp_gt 5                
    21 : unspill    i13, 2       
    22 : mov        i0, i13      
    23 : add        i0, i0, 3    
    24 : mov        i12, 0       
    25 : cmp        s0, i0       
    26 : iverson_ge i12          
    27 : mov        i0, 0        
    28 : cmp        s0, 4        
    29 : iverson_le i0           
    30 : and        i12, i12, i0 
    31 : mov        i0, 0        
    32 : cmp        s2, -2       
    33 : iverson_ge i0           
    34 : and        i12, i12, i0 
    35 : mov        i0, 0        
    36 : cmp        s2, 0        
    37 : iverson_le i0           
    38 : and        i12, i12, i0 
    39 : unspill    i13, 2       
    40 : mov        i0, i13      
    41 : sub        i0, i0, 1    
    42 : mov        i2, 0        
    43 : cmp        s0, i0       
    44 : iverson_le i2           
    45 : mov        i0, 0        
    46 : cmp        s2, 0        
    47 : iverson_ge i0           
    48 : and        i2, i2, i0   
    49 : mov        i0, 0        
    50 : cmp        s0, 0        
    51 : iverson_le i0           
    52 : and        i2, i2, i0   
    53 : unspill    i13, 2       
    54 : mov        i0, i13      
    55 : mul        i0, i0, s2   
    56 : unspill    i13, 0       
    57 : mov        i1, i13      
    58 : mul        i1, i1, s0   
    59 : add        i0, i0, i1   
    60 : mov        i1, 0        
    61 : cmp        i0, 9        
    62 : iverson_le i1           
    63 : and        i2, i2, i1   
    64 : or         i12, i12, i2 
    65 : mov        i1, 2        
    66 : mov        i2, 0        
    67 : mov        i0, 0        
    68 : cmp        s2, 2        
    69 : iverson_ge i0           
    70 : mov        i9, 0        
    71 : cmp        s2, 4        
    72 : iverson_le i9           
    73 : and        i0, i0, i9   
    74 : mov        i9, 0        
    75 : cmp        s0, 1        
    76 : iverson_ge i9           
    77 : mov        i8, 0        
    78 : cmp        s0, 3        
    79 : iverson_le i8           
    80 : and        i9, i9, i8   
    81 : or         i0, i0, i9   
    82 : unspill    i13, 2       
    83 : mov        i8, i13      
    84 : mul        i8, i8, s2   
    85 : unspill    i13, 0       
    86 : mov        i9, i13      
    87 : mul        i9, i9, s0   
    88 : add        i8, i8, i9   
    89 : mov        i9, 0        
    90 : cmp        i8, 16       
    91 : iverson_le i9           
    92 : and        i0, i0, i9   
    93 : cmp        i0, 0        
    94 : select_ne  i2, i1, i2   
    95 : add        i12, i12, i2 
    96 : unspill    i13, 0       
    97 : mov        i1, i13      
    98 : mul        i1, i1, 2    
    99 : cmp        i1, s2       
   100 : jmp_gt 8                
   101 : unspill    i13, 2       
   102 : mov        i1, i13      
   103 : add        i1, i1, 3    
   104 : neg        i1, i1       
   105 : unspill    i13, 2       
   106 : mov        i2, i13      
   107 : add        i2, i2, 3    
   108 : mul        i1, i1, i2   
   109 : cmp        s0, i1       
   110 : jmp_le 9                
   111 : label 8:                
   112 : unspill    i13, 2       
   113 : mov        i1, i13      
   114 : mul        i1, i1, 2    
   115 : cmp        s0, i1       
   116 : jmp_gt 7                
   117 : unspill    i13, 2       
   118 : mov        i1, i13      
   119 : add        i1, i1, 3    
   120 : neg        i1, i1       
   121 : unspill    i13, 2       
   122 : mov        i2, i13      
   123 : add        i2, i2, 3    
   124 : mul        i1, i1, i2   
   125 : cmp        s0, i1       
   126 : jmp_gt 7                
   127 : label 9:                
   128 : cmp        s2, 0        
   129 : jmp_gt 7                
   130 : add        i12, i12, 3  
   131 : label 7:                
   132 : unspill    i13, 1       
   133 : mov        i1, i13      
   134 : add        i1, i1, s3   
   135 : unspill    i13, 2       
   136 : mov        i2, i13      
   137 : add        i2, i2, 5    
   138 : shl        i2, i2, 3    
   139 : add        i1, i1, i2   
   140 : store.i64  i1, i12      
   141 : add        s2, s2, 1    
   142 : jmp        3            
   143 : label 5:                
   144 : add        s0, s0, 1    
   145 : jmp        0            
   146 : label 2:                
   147 : unspill    i12, 4       
   148 : unspill    i13, 5       
   149 : add        i4, i4, 56   
   150 : ret                     
