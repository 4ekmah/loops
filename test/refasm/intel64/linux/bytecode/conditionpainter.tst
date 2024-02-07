conditionpainter(i0)
     0 : sub        i4, i4, 56  
     1 : spill      1, i7       
     2 : spill      5, i13      
     3 : mov        i13, -5     
     4 : spill      0, i13      
     5 : label 0:               
     6 : cmp        s0, 5       
     7 : jmp_gt 2               
     8 : unspill    i13, 0      
     9 : mov        i2, i13     
    10 : add        i2, i2, 5   
    11 : mul        i2, i2, 11  
    12 : mov        i13, i2     
    13 : mul        i13, i13, 8 
    14 : spill      3, i13      
    15 : mov        i13, -5     
    16 : spill      2, i13      
    17 : label 3:               
    18 : cmp        s2, 5       
    19 : jmp_gt 5               
    20 : unspill    i13, 2      
    21 : mov        i0, i13     
    22 : add        i0, i0, 3   
    23 : mov        i6, 0       
    24 : cmp        s0, i0      
    25 : iverson_ge i6          
    26 : mov        i0, 0       
    27 : cmp        s0, 4       
    28 : iverson_le i0          
    29 : and        i6, i6, i0  
    30 : mov        i0, 0       
    31 : cmp        s2, -2      
    32 : iverson_ge i0          
    33 : and        i6, i6, i0  
    34 : mov        i0, 0       
    35 : cmp        s2, 0       
    36 : iverson_le i0          
    37 : and        i6, i6, i0  
    38 : unspill    i13, 2      
    39 : mov        i0, i13     
    40 : sub        i0, i0, 1   
    41 : mov        i7, 0       
    42 : cmp        s0, i0      
    43 : iverson_le i7          
    44 : mov        i0, 0       
    45 : cmp        s2, 0       
    46 : iverson_ge i0          
    47 : and        i7, i7, i0  
    48 : mov        i0, 0       
    49 : cmp        s0, 0       
    50 : iverson_le i0          
    51 : and        i7, i7, i0  
    52 : unspill    i13, 2      
    53 : mov        i0, i13     
    54 : mul        i0, i0, s2  
    55 : unspill    i13, 0      
    56 : mov        i1, i13     
    57 : mul        i1, i1, s0  
    58 : add        i0, i0, i1  
    59 : mov        i1, 0       
    60 : cmp        i0, 9       
    61 : iverson_le i1          
    62 : and        i7, i7, i1  
    63 : mov        s4, i6      
    64 : or         s4, s4, i7  
    65 : mov        i7, 2       
    66 : mov        i1, 0       
    67 : mov        i0, 0       
    68 : cmp        s2, 2       
    69 : iverson_ge i0          
    70 : mov        i2, 0       
    71 : cmp        s2, 4       
    72 : iverson_le i2          
    73 : and        i0, i0, i2  
    74 : mov        i2, 0       
    75 : cmp        s0, 1       
    76 : iverson_ge i2          
    77 : mov        i6, 0       
    78 : cmp        s0, 3       
    79 : iverson_le i6          
    80 : and        i2, i2, i6  
    81 : or         i0, i0, i2  
    82 : unspill    i13, 2      
    83 : mov        i6, i13     
    84 : mul        i6, i6, s2  
    85 : unspill    i13, 0      
    86 : mov        i2, i13     
    87 : mul        i2, i2, s0  
    88 : add        i6, i6, i2  
    89 : mov        i2, 0       
    90 : cmp        i6, 16      
    91 : iverson_le i2          
    92 : and        i0, i0, i2  
    93 : cmp        i0, 0       
    94 : select_ne  i1, i7, i1  
    95 : unspill    i13, 4      
    96 : add        i1, i1, i13 
    97 : unspill    i13, 0      
    98 : mov        i7, i13     
    99 : mul        i7, i7, 2   
   100 : cmp        i7, s2      
   101 : jmp_gt 8               
   102 : unspill    i13, 2      
   103 : mov        i7, i13     
   104 : add        i7, i7, 3   
   105 : neg        i7, i7      
   106 : unspill    i13, 2      
   107 : mov        i6, i13     
   108 : add        i6, i6, 3   
   109 : mul        i7, i7, i6  
   110 : cmp        s0, i7      
   111 : jmp_le 9               
   112 : label 8:               
   113 : unspill    i13, 2      
   114 : mov        i7, i13     
   115 : mul        i7, i7, 2   
   116 : cmp        s0, i7      
   117 : jmp_gt 7               
   118 : unspill    i13, 2      
   119 : mov        i7, i13     
   120 : add        i7, i7, 3   
   121 : neg        i7, i7      
   122 : unspill    i13, 2      
   123 : mov        i6, i13     
   124 : add        i6, i6, 3   
   125 : mul        i7, i7, i6  
   126 : cmp        s0, i7      
   127 : jmp_gt 7               
   128 : label 9:               
   129 : cmp        s2, 0       
   130 : jmp_gt 7               
   131 : add        i1, i1, 3   
   132 : label 7:               
   133 : unspill    i13, 1      
   134 : mov        i7, i13     
   135 : add        i7, i7, s3  
   136 : unspill    i13, 2      
   137 : mov        i6, i13     
   138 : add        i6, i6, 5   
   139 : shl        i6, i6, 3   
   140 : add        i7, i7, i6  
   141 : store.i64  i7, i1      
   142 : add        s2, s2, 1   
   143 : jmp        3           
   144 : label 5:               
   145 : add        s0, s0, 1   
   146 : jmp        0           
   147 : label 2:               
   148 : unspill    i13, 5      
   149 : add        i4, i4, 56  
   150 : ret                    
