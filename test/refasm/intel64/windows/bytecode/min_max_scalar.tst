min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 88  
     1 : spill     5, i8       
     2 : spill     4, i9       
     3 : spill     8, i12      
     4 : spill     9, i13      
     5 : mov       i0, 0       
     6 : mov       s7, 0       
     7 : mov       s6, 0       
     8 : load.i32  i8, i1      
     9 : mov       i9, i8      
    10 : mul       i2, i2, 4   
    11 : label 0:              
    12 : cmp       i0, i2      
    13 : jmp_ge 2              
    14 : load.i32  i12, i1, i0 
    15 : cmp       i12, i8     
    16 : jmp_ge 4              
    17 : mov       i8, i12     
    18 : mov       s7, i0      
    19 : label 4:              
    20 : cmp       i12, i9     
    21 : jmp_le 6              
    22 : mov       i9, i12     
    23 : mov       s6, i0      
    24 : label 6:              
    25 : add       i0, i0, 4   
    26 : jmp       0           
    27 : label 2:              
    28 : mov       i1, 4       
    29 : unspill   i13, 7      
    30 : spill     0, i0       
    31 : mov       i0, i13     
    32 : x86_cqo               
    33 : div       i0, i0, i1  
    34 : mov       i2, i0      
    35 : unspill   i0, 0       
    36 : unspill   i13, 6      
    37 : spill     0, i0       
    38 : spill     1, i2       
    39 : mov       i0, i13     
    40 : x86_cqo               
    41 : div       i0, i0, i1  
    42 : mov       i1, i0      
    43 : unspill   i0, 0       
    44 : unspill   i2, 1       
    45 : unspill   i13, 5      
    46 : store.i32 i13, i2     
    47 : unspill   i13, 4      
    48 : store.i32 i13, i1     
    49 : mov       i0, 0       
    50 : unspill   i12, 8      
    51 : unspill   i13, 9      
    52 : add       i4, i4, 88  
    53 : ret                   
