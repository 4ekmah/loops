min_max_select(i0, i1, i2, i3)
     0 : sub       i4, i4, 88      
     1 : spill     5, i2           
     2 : spill     4, i1           
     3 : spill     9, i12          
     4 : spill     10, i13         
     5 : mov       i0, 0           
     6 : mov       s7, 0           
     7 : mov       s6, 0           
     8 : load.i32  i2, 5, i7       
     9 : mov       i1, i2          
    10 : mov       s8, i6          
    11 : shl       s8, s8, 2       
    12 : label 0:                  
    13 : cmp       i0, s8          
    14 : jmp_ge    1               
    15 : load.i32  i12, 5, i7, i0  
    16 : cmp       i12, i2         
    17 : unspill   i13, 7          
    18 : mov       i6, i13         
    19 : select    i6, 2, i0, i6   
    20 : mov       s7, i6          
    21 : mov       i6, i2          
    22 : cmp       i6, i12         
    23 : select    i6, 3, i12, i6  
    24 : mov       i2, i6          
    25 : cmp       i12, i1         
    26 : unspill   i13, 6          
    27 : mov       i6, i13         
    28 : select    i6, 3, i0, i6   
    29 : mov       s6, i6          
    30 : cmp       i12, i1         
    31 : select    i12, 2, i1, i12 
    32 : mov       i1, i12         
    33 : add       i0, i0, 4       
    34 : jmp       0               
    35 : label 1:                  
    36 : mov       i7, 4           
    37 : unspill   i13, 7          
    38 : mov       i7, i13         
    39 : sar       i7, i7, 2       
    40 : unspill   i13, 6          
    41 : mov       i6, i13         
    42 : sar       i6, i6, 2       
    43 : unspill   i13, 5          
    44 : store.i32 5, i13, i7      
    45 : unspill   i13, 4          
    46 : store.i32 5, i13, i6      
    47 : mov       i0, 0           
    48 : unspill   i12, 9          
    49 : unspill   i13, 10         
    50 : add       i4, i4, 88      
    51 : ret                       
