min_max_scalar(i0, i1, i2, i3)
     0 : sub       i31, i31, 64 
     1 : spill     1, i2        
     2 : spill     0, i3        
     3 : spill     5, i25       
     4 : spill     6, i26       
     5 : mov       i25, 0       
     6 : mov       i26, 0       
     7 : spill     3, i26       
     8 : mov       i26, 0       
     9 : spill     2, i26       
    10 : load.i32  i2, i0       
    11 : mov       i26, i2      
    12 : spill     4, i26       
    13 : mov       i3, 4        
    14 : mul       i1, i1, i3   
    15 : label 0:               
    16 : cmp       i25, i1      
    17 : jmp_ge 2               
    18 : load.i32  i3, i0, i25  
    19 : cmp       i3, i2       
    20 : jmp_ge 4               
    21 : mov       i2, i3       
    22 : mov       i26, i25     
    23 : spill     3, i26       
    24 : label 4:               
    25 : unspill   i26, 4       
    26 : cmp       i3, i26      
    27 : jmp_le 6               
    28 : mov       i26, i3      
    29 : spill     4, i26       
    30 : mov       i26, i25     
    31 : spill     2, i26       
    32 : label 6:               
    33 : add       i25, i25, 4  
    34 : jmp       0            
    35 : label 2:               
    36 : mov       i0, 4        
    37 : unspill   i26, 3       
    38 : div       i1, i26, i0  
    39 : unspill   i26, 2       
    40 : div       i0, i26, i0  
    41 : unspill   i26, 1       
    42 : store.i32 i26, i1      
    43 : unspill   i26, 0       
    44 : store.i32 i26, i0      
    45 : mov       i0, 0        
    46 : unspill   i25, 5       
    47 : unspill   i26, 6       
    48 : add       i31, i31, 64 
    49 : ret                    
