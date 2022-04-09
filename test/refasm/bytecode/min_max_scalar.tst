min_max_scalar(i0, i1, i2, i3)
     0 : sub       i31, i31, 64  
     1 : mov       i4, 0         
     2 : spill     0, i4         
     3 : mov       i4, 0         
     4 : spill     1, i4         
     5 : mov       i4, 0         
     6 : spill     2, i4         
     7 : load.i32  i4, 5, i0     
     8 : spill     3, i4         
     9 : unspill   i4, 3         
    10 : mov       i5, i4        
    11 : spill     4, i5         
    12 : mov       i4, 4         
    13 : spill     5, i4         
    14 : unspill   i4, 5         
    15 : mul       i5, i1, i4    
    16 : spill     6, i5         
    17 : label 0:                
    18 : unspill   i4, 0         
    19 : load.i32  i1, 5, i0, i4 
    20 : unspill   i4, 3         
    21 : cmp       i1, i4        
    22 : jmp_ge    1             
    23 : mov       i4, i1        
    24 : spill     3, i4         
    25 : unspill   i4, 0         
    26 : mov       i5, i4        
    27 : spill     1, i5         
    28 : label 1:                
    29 : unspill   i4, 4         
    30 : cmp       i1, i4        
    31 : jmp_le    2             
    32 : mov       i4, i1        
    33 : spill     4, i4         
    34 : unspill   i4, 0         
    35 : mov       i5, i4        
    36 : spill     2, i5         
    37 : label 2:                
    38 : unspill   i4, 0         
    39 : unspill   i5, 5         
    40 : add       i4, i4, i5    
    41 : spill     0, i4         
    42 : unspill   i4, 0         
    43 : unspill   i5, 6         
    44 : cmp       i4, i5        
    45 : jmp_gt    0             
    46 : unspill   i4, 1         
    47 : unspill   i5, 5         
    48 : div       i0, i4, i5    
    49 : unspill   i4, 2         
    50 : unspill   i5, 5         
    51 : div       i1, i4, i5    
    52 : store.i32 5, i2, i0     
    53 : store.i32 5, i3, i1     
    54 : mov       i0, 0         
    55 : mov       iR, i0        
    56 : add       i31, i31, 64  
    57 : ret                     
