arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 72      
     1 : spill     6, i9           
     2 : spill     7, i12          
     3 : spill     8, i13          
     4 : mov       s5, 0           
     5 : mov       s4, 0           
     6 : label 0:                  
     7 : cmp       s4, i2          
     8 : jmp_ge    1               
     9 : unspill   i13, 5          
    10 : load.i32  i12, 5, i7, i13 
    11 : unspill   i13, 5          
    12 : load.i32  i0, 5, i6, i13  
    13 : mov       i9, i12         
    14 : add       i9, i9, i0      
    15 : store.i32 5, i1, i9       
    16 : mov       i9, i12         
    17 : sub       i9, i9, i0      
    18 : store.i32 5, i8, i9       
    19 : mov       i9, i12         
    20 : mul       i9, i9, i0      
    21 : unspill   i13, 6          
    22 : store.i32 5, i13, i9      
    23 : spill     0, i0           
    24 : spill     1, i2           
    25 : mov       i0, i12         
    26 : x86_cqo                   
    27 : div       i0, i0, s0      
    28 : mov       i9, i0          
    29 : unspill   i0, 0           
    30 : unspill   i2, 1           
    31 : unspill   i13, 10         
    32 : store.i32 5, i13, i9      
    33 : add       s4, s4, 1       
    34 : add       s5, s5, 4       
    35 : add       i1, i1, 4       
    36 : add       i8, i8, 4       
    37 : add       s6, s6, 4       
    38 : add       s10, s10, 4     
    39 : jmp       0               
    40 : label 1:                  
    41 : mov       i0, 0           
    42 : unspill   i12, 7          
    43 : unspill   i13, 8          
    44 : add       i4, i4, 72      
    45 : ret                       
