arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 72      
     1 : spill     6, i9           
     2 : spill     7, i12          
     3 : spill     8, i13          
     4 : mov       s4, 0           
     5 : mov       s5, 0           
     6 : label 0:                  
     7 : cmp       s5, i8          
     8 : jmp_ge    1               
     9 : unspill   i13, 4          
    10 : load.i32  i12, 5, i1, i13 
    11 : unspill   i13, 4          
    12 : load.i32  i0, 5, i2, i13  
    13 : mov       i9, i12         
    14 : add       i9, i9, i0      
    15 : unspill   i13, 6          
    16 : store.i32 5, i13, i9      
    17 : mov       i9, i12         
    18 : sub       i9, i9, i0      
    19 : unspill   i13, 14         
    20 : store.i32 5, i13, i9      
    21 : mov       i9, i12         
    22 : mul       i9, i9, i0      
    23 : unspill   i13, 15         
    24 : store.i32 5, i13, i9      
    25 : spill     0, i0           
    26 : spill     1, i2           
    27 : mov       i0, i12         
    28 : x86_cqo                   
    29 : div       i0, i0, s0      
    30 : mov       i9, i0          
    31 : unspill   i0, 0           
    32 : unspill   i2, 1           
    33 : unspill   i13, 16         
    34 : store.i32 5, i13, i9      
    35 : add       s5, s5, 1       
    36 : add       s4, s4, 4       
    37 : add       s6, s6, 4       
    38 : add       s14, s14, 4     
    39 : add       s15, s15, 4     
    40 : add       s16, s16, 4     
    41 : jmp       0               
    42 : label 1:                  
    43 : mov       i0, 0           
    44 : unspill   i12, 7          
    45 : unspill   i13, 8          
    46 : add       i4, i4, 72      
    47 : ret                       
