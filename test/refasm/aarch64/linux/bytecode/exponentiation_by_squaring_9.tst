exponentiation_by_squaring_9(i0, i1, i2)
     0 : sub       i31, i31, 32  
     1 : spill     1, i18        
     2 : spill     2, i19        
     3 : spill     3, i20        
     4 : mov       i3, 1         
     5 : mov       i3, 0         
     6 : mov       i20, 0        
     7 : spill     0, i20        
     8 : label 0:                
     9 : unspill   i20, 0        
    10 : cmp       i20, i1       
    11 : jmp_ge 2                
    12 : load.i32  i19, i0, i3   
    13 : mul       i18, i19, i19 
    14 : mul       i18, i18, i18 
    15 : mul       i18, i18, i18 
    16 : mul       i18, i19, i18 
    17 : store.i32 i2, i18       
    18 : unspill   i20, 0        
    19 : add       i20, i20, 1   
    20 : spill     0, i20        
    21 : add       i3, i3, 4     
    22 : add       i2, i2, 4     
    23 : jmp       0             
    24 : label 2:                
    25 : mov       i0, 0         
    26 : unspill   i18, 1        
    27 : unspill   i19, 2        
    28 : unspill   i20, 3        
    29 : add       i31, i31, 32  
    30 : ret                     
