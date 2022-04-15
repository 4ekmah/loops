triangle_types(i0, i1, i2)
     0 : sub       i31, i31, 48 
     1 : mov       i4, 0        
     2 : spill     0, i4        
     3 : unspill   i4, 0        
     4 : cmp       i0, i4       
     5 : jmp_gt    0            
     6 : mov       i3, 0        
     7 : mov       iR, i3       
     8 : jmp       29           
     9 : jmp       28           
    10 : label 0:               
    11 : unspill   i4, 0        
    12 : cmp       i1, i4       
    13 : jmp_gt    1            
    14 : mov       i3, 0        
    15 : mov       iR, i3       
    16 : jmp       29           
    17 : jmp       27           
    18 : label 1:               
    19 : unspill   i4, 0        
    20 : cmp       i2, i4       
    21 : jmp_gt    2            
    22 : mov       i3, 0        
    23 : mov       iR, i3       
    24 : jmp       29           
    25 : jmp       26           
    26 : label 2:               
    27 : add       i4, i0, i1   
    28 : spill     1, i4        
    29 : add       i4, i0, i2   
    30 : spill     2, i4        
    31 : add       i3, i1, i2   
    32 : cmp       i0, i3       
    33 : jmp_le    3            
    34 : mov       i3, 0        
    35 : mov       iR, i3       
    36 : jmp       29           
    37 : jmp       25           
    38 : label 3:               
    39 : unspill   i4, 2        
    40 : cmp       i1, i4       
    41 : jmp_le    4            
    42 : mov       i3, 0        
    43 : mov       iR, i3       
    44 : jmp       29           
    45 : jmp       24           
    46 : label 4:               
    47 : unspill   i4, 1        
    48 : cmp       i2, i4       
    49 : jmp_le    5            
    50 : mov       i3, 0        
    51 : mov       iR, i3       
    52 : jmp       29           
    53 : jmp       23           
    54 : label 5:               
    55 : cmp       i0, i1       
    56 : jmp_ne    8            
    57 : cmp       i1, i2       
    58 : jmp_ne    6            
    59 : mov       i3, 2        
    60 : mov       iR, i3       
    61 : jmp       29           
    62 : jmp       7            
    63 : label 6:               
    64 : mov       i3, 3        
    65 : mov       iR, i3       
    66 : jmp       29           
    67 : label 7:               
    68 : jmp       12           
    69 : label 8:               
    70 : cmp       i0, i2       
    71 : jmp_ne    9            
    72 : mov       i3, 3        
    73 : mov       iR, i3       
    74 : jmp       29           
    75 : jmp       11           
    76 : label 9:               
    77 : cmp       i1, i2       
    78 : jmp_ne    10           
    79 : mov       i3, 3        
    80 : mov       iR, i3       
    81 : jmp       29           
    82 : label 10:              
    83 : label 11:              
    84 : label 12:              
    85 : mul       i0, i0, i0   
    86 : mul       i1, i1, i1   
    87 : mul       i4, i2, i2   
    88 : spill     4, i4        
    89 : add       i4, i0, i1   
    90 : spill     3, i4        
    91 : unspill   i4, 4        
    92 : add       i5, i0, i4   
    93 : spill     5, i5        
    94 : unspill   i4, 4        
    95 : add       i2, i1, i4   
    96 : cmp       i0, i2       
    97 : jmp_ne    13           
    98 : mov       i3, 1        
    99 : mov       iR, i3       
   100 : jmp       29           
   101 : jmp       17           
   102 : label 13:              
   103 : unspill   i4, 5        
   104 : cmp       i1, i4       
   105 : jmp_ne    14           
   106 : mov       i3, 1        
   107 : mov       iR, i3       
   108 : jmp       29           
   109 : jmp       16           
   110 : label 14:              
   111 : unspill   i4, 4        
   112 : unspill   i5, 3        
   113 : cmp       i4, i5       
   114 : jmp_ne    15           
   115 : mov       i3, 1        
   116 : mov       iR, i3       
   117 : jmp       29           
   118 : label 15:              
   119 : label 16:              
   120 : label 17:              
   121 : sub       i0, i2, i0   
   122 : unspill   i4, 5        
   123 : sub       i1, i4, i1   
   124 : unspill   i4, 4        
   125 : unspill   i5, 3        
   126 : sub       i2, i5, i4   
   127 : unspill   i4, 0        
   128 : cmp       i0, i4       
   129 : jmp_ge    18           
   130 : mov       i0, 5        
   131 : mov       iR, i0       
   132 : jmp       29           
   133 : jmp       22           
   134 : label 18:              
   135 : unspill   i4, 0        
   136 : cmp       i1, i4       
   137 : jmp_ge    19           
   138 : mov       i0, 5        
   139 : mov       iR, i0       
   140 : jmp       29           
   141 : jmp       21           
   142 : label 19:              
   143 : unspill   i4, 0        
   144 : cmp       i2, i4       
   145 : jmp_ge    20           
   146 : mov       i0, 5        
   147 : mov       iR, i0       
   148 : jmp       29           
   149 : label 20:              
   150 : label 21:              
   151 : label 22:              
   152 : label 23:              
   153 : label 24:              
   154 : label 25:              
   155 : label 26:              
   156 : label 27:              
   157 : label 28:              
   158 : mov       i0, 4        
   159 : mov       iR, i0       
   160 : label 29:              
   161 : add       i31, i31, 48 
   162 : ret                    
