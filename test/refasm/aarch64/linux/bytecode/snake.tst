snake(i0, i1, i2)
     0 : sub          i31, i31, 512     
     1 : spill        56, i1            
     2 : spill        55, i2            
     3 : spill        57, i18           
     4 : spill        58, i19           
     5 : spill        59, i20           
     6 : spill        60, i21           
     7 : arm_stp      i31, 62, i29, i30 
     8 : mov          i29, i31          
     9 : unspill      i20, 56           
    10 : unspill      i21, 55           
    11 : add          i3, i20, i21      
    12 : sub          i20, i3, 1        
    13 : spill        54, i20           
    14 : mov          i20, 0            
    15 : spill        53, i20           
    16 : mov          i20, 1            
    17 : spill        52, i20           
    18 : unspill      i20, 52           
    19 : neg          i21, i20          
    20 : spill        50, i21           
    21 : mov          i20, 0            
    22 : spill        51, i20           
    23 : label 0:                       
    24 : unspill      i21, 54           
    25 : unspill      i20, 51           
    26 : cmp          i20, i21          
    27 : jmp_ge 2                       
    28 : mov          i19, 0            
    29 : mov          i18, 0            
    30 : unspill      i20, 51           
    31 : and          i3, i20, 1        
    32 : cmp          i3, 0             
    33 : jmp_eq 4                       
    34 : unspill      i20, 55           
    35 : sub          i3, i20, 1        
    36 : unspill      i20, 51           
    37 : cmp          i3, i20           
    38 : select_gt    i19, i3, i20      
    39 : unspill      i20, 51           
    40 : sub          i2, i20, i3       
    41 : mov          i1, 0             
    42 : unspill      i20, 51           
    43 : cmp          i20, i3           
    44 : select_gt    i18, i2, i1       
    45 : jmp          5                 
    46 : label 4:                       
    47 : unspill      i20, 56           
    48 : sub          i1, i20, 1        
    49 : unspill      i20, 51           
    50 : sub          i2, i20, i1       
    51 : mov          i3, 0             
    52 : unspill      i20, 51           
    53 : cmp          i20, i1           
    54 : select_gt    i19, i2, i3       
    55 : unspill      i20, 51           
    56 : cmp          i1, i20           
    57 : select_gt    i18, i1, i20      
    58 : label 5:                       
    59 : label 6:                       
    60 : cmp          i19, 0            
    61 : jmp_gt 8                       
    62 : unspill      i20, 55           
    63 : cmp          i19, i20          
    64 : jmp_ge 8                       
    65 : cmp          i18, 0            
    66 : jmp_gt 8                       
    67 : unspill      i20, 56           
    68 : cmp          i18, i20          
    69 : jmp_ge 8                       
    70 : mov          i1, 20244         
    71 : arm_movk     i1, 10, 16        
    72 : arm_movk     i1, 1, 32         
    73 : arm_stp      i31, 0, i0, i1    
    74 : arm_stp      i31, 2, i2, i3    
    75 : arm_stp      i31, 4, i4, i5    
    76 : arm_stp      i31, 6, i6, i7    
    77 : arm_stp      i31, 8, i8, i9    
    78 : arm_stp      i31, 10, i10, i11 
    79 : arm_stp      i31, 12, i12, i13 
    80 : arm_stp      i31, 14, i14, i15 
    81 : arm_stp      i31, 16, i16, i17 
    82 : mov          i0, i19           
    83 : mov          i1, i18           
    84 : unspill      i9, 1             
    85 : add          i10, i31, 144     
    86 : vst_lane.u64 i10, v0           
    87 : vst_lane.u64 i10, v4           
    88 : vst_lane.u64 i10, v8           
    89 : vst_lane.u64 i10, v12          
    90 : call_noret   [i9]()            
    91 : add          i10, i31, 144     
    92 : vld_lane.u64 v0, i10           
    93 : vld_lane.u64 v4, i10           
    94 : vld_lane.u64 v8, i10           
    95 : vld_lane.u64 v12, i10          
    96 : arm_ldp      i0, i1, i31, 0    
    97 : arm_ldp      i2, i3, i31, 2    
    98 : arm_ldp      i4, i5, i31, 4    
    99 : arm_ldp      i6, i7, i31, 6    
   100 : arm_ldp      i8, i9, i31, 8    
   101 : arm_ldp      i10, i11, i31, 10 
   102 : arm_ldp      i12, i13, i31, 12 
   103 : arm_ldp      i14, i15, i31, 14 
   104 : arm_ldp      i16, i17, i31, 16 
   105 : unspill      i20, 55           
   106 : mul          i1, i18, i20      
   107 : add          i1, i1, i19       
   108 : unspill      i20, 53           
   109 : store.u8     i0, i1, i20       
   110 : unspill      i20, 53           
   111 : add          i20, i20, 1       
   112 : spill        53, i20           
   113 : unspill      i20, 52           
   114 : add          i19, i19, i20     
   115 : unspill      i20, 50           
   116 : add          i18, i18, i20     
   117 : jmp          6                 
   118 : label 8:                       
   119 : unspill      i20, 52           
   120 : neg          i20, i20          
   121 : spill        52, i20           
   122 : unspill      i20, 50           
   123 : neg          i20, i20          
   124 : spill        50, i20           
   125 : unspill      i20, 51           
   126 : add          i20, i20, 1       
   127 : spill        51, i20           
   128 : jmp          0                 
   129 : label 2:                       
   130 : arm_ldp      i29, i30, i31, 62 
   131 : unspill      i18, 57           
   132 : unspill      i19, 58           
   133 : unspill      i20, 59           
   134 : unspill      i21, 60           
   135 : add          i31, i31, 512     
   136 : ret                            
