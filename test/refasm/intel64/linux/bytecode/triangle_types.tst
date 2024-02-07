triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 24 
     1 : spill     0, i6      
     2 : spill     1, i13     
     3 : cmp       i7, 0      
     4 : jmp_le 0             
     5 : cmp       s0, 0      
     6 : jmp_le 0             
     7 : cmp       i2, 0      
     8 : jmp_gt 1             
     9 : label 0:             
    10 : mov       i0, 0      
    11 : jmp       18         
    12 : label 1:             
    13 : unspill   i13, 0     
    14 : mov       i1, i13    
    15 : add       i1, i1, i2 
    16 : cmp       i7, i1     
    17 : jmp_gt 3             
    18 : mov       i1, i7     
    19 : add       i1, i1, i2 
    20 : cmp       s0, i1     
    21 : jmp_gt 3             
    22 : mov       i1, i7     
    23 : add       i1, i1, s0 
    24 : cmp       i2, i1     
    25 : jmp_le 4             
    26 : label 3:             
    27 : mov       i0, 0      
    28 : jmp       18         
    29 : label 4:             
    30 : cmp       i7, s0     
    31 : jmp_ne 7             
    32 : cmp       i7, i2     
    33 : jmp_ne 7             
    34 : mov       i0, 2      
    35 : jmp       18         
    36 : label 7:             
    37 : cmp       i7, s0     
    38 : jmp_eq 9             
    39 : cmp       i7, i2     
    40 : jmp_eq 9             
    41 : cmp       s0, i2     
    42 : jmp_ne 10            
    43 : label 9:             
    44 : mov       i0, 3      
    45 : jmp       18         
    46 : label 10:            
    47 : mov       i1, i7     
    48 : mul       i1, i1, i7 
    49 : unspill   i13, 0     
    50 : mov       i0, i13    
    51 : mul       i0, i0, s0 
    52 : mov       i6, i2     
    53 : mul       i6, i6, i2 
    54 : add       i0, i0, i6 
    55 : cmp       i1, i0     
    56 : jmp_eq 12            
    57 : unspill   i13, 0     
    58 : mov       i6, i13    
    59 : mul       i6, i6, s0 
    60 : mov       i1, i7     
    61 : mul       i1, i1, i7 
    62 : mov       i0, i2     
    63 : mul       i0, i0, i2 
    64 : add       i1, i1, i0 
    65 : cmp       i6, i1     
    66 : jmp_eq 12            
    67 : mov       i6, i2     
    68 : mul       i6, i6, i2 
    69 : mov       i1, i7     
    70 : mul       i1, i1, i7 
    71 : unspill   i13, 0     
    72 : mov       i0, i13    
    73 : mul       i0, i0, s0 
    74 : add       i1, i1, i0 
    75 : cmp       i6, i1     
    76 : jmp_ne 13            
    77 : label 12:            
    78 : mov       i0, 1      
    79 : jmp       18         
    80 : label 13:            
    81 : mov       i6, i7     
    82 : mul       i6, i6, i7 
    83 : unspill   i13, 0     
    84 : mov       i1, i13    
    85 : mul       i1, i1, s0 
    86 : mov       i0, i2     
    87 : mul       i0, i0, i2 
    88 : add       i1, i1, i0 
    89 : cmp       i6, i1     
    90 : jmp_gt 15            
    91 : unspill   i13, 0     
    92 : mov       i6, i13    
    93 : mul       i6, i6, s0 
    94 : mov       i1, i7     
    95 : mul       i1, i1, i7 
    96 : mov       i0, i2     
    97 : mul       i0, i0, i2 
    98 : add       i1, i1, i0 
    99 : cmp       i6, i1     
   100 : jmp_gt 15            
   101 : mul       i2, i2, i2 
   102 : mul       i7, i7, i7 
   103 : unspill   i13, 0     
   104 : mov       i6, i13    
   105 : mul       i6, i6, s0 
   106 : add       i7, i7, i6 
   107 : cmp       i2, i7     
   108 : jmp_le 16            
   109 : label 15:            
   110 : mov       i0, 5      
   111 : jmp       18         
   112 : label 16:            
   113 : mov       i0, 4      
   114 : jmp       18         
   115 : label 2:             
   116 : label 5:             
   117 : label 8:             
   118 : label 11:            
   119 : label 14:            
   120 : label 17:            
   121 : label 18:            
   122 : unspill   i13, 1     
   123 : add       i4, i4, 24 
   124 : ret                  
