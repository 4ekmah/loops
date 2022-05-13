triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 40   
     1 : spill     2, i12       
     2 : spill     3, i13       
     3 : mov       s0, 0        
     4 : cmp       i1, s0       
     5 : jmp_gt    0            
     6 : mov       i0, 0        
     7 : mov       i0, i0       
     8 : jmp       29           
     9 : jmp       28           
    10 : label 0:               
    11 : cmp       i2, s0       
    12 : jmp_gt    1            
    13 : mov       i0, 0        
    14 : mov       i0, i0       
    15 : jmp       29           
    16 : jmp       27           
    17 : label 1:               
    18 : cmp       i8, s0       
    19 : jmp_gt    2            
    20 : mov       i0, 0        
    21 : mov       i0, i0       
    22 : jmp       29           
    23 : jmp       26           
    24 : label 2:               
    25 : mov       i0, i1       
    26 : add       i0, i0, i2   
    27 : mov       i12, i1      
    28 : add       i12, i12, i8 
    29 : mov       i9, i2       
    30 : add       i9, i9, i8   
    31 : cmp       i1, i9       
    32 : jmp_le    3            
    33 : mov       i9, 0        
    34 : mov       i0, i9       
    35 : jmp       29           
    36 : jmp       25           
    37 : label 3:               
    38 : cmp       i2, i12      
    39 : jmp_le    4            
    40 : mov       i9, 0        
    41 : mov       i0, i9       
    42 : jmp       29           
    43 : jmp       24           
    44 : label 4:               
    45 : cmp       i8, i0       
    46 : jmp_le    5            
    47 : mov       i9, 0        
    48 : mov       i0, i9       
    49 : jmp       29           
    50 : jmp       23           
    51 : label 5:               
    52 : cmp       i1, i2       
    53 : jmp_ne    8            
    54 : cmp       i2, i8       
    55 : jmp_ne    6            
    56 : mov       i9, 2        
    57 : mov       i0, i9       
    58 : jmp       29           
    59 : jmp       7            
    60 : label 6:               
    61 : mov       i9, 3        
    62 : mov       i0, i9       
    63 : jmp       29           
    64 : label 7:               
    65 : jmp       12           
    66 : label 8:               
    67 : cmp       i1, i8       
    68 : jmp_ne    9            
    69 : mov       i9, 3        
    70 : mov       i0, i9       
    71 : jmp       29           
    72 : jmp       11           
    73 : label 9:               
    74 : cmp       i2, i8       
    75 : jmp_ne    10           
    76 : mov       i9, 3        
    77 : mov       i0, i9       
    78 : jmp       29           
    79 : label 10:              
    80 : label 11:              
    81 : label 12:              
    82 : mul       i1, i1, i1   
    83 : mul       i2, i2, i2   
    84 : mul       i8, i8, i8   
    85 : mov       s1, i1       
    86 : add       s1, s1, i2   
    87 : mov       i0, i1       
    88 : add       i0, i0, i8   
    89 : mov       i12, i2      
    90 : add       i12, i12, i8 
    91 : cmp       i1, i12      
    92 : jmp_ne    13           
    93 : mov       i9, 1        
    94 : mov       i0, i9       
    95 : jmp       29           
    96 : jmp       17           
    97 : label 13:              
    98 : cmp       i2, i0       
    99 : jmp_ne    14           
   100 : mov       i9, 1        
   101 : mov       i0, i9       
   102 : jmp       29           
   103 : jmp       16           
   104 : label 14:              
   105 : cmp       i8, s1       
   106 : jmp_ne    15           
   107 : mov       i9, 1        
   108 : mov       i0, i9       
   109 : jmp       29           
   110 : label 15:              
   111 : label 16:              
   112 : label 17:              
   113 : sub       i12, i12, i1 
   114 : sub       i0, i0, i2   
   115 : unspill   i13, 1       
   116 : mov       i1, i13      
   117 : sub       i1, i1, i8   
   118 : cmp       i12, s0      
   119 : jmp_ge    18           
   120 : mov       i2, 5        
   121 : mov       i0, i2       
   122 : jmp       29           
   123 : jmp       22           
   124 : label 18:              
   125 : cmp       i0, s0       
   126 : jmp_ge    19           
   127 : mov       i2, 5        
   128 : mov       i0, i2       
   129 : jmp       29           
   130 : jmp       21           
   131 : label 19:              
   132 : cmp       i1, s0       
   133 : jmp_ge    20           
   134 : mov       i1, 5        
   135 : mov       i0, i1       
   136 : jmp       29           
   137 : label 20:              
   138 : label 21:              
   139 : label 22:              
   140 : label 23:              
   141 : label 24:              
   142 : label 25:              
   143 : label 26:              
   144 : label 27:              
   145 : label 28:              
   146 : mov       i1, 4        
   147 : mov       i0, i1       
   148 : label 29:              
   149 : unspill   i12, 2       
   150 : unspill   i13, 3       
   151 : add       i4, i4, 40   
   152 : ret                    
