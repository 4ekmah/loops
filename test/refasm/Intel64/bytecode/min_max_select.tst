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
    13 : load.i32  i12, 5, i1, i0  
    14 : cmp       i12, i8         
    15 : unspill   i13, 4          
    16 : mov       i2, i13         
    17 : select    i2, 2, i0, i2   
    18 : mov       s4, i2          
    19 : mov       i2, i8          
    20 : cmp       i2, i12         
    21 : select    i2, 3, i12, i2  
    22 : mov       i8, i2          
    23 : cmp       i12, i9         
    24 : unspill   i13, 3          
    25 : mov       i2, i13         
    26 : select    i2, 3, i0, i2   
    27 : mov       s3, i2          
    28 : cmp       i12, i9         
    29 : select    i12, 2, i9, i12 
    30 : mov       i9, i12         
    31 : add       i0, i0, 4       
    32 : cmp       i0, s5          
    33 : jmp_gt    0               
    34 : mov       i1, 4           
    35 : unspill   i13, 4          
    36 : mov       i1, i13         
    37 : sar       i1, i1, 2       
    38 : unspill   i13, 3          
    39 : mov       i2, i13         
    40 : sar       i2, i2, 2       
    41 : unspill   i13, 2          
    42 : store.i32 5, i13, i1      
    43 : unspill   i13, 1          
    44 : store.i32 5, i13, i2      
    45 : mov       i0, 0           
    46 : unspill   i12, 6          
    47 : unspill   i13, 7          
    48 : add       i4, i4, 72      
    49 : ret                       
