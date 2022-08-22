nonnegative_odd(i0, i1)
     0 : sub      i31, i31, 32   
     1 : spill    1, i18         
     2 : spill    2, i19         
     3 : mov      i2, 0          
     4 : mov      i3, -4         
     5 : mov      i18, 4         
     6 : mul      i1, i1, i18    
     7 : label 0:                
     8 : cmp      i2, i1         
     9 : jmp_ge   3              
    10 : load.i32 i18, 5, i0, i2 
    11 : cmp      i18, 0         
    12 : jmp_ge   1              
    13 : add      i2, i2, 4      
    14 : jmp      0              
    15 : label 1:                
    16 : mov      i19, 2         
    17 : spill    0, i0          
    18 : mov      i0, i18        
    19 : div      i18, i0, i19   
    20 : mul      i18, i18, i19  
    21 : sub      i18, i0, i18   
    22 : unspill  i0, 0          
    23 : cmp      i18, 0         
    24 : jmp_eq   2              
    25 : mov      i3, i2         
    26 : jmp      3              
    27 : label 2:                
    28 : add      i2, i2, 4      
    29 : jmp      0              
    30 : label 3:                
    31 : mov      i0, 4          
    32 : div      i0, i3, i0     
    33 : unspill  i18, 1         
    34 : unspill  i19, 2         
    35 : add      i31, i31, 32   
    36 : ret                     
