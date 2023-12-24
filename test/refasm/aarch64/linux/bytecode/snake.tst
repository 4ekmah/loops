snake(i0, i1, i2)
     0 : sub          i31, i31, 512     
     1 : spill        57, i0            
     2 : spill        56, i1            
     3 : spill        55, i2            
     4 : spill        58, i25           
     5 : spill        59, i26           
     6 : spill        60, i27           
     7 : arm_stp      i31, 62, i29, i30 
     8 : mov          i29, i31          
     9 : unspill      i26, 56           
    10 : unspill      i27, 55           
    11 : add          i3, i26, i27      
    12 : sub          i26, i3, 1        
    13 : spill        54, i26           
    14 : mov          i26, 0            
    15 : spill        53, i26           
    16 : mov          i26, 1            
    17 : spill        50, i26           
    18 : unspill      i26, 50           
    19 : neg          i27, i26          
    20 : spill        51, i27           
    21 : mov          i26, 0            
    22 : spill        52, i26           
    23 : label 0:                       
    24 : unspill      i27, 54           
    25 : unspill      i26, 52           
    26 : cmp          i26, i27          
    27 : jmp_ge 2                       
    28 : mov          i25, 0            
    29 : mov          i3, 0             
    30 : unspill      i26, 52           
    31 : and          i2, i26, 1        
    32 : cmp          i2, 0             
    33 : jmp_eq 4                       
    34 : unspill      i26, 55           
    35 : sub          i2, i26, 1        
    36 : unspill      i26, 52           
    37 : cmp          i2, i26           
    38 : select_gt    i25, i2, i26      
    39 : unspill      i26, 52           
    40 : sub          i1, i26, i2       
    41 : mov          i0, 0             
    42 : unspill      i26, 52           
    43 : cmp          i26, i2           
    44 : select_gt    i3, i1, i0        
    45 : jmp          5                 
    46 : label 4:                       
    47 : unspill      i26, 56           
    48 : sub          i0, i26, 1        
    49 : unspill      i26, 52           
    50 : sub          i1, i26, i0       
    51 : mov          i2, 0             
    52 : unspill      i26, 52           
    53 : cmp          i26, i0           
    54 : select_gt    i25, i1, i2       
    55 : unspill      i26, 52           
    56 : cmp          i0, i26           
    57 : select_gt    i3, i0, i26       
    58 : label 5:                       
    59 : label 6:                       
    60 : cmp          i25, 0            
    61 : jmp_gt 8                       
    62 : unspill      i26, 55           
    63 : cmp          i25, i26          
    64 : jmp_ge 8                       
    65 : cmp          i3, 0             
    66 : jmp_gt 8                       
    67 : unspill      i26, 56           
    68 : cmp          i3, i26           
    69 : jmp_ge 8                       
    70 : mov          i0, 43900         
    71 : arm_movk     i0, 48117, 16     
    72 : arm_movk     i0, 43690, 32     
    73 : arm_stp      i31, 0, i0, i1    
    74 : arm_stp      i31, 2, i2, i3    
    75 : arm_stp      i31, 4, i4, i5    
    76 : arm_stp      i31, 6, i6, i7    
    77 : arm_stp      i31, 8, i8, i9    
    78 : arm_stp      i31, 10, i10, i11 
    79 : arm_stp      i31, 12, i12, i13 
    80 : arm_stp      i31, 14, i14, i15 
    81 : arm_stp      i31, 16, i16, i17 
    82 : mov          i0, i25           
    83 : mov          i1, i3            
    84 : unspill      i9, 0             
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
   105 : unspill      i26, 55           
   106 : mul          i0, i3, i26       
   107 : add          i0, i0, i25       
   108 : unspill      i26, 57           
   109 : unspill      i27, 53           
   110 : store.u8     i26, i0, i27      
   111 : unspill      i26, 53           
   112 : add          i26, i26, 1       
   113 : spill        53, i26           
   114 : unspill      i26, 50           
   115 : add          i25, i25, i26     
   116 : unspill      i26, 51           
   117 : add          i3, i3, i26       
   118 : jmp          6                 
   119 : label 8:                       
   120 : unspill      i26, 50           
   121 : neg          i26, i26          
   122 : spill        50, i26           
   123 : unspill      i26, 51           
   124 : neg          i26, i26          
   125 : spill        51, i26           
   126 : unspill      i26, 52           
   127 : add          i26, i26, 1       
   128 : spill        52, i26           
   129 : jmp          0                 
   130 : label 2:                       
   131 : arm_ldp      i29, i30, i31, 62 
   132 : unspill      i25, 58           
   133 : unspill      i26, 59           
   134 : unspill      i27, 60           
   135 : add          i31, i31, 512     
   136 : ret                            
