nonnegative_odd(i0, i1)
     0 : sub      i4, i4, 56 
     1 : spill    5, i13     
     2 : mov      i8, 0      
     3 : mov      i13, -4    
     4 : spill    4, i13     
     5 : mul      i2, i2, 4  
     6 : label 0:            
     7 : cmp      i8, i2     
     8 : jmp_ge 2            
     9 : load.i32 i0, i1, i8 
    10 : cmp      i0, 0      
    11 : jmp_ge 4            
    12 : add      i8, i8, 4  
    13 : jmp      0          
    14 : label 4:            
    15 : mov      i9, 2      
    16 : spill    0, i0      
    17 : spill    1, i2      
    18 : x86_cqo             
    19 : mod      i0, i0, i9 
    20 : mov      i9, i2     
    21 : unspill  i0, 0      
    22 : unspill  i2, 1      
    23 : cmp      i9, 0      
    24 : jmp_eq 6            
    25 : mov      s4, i8     
    26 : jmp      2          
    27 : label 6:            
    28 : add      i8, i8, 4  
    29 : jmp      0          
    30 : label 2:            
    31 : mov      i1, 4      
    32 : unspill  i13, 4     
    33 : spill    0, i0      
    34 : spill    1, i2      
    35 : mov      i0, i13    
    36 : x86_cqo             
    37 : div      i0, i0, i1 
    38 : mov      i1, i0     
    39 : unspill  i0, 0      
    40 : unspill  i2, 1      
    41 : mov      i0, i1     
    42 : unspill  i13, 5     
    43 : add      i4, i4, 56 
    44 : ret                 
