min_max_scalar(i0, i1, i2, i3)
     0 : sub       i4, i4, 56    
     1 : spill     2, i3         
     2 : spill     3, i5         
     3 : spill     4, i6         
     4 : spill     5, i7         
     5 : mov       i0, 0         
     6 : mov       i10, 0        
     7 : mov       i11, 0        
     8 : load.i32  i3, 5, i1     
     9 : mov       i6, i3        
    10 : mov       i7, 4         
    11 : mul       i2, i2, i7    
    12 : label 0:                
    13 : load.i32  i5, 5, i1, i0 
    14 : cmp       i5, i3        
    15 : jmp_ge    1             
    16 : mov       i3, i5        
    17 : mov       i10, i0       
    18 : label 1:                
    19 : cmp       i5, i6        
    20 : jmp_le    2             
    21 : mov       i6, i5        
    22 : mov       i11, i0       
    23 : label 2:                
    24 : add       i0, i0, i7    
    25 : cmp       i0, i2        
    26 : jmp_gt    0             
    27 : spill     0, i0         
    28 : spill     1, i2         
    29 : mov       i0, i10       
    30 : cqo                     
    31 : div       i0, i0, i7    
    32 : mov       i10, i0       
    33 : unspill   i0, 0         
    34 : unspill   i2, 1         
    35 : spill     0, i0         
    36 : spill     1, i2         
    37 : mov       i0, i11       
    38 : cqo                     
    39 : div       i0, i0, i7    
    40 : mov       i11, i0       
    41 : unspill   i0, 0         
    42 : unspill   i2, 1         
    43 : store.i32 5, i8, i10    
    44 : store.i32 5, i9, i11    
    45 : mov       i9, 0         
    46 : mov       i0, i9        
    47 : unspill   i3, 2         
    48 : unspill   i5, 3         
    49 : unspill   i6, 4         
    50 : unspill   i7, 5         
    51 : add       i4, i4, 56    
    52 : ret                     
