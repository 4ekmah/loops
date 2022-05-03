nonnegative_odd(i0, i1)
     0 : sub      i4, i4, 40    
     1 : spill    2, i3         
     2 : spill    3, i6         
     3 : mov      i8, 0         
     4 : mov      i9, 0         
     5 : mov      i0, 2         
     6 : mov      i10, 4        
     7 : mov      i11, i8       
     8 : sub      i11, i11, i10 
     9 : mul      i2, i2, i10   
    10 : label 0:               
    11 : cmp      i8, i2        
    12 : jmp_ge   3             
    13 : load.i32 i3, 5, i1, i8 
    14 : cmp      i3, i9        
    15 : jmp_ge   1             
    16 : add      i8, i8, i10   
    17 : jmp      0             
    18 : label 1:               
    19 : spill    0, i0         
    20 : spill    1, i2         
    21 : mov      i0, i3        
    22 : cqo                    
    23 : div      i0, i0, s0    
    24 : mov      i6, i0        
    25 : unspill  i0, 0         
    26 : unspill  i2, 1         
    27 : mul      i6, i6, i0    
    28 : sub      i3, i3, i6    
    29 : mov      i3, i3        
    30 : cmp      i3, i9        
    31 : jmp_eq   2             
    32 : mov      i11, i8       
    33 : jmp      3             
    34 : label 2:               
    35 : add      i8, i8, i10   
    36 : jmp      0             
    37 : label 3:               
    38 : spill    0, i0         
    39 : spill    1, i2         
    40 : mov      i0, i11       
    41 : cqo                    
    42 : div      i0, i0, i10   
    43 : mov      i11, i0       
    44 : unspill  i0, 0         
    45 : unspill  i2, 1         
    46 : mov      i0, i11       
    47 : unspill  i3, 2         
    48 : unspill  i6, 3         
    49 : add      i4, i4, 40    
    50 : ret                    
