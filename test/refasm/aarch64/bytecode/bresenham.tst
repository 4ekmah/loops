bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub      i31, i31, 64  
     1 : spill    4, i18        
     2 : spill    5, i19        
     3 : spill    6, i20        
     4 : spill    7, i21        
     5 : sub      i18, i4, i2   
     6 : cmp      i18, 0        
     7 : arm_cneg i18, i18, 31  
     8 : sub      i19, i4, i2   
     9 : cmp      i19, 0        
    10 : sar      i20, i19, 63  
    11 : arm_cinc i20, i20, 28  
    12 : spill    0, i20        
    13 : sub      i19, i5, i3   
    14 : cmp      i19, 0        
    15 : arm_cneg i19, i19, 31  
    16 : neg      i20, i19      
    17 : spill    1, i20        
    18 : sub      i19, i5, i3   
    19 : cmp      i19, 0        
    20 : sar      i20, i19, 63  
    21 : arm_cinc i20, i20, 28  
    22 : spill    3, i20        
    23 : unspill  i20, 1        
    24 : add      i21, i18, i20 
    25 : spill    2, i21        
    26 : label 0:               
    27 : cmp      i0, 0         
    28 : jmp_eq   7             
    29 : mul      i19, i3, i1   
    30 : add      i19, i19, i2  
    31 : store.u8 i0, i19, i6   
    32 : cmp      i2, i4        
    33 : jmp_ne   2             
    34 : cmp      i3, i5        
    35 : jmp_ne   1             
    36 : jmp      7             
    37 : label 1:               
    38 : label 2:               
    39 : unspill  i20, 2        
    40 : shl      i19, i20, 1   
    41 : unspill  i20, 1        
    42 : cmp      i19, i20      
    43 : jmp_gt   4             
    44 : cmp      i2, i4        
    45 : jmp_ne   3             
    46 : jmp      7             
    47 : label 3:               
    48 : unspill  i21, 1        
    49 : unspill  i20, 2        
    50 : add      i20, i20, i21 
    51 : spill    2, i20        
    52 : unspill  i20, 0        
    53 : add      i2, i2, i20   
    54 : label 4:               
    55 : cmp      i19, i18      
    56 : jmp_gt   6             
    57 : cmp      i3, i5        
    58 : jmp_ne   5             
    59 : jmp      7             
    60 : label 5:               
    61 : unspill  i20, 2        
    62 : add      i20, i20, i18 
    63 : spill    2, i20        
    64 : unspill  i20, 3        
    65 : add      i3, i3, i20   
    66 : label 6:               
    67 : jmp      0             
    68 : label 7:               
    69 : unspill  i18, 4        
    70 : unspill  i19, 5        
    71 : unspill  i20, 6        
    72 : unspill  i21, 7        
    73 : add      i31, i31, 64  
    74 : ret                    
