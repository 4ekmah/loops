min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 88  
     1 : spill     5, i8       
     2 : spill     4, i9       
     3 : spill     9, i13      
     4 : mov       i0, 0       
     5 : mov       s7, 0       
     6 : mov       s6, 0       
     7 : load.i32  i8, i1      
     8 : mov       i9, i8      
     9 : mov       i13, i2     
    10 : mul       i13, i13, 4 
    11 : spill     8, i13      
    12 : label 0:              
    13 : cmp       i0, s8      
    14 : jmp_ge 2              
    15 : load.i32  i2, i1, i0  
    16 : cmp       i2, i8      
    17 : jmp_ge 4              
    18 : mov       i8, i2      
    19 : mov       s7, i0      
    20 : label 4:              
    21 : cmp       i2, i9      
    22 : jmp_le 6              
    23 : mov       i9, i2      
    24 : mov       s6, i0      
    25 : label 6:              
    26 : add       i0, i0, 4   
    27 : jmp       0           
    28 : label 2:              
    29 : mov       i1, 4       
    30 : unspill   i13, 7      
    31 : spill     0, i0       
    32 : mov       i0, i13     
    33 : x86_cqo               
    34 : div       i0, i0, i1  
    35 : mov       i2, i0      
    36 : unspill   i0, 0       
    37 : unspill   i13, 6      
    38 : spill     0, i0       
    39 : spill     1, i2       
    40 : mov       i0, i13     
    41 : x86_cqo               
    42 : div       i0, i0, i1  
    43 : mov       i1, i0      
    44 : unspill   i0, 0       
    45 : unspill   i2, 1       
    46 : unspill   i13, 5      
    47 : store.i32 i13, i2     
    48 : unspill   i13, 4      
    49 : store.i32 i13, i1     
    50 : mov       i0, 0       
    51 : unspill   i13, 9      
    52 : add       i4, i4, 88  
    53 : ret                   
