exponentiation_by_squaring_9(i0, i1, i2)
     0 : sub       i4, i4, 24   
     1 : spill     1, i12       
     2 : mov       i9, 0        
     3 : mov       s0, 0        
     4 : label 0:               
     5 : cmp       s0, i2       
     6 : jmp_ge 2               
     7 : load.i32  i12, i1, i9  
     8 : mov       i0, i12      
     9 : mul       i0, i0, i12  
    10 : mul       i0, i0, i0   
    11 : mul       i0, i0, i0   
    12 : mul       i12, i12, i0 
    13 : store.i32 i8, i12      
    14 : add       s0, s0, 1    
    15 : add       i9, i9, 4    
    16 : add       i8, i8, 4    
    17 : jmp       0            
    18 : label 2:               
    19 : mov       i0, 0        
    20 : unspill   i12, 1       
    21 : add       i4, i4, 24   
    22 : ret                    
