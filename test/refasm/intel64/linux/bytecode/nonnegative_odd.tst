nonnegative_odd(i0, i1)
     0 : sub      i4, i4, 40  
     1 : spill    4, i12      
     2 : mov      i2, 0       
     3 : mov      i1, -4      
     4 : mul      i6, i6, 4   
     5 : label 0:             
     6 : cmp      i2, i6      
     7 : jmp_ge   3           
     8 : load.i32 i0, i7, i2  
     9 : cmp      i0, 0       
    10 : jmp_ge   1           
    11 : add      i2, i2, 4   
    12 : jmp      0           
    13 : label 1:             
    14 : mov      i12, 2      
    15 : spill    1, i2       
    16 : x86_cqo              
    17 : mod      i0, i0, i12 
    18 : mov      i0, i2      
    19 : unspill  i2, 1       
    20 : cmp      i0, 0       
    21 : jmp_eq   2           
    22 : mov      i1, i2      
    23 : jmp      3           
    24 : label 2:             
    25 : add      i2, i2, 4   
    26 : jmp      0           
    27 : label 3:             
    28 : mov      i7, 4       
    29 : spill    0, i0       
    30 : spill    1, i2       
    31 : mov      i0, i1      
    32 : x86_cqo              
    33 : div      i0, i0, i7  
    34 : mov      i7, i0      
    35 : unspill  i0, 0       
    36 : unspill  i2, 1       
    37 : mov      i0, i7      
    38 : unspill  i12, 4      
    39 : add      i4, i4, 40  
    40 : ret                  
