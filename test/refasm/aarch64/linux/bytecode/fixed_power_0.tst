exponentiation_by_squaring_0(i0, i1, i2)
     0 : sub       i31, i31, 16 
     1 : spill     0, i18       
     2 : spill     1, i19       
     3 : mov       i3, 0        
     4 : mov       i18, 0       
     5 : label 0:               
     6 : cmp       i18, i1      
     7 : jmp_ge 2               
     8 : load.i32  i19, i0, i3  
     9 : mov       i19, 1       
    10 : store.i32 i2, i19      
    11 : add       i18, i18, 1  
    12 : add       i3, i3, 4    
    13 : add       i2, i2, 4    
    14 : jmp       0            
    15 : label 2:               
    16 : mov       i0, 0        
    17 : unspill   i18, 0       
    18 : unspill   i19, 1       
    19 : add       i31, i31, 16 
    20 : ret                    
