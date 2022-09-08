max_pool_3_3(i0, i1, i2, i3, i4, i5, i6)
     0 : sub      i31, i31, 64  
     1 : spill    0, i18        
     2 : spill    1, i19        
     3 : spill    2, i20        
     4 : spill    3, i21        
     5 : spill    4, i22        
     6 : spill    5, i23        
     7 : spill    6, i24        
     8 : mov      i7, 0         
     9 : mov      i7, 0         
    10 : mov      i8, 4         
    11 : mul      i8, i6, i8    
    12 : mov      i9, 0         
    13 : mul      i9, i9, i2    
    14 : mov      i10, 0        
    15 : add      i9, i9, i10   
    16 : mov      i10, 4        
    17 : mul      i9, i9, i10   
    18 : mov      i10, 0        
    19 : mul      i10, i10, i2  
    20 : mov      i11, 1        
    21 : add      i10, i10, i11 
    22 : mov      i11, 4        
    23 : mul      i10, i10, i11 
    24 : mov      i11, 0        
    25 : mul      i11, i11, i2  
    26 : mov      i12, 2        
    27 : add      i11, i11, i12 
    28 : mov      i12, 4        
    29 : mul      i11, i11, i12 
    30 : mov      i12, 1        
    31 : mul      i12, i12, i2  
    32 : mov      i13, 0        
    33 : add      i12, i12, i13 
    34 : mov      i13, 4        
    35 : mul      i12, i12, i13 
    36 : mov      i13, 1        
    37 : mul      i13, i13, i2  
    38 : mov      i14, 1        
    39 : add      i13, i13, i14 
    40 : mov      i14, 4        
    41 : mul      i13, i13, i14 
    42 : mov      i14, 1        
    43 : mul      i14, i14, i2  
    44 : mov      i15, 2        
    45 : add      i14, i14, i15 
    46 : mov      i15, 4        
    47 : mul      i14, i14, i15 
    48 : mov      i15, 2        
    49 : mul      i15, i15, i2  
    50 : mov      i16, 0        
    51 : add      i15, i15, i16 
    52 : mov      i16, 4        
    53 : mul      i15, i15, i16 
    54 : mov      i16, 2        
    55 : mul      i16, i16, i2  
    56 : mov      i17, 1        
    57 : add      i16, i16, i17 
    58 : mov      i17, 4        
    59 : mul      i16, i16, i17 
    60 : mov      i17, 2        
    61 : mul      i17, i17, i2  
    62 : mov      i18, 2        
    63 : add      i17, i17, i18 
    64 : mov      i18, 4        
    65 : mul      i17, i17, i18 
    66 : label 0:               
    67 : cmp      i7, i3        
    68 : jmp_ge   6             
    69 : mov      i18, 0        
    70 : mul      i19, i7, i6   
    71 : mul      i19, i19, i5  
    72 : mov      i20, 4        
    73 : mul      i19, i19, i20 
    74 : add      i19, i4, i19  
    75 : mul      i20, i7, i2   
    76 : mul      i20, i20, i1  
    77 : mov      i21, 4        
    78 : mul      i20, i20, i21 
    79 : add      i20, i0, i20  
    80 : label 1:               
    81 : cmp      i18, i5       
    82 : jmp_ge   5             
    83 : mov      i21, 0        
    84 : mov      i22, 16       
    85 : label 2:               
    86 : cmp      i21, i8       
    87 : jmp_ge   4             
    88 : cmp      i22, i8       
    89 : jmp_le   3             
    90 : sub      i23, i22, i8  
    91 : sub      i21, i21, i23 
    92 : sub      i22, i22, i23 
    93 : label 3:               
    94 : add      i23, i20, i21 
    95 : add      i24, i19, i21 
    96 : vld.fp32 v0, i23, i9   
    97 : vld.fp32 v1, i23, i10  
    98 : max.fp32 v0, v0, v1    
    99 : vld.fp32 v1, i23, i11  
   100 : max.fp32 v0, v0, v1    
   101 : vld.fp32 v1, i23, i12  
   102 : max.fp32 v0, v0, v1    
   103 : vld.fp32 v1, i23, i13  
   104 : max.fp32 v0, v0, v1    
   105 : vld.fp32 v1, i23, i14  
   106 : max.fp32 v0, v0, v1    
   107 : vld.fp32 v1, i23, i15  
   108 : max.fp32 v0, v0, v1    
   109 : vld.fp32 v1, i23, i16  
   110 : max.fp32 v0, v0, v1    
   111 : vld.fp32 v1, i23, i17  
   112 : max.fp32 v0, v0, v1    
   113 : vst.fp32 i24, v0       
   114 : mov      i21, i22      
   115 : add      i22, i22, 16  
   116 : jmp      2             
   117 : label 4:               
   118 : add      i18, i18, 1   
   119 : mov      i21, 4        
   120 : mul      i21, i6, i21  
   121 : add      i19, i19, i21 
   122 : mov      i21, 4        
   123 : mul      i21, i2, i21  
   124 : add      i20, i20, i21 
   125 : jmp      1             
   126 : label 5:               
   127 : add      i7, i7, 1     
   128 : jmp      0             
   129 : label 6:               
   130 : mov      i0, 0         
   131 : unspill  i18, 0        
   132 : unspill  i19, 1        
   133 : unspill  i20, 2        
   134 : unspill  i21, 3        
   135 : unspill  i22, 4        
   136 : unspill  i23, 5        
   137 : unspill  i24, 6        
   138 : add      i31, i31, 64  
   139 : ret                    
