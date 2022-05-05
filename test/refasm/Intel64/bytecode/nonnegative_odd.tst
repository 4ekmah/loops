nonnegative_odd(i0, i1)
     0 : sub      i4, i4, 56     
     1 : spill    5, i12         
     2 : spill    6, i13         
     3 : mov      i8, 0          
     4 : mov      i9, 0          
     5 : mov      s4, 2          
     6 : mov      s3, 4          
     7 : unspill  i13, 3         
     8 : mov      s2, i8         
     9 : sub      s2, s2, i13    
    10 : mul      i2, i2, s3     
    11 : label 0:                
    12 : cmp      i8, i2         
    13 : jmp_ge   3              
    14 : load.i32 i12, 5, i1, i8 
    15 : cmp      i12, i9        
    16 : jmp_ge   1              
    17 : add      i8, i8, s3     
    18 : jmp      0              
    19 : label 1:                
    20 : spill    1, i2          
    21 : mov      i0, i12        
    22 : cqo                     
    23 : div      i0, i0, s4     
    24 : unspill  i2, 1          
    25 : mul      i0, i0, s4     
    26 : sub      i12, i12, i0   
    27 : mov      i0, i12        
    28 : cmp      i0, i9         
    29 : jmp_eq   2              
    30 : mov      s2, i8         
    31 : jmp      3              
    32 : label 2:                
    33 : add      i8, i8, s3     
    34 : jmp      0              
    35 : label 3:                
    36 : unspill  i13, 2         
    37 : spill    0, i0          
    38 : mov      i0, i13        
    39 : cqo                     
    40 : div      i0, i0, s3     
    41 : mov      i2, i0         
    42 : unspill  i0, 0          
    43 : mov      i0, i2         
    44 : unspill  i12, 5         
    45 : unspill  i13, 6         
    46 : add      i4, i4, 56     
    47 : ret                     
