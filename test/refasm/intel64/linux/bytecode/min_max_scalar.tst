min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 88  
     1 : spill     5, i2       
     2 : spill     4, i1       
     3 : spill     8, i12      
     4 : spill     9, i13      
     5 : mov       i0, 0       
     6 : mov       s7, 0       
     7 : mov       s6, 0       
     8 : load.i32  i2, i7      
     9 : mov       i1, i2      
    10 : mul       i6, i6, 4   
    11 : label 0:              
    12 : cmp       i0, i6      
    13 : jmp_ge 2              
    14 : load.i32  i12, i7, i0 
    15 : cmp       i12, i2     
    16 : jmp_ge 4              
    17 : mov       i2, i12     
    18 : mov       s7, i0      
    19 : label 4:              
    20 : cmp       i12, i1     
    21 : jmp_le 6              
    22 : mov       i1, i12     
    23 : mov       s6, i0      
    24 : label 6:              
    25 : add       i0, i0, 4   
    26 : jmp       0           
    27 : label 2:              
    28 : mov       i7, 4       
    29 : unspill   i13, 7      
    30 : spill     0, i0       
    31 : spill     1, i2       
    32 : mov       i0, i13     
    33 : x86_cqo               
    34 : div       i0, i0, i7  
    35 : mov       i6, i0      
    36 : unspill   i0, 0       
    37 : unspill   i2, 1       
    38 : unspill   i13, 6      
    39 : spill     0, i0       
    40 : spill     1, i2       
    41 : mov       i0, i13     
    42 : x86_cqo               
    43 : div       i0, i0, i7  
    44 : mov       i7, i0      
    45 : unspill   i0, 0       
    46 : unspill   i2, 1       
    47 : unspill   i13, 5      
    48 : store.i32 i13, i6     
    49 : unspill   i13, 4      
    50 : store.i32 i13, i7     
    51 : mov       i0, 0       
    52 : unspill   i12, 8      
    53 : unspill   i13, 9      
    54 : add       i4, i4, 88  
    55 : ret                   
