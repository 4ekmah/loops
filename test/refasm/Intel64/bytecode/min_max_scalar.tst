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
    12 : cmp       i0, i2         
    13 : jmp_ge    3              
    14 : load.i32  i12, 5, i1, i0 
    15 : cmp       i12, i8        
    16 : jmp_ge    1              
    17 : mov       i8, i12        
    18 : mov       s5, i0         
    19 : label 1:                 
    20 : cmp       i12, i9        
    21 : jmp_le    2              
    22 : mov       i9, i12        
    23 : mov       s4, i0         
    24 : label 2:                 
    25 : add       i0, i0, 4      
    26 : jmp       0              
    27 : label 3:                 
    28 : mov       i1, 4          
    29 : unspill   i13, 5         
    30 : spill     0, i0          
    31 : mov       i0, i13        
    32 : x86_cqo                  
    33 : div       i0, i0, i1     
    34 : mov       i2, i0         
    35 : unspill   i0, 0          
    36 : unspill   i13, 4         
    37 : spill     0, i0          
    38 : spill     1, i2          
    39 : mov       i0, i13        
    40 : x86_cqo                  
    41 : div       i0, i0, i1     
    42 : mov       i1, i0         
    43 : unspill   i0, 0          
    44 : unspill   i2, 1          
    45 : unspill   i13, 3         
    46 : store.i32 5, i13, i2     
    47 : unspill   i13, 2         
    48 : store.i32 5, i13, i1     
    49 : mov       i0, 0          
    50 : unspill   i12, 6         
    51 : unspill   i13, 7         
    52 : add       i4, i4, 72     
    53 : ret                      
