triangle_types(i0, i1, i2)
     0 : sub       i31, i31, 16  
     1 : spill     0, i18        
     2 : spill     1, i19        
     3 : cmp       i0, 0         
     4 : jmp_le 0                
     5 : cmp       i1, 0         
     6 : jmp_le 0                
     7 : cmp       i2, 0         
     8 : jmp_gt 1                
     9 : label 0:                
    10 : mov       i0, 0         
    11 : jmp       18            
    12 : label 1:                
    13 : add       i3, i1, i2    
    14 : cmp       i0, i3        
    15 : jmp_gt 3                
    16 : add       i3, i0, i2    
    17 : cmp       i1, i3        
    18 : jmp_gt 3                
    19 : add       i3, i0, i1    
    20 : cmp       i2, i3        
    21 : jmp_le 4                
    22 : label 3:                
    23 : mov       i0, 0         
    24 : jmp       18            
    25 : label 4:                
    26 : cmp       i0, i1        
    27 : jmp_ne 7                
    28 : cmp       i0, i2        
    29 : jmp_ne 7                
    30 : mov       i0, 2         
    31 : jmp       18            
    32 : label 7:                
    33 : cmp       i0, i1        
    34 : jmp_eq 9                
    35 : cmp       i0, i2        
    36 : jmp_eq 9                
    37 : cmp       i1, i2        
    38 : jmp_ne 10               
    39 : label 9:                
    40 : mov       i0, 3         
    41 : jmp       18            
    42 : label 10:               
    43 : mul       i3, i0, i0    
    44 : mul       i18, i1, i1   
    45 : mul       i19, i2, i2   
    46 : add       i18, i18, i19 
    47 : cmp       i3, i18       
    48 : jmp_eq 12               
    49 : mul       i3, i1, i1    
    50 : mul       i18, i0, i0   
    51 : mul       i19, i2, i2   
    52 : add       i18, i18, i19 
    53 : cmp       i3, i18       
    54 : jmp_eq 12               
    55 : mul       i3, i2, i2    
    56 : mul       i18, i0, i0   
    57 : mul       i19, i1, i1   
    58 : add       i18, i18, i19 
    59 : cmp       i3, i18       
    60 : jmp_ne 13               
    61 : label 12:               
    62 : mov       i0, 1         
    63 : jmp       18            
    64 : label 13:               
    65 : mul       i3, i0, i0    
    66 : mul       i18, i1, i1   
    67 : mul       i19, i2, i2   
    68 : add       i18, i18, i19 
    69 : cmp       i3, i18       
    70 : jmp_gt 15               
    71 : mul       i3, i1, i1    
    72 : mul       i18, i0, i0   
    73 : mul       i19, i2, i2   
    74 : add       i18, i18, i19 
    75 : cmp       i3, i18       
    76 : jmp_gt 15               
    77 : mul       i2, i2, i2    
    78 : mul       i0, i0, i0    
    79 : mul       i1, i1, i1    
    80 : add       i0, i0, i1    
    81 : cmp       i2, i0        
    82 : jmp_le 16               
    83 : label 15:               
    84 : mov       i0, 5         
    85 : jmp       18            
    86 : label 16:               
    87 : mov       i0, 4         
    88 : jmp       18            
    89 : label 2:                
    90 : label 5:                
    91 : label 8:                
    92 : label 11:               
    93 : label 14:               
    94 : label 17:               
    95 : label 18:               
    96 : unspill   i18, 0        
    97 : unspill   i19, 1        
    98 : add       i31, i31, 16  
    99 : ret                     
