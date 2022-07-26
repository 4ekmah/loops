min_max_select(i0, i1, i2, i3)
     0 : sub       i4, i4, 72      
     1 : spill     2, i2           
     2 : spill     1, i1           
     3 : spill     6, i12          
     4 : spill     7, i13          
     5 : mov       i0, 0           
     6 : mov       s4, 0           
     7 : mov       s3, 0           
     8 : load.i32  i2, 5, i7       
     9 : mov       i1, i2          
    10 : mov       s5, i6          
    11 : shl       s5, s5, 2       
    12 : label 0:                  
    13 : cmp       i0, s5          
    14 : jmp_ge    1               
    15 : load.i32  i12, 5, i7, i0  
    16 : cmp       i12, i2         
    17 : unspill   i13, 4          
    18 : mov       i6, i13         
    19 : select    i6, 2, i0, i6   
    20 : mov       s4, i6          
    21 : mov       i6, i2          
    22 : cmp       i6, i12         
    23 : select    i6, 3, i12, i6  
    24 : mov       i2, i6          
    25 : cmp       i12, i1         
    26 : unspill   i13, 3          
    27 : mov       i6, i13         
    28 : select    i6, 3, i0, i6   
    29 : mov       s3, i6          
    30 : cmp       i12, i1         
    31 : select    i12, 2, i1, i12 
    32 : mov       i1, i12         
    33 : add       i0, i0, 4       
    34 : jmp       0               
    35 : label 1:                  
    36 : mov       i7, 4           
    37 : unspill   i13, 4          
    38 : mov       i7, i13         
    39 : sar       i7, i7, 2       
    40 : unspill   i13, 3          
    41 : mov       i6, i13         
    42 : sar       i6, i6, 2       
    43 : unspill   i13, 2          
    44 : store.i32 5, i13, i7      
    45 : unspill   i13, 1          
    46 : store.i32 5, i13, i6      
    47 : mov       i0, 0           
    48 : unspill   i12, 6          
    49 : unspill   i13, 7          
    50 : add       i4, i4, 72      
    51 : ret                       
