nonnegative_odd(i0, i1)
     0 : sub      i31, i31, 48  
     1 : mov      i4, 0         
     2 : spill    4, i4         
     3 : mov      i4, 0         
     4 : spill    3, i4         
     5 : mov      i4, 2         
     6 : spill    0, i4         
     7 : mov      i4, 4         
     8 : spill    1, i4         
     9 : unspill  i4, 4         
    10 : unspill  i5, 1         
    11 : sub      i6, i4, i5    
    12 : spill    2, i6         
    13 : unspill  i4, 1         
    14 : mul      i1, i1, i4    
    15 : unspill  i4, 4         
    16 : label 0:               
    17 : cmp      i4, i1        
    18 : jmp_ge   3             
    19 : unspill  i4, 4         
    20 : load.i32 i3, 5, i0, i4 
    21 : unspill  i4, 3         
    22 : cmp      i3, i4        
    23 : jmp_ge   1             
    24 : unspill  i4, 4         
    25 : unspill  i5, 1         
    26 : add      i4, i4, i5    
    27 : spill    4, i4         
    28 : jmp      0             
    29 : label 1:               
    30 : unspill  i4, 0         
    31 : div      i2, i3, i4    
    32 : unspill  i4, 0         
    33 : mul      i2, i2, i4    
    34 : sub      i2, i3, i2    
    35 : mov      i2, i2        
    36 : unspill  i4, 3         
    37 : cmp      i2, i4        
    38 : jmp_eq   2             
    39 : unspill  i4, 4         
    40 : mov      i5, i4        
    41 : spill    2, i5         
    42 : jmp      3             
    43 : label 2:               
    44 : unspill  i4, 4         
    45 : unspill  i5, 1         
    46 : add      i4, i4, i5    
    47 : spill    4, i4         
    48 : jmp      0             
    49 : label 3:               
    50 : unspill  i4, 1         
    51 : unspill  i5, 2         
    52 : div      i0, i5, i4    
    53 : mov      iR, i0        
    54 : add      i31, i31, 48  
    55 : ret                    
