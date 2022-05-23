min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 72     
     1 : spill     3, i8          
     2 : spill     2, i9          
     3 : spill     6, i12         
     4 : spill     7, i13         
     5 : mov       i0, 0          
     6 : mov       s5, 0          
     7 : mov       s4, 0          
     8 : load.i32  i8, 5, i1      
     9 : mov       i9, i8         
    10 : mul       i2, i2, 4      
    11 : label 0:                 
    12 : load.i32  i12, 5, i1, i0 
    13 : cmp       i12, i8        
    14 : jmp_ge    1              
    15 : mov       i8, i12        
    16 : mov       s5, i0         
    17 : label 1:                 
    18 : cmp       i12, i9        
    19 : jmp_le    2              
    20 : mov       i9, i12        
    21 : mov       s4, i0         
    22 : label 2:                 
    23 : add       i0, i0, 4      
    24 : cmp       i0, i2         
    25 : jmp_gt    0              
    26 : mov       i1, 4          
    27 : unspill   i13, 5         
    28 : spill     0, i0          
    29 : mov       i0, i13        
    30 : cqo                      
    31 : div       i0, i0, i1     
    32 : mov       i2, i0         
    33 : unspill   i0, 0          
    34 : unspill   i13, 4         
    35 : spill     0, i0          
    36 : spill     1, i2          
    37 : mov       i0, i13        
    38 : cqo                      
    39 : div       i0, i0, i1     
    40 : mov       i1, i0         
    41 : unspill   i0, 0          
    42 : unspill   i2, 1          
    43 : unspill   i13, 3         
    44 : store.i32 5, i13, i2     
    45 : unspill   i13, 2         
    46 : store.i32 5, i13, i1     
    47 : mov       i0, 0          
    48 : unspill   i12, 6         
    49 : unspill   i13, 7         
    50 : add       i4, i4, 72     
    51 : ret                      
