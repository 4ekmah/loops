bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i31, i31, 80  
     1 : spill     5, i25        
     2 : spill     6, i26        
     3 : spill     7, i27        
     4 : spill     8, i28        
     5 : sub       i25, i4, i2   
     6 : cmp       i25, 0        
     7 : arm_cneg  i26, i25, 32  
     8 : spill     0, i26        
     9 : sub       i25, i4, i2   
    10 : cmp       i25, 0        
    11 : sar       i26, i25, 63  
    12 : arm_cinc  i26, i26, 29  
    13 : spill     1, i26        
    14 : sub       i25, i5, i3   
    15 : cmp       i25, 0        
    16 : arm_cneg  i25, i25, 32  
    17 : neg       i26, i25      
    18 : spill     2, i26        
    19 : sub       i25, i5, i3   
    20 : cmp       i25, 0        
    21 : sar       i26, i25, 63  
    22 : arm_cinc  i26, i26, 29  
    23 : spill     4, i26        
    24 : unspill   i26, 0        
    25 : unspill   i27, 2        
    26 : add       i28, i26, i27 
    27 : spill     3, i28        
    28 : label 0:                
    29 : cmp       i0, 0         
    30 : jmp_eq 2                
    31 : mul       i25, i3, i1   
    32 : add       i25, i25, i2  
    33 : store.u8  i0, i25, i6   
    34 : cmp       i2, i4        
    35 : jmp_ne 4                
    36 : cmp       i3, i5        
    37 : jmp_ne 4                
    38 : jmp       2             
    39 : label 4:                
    40 : unspill   i26, 3        
    41 : shl       i25, i26, 1   
    42 : unspill   i26, 2        
    43 : cmp       i25, i26      
    44 : jmp_gt 6                
    45 : cmp       i2, i4        
    46 : jmp_ne 8                
    47 : jmp       2             
    48 : label 8:                
    49 : unspill   i27, 2        
    50 : unspill   i26, 3        
    51 : add       i26, i26, i27 
    52 : spill     3, i26        
    53 : unspill   i26, 1        
    54 : add       i2, i2, i26   
    55 : label 6:                
    56 : unspill   i26, 0        
    57 : cmp       i25, i26      
    58 : jmp_gt 10               
    59 : cmp       i3, i5        
    60 : jmp_ne 12               
    61 : jmp       2             
    62 : label 12:               
    63 : unspill   i27, 0        
    64 : unspill   i26, 3        
    65 : add       i26, i26, i27 
    66 : spill     3, i26        
    67 : unspill   i26, 4        
    68 : add       i3, i3, i26   
    69 : label 10:               
    70 : jmp       0             
    71 : label 2:                
    72 : unspill   i25, 5        
    73 : unspill   i26, 6        
    74 : unspill   i27, 7        
    75 : unspill   i28, 8        
    76 : add       i31, i31, 80  
    77 : ret                     
