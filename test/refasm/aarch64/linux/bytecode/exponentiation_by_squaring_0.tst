exponentiation_by_squaring_0(i0, i1, i2)
     0 : sub       i31, i31, 16 
     1 : spill     0, i18       
     2 : spill     1, i19       
     3 : mov       i3, 1        
     4 : mov       i3, 0        
     5 : mov       i18, 0       
     6 : label 0:               
     7 : cmp       i18, i1      
     8 : jmp_ge 2               
     9 : load.i32  i19, i0, i3  
    10 : mov       i19, 1       
    11 : store.i32 i2, i19      
    12 : add       i18, i18, 1  
    13 : add       i3, i3, 4    
    14 : add       i2, i2, 4    
    15 : jmp       0            
    16 : label 2:               
    17 : mov       i0, 0        
    18 : unspill   i18, 0       
    19 : unspill   i19, 1       
    20 : add       i31, i31, 16 
    21 : ret                    
