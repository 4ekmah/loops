triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 24 
     1 : spill     0, i2      
     2 : spill     1, i13     
     3 : cmp       i1, 0      
     4 : jmp_le 0             
     5 : cmp       s0, 0      
     6 : jmp_le 0             
     7 : cmp       i8, 0      
     8 : jmp_gt 1             
     9 : label 0:             
    10 : mov       i0, 0      
    11 : jmp       18         
    12 : label 1:             
    13 : unspill   i13, 0     
    14 : mov       i9, i13    
    15 : add       i9, i9, i8 
    16 : cmp       i1, i9     
    17 : jmp_gt 3             
    18 : mov       i9, i1     
    19 : add       i9, i9, i8 
    20 : cmp       s0, i9     
    21 : jmp_gt 3             
    22 : mov       i9, i1     
    23 : add       i9, i9, s0 
    24 : cmp       i8, i9     
    25 : jmp_le 4             
    26 : label 3:             
    27 : mov       i0, 0      
    28 : jmp       18         
    29 : label 4:             
    30 : cmp       i1, s0     
    31 : jmp_ne 7             
    32 : cmp       i1, i8     
    33 : jmp_ne 7             
    34 : mov       i0, 2      
    35 : jmp       18         
    36 : label 7:             
    37 : cmp       i1, s0     
    38 : jmp_eq 9             
    39 : cmp       i1, i8     
    40 : jmp_eq 9             
    41 : cmp       s0, i8     
    42 : jmp_ne 10            
    43 : label 9:             
    44 : mov       i0, 3      
    45 : jmp       18         
    46 : label 10:            
    47 : mov       i9, i1     
    48 : mul       i9, i9, i1 
    49 : unspill   i13, 0     
    50 : mov       i0, i13    
    51 : mul       i0, i0, s0 
    52 : mov       i2, i8     
    53 : mul       i2, i2, i8 
    54 : add       i0, i0, i2 
    55 : cmp       i9, i0     
    56 : jmp_eq 12            
    57 : unspill   i13, 0     
    58 : mov       i2, i13    
    59 : mul       i2, i2, s0 
    60 : mov       i9, i1     
    61 : mul       i9, i9, i1 
    62 : mov       i0, i8     
    63 : mul       i0, i0, i8 
    64 : add       i9, i9, i0 
    65 : cmp       i2, i9     
    66 : jmp_eq 12            
    67 : mov       i2, i8     
    68 : mul       i2, i2, i8 
    69 : mov       i9, i1     
    70 : mul       i9, i9, i1 
    71 : unspill   i13, 0     
    72 : mov       i0, i13    
    73 : mul       i0, i0, s0 
    74 : add       i9, i9, i0 
    75 : cmp       i2, i9     
    76 : jmp_ne 13            
    77 : label 12:            
    78 : mov       i0, 1      
    79 : jmp       18         
    80 : label 13:            
    81 : mov       i2, i1     
    82 : mul       i2, i2, i1 
    83 : unspill   i13, 0     
    84 : mov       i9, i13    
    85 : mul       i9, i9, s0 
    86 : mov       i0, i8     
    87 : mul       i0, i0, i8 
    88 : add       i9, i9, i0 
    89 : cmp       i2, i9     
    90 : jmp_gt 15            
    91 : unspill   i13, 0     
    92 : mov       i2, i13    
    93 : mul       i2, i2, s0 
    94 : mov       i9, i1     
    95 : mul       i9, i9, i1 
    96 : mov       i0, i8     
    97 : mul       i0, i0, i8 
    98 : add       i9, i9, i0 
    99 : cmp       i2, i9     
   100 : jmp_gt 15            
   101 : mul       i8, i8, i8 
   102 : mul       i1, i1, i1 
   103 : unspill   i13, 0     
   104 : mov       i2, i13    
   105 : mul       i2, i2, s0 
   106 : add       i1, i1, i2 
   107 : cmp       i8, i1     
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
