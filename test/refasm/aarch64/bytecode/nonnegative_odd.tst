nonnegative_odd(i0, i1)
     0 : sub      i31, i31, 48   
     1 : spill    2, i18         
     2 : spill    3, i19         
     3 : spill    4, i20         
     4 : mov      i2, 0          
     5 : mov      i20, -4        
     6 : spill    0, i20         
     7 : mov      i20, 4         
     8 : spill    1, i20         
     9 : unspill  i20, 1         
    10 : mul      i1, i1, i20    
    11 : label 0:                
    12 : cmp      i2, i1         
    13 : jmp_ge   3              
    14 : load.i32 i19, 5, i0, i2 
    15 : mov      i3, 0          
    16 : cmp      i19, i3        
    17 : jmp_ge   1              
    18 : add      i2, i2, 4      
    19 : jmp      0              
    20 : label 1:                
    21 : mov      i3, 2          
    22 : div      i3, i19, i3    
    23 : mov      i18, 2         
    24 : mul      i3, i3, i18    
    25 : sub      i3, i19, i3    
    26 : mov      i3, i3         
    27 : mov      i18, 0         
    28 : cmp      i3, i18        
    29 : jmp_eq   2              
    30 : mov      i20, i2        
    31 : spill    0, i20         
    32 : jmp      3              
    33 : label 2:                
    34 : add      i2, i2, 4      
    35 : jmp      0              
    36 : label 3:                
    37 : mov      i0, 4          
    38 : unspill  i20, 0         
    39 : div      i0, i20, i0    
    40 : mov      i0, i0         
    41 : unspill  i18, 2         
    42 : unspill  i19, 3         
    43 : unspill  i20, 4         
    44 : add      i31, i31, 48   
    45 : ret                     
