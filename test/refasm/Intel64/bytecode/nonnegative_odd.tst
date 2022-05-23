nonnegative_odd(i0, i1)
     0 : sub      i4, i4, 24    
     1 : spill    2, i12        
     2 : mov      i8, 0         
     3 : mov      i9, -4        
     4 : mul      i2, i2, 4     
     5 : label 0:               
     6 : cmp      i8, i2        
     7 : jmp_ge   3             
     8 : load.i32 i0, 5, i1, i8 
     9 : mov      i12, 0        
    10 : cmp      i0, i12       
    11 : jmp_ge   1             
    12 : add      i8, i8, 4     
    13 : jmp      0             
    14 : label 1:               
    15 : mov      i12, 2        
    16 : spill    0, i0         
    17 : spill    1, i2         
    18 : cqo                    
    19 : div      i0, i0, i12   
    20 : mov      i12, i0       
    21 : unspill  i0, 0         
    22 : unspill  i2, 1         
    23 : mul      i12, i12, 2   
    24 : sub      i0, i0, i12   
    25 : mov      i0, i0        
    26 : mov      i12, 0        
    27 : cmp      i0, i12       
    28 : jmp_eq   2             
    29 : mov      i9, i8        
    30 : jmp      3             
    31 : label 2:               
    32 : add      i8, i8, 4     
    33 : jmp      0             
    34 : label 3:               
    35 : mov      i1, 4         
    36 : spill    0, i0         
    37 : spill    1, i2         
    38 : mov      i0, i9        
    39 : cqo                    
    40 : div      i0, i0, i1    
    41 : mov      i1, i0        
    42 : unspill  i0, 0         
    43 : unspill  i2, 1         
    44 : mov      i0, i1        
    45 : unspill  i12, 2        
    46 : add      i4, i4, 24    
    47 : ret                    
