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
    38 : spill    1, i2          
    39 : mov      i0, i13        
    40 : cqo                     
    41 : div      i0, i0, s3     
    42 : mov      i1, i0         
    43 : unspill  i0, 0          
    44 : unspill  i2, 1          
    45 : mov      i0, i1         
    46 : unspill  i12, 5         
    47 : unspill  i13, 6         
    48 : add      i4, i4, 56     
    49 : ret                     
