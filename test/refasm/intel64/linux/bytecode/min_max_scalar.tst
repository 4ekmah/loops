min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 88  
     1 : spill     5, i2       
     2 : spill     4, i1       
     3 : spill     9, i13      
     4 : mov       i0, 0       
     5 : mov       s7, 0       
     6 : mov       s6, 0       
     7 : load.i32  i2, i7      
     8 : mov       i1, i2      
     9 : mov       i13, i6     
    10 : mul       i13, i13, 4 
    11 : spill     8, i13      
    12 : label 0:              
    13 : cmp       i0, s8      
    14 : jmp_ge 2              
    15 : load.i32  i6, i7, i0  
    16 : cmp       i6, i2      
    17 : jmp_ge 4              
    18 : mov       i2, i6      
    19 : mov       s7, i0      
    20 : label 4:              
    21 : cmp       i6, i1      
    22 : jmp_le 6              
    23 : mov       i1, i6      
    24 : mov       s6, i0      
    25 : label 6:              
    26 : add       i0, i0, 4   
    27 : jmp       0           
    28 : label 2:              
    29 : mov       i7, 4       
    30 : unspill   i13, 7      
    31 : spill     0, i0       
    32 : spill     1, i2       
    33 : mov       i0, i13     
    34 : x86_cqo               
    35 : div       i0, i0, i7  
    36 : mov       i6, i0      
    37 : unspill   i0, 0       
    38 : unspill   i2, 1       
    39 : unspill   i13, 6      
    40 : spill     0, i0       
    41 : spill     1, i2       
    42 : mov       i0, i13     
    43 : x86_cqo               
    44 : div       i0, i0, i7  
    45 : mov       i7, i0      
    46 : unspill   i0, 0       
    47 : unspill   i2, 1       
    48 : unspill   i13, 5      
    49 : store.i32 i13, i6     
    50 : unspill   i13, 4      
    51 : store.i32 i13, i7     
    52 : mov       i0, 0       
    53 : unspill   i13, 9      
    54 : add       i4, i4, 88  
    55 : ret                   
