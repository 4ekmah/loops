conditionpainter(i0)
     0 : sub        i4, i4, 88   
     1 : spill      5, i7        
     2 : spill      8, i12       
     3 : spill      9, i13       
     4 : mov        i13, -5      
     5 : spill      4, i13       
     6 : label 0:                
     7 : cmp        s4, 5        
     8 : jmp_gt 2                
     9 : unspill    i13, 4       
    10 : mov        i2, i13      
    11 : add        i2, i2, 5    
    12 : mul        i2, i2, 11   
    13 : mov        i13, i2      
    14 : mul        i13, i13, 8  
    15 : spill      7, i13       
    16 : mov        i13, -5      
    17 : spill      6, i13       
    18 : label 3:                
    19 : cmp        s6, 5        
    20 : jmp_gt 5                
    21 : unspill    i13, 6       
    22 : mov        i0, i13      
    23 : add        i0, i0, 3    
    24 : mov        i12, 0       
    25 : cmp        s4, i0       
    26 : iverson_ge i12          
    27 : mov        i0, 0        
    28 : cmp        s4, 4        
    29 : iverson_le i0           
    30 : and        i12, i12, i0 
    31 : mov        i0, 0        
    32 : cmp        s6, -2       
    33 : iverson_ge i0           
    34 : and        i12, i12, i0 
    35 : mov        i0, 0        
    36 : cmp        s6, 0        
    37 : iverson_le i0           
    38 : and        i12, i12, i0 
    39 : unspill    i13, 6       
    40 : mov        i0, i13      
    41 : sub        i0, i0, 1    
    42 : mov        i6, 0        
    43 : cmp        s4, i0       
    44 : iverson_le i6           
    45 : mov        i0, 0        
    46 : cmp        s6, 0        
    47 : iverson_ge i0           
    48 : and        i6, i6, i0   
    49 : mov        i0, 0        
    50 : cmp        s4, 0        
    51 : iverson_le i0           
    52 : and        i6, i6, i0   
    53 : unspill    i13, 6       
    54 : mov        i0, i13      
    55 : mul        i0, i0, s6   
    56 : unspill    i13, 4       
    57 : mov        i7, i13      
    58 : mul        i7, i7, s4   
    59 : add        i0, i0, i7   
    60 : mov        i7, 0        
    61 : cmp        i0, 9        
    62 : iverson_le i7           
    63 : and        i6, i6, i7   
    64 : or         i12, i12, i6 
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
   135 : unspill    i13, 6       
   136 : mov        i6, i13      
   137 : add        i6, i6, 5    
   138 : shl        i6, i6, 3    
   139 : add        i7, i7, i6   
   140 : store.i64  i7, i12      
   141 : add        s6, s6, 1    
   142 : jmp        3            
   143 : label 5:                
   144 : add        s4, s4, 1    
   145 : jmp        0            
   146 : label 2:                
   147 : unspill    i12, 8       
   148 : unspill    i13, 9       
   149 : add        i4, i4, 88   
   150 : ret                     
