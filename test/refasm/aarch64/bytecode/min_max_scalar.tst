min_max_scalar(i0, i1, i2, i3)
     0 : sub       i31, i31, 64 
     1 : spill     1, i2        
     2 : spill     0, i3        
     3 : spill     4, i18       
     4 : spill     5, i19       
     5 : spill     6, i20       
     6 : mov       i18, 0       
     7 : mov       i20, 0       
     8 : spill     3, i20       
     9 : mov       i20, 0       
    10 : spill     2, i20       
    11 : load.i32  i2, i0       
    12 : mov       i3, i2       
    13 : mov       i19, 4       
    14 : mul       i1, i1, i19  
    15 : label 0:               
    16 : cmp       i18, i1      
    17 : jmp_ge    3            
    18 : load.i32  i19, i0, i18 
    19 : cmp       i19, i2      
    20 : jmp_ge    1            
    21 : mov       i2, i19      
    22 : mov       i20, i18     
    23 : spill     3, i20       
    24 : label 1:               
    25 : cmp       i19, i3      
    26 : jmp_le    2            
    27 : mov       i3, i19      
    28 : mov       i20, i18     
    29 : spill     2, i20       
    30 : label 2:               
    31 : add       i18, i18, 4  
    32 : jmp       0            
    33 : label 3:               
    34 : mov       i0, 4        
    35 : unspill   i20, 3       
    36 : div       i1, i20, i0  
    37 : unspill   i20, 2       
    38 : div       i0, i20, i0  
    39 : unspill   i20, 1       
    40 : store.i32 i20, i1      
    41 : unspill   i20, 0       
    42 : store.i32 i20, i0      
    43 : mov       i0, 0        
    44 : unspill   i18, 4       
    45 : unspill   i19, 5       
    46 : unspill   i20, 6       
    47 : add       i31, i31, 64 
    48 : ret                    
