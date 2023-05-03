triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 8    
     1 : spill     0, i12       
     2 : cmp       i7, 0        
     3 : jmp_le 0               
     4 : cmp       i6, 0        
     5 : jmp_le 0               
     6 : cmp       i2, 0        
     7 : jmp_gt 1               
     8 : label 0:               
     9 : mov       i0, 0        
    10 : jmp       18           
    11 : label 1:               
    12 : mov       i1, i6       
    13 : add       i1, i1, i2   
    14 : cmp       i7, i1       
    15 : jmp_gt 3               
    16 : mov       i1, i7       
    17 : add       i1, i1, i2   
    18 : cmp       i6, i1       
    19 : jmp_gt 3               
    20 : mov       i1, i7       
    21 : add       i1, i1, i6   
    22 : cmp       i2, i1       
    23 : jmp_le 4               
    24 : label 3:               
    25 : mov       i0, 0        
    26 : jmp       18           
    27 : label 4:               
    28 : cmp       i7, i6       
    29 : jmp_ne 7               
    30 : cmp       i7, i2       
    31 : jmp_ne 7               
    32 : mov       i0, 2        
    33 : jmp       18           
    34 : label 7:               
    35 : cmp       i7, i6       
    36 : jmp_eq 9               
    37 : cmp       i7, i2       
    38 : jmp_eq 9               
    39 : cmp       i6, i2       
    40 : jmp_ne 10              
    41 : label 9:               
    42 : mov       i0, 3        
    43 : jmp       18           
    44 : label 10:              
    45 : mov       i1, i7       
    46 : mul       i1, i1, i7   
    47 : mov       i0, i6       
    48 : mul       i0, i0, i6   
    49 : mov       i12, i2      
    50 : mul       i12, i12, i2 
    51 : add       i0, i0, i12  
    52 : cmp       i1, i0       
    53 : jmp_eq 12              
    54 : mov       i1, i6       
    55 : mul       i1, i1, i6   
    56 : mov       i0, i7       
    57 : mul       i0, i0, i7   
    58 : mov       i12, i2      
    59 : mul       i12, i12, i2 
    60 : add       i0, i0, i12  
    61 : cmp       i1, i0       
    62 : jmp_eq 12              
    63 : mov       i1, i2       
    64 : mul       i1, i1, i2   
    65 : mov       i0, i7       
    66 : mul       i0, i0, i7   
    67 : mov       i12, i6      
    68 : mul       i12, i12, i6 
    69 : add       i0, i0, i12  
    70 : cmp       i1, i0       
    71 : jmp_ne 13              
    72 : label 12:              
    73 : mov       i0, 1        
    74 : jmp       18           
    75 : label 13:              
    76 : mov       i1, i7       
    77 : mul       i1, i1, i7   
    78 : mov       i0, i6       
    79 : mul       i0, i0, i6   
    80 : mov       i12, i2      
    81 : mul       i12, i12, i2 
    82 : add       i0, i0, i12  
    83 : cmp       i1, i0       
    84 : jmp_gt 15              
    85 : mov       i1, i6       
    86 : mul       i1, i1, i6   
    87 : mov       i0, i7       
    88 : mul       i0, i0, i7   
    89 : mov       i12, i2      
    90 : mul       i12, i12, i2 
    91 : add       i0, i0, i12  
    92 : cmp       i1, i0       
    93 : jmp_gt 15              
    94 : mul       i2, i2, i2   
    95 : mul       i7, i7, i7   
    96 : mul       i6, i6, i6   
    97 : add       i7, i7, i6   
    98 : cmp       i2, i7       
    99 : jmp_le 16              
   100 : label 15:              
   101 : mov       i0, 5        
   102 : jmp       18           
   103 : label 16:              
   104 : mov       i0, 4        
   105 : jmp       18           
   106 : label 2:               
   107 : label 5:               
   108 : label 8:               
   109 : label 11:              
   110 : label 14:              
   111 : label 17:              
   112 : label 18:              
   113 : unspill   i12, 0       
   114 : add       i4, i4, 8    
   115 : ret                    
