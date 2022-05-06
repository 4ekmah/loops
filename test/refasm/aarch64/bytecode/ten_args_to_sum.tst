ten_args_to_sum(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9)
     0 : sub     i31, i31, 48 
     1 : spill   2, i18       
     2 : spill   3, i19       
     3 : spill   4, i20       
     4 : spill   5, i21       
     5 : unspill i18, 6       
     6 : unspill i19, 7       
     7 : mov     i20, 1       
     8 : spill   0, i20       
     9 : unspill i20, 0       
    10 : mul     i21, i0, i20 
    11 : spill   1, i21       
    12 : mov     i0, 2        
    13 : mul     i1, i1, i0   
    14 : unspill i20, 1       
    15 : add     i1, i20, i1  
    16 : mov     i0, 3        
    17 : mul     i2, i2, i0   
    18 : add     i2, i1, i2   
    19 : mov     i1, 4        
    20 : mul     i3, i3, i1   
    21 : add     i3, i2, i3   
    22 : mov     i2, 5        
    23 : mul     i4, i4, i2   
    24 : add     i4, i3, i4   
    25 : mov     i3, 6        
    26 : mul     i5, i5, i3   
    27 : add     i5, i4, i5   
    28 : mov     i4, 7        
    29 : mul     i6, i6, i4   
    30 : add     i6, i5, i6   
    31 : mov     i5, 8        
    32 : mul     i7, i7, i5   
    33 : add     i7, i6, i7   
    34 : mov     i6, 3        
    35 : mul     i6, i18, i6  
    36 : add     i6, i7, i6   
    37 : mov     i7, 2        
    38 : mul     i7, i19, i7  
    39 : add     i7, i6, i7   
    40 : mov     i0, i7       
    41 : unspill i18, 2       
    42 : unspill i19, 3       
    43 : unspill i20, 4       
    44 : unspill i21, 5       
    45 : add     i31, i31, 48 
    46 : ret                  
