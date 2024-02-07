arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 72  
     1 : spill     7, i8       
     2 : spill     6, i9       
     3 : spill     8, i13      
     4 : mov       s4, 0       
     5 : mov       s5, 0       
     6 : label 0:              
     7 : unspill   i13, 7      
     8 : cmp       s5, i13     
     9 : jmp_ge 2              
    10 : unspill   i13, 4      
    11 : load.i32  i0, i1, i13 
    12 : unspill   i13, 4      
    13 : load.i32  i9, i2, i13 
    14 : mov       i8, i0      
    15 : add       i8, i8, i9  
    16 : unspill   i13, 6      
    17 : store.i32 i13, i8     
    18 : mov       i8, i0      
    19 : sub       i8, i8, i9  
    20 : unspill   i13, 14     
    21 : store.i32 i13, i8     
    22 : mov       i8, i0      
    23 : mul       i8, i8, i9  
    24 : unspill   i13, 15     
    25 : store.i32 i13, i8     
    26 : spill     0, i0       
    27 : spill     1, i2       
    28 : x86_cqo               
    29 : div       i0, i0, i9  
    30 : mov       i8, i0      
    31 : unspill   i0, 0       
    32 : unspill   i2, 1       
    33 : unspill   i13, 16     
    34 : store.i32 i13, i8     
    35 : add       s5, s5, 1   
    36 : add       s4, s4, 4   
    37 : add       s6, s6, 4   
    38 : add       s14, s14, 4 
    39 : add       s15, s15, 4 
    40 : add       s16, s16, 4 
    41 : jmp       0           
    42 : label 2:              
    43 : mov       i0, 0       
    44 : unspill   i13, 8      
    45 : add       i4, i4, 72  
    46 : ret                   
