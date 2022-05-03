triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 8    
     1 : spill     0, i3        
     2 : mov       i9, 0        
     3 : cmp       i1, i9       
     4 : jmp_gt    0            
     5 : mov       i0, 0        
     6 : mov       i0, i0       
     7 : jmp       29           
     8 : jmp       28           
     9 : label 0:               
    10 : cmp       i2, i9       
    11 : jmp_gt    1            
    12 : mov       i0, 0        
    13 : mov       i0, i0       
    14 : jmp       29           
    15 : jmp       27           
    16 : label 1:               
    17 : cmp       i8, i9       
    18 : jmp_gt    2            
    19 : mov       i0, 0        
    20 : mov       i0, i0       
    21 : jmp       29           
    22 : jmp       26           
    23 : label 2:               
    24 : mov       i0, i1       
    25 : add       i0, i0, i2   
    26 : mov       i10, i1      
    27 : add       i10, i10, i8 
    28 : mov       i11, i2      
    29 : add       i11, i11, i8 
    30 : cmp       i1, i11      
    31 : jmp_le    3            
    32 : mov       i11, 0       
    33 : mov       i0, i11      
    34 : jmp       29           
    35 : jmp       25           
    36 : label 3:               
    37 : cmp       i2, i10      
    38 : jmp_le    4            
    39 : mov       i10, 0       
    40 : mov       i0, i10      
    41 : jmp       29           
    42 : jmp       24           
    43 : label 4:               
    44 : cmp       i8, i0       
    45 : jmp_le    5            
    46 : mov       i0, 0        
    47 : mov       i0, i0       
    48 : jmp       29           
    49 : jmp       23           
    50 : label 5:               
    51 : cmp       i1, i2       
    52 : jmp_ne    8            
    53 : cmp       i2, i8       
    54 : jmp_ne    6            
    55 : mov       i0, 2        
    56 : mov       i0, i0       
    57 : jmp       29           
    58 : jmp       7            
    59 : label 6:               
    60 : mov       i0, 3        
    61 : mov       i0, i0       
    62 : jmp       29           
    63 : label 7:               
    64 : jmp       12           
    65 : label 8:               
    66 : cmp       i1, i8       
    67 : jmp_ne    9            
    68 : mov       i0, 3        
    69 : mov       i0, i0       
    70 : jmp       29           
    71 : jmp       11           
    72 : label 9:               
    73 : cmp       i2, i8       
    74 : jmp_ne    10           
    75 : mov       i0, 3        
    76 : mov       i0, i0       
    77 : jmp       29           
    78 : label 10:              
    79 : label 11:              
    80 : label 12:              
    81 : mul       i1, i1, i1   
    82 : mul       i2, i2, i2   
    83 : mul       i8, i8, i8   
    84 : mov       i0, i1       
    85 : add       i0, i0, i2   
    86 : mov       i10, i1      
    87 : add       i10, i10, i8 
    88 : mov       i11, i2      
    89 : add       i11, i11, i8 
    90 : cmp       i1, i11      
    91 : jmp_ne    13           
    92 : mov       i3, 1        
    93 : mov       i0, i3       
    94 : jmp       29           
    95 : jmp       17           
    96 : label 13:              
    97 : cmp       i2, i10      
    98 : jmp_ne    14           
    99 : mov       i3, 1        
   100 : mov       i0, i3       
   101 : jmp       29           
   102 : jmp       16           
   103 : label 14:              
   104 : cmp       i8, i0       
   105 : jmp_ne    15           
   106 : mov       i3, 1        
   107 : mov       i0, i3       
   108 : jmp       29           
   109 : label 15:              
   110 : label 16:              
   111 : label 17:              
   112 : sub       i11, i11, i1 
   113 : sub       i10, i10, i2 
   114 : sub       i0, i0, i8   
   115 : cmp       i11, i9      
   116 : jmp_ge    18           
   117 : mov       i8, 5        
   118 : mov       i0, i8       
   119 : jmp       29           
   120 : jmp       22           
   121 : label 18:              
   122 : cmp       i10, i9      
   123 : jmp_ge    19           
   124 : mov       i8, 5        
   125 : mov       i0, i8       
   126 : jmp       29           
   127 : jmp       21           
   128 : label 19:              
   129 : cmp       i0, i9       
   130 : jmp_ge    20           
   131 : mov       i9, 5        
   132 : mov       i0, i9       
   133 : jmp       29           
   134 : label 20:              
   135 : label 21:              
   136 : label 22:              
   137 : label 23:              
   138 : label 24:              
   139 : label 25:              
   140 : label 26:              
   141 : label 27:              
   142 : label 28:              
   143 : mov       i9, 4        
   144 : mov       i0, i9       
   145 : label 29:              
   146 : unspill   i3, 0        
   147 : add       i4, i4, 8    
   148 : ret                    
