min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 72     
     1 : spill     7, i12         
     2 : spill     8, i13         
     3 : mov       i0, 0          
     4 : mov       s3, 0          
     5 : mov       s2, 0          
     6 : load.i32  i13, 5, i1     
     7 : spill     6, i13         
     8 : unspill   i13, 6         
     9 : mov       s4, i13        
    10 : mov       s5, 4          
    11 : mul       i2, i2, s5     
    12 : label 0:                 
    13 : load.i32  i12, 5, i1, i0 
    14 : cmp       i12, s6        
    15 : jmp_ge    1              
    16 : mov       s6, i12        
    17 : mov       s3, i0         
    18 : label 1:                 
    19 : cmp       i12, s4        
    20 : jmp_le    2              
    21 : mov       s4, i12        
    22 : mov       s2, i0         
    23 : label 2:                 
    24 : add       i0, i0, s5     
    25 : cmp       i0, i2         
    26 : jmp_gt    0              
    27 : unspill   i13, 3         
    28 : spill     0, i0          
    29 : mov       i0, i13        
    30 : cqo                      
    31 : div       i0, i0, s5     
    32 : mov       i2, i0         
    33 : unspill   i0, 0          
    34 : unspill   i13, 2         
    35 : spill     0, i0          
    36 : spill     1, i2          
    37 : mov       i0, i13        
    38 : cqo                      
    39 : div       i0, i0, s5     
    40 : mov       i1, i0         
    41 : unspill   i0, 0          
    42 : unspill   i2, 1          
    43 : store.i32 5, i8, i2      
    44 : store.i32 5, i9, i1      
    45 : mov       i9, 0          
    46 : mov       i0, i9         
    47 : unspill   i12, 7         
    48 : unspill   i13, 8         
    49 : add       i4, i4, 72     
    50 : ret                      
