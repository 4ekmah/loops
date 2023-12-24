nonnegative_odd(i0, i1)
     0 : sub      i31, i31, 48 
     1 : spill    3, i25       
     2 : spill    4, i26       
     3 : mov      i2, 0        
     4 : mov      i26, -4      
     5 : spill    2, i26       
     6 : mov      i25, 4       
     7 : mul      i1, i1, i25  
     8 : label 0:              
     9 : cmp      i2, i1       
    10 : jmp_ge 2              
    11 : load.i32 i25, i0, i2  
    12 : cmp      i25, 0       
    13 : jmp_ge 4              
    14 : add      i2, i2, 4    
    15 : jmp      0            
    16 : label 4:              
    17 : mov      i3, 2        
    18 : spill    0, i0        
    19 : mov      i0, i3       
    20 : div      i3, i25, i0  
    21 : mul      i3, i3, i0   
    22 : sub      i3, i25, i3  
    23 : unspill  i0, 0        
    24 : cmp      i3, 0        
    25 : jmp_eq 6              
    26 : mov      i26, i2      
    27 : spill    2, i26       
    28 : jmp      2            
    29 : label 6:              
    30 : add      i2, i2, 4    
    31 : jmp      0            
    32 : label 2:              
    33 : mov      i0, 4        
    34 : unspill  i26, 2       
    35 : div      i0, i26, i0  
    36 : unspill  i25, 3       
    37 : unspill  i26, 4       
    38 : add      i31, i31, 48 
    39 : ret                   
