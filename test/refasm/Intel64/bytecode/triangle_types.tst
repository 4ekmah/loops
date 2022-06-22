triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 8    
     1 : spill     0, i12       
     2 : cmp       i1, 0        
     3 : jmp_gt    0            
     4 : mov       i0, 0        
     5 : jmp       29           
     6 : jmp       28           
     7 : label 0:               
     8 : cmp       i2, 0        
     9 : jmp_gt    1            
    10 : mov       i0, 0        
    11 : jmp       29           
    12 : jmp       27           
    13 : label 1:               
    14 : cmp       i8, 0        
    15 : jmp_gt    2            
    16 : mov       i0, 0        
    17 : jmp       29           
    18 : jmp       26           
    19 : label 2:               
    20 : mov       i9, i1       
    21 : add       i9, i9, i2   
    22 : mov       i0, i1       
    23 : add       i0, i0, i8   
    24 : mov       i12, i2      
    25 : add       i12, i12, i8 
    26 : cmp       i1, i12      
    27 : jmp_le    3            
    28 : mov       i0, 0        
    29 : jmp       29           
    30 : jmp       25           
    31 : label 3:               
    32 : cmp       i2, i0       
    33 : jmp_le    4            
    34 : mov       i0, 0        
    35 : jmp       29           
    36 : jmp       24           
    37 : label 4:               
    38 : cmp       i8, i9       
    39 : jmp_le    5            
    40 : mov       i0, 0        
    41 : jmp       29           
    42 : jmp       23           
    43 : label 5:               
    44 : cmp       i1, i2       
    45 : jmp_ne    8            
    46 : cmp       i2, i8       
    47 : jmp_ne    6            
    48 : mov       i0, 2        
    49 : jmp       29           
    50 : jmp       7            
    51 : label 6:               
    52 : mov       i0, 3        
    53 : jmp       29           
    54 : label 7:               
    55 : jmp       12           
    56 : label 8:               
    57 : cmp       i1, i8       
    58 : jmp_ne    9            
    59 : mov       i0, 3        
    60 : jmp       29           
    61 : jmp       11           
    62 : label 9:               
    63 : cmp       i2, i8       
    64 : jmp_ne    10           
    65 : mov       i0, 3        
    66 : jmp       29           
    67 : label 10:              
    68 : label 11:              
    69 : label 12:              
    70 : mul       i1, i1, i1   
    71 : mul       i2, i2, i2   
    72 : mul       i8, i8, i8   
    73 : mov       i9, i1       
    74 : add       i9, i9, i2   
    75 : mov       i0, i1       
    76 : add       i0, i0, i8   
    77 : mov       i12, i2      
    78 : add       i12, i12, i8 
    79 : cmp       i1, i12      
    80 : jmp_ne    13           
    81 : mov       i0, 1        
    82 : jmp       29           
    83 : jmp       17           
    84 : label 13:              
    85 : cmp       i2, i0       
    86 : jmp_ne    14           
    87 : mov       i0, 1        
    88 : jmp       29           
    89 : jmp       16           
    90 : label 14:              
    91 : cmp       i8, i9       
    92 : jmp_ne    15           
    93 : mov       i0, 1        
    94 : jmp       29           
    95 : label 15:              
    96 : label 16:              
    97 : label 17:              
    98 : sub       i12, i12, i1 
    99 : sub       i0, i0, i2   
   100 : sub       i9, i9, i8   
   101 : cmp       i12, 0       
   102 : jmp_ge    18           
   103 : mov       i0, 5        
   104 : jmp       29           
   105 : jmp       22           
   106 : label 18:              
   107 : cmp       i0, 0        
   108 : jmp_ge    19           
   109 : mov       i0, 5        
   110 : jmp       29           
   111 : jmp       21           
   112 : label 19:              
   113 : cmp       i9, 0        
   114 : jmp_ge    20           
   115 : mov       i0, 5        
   116 : jmp       29           
   117 : label 20:              
   118 : label 21:              
   119 : label 22:              
   120 : label 23:              
   121 : label 24:              
   122 : label 25:              
   123 : label 26:              
   124 : label 27:              
   125 : label 28:              
   126 : mov       i0, 4        
   127 : label 29:              
   128 : unspill   i12, 0       
   129 : add       i4, i4, 8    
   130 : ret                    
