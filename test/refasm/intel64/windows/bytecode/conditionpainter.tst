conditionpainter(i0)
     0 : sub        i4, i4, 56  
     1 : spill      1, i1       
     2 : spill      5, i13      
     3 : mov        i13, -5     
     4 : spill      0, i13      
     5 : label 0:               
     6 : cmp        s0, 5       
     7 : jmp_gt 2               
     8 : unspill    i13, 0      
     9 : mov        i8, i13     
    10 : add        i8, i8, 5   
    11 : mul        i8, i8, 11  
    12 : mov        i13, i8     
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
    23 : mov        i2, 0       
    24 : cmp        s0, i0      
    25 : iverson_ge i2          
    26 : mov        i0, 0       
    27 : cmp        s0, 4       
    28 : iverson_le i0          
    29 : and        i2, i2, i0  
    30 : mov        i0, 0       
    31 : cmp        s2, -2      
    32 : iverson_ge i0          
    33 : and        i2, i2, i0  
    34 : mov        i0, 0       
    35 : cmp        s2, 0       
    36 : iverson_le i0          
    37 : and        i2, i2, i0  
    38 : unspill    i13, 2      
    39 : mov        i0, i13     
    40 : sub        i0, i0, 1   
    41 : mov        i1, 0       
    42 : cmp        s0, i0      
    43 : iverson_le i1          
    44 : mov        i0, 0       
    45 : cmp        s2, 0       
    46 : iverson_ge i0          
    47 : and        i1, i1, i0  
    48 : mov        i0, 0       
    49 : cmp        s0, 0       
    50 : iverson_le i0          
    51 : and        i1, i1, i0  
    52 : unspill    i13, 2      
    53 : mov        i0, i13     
    54 : mul        i0, i0, s2  
    55 : unspill    i13, 0      
    56 : mov        i9, i13     
    57 : mul        i9, i9, s0  
    58 : add        i0, i0, i9  
    59 : mov        i9, 0       
    60 : cmp        i0, 9       
    61 : iverson_le i9          
    62 : and        i1, i1, i9  
    63 : mov        s4, i2      
    64 : or         s4, s4, i1  
    65 : mov        i1, 2       
    66 : mov        i9, 0       
    67 : mov        i0, 0       
    68 : cmp        s2, 2       
    69 : iverson_ge i0          
    70 : mov        i8, 0       
    71 : cmp        s2, 4       
    72 : iverson_le i8          
    73 : and        i0, i0, i8  
    74 : mov        i8, 0       
    75 : cmp        s0, 1       
    76 : iverson_ge i8          
    77 : mov        i2, 0       
    78 : cmp        s0, 3       
    79 : iverson_le i2          
    80 : and        i8, i8, i2  
    81 : or         i0, i0, i8  
    82 : unspill    i13, 2      
    83 : mov        i2, i13     
    84 : mul        i2, i2, s2  
    85 : unspill    i13, 0      
    86 : mov        i8, i13     
    87 : mul        i8, i8, s0  
    88 : add        i2, i2, i8  
    89 : mov        i8, 0       
    90 : cmp        i2, 16      
    91 : iverson_le i8          
    92 : and        i0, i0, i8  
    93 : cmp        i0, 0       
    94 : select_ne  i9, i1, i9  
    95 : unspill    i13, 4      
    96 : add        i9, i9, i13 
    97 : unspill    i13, 0      
    98 : mov        i1, i13     
    99 : mul        i1, i1, 2   
   100 : cmp        i1, s2      
   101 : jmp_gt 8               
   102 : unspill    i13, 2      
   103 : mov        i1, i13     
   104 : add        i1, i1, 3   
   105 : neg        i1, i1      
   106 : unspill    i13, 2      
   107 : mov        i2, i13     
   108 : add        i2, i2, 3   
   109 : mul        i1, i1, i2  
   110 : cmp        s0, i1      
   111 : jmp_le 9               
   112 : label 8:               
   113 : unspill    i13, 2      
   114 : mov        i1, i13     
   115 : mul        i1, i1, 2   
   116 : cmp        s0, i1      
   117 : jmp_gt 7               
   118 : unspill    i13, 2      
   119 : mov        i1, i13     
   120 : add        i1, i1, 3   
   121 : neg        i1, i1      
   122 : unspill    i13, 2      
   123 : mov        i2, i13     
   124 : add        i2, i2, 3   
   125 : mul        i1, i1, i2  
   126 : cmp        s0, i1      
   127 : jmp_gt 7               
   128 : label 9:               
   129 : cmp        s2, 0       
   130 : jmp_gt 7               
   131 : add        i9, i9, 3   
   132 : label 7:               
   133 : unspill    i13, 1      
   134 : mov        i1, i13     
   135 : add        i1, i1, s3  
   136 : unspill    i13, 2      
   137 : mov        i2, i13     
   138 : add        i2, i2, 5   
   139 : shl        i2, i2, 3   
   140 : add        i1, i1, i2  
   141 : store.i64  i1, i9      
   142 : add        s2, s2, 1   
   143 : jmp        3           
   144 : label 5:               
   145 : add        s0, s0, 1   
   146 : jmp        0           
   147 : label 2:               
   148 : unspill    i13, 5      
   149 : add        i4, i4, 56  
   150 : ret                    
