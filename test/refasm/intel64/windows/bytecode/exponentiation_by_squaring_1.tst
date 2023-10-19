exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub       i4, i4, 8   
     1 : spill     0, i12      
     2 : mov       i9, 0       
     3 : mov       i0, 0       
     4 : label 0:              
     5 : cmp       i0, i2      
     6 : jmp_ge 2              
     7 : load.i32  i12, i1, i9 
     8 : store.i32 i8, i12     
     9 : add       i0, i0, 1   
    10 : add       i9, i9, 4   
    11 : add       i8, i8, 4   
    12 : jmp       0           
    13 : label 2:              
    14 : mov       i0, 0       
    15 : unspill   i12, 0      
    16 : add       i4, i4, 8   
    17 : ret                   
