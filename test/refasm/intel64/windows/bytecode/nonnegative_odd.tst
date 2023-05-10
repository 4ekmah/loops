nonnegative_odd(i0, i1)
     0 : sub      i4, i4, 40  
     1 : spill    4, i12      
     2 : mov      i8, 0       
     3 : mov      i9, -4      
     4 : mul      i2, i2, 4   
     5 : label 0:             
     6 : cmp      i8, i2      
     7 : jmp_ge 2             
     8 : load.i32 i0, i1, i8  
     9 : cmp      i0, 0       
    10 : jmp_ge 4             
    11 : add      i8, i8, 4   
    12 : jmp      0           
    13 : label 4:             
    14 : mov      i12, 2      
    15 : spill    1, i2       
    16 : x86_cqo              
    17 : mod      i0, i0, i12 
    18 : mov      i0, i2      
    19 : unspill  i2, 1       
    20 : cmp      i0, 0       
    21 : jmp_eq 6             
    22 : mov      i9, i8      
    23 : jmp      2           
    24 : label 6:             
    25 : add      i8, i8, 4   
    26 : jmp      0           
    27 : label 2:             
    28 : mov      i1, 4       
    29 : spill    0, i0       
    30 : spill    1, i2       
    31 : mov      i0, i9      
    32 : x86_cqo              
    33 : div      i0, i0, i1  
    34 : mov      i1, i0      
    35 : unspill  i0, 0       
    36 : unspill  i2, 1       
    37 : mov      i0, i1      
    38 : unspill  i12, 4      
    39 : add      i4, i4, 40  
    40 : ret                  
