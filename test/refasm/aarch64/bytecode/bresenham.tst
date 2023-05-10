bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i31, i31, 64  
     1 : spill     4, i18        
     2 : spill     5, i19        
     3 : spill     6, i20        
     4 : spill     7, i21        
     5 : sub       i18, i4, i2   
     6 : cmp       i18, 0        
     7 : arm_cneg  i18, i18, 31  
     8 : sub       i19, i4, i2   
     9 : cmp       i19, 0        
    10 : sar       i20, i19, 63  
    11 : arm_cinc  i20, i20, 28  
    12 : spill     0, i20        
    13 : sub       i19, i5, i3   
    14 : cmp       i19, 0        
    15 : arm_cneg  i19, i19, 31  
    16 : neg       i20, i19      
    17 : spill     1, i20        
    18 : sub       i19, i5, i3   
    19 : cmp       i19, 0        
    20 : sar       i20, i19, 63  
    21 : arm_cinc  i20, i20, 28  
    22 : spill     3, i20        
    23 : unspill   i20, 1        
    24 : add       i21, i18, i20 
    25 : spill     2, i21        
    26 : label 0:                
    27 : cmp       i0, 0         
    28 : jmp_eq 2                
    29 : mul       i19, i3, i1   
    30 : add       i19, i19, i2  
    31 : store.u8  i0, i19, i6   
    32 : cmp       i2, i4        
    33 : jmp_ne 4                
    34 : cmp       i3, i5        
    35 : jmp_ne 4                
    36 : jmp       2             
    37 : label 4:                
    38 : unspill   i20, 2        
    39 : shl       i19, i20, 1   
    40 : unspill   i20, 1        
    41 : cmp       i19, i20      
    42 : jmp_gt 6                
    43 : cmp       i2, i4        
    44 : jmp_ne 8                
    45 : jmp       2             
    46 : label 8:                
    47 : unspill   i21, 1        
    48 : unspill   i20, 2        
    49 : add       i20, i20, i21 
    50 : spill     2, i20        
    51 : unspill   i20, 0        
    52 : add       i2, i2, i20   
    53 : label 6:                
    54 : cmp       i19, i18      
    55 : jmp_gt 10               
    56 : cmp       i3, i5        
    57 : jmp_ne 12               
    58 : jmp       2             
    59 : label 12:               
    60 : unspill   i20, 2        
    61 : add       i20, i20, i18 
    62 : spill     2, i20        
    63 : unspill   i20, 3        
    64 : add       i3, i3, i20   
    65 : label 10:               
    66 : jmp       0             
    67 : label 2:                
    68 : unspill   i18, 4        
    69 : unspill   i19, 5        
    70 : unspill   i20, 6        
    71 : unspill   i21, 7        
    72 : add       i31, i31, 64  
    73 : ret                     
