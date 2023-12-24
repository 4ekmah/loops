ten_args_to_sum(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9)
     0 : sub     i31, i31, 16 
     1 : spill   0, i25       
     2 : spill   1, i26       
     3 : mov     i25, 1       
     4 : mul     i0, i0, i25  
     5 : mov     i25, 2       
     6 : mul     i1, i1, i25  
     7 : add     i0, i0, i1   
     8 : mov     i1, 3        
     9 : mul     i1, i2, i1   
    10 : add     i0, i0, i1   
    11 : mov     i1, 4        
    12 : mul     i1, i3, i1   
    13 : add     i0, i0, i1   
    14 : mov     i1, 5        
    15 : mul     i1, i4, i1   
    16 : add     i0, i0, i1   
    17 : mov     i1, 6        
    18 : mul     i1, i5, i1   
    19 : add     i0, i0, i1   
    20 : mov     i1, 7        
    21 : mul     i1, i6, i1   
    22 : add     i0, i0, i1   
    23 : mov     i1, 8        
    24 : mul     i1, i7, i1   
    25 : add     i0, i0, i1   
    26 : mov     i1, 3        
    27 : unspill i26, 2       
    28 : mul     i1, i26, i1  
    29 : add     i0, i0, i1   
    30 : mov     i1, 2        
    31 : unspill i26, 3       
    32 : mul     i1, i26, i1  
    33 : add     i0, i0, i1   
    34 : unspill i25, 0       
    35 : unspill i26, 1       
    36 : add     i31, i31, 16 
    37 : ret                  
