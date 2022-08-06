exponentiation_by_squaring_0(i0, i1, i2)
     0 : sub       i4, i4, 8      
     1 : spill     0, i12         
     2 : mov       i9, 1          
     3 : mov       i9, 0          
     4 : mov       i0, 0          
     5 : label 0:                 
     6 : cmp       i0, i2         
     7 : jmp_ge    1              
     8 : load.i32  i12, 5, i1, i9 
     9 : mov       i12, 1         
    10 : store.i32 5, i8, i12     
    11 : add       i0, i0, 1      
    12 : add       i9, i9, 4      
    13 : add       i8, i8, 4      
    14 : jmp       0              
    15 : label 1:                 
    16 : mov       i0, 0          
    17 : unspill   i12, 0         
    18 : add       i4, i4, 8      
    19 : ret                      
