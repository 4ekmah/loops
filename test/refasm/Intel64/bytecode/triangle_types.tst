triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 8    
     1 : spill     0, i12       
     2 : mov       i9, 0        
     3 : cmp       i1, i9       
     4 : jmp_gt    0            
     5 : mov       i0, 0        
     6 : jmp       29           
     7 : mov       i9, 0        
     8 : jmp       28           
     9 : label 0:               
    10 : cmp       i2, i9       
    11 : jmp_gt    1            
    12 : mov       i0, 0        
    13 : jmp       29           
    14 : mov       i9, 0        
    15 : jmp       27           
    16 : label 1:               
    17 : cmp       i8, i9       
    18 : jmp_gt    2            
    19 : mov       i0, 0        
    20 : jmp       29           
    21 : jmp       26           
    22 : label 2:               
    23 : mov       i9, i1       
    24 : add       i9, i9, i2   
    25 : mov       i0, i1       
    26 : add       i0, i0, i8   
    27 : mov       i12, i2      
    28 : add       i12, i12, i8 
    29 : cmp       i1, i12      
    30 : jmp_le    3            
    31 : mov       i0, 0        
    32 : jmp       29           
    33 : jmp       25           
    34 : label 3:               
    35 : cmp       i2, i0       
    36 : jmp_le    4            
    37 : mov       i0, 0        
    38 : jmp       29           
    39 : jmp       24           
    40 : label 4:               
    41 : cmp       i8, i9       
    42 : jmp_le    5            
    43 : mov       i0, 0        
    44 : jmp       29           
    45 : jmp       23           
    46 : label 5:               
    47 : cmp       i1, i2       
    48 : jmp_ne    8            
    49 : cmp       i2, i8       
    50 : jmp_ne    6            
    51 : mov       i0, 2        
    52 : jmp       29           
    53 : jmp       7            
    54 : label 6:               
    55 : mov       i0, 3        
    56 : jmp       29           
    57 : label 7:               
    58 : jmp       12           
    59 : label 8:               
    60 : cmp       i1, i8       
    61 : jmp_ne    9            
    62 : mov       i0, 3        
    63 : jmp       29           
    64 : jmp       11           
    65 : label 9:               
    66 : cmp       i2, i8       
    67 : jmp_ne    10           
    68 : mov       i0, 3        
    69 : jmp       29           
    70 : label 10:              
    71 : label 11:              
    72 : label 12:              
    73 : mul       i1, i1, i1   
    74 : mul       i2, i2, i2   
    75 : mul       i8, i8, i8   
    76 : mov       i9, i1       
    77 : add       i9, i9, i2   
    78 : mov       i0, i1       
    79 : add       i0, i0, i8   
    80 : mov       i12, i2      
    81 : add       i12, i12, i8 
    82 : cmp       i1, i12      
    83 : jmp_ne    13           
    84 : mov       i0, 1        
    85 : jmp       29           
    86 : jmp       17           
    87 : label 13:              
    88 : cmp       i2, i0       
    89 : jmp_ne    14           
    90 : mov       i0, 1        
    91 : jmp       29           
    92 : jmp       16           
    93 : label 14:              
    94 : cmp       i8, i9       
    95 : jmp_ne    15           
    96 : mov       i0, 1        
    97 : jmp       29           
    98 : label 15:              
    99 : label 16:              
   100 : label 17:              
   101 : sub       i12, i12, i1 
   102 : sub       i0, i0, i2   
   103 : sub       i9, i9, i8   
   104 : mov       i1, 0        
   105 : cmp       i12, i1      
   106 : jmp_ge    18           
   107 : mov       i0, 5        
   108 : jmp       29           
   109 : mov       i1, 0        
   110 : jmp       22           
   111 : label 18:              
   112 : cmp       i0, i1       
   113 : jmp_ge    19           
   114 : mov       i0, 5        
   115 : jmp       29           
   116 : mov       i1, 0        
   117 : jmp       21           
   118 : label 19:              
   119 : cmp       i9, i1       
   120 : jmp_ge    20           
   121 : mov       i0, 5        
   122 : jmp       29           
   123 : label 20:              
   124 : label 21:              
   125 : label 22:              
   126 : label 23:              
   127 : label 24:              
   128 : label 25:              
   129 : label 26:              
   130 : label 27:              
   131 : label 28:              
   132 : mov       i0, 4        
   133 : label 29:              
   134 : unspill   i12, 0       
   135 : add       i4, i4, 8    
   136 : ret                    
