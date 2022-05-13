min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 72     
     1 : spill     3, i8          
     2 : spill     2, i9          
     3 : spill     7, i12         
     4 : spill     8, i13         
     5 : mov       i0, 0          
     6 : mov       s6, 0          
     7 : mov       s4, 0          
     8 : load.i32  i8, 5, i1      
     9 : mov       i9, i8         
    10 : mov       s5, 4          
    11 : mul       i2, i2, s5     
    12 : label 0:                 
    13 : load.i32  i12, 5, i1, i0 
    14 : cmp       i12, i8        
    15 : jmp_ge    1              
    16 : mov       i8, i12        
    17 : mov       s6, i0         
    18 : label 1:                 
    19 : cmp       i12, i9        
    20 : jmp_le    2              
    21 : mov       i9, i12        
    22 : mov       s4, i0         
    23 : label 2:                 
    24 : add       i0, i0, s5     
    25 : cmp       i0, i2         
    26 : jmp_gt    0              
    27 : unspill   i13, 6         
    28 : spill     0, i0          
    29 : spill     1, i2          
    30 : mov       i0, i13        
    31 : cqo                      
    32 : div       i0, i0, s5     
    33 : mov       i1, i0         
    34 : unspill   i0, 0          
    35 : unspill   i2, 1          
    36 : unspill   i13, 4         
    37 : spill     0, i0          
    38 : mov       i0, i13        
    39 : cqo                      
    40 : div       i0, i0, s5     
    41 : mov       i2, i0         
    42 : unspill   i0, 0          
    43 : unspill   i13, 3         
    44 : store.i32 5, i13, i1     
    45 : unspill   i13, 2         
    46 : store.i32 5, i13, i2     
    47 : mov       i1, 0          
    48 : mov       i0, i1         
    49 : unspill   i12, 7         
    50 : unspill   i13, 8         
    51 : add       i4, i4, 72     
    52 : ret                      
