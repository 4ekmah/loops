arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : sub       i4, i4, 40      
     1 : spill     2, i9           
     2 : spill     3, i12          
     3 : spill     4, i13          
     4 : mov       s0, 0           
     5 : mov       s1, 0           
     6 : label 0:                  
     7 : unspill   i13, 0          
     8 : load.i32  i12, 5, i1, i13 
     9 : unspill   i13, 0          
    10 : load.i32  i0, 5, i2, i13  
    11 : mov       i9, i12         
    12 : add       i9, i9, i0      
    13 : unspill   i13, 2          
    14 : store.i32 5, i13, i9      
    15 : mov       i9, i12         
    16 : sub       i9, i9, i0      
    17 : unspill   i13, 10         
    18 : store.i32 5, i13, i9      
    19 : mul       i12, i12, i0    
    20 : unspill   i13, 11         
    21 : store.i32 5, i13, i12     
    22 : add       s1, s1, 1       
    23 : add       s0, s0, 4       
    24 : add       s2, s2, 4       
    25 : add       s10, s10, 4     
    26 : add       s11, s11, 4     
    27 : cmp       s1, i8          
    28 : jmp_gt    0               
    29 : mov       i0, 0           
    30 : unspill   i12, 3          
    31 : unspill   i13, 4          
    32 : add       i4, i4, 40      
    33 : ret                       
