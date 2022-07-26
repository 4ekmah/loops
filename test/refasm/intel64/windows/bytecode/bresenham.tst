bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub      i4, i4, 72       
     1 : spill    6, i12           
     2 : spill    7, i13           
     3 : spill    8, i14           
     4 : unspill  i13, 14          
     5 : mov      i12, i13         
     6 : sub      i12, i12, i8     
     7 : mov      i13, i12         
     8 : neg      i13, i13         
     9 : select   i13, 6, i12, i13 
    10 : spill    1, i13           
    11 : unspill  i13, 14          
    12 : mov      i12, i13         
    13 : sub      i12, i12, i8     
    14 : spill    0, i1            
    15 : mov      i13, i12         
    16 : mov      i1, i13          
    17 : sar      i13, i13, 63     
    18 : neg      i1, i1           
    19 : x86_adc  i13, i13, i13    
    20 : unspill  i1, 0            
    21 : spill    2, i13           
    22 : unspill  i13, 15          
    23 : mov      i12, i13         
    24 : sub      i12, i12, i9     
    25 : spill    0, i12           
    26 : neg      i12, i12         
    27 : select   i12, 6, s0, i12  
    28 : mov      s3, i12          
    29 : neg      s3, s3           
    30 : unspill  i13, 15          
    31 : mov      i12, i13         
    32 : sub      i12, i12, i9     
    33 : spill    0, i1            
    34 : mov      i13, i12         
    35 : mov      i1, i13          
    36 : sar      i13, i13, 63     
    37 : neg      i1, i1           
    38 : x86_adc  i13, i13, i13    
    39 : unspill  i1, 0            
    40 : spill    5, i13           
    41 : unspill  i13, 1           
    42 : unspill  i14, 3           
    43 : mov      s4, i13          
    44 : add      s4, s4, i14      
    45 : label 0:                  
    46 : cmp      i1, 0            
    47 : jmp_eq   7                
    48 : mov      i12, i9          
    49 : mul      i12, i12, i2     
    50 : add      i12, i12, i8     
    51 : unspill  i13, 16          
    52 : store.u8 0, i1, i12, i13  
    53 : cmp      i8, s14          
    54 : jmp_ne   2                
    55 : cmp      i9, s15          
    56 : jmp_ne   1                
    57 : jmp      7                
    58 : label 1:                  
    59 : label 2:                  
    60 : unspill  i13, 4           
    61 : mov      i12, i13         
    62 : shl      i12, i12, 1      
    63 : cmp      i12, s3          
    64 : jmp_gt   4                
    65 : cmp      i8, s14          
    66 : jmp_ne   3                
    67 : jmp      7                
    68 : label 3:                  
    69 : unspill  i13, 4           
    70 : mov      i0, i13          
    71 : add      i0, i0, s3       
    72 : mov      s4, i0           
    73 : mov      i0, i8           
    74 : add      i0, i0, s2       
    75 : mov      i8, i0           
    76 : label 4:                  
    77 : cmp      i12, s1          
    78 : jmp_gt   6                
    79 : cmp      i9, s15          
    80 : jmp_ne   5                
    81 : jmp      7                
    82 : label 5:                  
    83 : unspill  i13, 4           
    84 : mov      i0, i13          
    85 : add      i0, i0, s1       
    86 : mov      s4, i0           
    87 : mov      i0, i9           
    88 : add      i0, i0, s5       
    89 : mov      i9, i0           
    90 : label 6:                  
    91 : jmp      0                
    92 : label 7:                  
    93 : unspill  i12, 6           
    94 : unspill  i13, 7           
    95 : unspill  i14, 8           
    96 : add      i4, i4, 72       
    97 : ret                       
