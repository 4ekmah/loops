bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub      i4, i4, 72       
     1 : spill    6, i12           
     2 : spill    7, i13           
     3 : spill    8, i14           
     4 : mov      i12, i8          
     5 : sub      i12, i12, i2     
     6 : mov      i13, i12         
     7 : neg      i13, i13         
     8 : select   i13, 6, i12, i13 
     9 : spill    1, i13           
    10 : mov      i12, i8          
    11 : sub      i12, i12, i2     
    12 : spill    0, i1            
    13 : mov      i13, i12         
    14 : mov      i1, i13          
    15 : sar      i13, i13, 63     
    16 : neg      i1, i1           
    17 : x86_adc  i13, i13, i13    
    18 : unspill  i1, 0            
    19 : spill    2, i13           
    20 : mov      i12, i9          
    21 : sub      i12, i12, i1     
    22 : spill    0, i12           
    23 : neg      i12, i12         
    24 : select   i12, 6, s0, i12  
    25 : mov      s3, i12          
    26 : neg      s3, s3           
    27 : mov      i12, i9          
    28 : sub      i12, i12, i1     
    29 : spill    0, i1            
    30 : mov      i13, i12         
    31 : mov      i1, i13          
    32 : sar      i13, i13, 63     
    33 : neg      i1, i1           
    34 : x86_adc  i13, i13, i13    
    35 : unspill  i1, 0            
    36 : spill    5, i13           
    37 : unspill  i13, 1           
    38 : unspill  i14, 3           
    39 : mov      s4, i13          
    40 : add      s4, s4, i14      
    41 : label 0:                  
    42 : cmp      i7, 0            
    43 : jmp_eq   7                
    44 : mov      i12, i1          
    45 : mul      i12, i12, i6     
    46 : add      i12, i12, i2     
    47 : unspill  i13, 10          
    48 : store.u8 0, i7, i12, i13  
    49 : cmp      i2, i8           
    50 : jmp_ne   2                
    51 : cmp      i1, i9           
    52 : jmp_ne   1                
    53 : jmp      7                
    54 : label 1:                  
    55 : label 2:                  
    56 : unspill  i13, 4           
    57 : mov      i12, i13         
    58 : shl      i12, i12, 1      
    59 : cmp      i12, s3          
    60 : jmp_gt   4                
    61 : cmp      i2, i8           
    62 : jmp_ne   3                
    63 : jmp      7                
    64 : label 3:                  
    65 : unspill  i13, 4           
    66 : mov      i0, i13          
    67 : add      i0, i0, s3       
    68 : mov      s4, i0           
    69 : mov      i0, i2           
    70 : add      i0, i0, s2       
    71 : mov      i2, i0           
    72 : label 4:                  
    73 : cmp      i12, s1          
    74 : jmp_gt   6                
    75 : cmp      i1, i9           
    76 : jmp_ne   5                
    77 : jmp      7                
    78 : label 5:                  
    79 : unspill  i13, 4           
    80 : mov      i0, i13          
    81 : add      i0, i0, s1       
    82 : mov      s4, i0           
    83 : mov      i0, i1           
    84 : add      i0, i0, s5       
    85 : mov      i1, i0           
    86 : label 6:                  
    87 : jmp      0                
    88 : label 7:                  
    89 : unspill  i12, 6           
    90 : unspill  i13, 7           
    91 : unspill  i14, 8           
    92 : add      i4, i4, 72       
    93 : ret                       
