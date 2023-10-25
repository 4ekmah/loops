exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub       i4, i4, 8   
     1 : spill     0, i12      
     2 : mov       i1, 0       
     3 : mov       i0, 0       
     4 : label 0:              
     5 : cmp       i0, i6      
     6 : jmp_ge 2              
     7 : load.i32  i12, i7, i1 
     8 : store.i32 i2, i12     
     9 : add       i0, i0, 1   
    10 : add       i1, i1, 4   
    11 : add       i2, i2, 4   
    12 : jmp       0           
    13 : label 2:              
    14 : mov       i0, 0       
    15 : unspill   i12, 0      
    16 : add       i4, i4, 8   
    17 : ret                   
