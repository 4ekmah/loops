arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 72  
     1 : spill     7, i8       
     2 : spill     6, i9       
     3 : spill     8, i13      
     4 : mov       s4, 0       
     5 : mov       s5, 0       
     6 : label 0:              
     7 : cmp       s5, i2      
     8 : jmp_ge 2              
     9 : unspill   i13, 4      
    10 : load.i32  i0, i7, i13 
    11 : unspill   i13, 4      
    12 : load.i32  i9, i6, i13 
    13 : mov       i8, i0      
    14 : add       i8, i8, i9  
    15 : store.i32 i1, i8      
    16 : mov       i8, i0      
    17 : sub       i8, i8, i9  
    18 : unspill   i13, 7      
    19 : store.i32 i13, i8     
    20 : mov       i8, i0      
    21 : mul       i8, i8, i9  
    22 : unspill   i13, 6      
    23 : store.i32 i13, i8     
    24 : spill     0, i0       
    25 : spill     1, i2       
    26 : x86_cqo               
    27 : div       i0, i0, i9  
    28 : mov       i8, i0      
    29 : unspill   i0, 0       
    30 : unspill   i2, 1       
    31 : unspill   i13, 10     
    32 : store.i32 i13, i8     
    33 : add       s5, s5, 1   
    34 : add       s4, s4, 4   
    35 : add       i1, i1, 4   
    36 : add       s7, s7, 4   
    37 : add       s6, s6, 4   
    38 : add       s10, s10, 4 
    39 : jmp       0           
    40 : label 2:              
    41 : mov       i0, 0       
    42 : unspill   i13, 8      
    43 : add       i4, i4, 72  
    44 : ret                   
