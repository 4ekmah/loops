triangle_types(i0, i1, i2)
     0 : sub       i4, i4, 8    
     1 : spill     0, i12       
     2 : cmp       i1, 0        
     3 : jmp_le 0               
     4 : cmp       i2, 0        
     5 : jmp_le 0               
     6 : cmp       i8, 0        
     7 : jmp_gt 1               
     8 : label 0:               
     9 : mov       i0, 0        
    10 : jmp       18           
    11 : label 1:               
    12 : mov       i9, i2       
    13 : add       i9, i9, i8   
    14 : cmp       i1, i9       
    15 : jmp_gt 3               
    16 : mov       i9, i1       
    17 : add       i9, i9, i8   
    18 : cmp       i2, i9       
    19 : jmp_gt 3               
    20 : mov       i9, i1       
    21 : add       i9, i9, i2   
    22 : cmp       i8, i9       
    23 : jmp_le 4               
    24 : label 3:               
    25 : mov       i0, 0        
    26 : jmp       18           
    27 : label 4:               
    28 : cmp       i1, i2       
    29 : jmp_ne 7               
    30 : cmp       i1, i8       
    31 : jmp_ne 7               
    32 : mov       i0, 2        
    33 : jmp       18           
    34 : label 7:               
    35 : cmp       i1, i2       
    36 : jmp_eq 9               
    37 : cmp       i1, i8       
    38 : jmp_eq 9               
    39 : cmp       i2, i8       
    40 : jmp_ne 10              
    41 : label 9:               
    42 : mov       i0, 3        
    43 : jmp       18           
    44 : label 10:              
    45 : mov       i9, i1       
    46 : mul       i9, i9, i1   
    47 : mov       i0, i2       
    48 : mul       i0, i0, i2   
    49 : mov       i12, i8      
    50 : mul       i12, i12, i8 
    51 : add       i0, i0, i12  
    52 : cmp       i9, i0       
    53 : jmp_eq 12              
    54 : mov       i9, i2       
    55 : mul       i9, i9, i2   
    56 : mov       i0, i1       
    57 : mul       i0, i0, i1   
    58 : mov       i12, i8      
    59 : mul       i12, i12, i8 
    60 : add       i0, i0, i12  
    61 : cmp       i9, i0       
    62 : jmp_eq 12              
    63 : mov       i9, i8       
    64 : mul       i9, i9, i8   
    65 : mov       i0, i1       
    66 : mul       i0, i0, i1   
    67 : mov       i12, i2      
    68 : mul       i12, i12, i2 
    69 : add       i0, i0, i12  
    70 : cmp       i9, i0       
    71 : jmp_ne 13              
    72 : label 12:              
    73 : mov       i0, 1        
    74 : jmp       18           
    75 : label 13:              
    76 : mov       i9, i1       
    77 : mul       i9, i9, i1   
    78 : mov       i0, i2       
    79 : mul       i0, i0, i2   
    80 : mov       i12, i8      
    81 : mul       i12, i12, i8 
    82 : add       i0, i0, i12  
    83 : cmp       i9, i0       
    84 : jmp_gt 15              
    85 : mov       i9, i2       
    86 : mul       i9, i9, i2   
    87 : mov       i0, i1       
    88 : mul       i0, i0, i1   
    89 : mov       i12, i8      
    90 : mul       i12, i12, i8 
    91 : add       i0, i0, i12  
    92 : cmp       i9, i0       
    93 : jmp_gt 15              
    94 : mul       i8, i8, i8   
    95 : mul       i1, i1, i1   
    96 : mul       i2, i2, i2   
    97 : add       i1, i1, i2   
    98 : cmp       i8, i1       
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
