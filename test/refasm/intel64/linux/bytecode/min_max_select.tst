min_max_select(i0, i1, i2, i3)
     0 : sub       i4, i4, 88       
     1 : spill     5, i2            
     2 : spill     4, i1            
     3 : spill     8, i12           
     4 : spill     9, i13           
     5 : mov       i0, 0            
     6 : mov       s7, 0            
     7 : mov       s6, 0            
     8 : load.i32  i2, i7           
     9 : mov       i1, i2           
    10 : shl       i6, i6, 2        
    11 : label 0:                   
    12 : cmp       i0, i6           
    13 : jmp_ge 2                   
    14 : load.i32  i12, i7, i0      
    15 : cmp       i12, i2          
    16 : unspill   i13, 7           
    17 : select    i13, 31, i0, i13 
    18 : spill     7, i13           
    19 : cmp       i2, i12          
    20 : select    i2, 28, i12, i2  
    21 : cmp       i12, i1          
    22 : unspill   i13, 6           
    23 : select    i13, 28, i0, i13 
    24 : spill     6, i13           
    25 : cmp       i1, i12          
    26 : select    i1, 31, i12, i1  
    27 : add       i0, i0, 4        
    28 : jmp       0                
    29 : label 2:                   
    30 : mov       i7, 4            
    31 : unspill   i13, 7           
    32 : mov       i7, i13          
    33 : sar       i7, i7, 2        
    34 : unspill   i13, 6           
    35 : mov       i6, i13          
    36 : sar       i6, i6, 2        
    37 : unspill   i13, 5           
    38 : store.i32 i13, i7          
    39 : unspill   i13, 4           
    40 : store.i32 i13, i6          
    41 : mov       i0, 0            
    42 : unspill   i12, 8           
    43 : unspill   i13, 9           
    44 : add       i4, i4, 88       
    45 : ret                        
