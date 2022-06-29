min_max_select(i0, i1, i2, i3)
     0 : sub       i4, i4, 72      
     1 : spill     2, i8           
     2 : spill     1, i9           
     3 : spill     6, i12          
     4 : spill     7, i13          
     5 : mov       i0, 0           
     6 : mov       s4, 0           
     7 : mov       s3, 0           
     8 : load.i32  i8, 5, i1       
     9 : mov       i9, i8          
    10 : mov       s5, i2          
    11 : shl       s5, s5, 2       
    12 : label 0:                  
    13 : cmp       i0, s5          
    14 : jmp_ge    1               
    15 : load.i32  i12, 5, i1, i0  
    16 : cmp       i12, i8         
    17 : unspill   i13, 4          
    18 : mov       i2, i13         
    19 : select    i2, 2, i0, i2   
    20 : mov       s4, i2          
    21 : mov       i2, i8          
    22 : cmp       i2, i12         
    23 : select    i2, 3, i12, i2  
    24 : mov       i8, i2          
    25 : cmp       i12, i9         
    26 : unspill   i13, 3          
    27 : mov       i2, i13         
    28 : select    i2, 3, i0, i2   
    29 : mov       s3, i2          
    30 : cmp       i12, i9         
    31 : select    i12, 2, i9, i12 
    32 : mov       i9, i12         
    33 : add       i0, i0, 4       
    34 : jmp       0               
    35 : label 1:                  
    36 : mov       i1, 4           
    37 : unspill   i13, 4          
    38 : mov       i1, i13         
    39 : sar       i1, i1, 2       
    40 : unspill   i13, 3          
    41 : mov       i2, i13         
    42 : sar       i2, i2, 2       
    43 : unspill   i13, 2          
    44 : store.i32 5, i13, i1      
    45 : unspill   i13, 1          
    46 : store.i32 5, i13, i2      
    47 : mov       i0, 0           
    48 : unspill   i12, 6          
    49 : unspill   i13, 7          
    50 : add       i4, i4, 72      
    51 : ret                       
