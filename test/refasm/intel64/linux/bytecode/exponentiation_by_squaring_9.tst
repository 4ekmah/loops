exponentiation_by_squaring_9(i0, i1, i2)
     0 : sub       i4, i4, 24   
     1 : spill     1, i12       
     2 : mov       i1, 1        
     3 : mov       i1, 0        
     4 : mov       s0, 0        
     5 : label 0:               
     6 : cmp       s0, i6       
     7 : jmp_ge    1            
     8 : load.i32  i12, i7, i1  
     9 : mov       i0, i12      
    10 : mul       i0, i0, i12  
    11 : mul       i0, i0, i0   
    12 : mul       i0, i0, i0   
    13 : mul       i12, i12, i0 
    14 : store.i32 i2, i12      
    15 : add       s0, s0, 1    
    16 : add       i1, i1, 4    
    17 : add       i2, i2, 4    
    18 : jmp       0            
    19 : label 1:               
    20 : mov       i0, 0        
    21 : unspill   i12, 1       
    22 : add       i4, i4, 24   
    23 : ret                    
