ten_args_to_sum(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9)
     0 : sub     i31, i31, 32 
     1 : spill   0, i18       
     2 : spill   1, i19       
     3 : spill   2, i20       
     4 : unspill i18, 4       
     5 : mov     i19, 1       
     6 : mul     i0, i0, i19  
     7 : mov     i19, 2       
     8 : mul     i1, i1, i19  
     9 : add     i0, i0, i1   
    10 : mov     i1, 3        
    11 : mul     i1, i2, i1   
    12 : add     i0, i0, i1   
    13 : mov     i1, 4        
    14 : mul     i1, i3, i1   
    15 : add     i0, i0, i1   
    16 : mov     i1, 5        
    17 : mul     i1, i4, i1   
    18 : add     i0, i0, i1   
    19 : mov     i1, 6        
    20 : mul     i1, i5, i1   
    21 : add     i0, i0, i1   
    22 : mov     i1, 7        
    23 : mul     i1, i6, i1   
    24 : add     i0, i0, i1   
    25 : mov     i1, 8        
    26 : mul     i1, i7, i1   
    27 : add     i0, i0, i1   
    28 : mov     i1, 3        
    29 : mul     i1, i18, i1  
    30 : add     i0, i0, i1   
    31 : mov     i1, 2        
    32 : unspill i20, 5       
    33 : mul     i1, i20, i1  
    34 : add     i0, i0, i1   
    35 : mov     i0, i0       
    36 : unspill i18, 0       
    37 : unspill i19, 1       
    38 : unspill i20, 2       
    39 : add     i31, i31, 32 
    40 : ret                  
