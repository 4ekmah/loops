exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub       i4, i4, 8      
     1 : spill     0, i12         
     2 : mov       i1, 1          
     3 : mov       i1, 0          
     4 : mov       i0, 0          
     5 : label 0:                 
     6 : cmp       i0, i6         
     7 : jmp_ge    1              
     8 : load.i32  i12, 5, i7, i1 
     9 : store.i32 5, i2, i12     
    10 : add       i0, i0, 1      
    11 : add       i1, i1, 4      
    12 : add       i2, i2, 4      
    13 : jmp       0              
    14 : label 1:                 
    15 : mov       i0, 0          
    16 : unspill   i12, 0         
    17 : add       i4, i4, 8      
    18 : ret                      
