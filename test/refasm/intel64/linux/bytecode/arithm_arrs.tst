arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 56      
     1 : spill     4, i9           
     2 : spill     5, i12          
     3 : spill     6, i13          
     4 : mov       s3, 0           
     5 : mov       s2, 0           
     6 : label 0:                  
     7 : cmp       s2, i2          
     8 : jmp_ge    1               
     9 : unspill   i13, 3          
    10 : load.i32  i12, 5, i7, i13 
    11 : unspill   i13, 3          
    12 : load.i32  i0, 5, i6, i13  
    13 : mov       i9, i12         
    14 : add       i9, i9, i0      
    15 : store.i32 5, i1, i9       
    16 : mov       i9, i12         
    17 : sub       i9, i9, i0      
    18 : store.i32 5, i8, i9       
    19 : mov       i9, i12         
    20 : mul       i9, i9, i0      
    21 : unspill   i13, 4          
    22 : store.i32 5, i13, i9      
    23 : spill     0, i0           
    24 : spill     1, i2           
    25 : mov       i0, i12         
    26 : x86_cqo                   
    27 : div       i0, i0, s0      
    28 : mov       i9, i0          
    29 : unspill   i0, 0           
    30 : unspill   i2, 1           
    31 : unspill   i13, 8          
    32 : store.i32 5, i13, i9      
    33 : add       s2, s2, 1       
    34 : add       s3, s3, 4       
    35 : add       i1, i1, 4       
    36 : add       i8, i8, 4       
    37 : add       s4, s4, 4       
    38 : add       s8, s8, 4       
    39 : jmp       0               
    40 : label 1:                  
    41 : mov       i0, 0           
    42 : unspill   i12, 5          
    43 : unspill   i13, 6          
    44 : add       i4, i4, 56      
    45 : ret                       
