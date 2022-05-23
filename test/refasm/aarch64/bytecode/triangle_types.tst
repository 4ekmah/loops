triangle_types(i0, i1, i2)
     0 : sub       i31, i31, 16 
     1 : spill     0, i18       
     2 : spill     1, i19       
     3 : mov       i3, 0        
     4 : cmp       i0, i3       
     5 : jmp_gt    0            
     6 : mov       i0, 0        
     7 : jmp       29           
     8 : mov       i3, 0        
     9 : jmp       28           
    10 : label 0:               
    11 : cmp       i1, i3       
    12 : jmp_gt    1            
    13 : mov       i0, 0        
    14 : jmp       29           
    15 : mov       i3, 0        
    16 : jmp       27           
    17 : label 1:               
    18 : cmp       i2, i3       
    19 : jmp_gt    2            
    20 : mov       i0, 0        
    21 : jmp       29           
    22 : jmp       26           
    23 : label 2:               
    24 : add       i3, i0, i1   
    25 : add       i18, i0, i2  
    26 : add       i19, i1, i2  
    27 : cmp       i0, i19      
    28 : jmp_le    3            
    29 : mov       i0, 0        
    30 : jmp       29           
    31 : jmp       25           
    32 : label 3:               
    33 : cmp       i1, i18      
    34 : jmp_le    4            
    35 : mov       i0, 0        
    36 : jmp       29           
    37 : jmp       24           
    38 : label 4:               
    39 : cmp       i2, i3       
    40 : jmp_le    5            
    41 : mov       i0, 0        
    42 : jmp       29           
    43 : jmp       23           
    44 : label 5:               
    45 : cmp       i0, i1       
    46 : jmp_ne    8            
    47 : cmp       i1, i2       
    48 : jmp_ne    6            
    49 : mov       i0, 2        
    50 : jmp       29           
    51 : jmp       7            
    52 : label 6:               
    53 : mov       i0, 3        
    54 : jmp       29           
    55 : label 7:               
    56 : jmp       12           
    57 : label 8:               
    58 : cmp       i0, i2       
    59 : jmp_ne    9            
    60 : mov       i0, 3        
    61 : jmp       29           
    62 : jmp       11           
    63 : label 9:               
    64 : cmp       i1, i2       
    65 : jmp_ne    10           
    66 : mov       i0, 3        
    67 : jmp       29           
    68 : label 10:              
    69 : label 11:              
    70 : label 12:              
    71 : mul       i0, i0, i0   
    72 : mul       i1, i1, i1   
    73 : mul       i2, i2, i2   
    74 : add       i3, i0, i1   
    75 : add       i18, i0, i2  
    76 : add       i19, i1, i2  
    77 : cmp       i0, i19      
    78 : jmp_ne    13           
    79 : mov       i0, 1        
    80 : jmp       29           
    81 : jmp       17           
    82 : label 13:              
    83 : cmp       i1, i18      
    84 : jmp_ne    14           
    85 : mov       i0, 1        
    86 : jmp       29           
    87 : jmp       16           
    88 : label 14:              
    89 : cmp       i2, i3       
    90 : jmp_ne    15           
    91 : mov       i0, 1        
    92 : jmp       29           
    93 : label 15:              
    94 : label 16:              
    95 : label 17:              
    96 : sub       i0, i19, i0  
    97 : sub       i1, i18, i1  
    98 : sub       i2, i3, i2   
    99 : mov       i3, 0        
   100 : cmp       i0, i3       
   101 : jmp_ge    18           
   102 : mov       i0, 5        
   103 : jmp       29           
   104 : mov       i0, 0        
   105 : jmp       22           
   106 : label 18:              
   107 : cmp       i1, i0       
   108 : jmp_ge    19           
   109 : mov       i0, 5        
   110 : jmp       29           
   111 : mov       i0, 0        
   112 : jmp       21           
   113 : label 19:              
   114 : cmp       i2, i0       
   115 : jmp_ge    20           
   116 : mov       i0, 5        
   117 : jmp       29           
   118 : label 20:              
   119 : label 21:              
   120 : label 22:              
   121 : label 23:              
   122 : label 24:              
   123 : label 25:              
   124 : label 26:              
   125 : label 27:              
   126 : label 28:              
   127 : mov       i0, 4        
   128 : label 29:              
   129 : unspill   i18, 0       
   130 : unspill   i19, 1       
   131 : add       i31, i31, 16 
   132 : ret                    
