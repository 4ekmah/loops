exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub       i31, i31, 16 
     1 : spill     0, i18       
     2 : spill     1, i19       
     3 : mov       i3, 0        
     4 : mov       i18, 0       
     5 : label 0:               
     6 : cmp       i18, i1      
     7 : jmp_ge 2               
     8 : load.i32  i19, i0, i3  
     9 : store.i32 i2, i19      
    10 : add       i18, i18, 1  
    11 : add       i3, i3, 4    
    12 : add       i2, i2, 4    
    13 : jmp       0            
    14 : label 2:               
    15 : mov       i0, 0        
    16 : unspill   i18, 0       
    17 : unspill   i19, 1       
    18 : add       i31, i31, 16 
    19 : ret                    
