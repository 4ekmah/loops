arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : sub       i4, i4, 40      
     1 : spill     2, i9           
     2 : spill     3, i12          
     3 : spill     4, i13          
     4 : mov       s0, 0           
     5 : mov       s1, 0           
     6 : label 0:                  
     7 : cmp       s1, i8          
     8 : jmp_ge    1               
     9 : unspill   i13, 0          
    10 : load.i32  i12, 5, i1, i13 
    11 : unspill   i13, 0          
    12 : load.i32  i0, 5, i2, i13  
    13 : mov       i9, i12         
    14 : add       i9, i9, i0      
    15 : unspill   i13, 2          
    16 : store.i32 5, i13, i9      
    17 : mov       i9, i12         
    18 : sub       i9, i9, i0      
    19 : unspill   i13, 10         
    20 : store.i32 5, i13, i9      
    21 : mul       i12, i12, i0    
    22 : unspill   i13, 11         
    23 : store.i32 5, i13, i12     
    24 : add       s1, s1, 1       
    25 : add       s0, s0, 4       
    26 : add       s2, s2, 4       
    27 : add       s10, s10, 4     
    28 : add       s11, s11, 4     
    29 : jmp       0               
    30 : label 1:                  
    31 : mov       i0, 0           
    32 : unspill   i12, 3          
    33 : unspill   i13, 4          
    34 : add       i4, i4, 40      
    35 : ret                       
