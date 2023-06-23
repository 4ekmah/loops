sort_double(i0, i1)
     0 : sub          i31, i31, 480     
     1 : spill        53, i18           
     2 : spill        54, i19           
     3 : spill        55, i20           
     4 : spill        56, i21           
     5 : arm_stp      i31, 58, i29, i30 
     6 : mov          i29, i31          
     7 : shl          i20, i1, 3        
     8 : spill        52, i20           
     9 : unspill      i20, 52           
    10 : sub          i21, i20, 8       
    11 : spill        51, i21           
    12 : mov          i20, 0            
    13 : spill        50, i20           
    14 : label 0:                       
    15 : unspill      i21, 51           
    16 : unspill      i20, 50           
    17 : cmp          i20, i21          
    18 : jmp_ge 2                       
    19 : unspill      i20, 50           
    20 : mov          i18, i20          
    21 : add          i19, i18, 8       
    22 : label 3:                       
    23 : unspill      i20, 52           
    24 : cmp          i19, i20          
    25 : jmp_ge 5                       
    26 : load.fp64    i3, i0, i19       
    27 : load.fp64    i2, i0, i18       
    28 : mov          i1, 30828         
    29 : arm_movk     i1, 10, 16        
    30 : arm_movk     i1, 1, 32         
    31 : arm_stp      i31, 0, i0, i1    
    32 : arm_stp      i31, 2, i2, i3    
    33 : arm_stp      i31, 4, i4, i5    
    34 : arm_stp      i31, 6, i6, i7    
    35 : arm_stp      i31, 8, i8, i9    
    36 : arm_stp      i31, 10, i10, i11 
    37 : arm_stp      i31, 12, i12, i13 
    38 : arm_stp      i31, 14, i14, i15 
    39 : arm_stp      i31, 16, i16, i17 
    40 : mov          i0, i3            
    41 : mov          i1, i2            
    42 : unspill      i9, 1             
    43 : add          i10, i31, 144     
    44 : vst_lane.u64 i10, v0           
    45 : vst_lane.u64 i10, v4           
    46 : vst_lane.u64 i10, v8           
    47 : vst_lane.u64 i10, v12          
    48 : call_noret   [i9]()            
    49 : add          i10, i31, 144     
    50 : vld_lane.u64 v0, i10           
    51 : vld_lane.u64 v4, i10           
    52 : vld_lane.u64 v8, i10           
    53 : vld_lane.u64 v12, i10          
    54 : mov          i1, i0            
    55 : unspill      i0, 0             
    56 : arm_ldp      i2, i3, i31, 2    
    57 : arm_ldp      i4, i5, i31, 4    
    58 : arm_ldp      i6, i7, i31, 6    
    59 : arm_ldp      i8, i9, i31, 8    
    60 : arm_ldp      i10, i11, i31, 10 
    61 : arm_ldp      i12, i13, i31, 12 
    62 : arm_ldp      i14, i15, i31, 14 
    63 : arm_ldp      i16, i17, i31, 16 
    64 : cmp          i1, 0             
    65 : jmp_eq 7                       
    66 : mov          i18, i19          
    67 : label 7:                       
    68 : add          i19, i19, 8       
    69 : jmp          3                 
    70 : label 5:                       
    71 : unspill      i20, 50           
    72 : cmp          i18, i20          
    73 : jmp_eq 9                       
    74 : unspill      i20, 50           
    75 : load.fp64    i1, i0, i20       
    76 : load.fp64    i2, i0, i18       
    77 : store.fp64   i0, i18, i1       
    78 : unspill      i20, 50           
    79 : store.fp64   i0, i20, i2       
    80 : label 9:                       
    81 : unspill      i20, 50           
    82 : add          i20, i20, 8       
    83 : spill        50, i20           
    84 : jmp          0                 
    85 : label 2:                       
    86 : arm_ldp      i29, i30, i31, 58 
    87 : unspill      i18, 53           
    88 : unspill      i19, 54           
    89 : unspill      i20, 55           
    90 : unspill      i21, 56           
    91 : add          i31, i31, 480     
    92 : ret                            
