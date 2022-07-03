bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub      i31, i31, 80   
     1 : spill    5, i18         
     2 : spill    6, i19         
     3 : spill    7, i20         
     4 : spill    8, i21         
     5 : spill    9, i22         
     6 : sub      i18, i4, i2    
     7 : cmp      i18, 0         
     8 : arm_cneg i20, i18, 2    
     9 : spill    4, i20         
    10 : sub      i19, i4, i2    
    11 : cmp      i19, 0         
    12 : sar      i20, i19, 63   
    13 : arm_cinc i20, i20, 3    
    14 : spill    0, i20         
    15 : sub      i19, i5, i3    
    16 : cmp      i19, 0         
    17 : arm_cneg i19, i19, 2    
    18 : neg      i20, i19       
    19 : spill    1, i20         
    20 : sub      i19, i5, i3    
    21 : cmp      i19, 0         
    22 : sar      i20, i19, 63   
    23 : arm_cinc i20, i20, 3    
    24 : spill    3, i20         
    25 : unspill  i20, 4         
    26 : unspill  i21, 1         
    27 : add      i22, i20, i21  
    28 : spill    2, i22         
    29 : label 0:                
    30 : cmp      i0, 0          
    31 : jmp_eq   7              
    32 : mul      i19, i3, i1    
    33 : add      i19, i19, i2   
    34 : store.u8 0, i0, i19, i6 
    35 : cmp      i2, i4         
    36 : jmp_ne   2              
    37 : cmp      i3, i5         
    38 : jmp_ne   1              
    39 : jmp      7              
    40 : label 1:                
    41 : label 2:                
    42 : unspill  i20, 2         
    43 : shl      i19, i20, 1    
    44 : unspill  i20, 1         
    45 : cmp      i19, i20       
    46 : jmp_gt   4              
    47 : cmp      i2, i4         
    48 : jmp_ne   3              
    49 : jmp      7              
    50 : label 3:                
    51 : unspill  i20, 1         
    52 : unspill  i21, 2         
    53 : add      i18, i21, i20  
    54 : mov      i20, i18       
    55 : spill    2, i20         
    56 : unspill  i20, 0         
    57 : add      i18, i2, i20   
    58 : mov      i2, i18        
    59 : label 4:                
    60 : unspill  i20, 4         
    61 : cmp      i19, i20       
    62 : jmp_gt   6              
    63 : cmp      i3, i5         
    64 : jmp_ne   5              
    65 : jmp      7              
    66 : label 5:                
    67 : unspill  i20, 4         
    68 : unspill  i21, 2         
    69 : add      i18, i21, i20  
    70 : mov      i20, i18       
    71 : spill    2, i20         
    72 : unspill  i20, 3         
    73 : add      i18, i3, i20   
    74 : mov      i3, i18        
    75 : label 6:                
    76 : jmp      0              
    77 : label 7:                
    78 : unspill  i18, 5         
    79 : unspill  i19, 6         
    80 : unspill  i20, 7         
    81 : unspill  i21, 8         
    82 : unspill  i22, 9         
    83 : add      i31, i31, 80   
    84 : ret                     
