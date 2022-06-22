triangle_types(i0, i1, i2)
     0 : sub       i31, i31, 16 
     1 : spill     0, i18       
     2 : spill     1, i19       
     3 : cmp       i0, 0        
     4 : jmp_gt    0            
     5 : mov       i0, 0        
     6 : jmp       29           
     7 : jmp       28           
     8 : label 0:               
     9 : cmp       i1, 0        
    10 : jmp_gt    1            
    11 : mov       i0, 0        
    12 : jmp       29           
    13 : jmp       27           
    14 : label 1:               
    15 : cmp       i2, 0        
    16 : jmp_gt    2            
    17 : mov       i0, 0        
    18 : jmp       29           
    19 : jmp       26           
    20 : label 2:               
    21 : add       i3, i0, i1   
    22 : add       i18, i0, i2  
    23 : add       i19, i1, i2  
    24 : cmp       i0, i19      
    25 : jmp_le    3            
    26 : mov       i0, 0        
    27 : jmp       29           
    28 : jmp       25           
    29 : label 3:               
    30 : cmp       i1, i18      
    31 : jmp_le    4            
    32 : mov       i0, 0        
    33 : jmp       29           
    34 : jmp       24           
    35 : label 4:               
    36 : cmp       i2, i3       
    37 : jmp_le    5            
    38 : mov       i0, 0        
    39 : jmp       29           
    40 : jmp       23           
    41 : label 5:               
    42 : cmp       i0, i1       
    43 : jmp_ne    8            
    44 : cmp       i1, i2       
    45 : jmp_ne    6            
    46 : mov       i0, 2        
    47 : jmp       29           
    48 : jmp       7            
    49 : label 6:               
    50 : mov       i0, 3        
    51 : jmp       29           
    52 : label 7:               
    53 : jmp       12           
    54 : label 8:               
    55 : cmp       i0, i2       
    56 : jmp_ne    9            
    57 : mov       i0, 3        
    58 : jmp       29           
    59 : jmp       11           
    60 : label 9:               
    61 : cmp       i1, i2       
    62 : jmp_ne    10           
    63 : mov       i0, 3        
    64 : jmp       29           
    65 : label 10:              
    66 : label 11:              
    67 : label 12:              
    68 : mul       i0, i0, i0   
    69 : mul       i1, i1, i1   
    70 : mul       i2, i2, i2   
    71 : add       i3, i0, i1   
    72 : add       i18, i0, i2  
    73 : add       i19, i1, i2  
    74 : cmp       i0, i19      
    75 : jmp_ne    13           
    76 : mov       i0, 1        
    77 : jmp       29           
    78 : jmp       17           
    79 : label 13:              
    80 : cmp       i1, i18      
    81 : jmp_ne    14           
    82 : mov       i0, 1        
    83 : jmp       29           
    84 : jmp       16           
    85 : label 14:              
    86 : cmp       i2, i3       
    87 : jmp_ne    15           
    88 : mov       i0, 1        
    89 : jmp       29           
    90 : label 15:              
    91 : label 16:              
    92 : label 17:              
    93 : sub       i0, i19, i0  
    94 : sub       i1, i18, i1  
    95 : sub       i2, i3, i2   
    96 : cmp       i0, 0        
    97 : jmp_ge    18           
    98 : mov       i0, 5        
    99 : jmp       29           
   100 : jmp       22           
   101 : label 18:              
   102 : cmp       i1, 0        
   103 : jmp_ge    19           
   104 : mov       i0, 5        
   105 : jmp       29           
   106 : jmp       21           
   107 : label 19:              
   108 : cmp       i2, 0        
   109 : jmp_ge    20           
   110 : mov       i0, 5        
   111 : jmp       29           
   112 : label 20:              
   113 : label 21:              
   114 : label 22:              
   115 : label 23:              
   116 : label 24:              
   117 : label 25:              
   118 : label 26:              
   119 : label 27:              
   120 : label 28:              
   121 : mov       i0, 4        
   122 : label 29:              
   123 : unspill   i18, 0       
   124 : unspill   i19, 1       
   125 : add       i31, i31, 16 
   126 : ret                    
