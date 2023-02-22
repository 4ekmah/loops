bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub      i4, i4, 104       
     1 : spill    9, i12            
     2 : spill    10, i13           
     3 : spill    11, i14           
     4 : unspill  i0, 18            
     5 : mov      i12, i0           
     6 : sub      i12, i12, i8      
     7 : mov      i13, i12          
     8 : neg      i13, i13          
     9 : select   i13, 36, i12, i13 
    10 : spill    4, i13            
    11 : mov      i12, i0           
    12 : sub      i12, i12, i8      
    13 : spill    0, i1             
    14 : mov      i13, i12          
    15 : mov      i1, i13           
    16 : sar      i13, i13, 63      
    17 : neg      i1, i1            
    18 : x86_adc  i13, i13, i13     
    19 : unspill  i1, 0             
    20 : spill    5, i13            
    21 : unspill  i13, 19           
    22 : mov      i12, i13          
    23 : sub      i12, i12, i9      
    24 : spill    0, i12            
    25 : neg      i12, i12          
    26 : select   i12, 36, s0, i12  
    27 : mov      s6, i12           
    28 : neg      s6, s6            
    29 : unspill  i13, 19           
    30 : mov      i12, i13          
    31 : sub      i12, i12, i9      
    32 : spill    0, i1             
    33 : mov      i13, i12          
    34 : mov      i1, i13           
    35 : sar      i13, i13, 63      
    36 : neg      i1, i1            
    37 : x86_adc  i13, i13, i13     
    38 : unspill  i1, 0             
    39 : spill    8, i13            
    40 : unspill  i13, 4            
    41 : unspill  i14, 6            
    42 : mov      s7, i13           
    43 : add      s7, s7, i14       
    44 : label 0:                   
    45 : cmp      i1, 0             
    46 : jmp_eq   7                 
    47 : mov      i12, i9           
    48 : mul      i12, i12, i2      
    49 : add      i12, i12, i8      
    50 : unspill  i13, 20           
    51 : store.u8 i1, i12, i13      
    52 : cmp      i8, i0            
    53 : jmp_ne   2                 
    54 : cmp      i9, s19           
    55 : jmp_ne   1                 
    56 : jmp      7                 
    57 : label 1:                   
    58 : label 2:                   
    59 : unspill  i13, 7            
    60 : mov      i12, i13          
    61 : shl      i12, i12, 1       
    62 : cmp      i12, s6           
    63 : jmp_gt   4                 
    64 : cmp      i8, i0            
    65 : jmp_ne   3                 
    66 : jmp      7                 
    67 : label 3:                   
    68 : unspill  i13, 6            
    69 : add      s7, s7, i13       
    70 : add      i8, i8, s5        
    71 : label 4:                   
    72 : cmp      i12, s4           
    73 : jmp_gt   6                 
    74 : cmp      i9, s19           
    75 : jmp_ne   5                 
    76 : jmp      7                 
    77 : label 5:                   
    78 : unspill  i13, 4            
    79 : add      s7, s7, i13       
    80 : add      i9, i9, s8        
    81 : label 6:                   
    82 : jmp      0                 
    83 : label 7:                   
    84 : unspill  i12, 9            
    85 : unspill  i13, 10           
    86 : unspill  i14, 11           
    87 : add      i4, i4, 104       
    88 : ret                        
