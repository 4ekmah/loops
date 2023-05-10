min_max_select(i0, i1, i2, i3)
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
    10 : shl       i2, i2, 2        
    11 : label 0:                   
    12 : cmp       i0, i2           
    13 : jmp_ge 2                   
    14 : load.i32  i12, i1, i0      
    15 : cmp       i12, i8          
    16 : unspill   i13, 7           
    17 : select    i13, 31, i0, i13 
    18 : spill     7, i13           
    19 : cmp       i8, i12          
    20 : select    i8, 28, i12, i8  
    21 : cmp       i12, i9          
    22 : unspill   i13, 6           
    23 : select    i13, 28, i0, i13 
    24 : spill     6, i13           
    25 : cmp       i9, i12          
    26 : select    i9, 31, i12, i9  
    27 : add       i0, i0, 4        
    28 : jmp       0                
    29 : label 2:                   
    30 : mov       i1, 4            
    31 : unspill   i13, 7           
    32 : mov       i1, i13          
    33 : sar       i1, i1, 2        
    34 : unspill   i13, 6           
    35 : mov       i2, i13          
    36 : sar       i2, i2, 2        
    37 : unspill   i13, 5           
    38 : store.i32 i13, i1          
    39 : unspill   i13, 4           
    40 : store.i32 i13, i2          
    41 : mov       i0, 0            
    42 : unspill   i12, 8           
    43 : unspill   i13, 9           
    44 : add       i4, i4, 88       
    45 : ret                        
