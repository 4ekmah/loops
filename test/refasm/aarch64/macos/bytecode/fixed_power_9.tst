exponentiation_by_squaring_9(i0, i1, i2)
     0 : sub       i31, i31, 32  
     1 : spill     1, i18        
     2 : spill     2, i19        
     3 : spill     3, i20        
     4 : mov       i3, 0         
     5 : mov       i20, 0        
     6 : spill     0, i20        
     7 : label 0:                
     8 : unspill   i20, 0        
     9 : cmp       i20, i1       
    10 : jmp_ge 2                
    11 : load.i32  i19, i0, i3   
    12 : mul       i18, i19, i19 
    13 : mul       i18, i18, i18 
    14 : mul       i18, i18, i18 
    15 : mul       i18, i19, i18 
    16 : store.i32 i2, i18       
    17 : unspill   i20, 0        
    18 : add       i20, i20, 1   
    19 : spill     0, i20        
    20 : add       i3, i3, 4     
    21 : add       i2, i2, 4     
    22 : jmp       0             
    23 : label 2:                
    24 : mov       i0, 0         
    25 : unspill   i18, 1        
    26 : unspill   i19, 2        
    27 : unspill   i20, 3        
    28 : add       i31, i31, 32  
    29 : ret                     
