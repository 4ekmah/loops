min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 72     
     1 : spill     3, i2          
     2 : spill     2, i1          
     3 : spill     6, i12         
     4 : spill     7, i13         
     5 : mov       i0, 0          
     6 : mov       s5, 0          
     7 : mov       s4, 0          
     8 : load.i32  i2, 5, i7      
     9 : mov       i1, i2         
    10 : mul       i6, i6, 4      
    11 : label 0:                 
    12 : cmp       i0, i6         
    13 : jmp_ge    3              
    14 : load.i32  i12, 5, i7, i0 
    15 : cmp       i12, i2        
    16 : jmp_ge    1              
    17 : mov       i2, i12        
    18 : mov       s5, i0         
    19 : label 1:                 
    20 : cmp       i12, i1        
    21 : jmp_le    2              
    22 : mov       i1, i12        
    23 : mov       s4, i0         
    24 : label 2:                 
    25 : add       i0, i0, 4      
    26 : jmp       0              
    27 : label 3:                 
    28 : mov       i7, 4          
    29 : unspill   i13, 5         
    30 : spill     0, i0          
    31 : spill     1, i2          
    32 : mov       i0, i13        
    33 : x86_cqo                  
    34 : div       i0, i0, i7     
    35 : mov       i6, i0         
    36 : unspill   i0, 0          
    37 : unspill   i2, 1          
    38 : unspill   i13, 4         
    39 : spill     0, i0          
    40 : spill     1, i2          
    41 : mov       i0, i13        
    42 : x86_cqo                  
    43 : div       i0, i0, i7     
    44 : mov       i7, i0         
    45 : unspill   i0, 0          
    46 : unspill   i2, 1          
    47 : unspill   i13, 3         
    48 : store.i32 5, i13, i6     
    49 : unspill   i13, 2         
    50 : store.i32 5, i13, i7     
    51 : mov       i0, 0          
    52 : unspill   i12, 6         
    53 : unspill   i13, 7         
    54 : add       i4, i4, 72     
    55 : ret                      
