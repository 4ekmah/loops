nonnegative_odd(i0, i1)
     0 : sub      i31, i31, 64   
     1 : spill    3, i18         
     2 : spill    4, i19         
     3 : spill    5, i20         
     4 : spill    6, i21         
     5 : mov      i2, 0          
     6 : mov      i3, 0          
     7 : mov      i20, 2         
     8 : spill    2, i20         
     9 : mov      i20, 4         
    10 : spill    1, i20         
    11 : unspill  i20, 1         
    12 : sub      i21, i2, i20   
    13 : spill    0, i21         
    14 : unspill  i20, 1         
    15 : mul      i1, i1, i20    
    16 : label 0:                
    17 : cmp      i2, i1         
    18 : jmp_ge   3              
    19 : load.i32 i19, 5, i0, i2 
    20 : cmp      i19, i3        
    21 : jmp_ge   1              
    22 : unspill  i20, 1         
    23 : add      i2, i2, i20    
    24 : jmp      0              
    25 : label 1:                
    26 : unspill  i20, 2         
    27 : div      i18, i19, i20  
    28 : unspill  i20, 2         
    29 : mul      i18, i18, i20  
    30 : sub      i18, i19, i18  
    31 : mov      i18, i18       
    32 : cmp      i18, i3        
    33 : jmp_eq   2              
    34 : mov      i20, i2        
    35 : spill    0, i20         
    36 : jmp      3              
    37 : label 2:                
    38 : unspill  i20, 1         
    39 : add      i2, i2, i20    
    40 : jmp      0              
    41 : label 3:                
    42 : unspill  i20, 1         
    43 : unspill  i21, 0         
    44 : div      i1, i21, i20   
    45 : mov      i0, i1         
    46 : unspill  i18, 3         
    47 : unspill  i19, 4         
    48 : unspill  i20, 5         
    49 : unspill  i21, 6         
    50 : add      i31, i31, 64   
    51 : ret                     
