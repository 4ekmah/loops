conditionpainter(i0)
     0 : sub        i4, i4, 88   
     1 : spill      5, i1        
     2 : spill      8, i12       
     3 : spill      9, i13       
     4 : mov        s4, -5       
     5 : label 0:                
     6 : cmp        s4, 5        
     7 : jmp_gt 2                
     8 : unspill    i13, 4       
     9 : mov        i8, i13      
    10 : sub        i8, i8, -5   
    11 : mul        i8, i8, 11   
    12 : mov        i13, i8      
    13 : mul        i13, i13, 8  
    14 : spill      7, i13       
    15 : mov        s6, -5       
    16 : label 3:                
    17 : cmp        s6, 5        
    18 : jmp_gt 5                
    19 : unspill    i13, 6       
    20 : mov        i0, i13      
    21 : add        i0, i0, 3    
    22 : mov        i12, 0       
    23 : cmp        s4, i0       
    24 : iverson_ge i12          
    25 : mov        i0, 0        
    26 : cmp        s4, 4        
    27 : iverson_le i0           
    28 : and        i12, i12, i0 
    29 : mov        i0, 0        
    30 : cmp        s6, -2       
    31 : iverson_ge i0           
    32 : and        i12, i12, i0 
    33 : mov        i0, 0        
    34 : cmp        s6, 0        
    35 : iverson_le i0           
    36 : and        i12, i12, i0 
    37 : unspill    i13, 6       
    38 : mov        i0, i13      
    39 : sub        i0, i0, 1    
    40 : mov        i2, 0        
    41 : cmp        s4, i0       
    42 : iverson_le i2           
    43 : mov        i0, 0        
    44 : cmp        s6, 0        
    45 : iverson_ge i0           
    46 : and        i2, i2, i0   
    47 : mov        i0, 0        
    48 : cmp        s4, 0        
    49 : iverson_le i0           
    50 : and        i2, i2, i0   
    51 : unspill    i13, 6       
    52 : mov        i0, i13      
    53 : mul        i0, i0, s6   
    54 : unspill    i13, 4       
    55 : mov        i1, i13      
    56 : mul        i1, i1, s4   
    57 : add        i0, i0, i1   
    58 : mov        i1, 0        
    59 : cmp        i0, 9        
    60 : iverson_le i1           
    61 : and        i2, i2, i1   
    62 : or         i12, i12, i2 
    63 : mov        i1, 2        
    64 : mov        i2, 0        
    65 : mov        i0, 0        
    66 : cmp        s6, 2        
    67 : iverson_ge i0           
    68 : mov        i9, 0        
    69 : cmp        s6, 4        
    70 : iverson_le i9           
    71 : and        i0, i0, i9   
    72 : mov        i9, 0        
    73 : cmp        s4, 1        
    74 : iverson_ge i9           
    75 : mov        i8, 0        
    76 : cmp        s4, 3        
    77 : iverson_le i8           
    78 : and        i9, i9, i8   
    79 : or         i0, i0, i9   
    80 : unspill    i13, 6       
    81 : mov        i8, i13      
    82 : mul        i8, i8, s6   
    83 : unspill    i13, 4       
    84 : mov        i9, i13      
    85 : mul        i9, i9, s4   
    86 : add        i8, i8, i9   
    87 : mov        i9, 0        
    88 : cmp        i8, 16       
    89 : iverson_le i9           
    90 : and        i0, i0, i9   
    91 : cmp        i0, 0        
    92 : select_ne  i2, i1, i2   
    93 : add        i12, i12, i2 
    94 : unspill    i13, 4       
    95 : mov        i1, i13      
    96 : mul        i1, i1, 2    
    97 : cmp        i1, s6       
    98 : jmp_gt 8                
    99 : unspill    i13, 6       
   100 : mov        i1, i13      
   101 : add        i1, i1, 3    
   102 : neg        i1, i1       
   103 : unspill    i13, 6       
   104 : mov        i2, i13      
   105 : add        i2, i2, 3    
   106 : mul        i1, i1, i2   
   107 : cmp        s4, i1       
   108 : jmp_le 9                
   109 : label 8:                
   110 : unspill    i13, 6       
   111 : mov        i1, i13      
   112 : mul        i1, i1, 2    
   113 : cmp        s4, i1       
   114 : jmp_gt 7                
   115 : unspill    i13, 6       
   116 : mov        i1, i13      
   117 : add        i1, i1, 3    
   118 : neg        i1, i1       
   119 : unspill    i13, 6       
   120 : mov        i2, i13      
   121 : add        i2, i2, 3    
   122 : mul        i1, i1, i2   
   123 : cmp        s4, i1       
   124 : jmp_gt 7                
   125 : label 9:                
   126 : cmp        s6, 0        
   127 : jmp_gt 7                
   128 : add        i12, i12, 3  
   129 : label 7:                
   130 : unspill    i13, 5       
   131 : mov        i1, i13      
   132 : add        i1, i1, s7   
   133 : unspill    i13, 6       
   134 : mov        i2, i13      
   135 : sub        i2, i2, -5   
   136 : shl        i2, i2, 3    
   137 : add        i1, i1, i2   
   138 : store.i64  i1, i12      
   139 : add        s6, s6, 1    
   140 : jmp        3            
   141 : label 5:                
   142 : add        s4, s4, 1    
   143 : jmp        0            
   144 : label 2:                
   145 : unspill    i12, 8       
   146 : unspill    i13, 9       
   147 : add        i4, i4, 88   
   148 : ret                     
