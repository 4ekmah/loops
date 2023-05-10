exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub       i4, i4, 8   
     1 : spill     0, i12      
     2 : mov       i9, 1       
     3 : mov       i9, 0       
     4 : mov       i0, 0       
     5 : label 0:              
     6 : cmp       i0, i2      
     7 : jmp_ge 2              
     8 : load.i32  i12, i1, i9 
     9 : store.i32 i8, i12     
    10 : add       i0, i0, 1   
    11 : add       i9, i9, 4   
    12 : add       i8, i8, 4   
    13 : jmp       0           
    14 : label 2:              
    15 : mov       i0, 0       
    16 : unspill   i12, 0      
    17 : add       i4, i4, 8   
    18 : ret                   
