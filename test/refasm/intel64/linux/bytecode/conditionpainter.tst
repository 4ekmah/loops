conditionpainter(i0)
     0 : sub        i4, i4, 88   
     1 : spill      5, i7        
     2 : spill      8, i12       
     3 : spill      9, i13       
     4 : mov        s4, -5       
     5 : label 0:                
     6 : cmp        s4, 5        
     7 : jmp_gt 2                
     8 : mov        i2, -5       
     9 : unspill    i13, 4       
    10 : sub        i2, i2, i13  
    11 : neg        i2, i2       
    12 : mul        i2, i2, 11   
    13 : mov        i13, i2      
    14 : mul        i13, i13, 8  
    15 : spill      7, i13       
    16 : mov        s6, -5       
    17 : label 3:                
    18 : cmp        s6, 5        
    19 : jmp_gt 5                
    20 : unspill    i13, 6       
    21 : mov        i0, i13      
    22 : add        i0, i0, 3    
    23 : mov        i12, 0       
    24 : cmp        s4, i0       
    25 : iverson_ge i12          
    26 : mov        i0, 0        
    27 : cmp        s4, 4        
    28 : iverson_le i0           
    29 : and        i12, i12, i0 
    30 : mov        i0, -2       
    31 : mov        i6, 0        
    32 : cmp        s6, i0       
    33 : iverson_ge i6           
    34 : and        i12, i12, i6 
    35 : mov        i6, 0        
    36 : cmp        s6, 0        
    37 : iverson_le i6           
    38 : and        i12, i12, i6 
    39 : unspill    i13, 6       
    40 : mov        i6, i13      
    41 : sub        i6, i6, 1    
    42 : mov        i0, 0        
    43 : cmp        s4, i6       
    44 : iverson_le i0           
    45 : mov        i6, 0        
    46 : cmp        s6, 0        
    47 : iverson_ge i6           
    48 : and        i0, i0, i6   
    49 : mov        i6, 0        
    50 : cmp        s4, 0        
    51 : iverson_le i6           
    52 : and        i0, i0, i6   
    53 : unspill    i13, 6       
    54 : mov        i6, i13      
    55 : mul        i6, i6, s6   
    56 : unspill    i13, 4       
    57 : mov        i7, i13      
    58 : mul        i7, i7, s4   
    59 : add        i6, i6, i7   
    60 : mov        i7, 0        
    61 : cmp        i6, 9        
    62 : iverson_le i7           
    63 : and        i0, i0, i7   
    64 : or         i12, i12, i0 
    65 : mov        i7, 2        
    66 : mov        i6, 0        
    67 : mov        i0, 0        
    68 : cmp        s6, 2        
    69 : iverson_ge i0           
    70 : mov        i1, 0        
    71 : cmp        s6, 4        
    72 : iverson_le i1           
    73 : and        i0, i0, i1   
    74 : mov        i1, 0        
    75 : cmp        s4, 1        
    76 : iverson_ge i1           
    77 : mov        i2, 0        
    78 : cmp        s4, 3        
    79 : iverson_le i2           
    80 : and        i1, i1, i2   
    81 : or         i0, i0, i1   
    82 : unspill    i13, 6       
    83 : mov        i2, i13      
    84 : mul        i2, i2, s6   
    85 : unspill    i13, 4       
    86 : mov        i1, i13      
    87 : mul        i1, i1, s4   
    88 : add        i2, i2, i1   
    89 : mov        i1, 0        
    90 : cmp        i2, 16       
    91 : iverson_le i1           
    92 : and        i0, i0, i1   
    93 : cmp        i0, 0        
    94 : select_ne  i6, i7, i6   
    95 : add        i12, i12, i6 
    96 : unspill    i13, 4       
    97 : mov        i7, i13      
    98 : mul        i7, i7, 2    
    99 : cmp        i7, s6       
   100 : jmp_gt 8                
   101 : unspill    i13, 6       
   102 : mov        i7, i13      
   103 : add        i7, i7, 3    
   104 : neg        i7, i7       
   105 : unspill    i13, 6       
   106 : mov        i6, i13      
   107 : add        i6, i6, 3    
   108 : mul        i7, i7, i6   
   109 : cmp        s4, i7       
   110 : jmp_le 9                
   111 : label 8:                
   112 : unspill    i13, 6       
   113 : mov        i7, i13      
   114 : mul        i7, i7, 2    
   115 : cmp        s4, i7       
   116 : jmp_gt 7                
   117 : unspill    i13, 6       
   118 : mov        i7, i13      
   119 : add        i7, i7, 3    
   120 : neg        i7, i7       
   121 : unspill    i13, 6       
   122 : mov        i6, i13      
   123 : add        i6, i6, 3    
   124 : mul        i7, i7, i6   
   125 : cmp        s4, i7       
   126 : jmp_gt 7                
   127 : label 9:                
   128 : cmp        s6, 0        
   129 : jmp_gt 7                
   130 : add        i12, i12, 3  
   131 : label 7:                
   132 : unspill    i13, 5       
   133 : mov        i7, i13      
   134 : add        i7, i7, s7   
   135 : mov        i6, -5       
   136 : unspill    i13, 6       
   137 : sub        i6, i6, i13  
   138 : neg        i6, i6       
   139 : shl        i6, i6, 3    
   140 : add        i7, i7, i6   
   141 : store.i64  i7, i12      
   142 : add        s6, s6, 1    
   143 : jmp        3            
   144 : label 5:                
   145 : add        s4, s4, 1    
   146 : jmp        0            
   147 : label 2:                
   148 : unspill    i12, 8       
   149 : unspill    i13, 9       
   150 : add        i4, i4, 88   
   151 : ret                     
