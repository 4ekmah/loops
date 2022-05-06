triangle_types(i0, i1, i2)
     0 : sub       i31, i31, 48 
     1 : spill     2, i18       
     2 : spill     3, i19       
     3 : spill     4, i20       
     4 : mov       i20, 0       
     5 : spill     0, i20       
     6 : unspill   i20, 0       
     7 : cmp       i0, i20      
     8 : jmp_gt    0            
     9 : mov       i18, 0       
    10 : mov       i0, i18      
    11 : jmp       29           
    12 : jmp       28           
    13 : label 0:               
    14 : unspill   i20, 0       
    15 : cmp       i1, i20      
    16 : jmp_gt    1            
    17 : mov       i18, 0       
    18 : mov       i0, i18      
    19 : jmp       29           
    20 : jmp       27           
    21 : label 1:               
    22 : unspill   i20, 0       
    23 : cmp       i2, i20      
    24 : jmp_gt    2            
    25 : mov       i18, 0       
    26 : mov       i0, i18      
    27 : jmp       29           
    28 : jmp       26           
    29 : label 2:               
    30 : add       i18, i0, i1  
    31 : add       i19, i0, i2  
    32 : add       i3, i1, i2   
    33 : cmp       i0, i3       
    34 : jmp_le    3            
    35 : mov       i3, 0        
    36 : mov       i0, i3       
    37 : jmp       29           
    38 : jmp       25           
    39 : label 3:               
    40 : cmp       i1, i19      
    41 : jmp_le    4            
    42 : mov       i3, 0        
    43 : mov       i0, i3       
    44 : jmp       29           
    45 : jmp       24           
    46 : label 4:               
    47 : cmp       i2, i18      
    48 : jmp_le    5            
    49 : mov       i3, 0        
    50 : mov       i0, i3       
    51 : jmp       29           
    52 : jmp       23           
    53 : label 5:               
    54 : cmp       i0, i1       
    55 : jmp_ne    8            
    56 : cmp       i1, i2       
    57 : jmp_ne    6            
    58 : mov       i3, 2        
    59 : mov       i0, i3       
    60 : jmp       29           
    61 : jmp       7            
    62 : label 6:               
    63 : mov       i3, 3        
    64 : mov       i0, i3       
    65 : jmp       29           
    66 : label 7:               
    67 : jmp       12           
    68 : label 8:               
    69 : cmp       i0, i2       
    70 : jmp_ne    9            
    71 : mov       i3, 3        
    72 : mov       i0, i3       
    73 : jmp       29           
    74 : jmp       11           
    75 : label 9:               
    76 : cmp       i1, i2       
    77 : jmp_ne    10           
    78 : mov       i3, 3        
    79 : mov       i0, i3       
    80 : jmp       29           
    81 : label 10:              
    82 : label 11:              
    83 : label 12:              
    84 : mul       i0, i0, i0   
    85 : mul       i1, i1, i1   
    86 : mul       i2, i2, i2   
    87 : add       i20, i0, i1  
    88 : spill     1, i20       
    89 : add       i18, i0, i2  
    90 : add       i19, i1, i2  
    91 : cmp       i0, i19      
    92 : jmp_ne    13           
    93 : mov       i3, 1        
    94 : mov       i0, i3       
    95 : jmp       29           
    96 : jmp       17           
    97 : label 13:              
    98 : cmp       i1, i18      
    99 : jmp_ne    14           
   100 : mov       i3, 1        
   101 : mov       i0, i3       
   102 : jmp       29           
   103 : jmp       16           
   104 : label 14:              
   105 : unspill   i20, 1       
   106 : cmp       i2, i20      
   107 : jmp_ne    15           
   108 : mov       i3, 1        
   109 : mov       i0, i3       
   110 : jmp       29           
   111 : label 15:              
   112 : label 16:              
   113 : label 17:              
   114 : sub       i0, i19, i0  
   115 : sub       i1, i18, i1  
   116 : unspill   i20, 1       
   117 : sub       i2, i20, i2  
   118 : unspill   i20, 0       
   119 : cmp       i0, i20      
   120 : jmp_ge    18           
   121 : mov       i0, 5        
   122 : mov       i0, i0       
   123 : jmp       29           
   124 : jmp       22           
   125 : label 18:              
   126 : unspill   i20, 0       
   127 : cmp       i1, i20      
   128 : jmp_ge    19           
   129 : mov       i1, 5        
   130 : mov       i0, i1       
   131 : jmp       29           
   132 : jmp       21           
   133 : label 19:              
   134 : unspill   i20, 0       
   135 : cmp       i2, i20      
   136 : jmp_ge    20           
   137 : mov       i2, 5        
   138 : mov       i0, i2       
   139 : jmp       29           
   140 : label 20:              
   141 : label 21:              
   142 : label 22:              
   143 : label 23:              
   144 : label 24:              
   145 : label 25:              
   146 : label 26:              
   147 : label 27:              
   148 : label 28:              
   149 : mov       i2, 4        
   150 : mov       i0, i2       
   151 : label 29:              
   152 : unspill   i18, 2       
   153 : unspill   i19, 3       
   154 : unspill   i20, 4       
   155 : add       i31, i31, 48 
   156 : ret                    
