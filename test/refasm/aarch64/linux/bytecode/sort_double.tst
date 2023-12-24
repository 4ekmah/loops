sort_double(i0, i1)
     0 : sub          i31, i31, 480     
     1 : spill        53, i0            
     2 : spill        54, i25           
     3 : spill        55, i26           
     4 : spill        56, i27           
     5 : arm_stp      i31, 58, i29, i30 
     6 : mov          i29, i31          
     7 : shl          i26, i1, 3        
     8 : spill        52, i26           
     9 : unspill      i26, 52           
    10 : sub          i27, i26, 8       
    11 : spill        51, i27           
    12 : mov          i26, 0            
    13 : spill        50, i26           
    14 : label 0:                       
    15 : unspill      i27, 51           
    16 : unspill      i26, 50           
    17 : cmp          i26, i27          
    18 : jmp_ge 2                       
    19 : unspill      i26, 50           
    20 : mov          i25, i26          
    21 : add          i3, i25, 8        
    22 : label 3:                       
    23 : unspill      i26, 52           
    24 : cmp          i3, i26           
    25 : jmp_ge 5                       
    26 : unspill      i26, 53           
    27 : load.fp64    i2, i26, i3       
    28 : unspill      i26, 53           
    29 : load.fp64    i1, i26, i25      
    30 : mov          i0, 50064         
    31 : arm_movk     i0, 48117, 16     
    32 : arm_movk     i0, 43690, 32     
    33 : arm_stp      i31, 0, i0, i1    
    34 : arm_stp      i31, 2, i2, i3    
    35 : arm_stp      i31, 4, i4, i5    
    36 : arm_stp      i31, 6, i6, i7    
    37 : arm_stp      i31, 8, i8, i9    
    38 : arm_stp      i31, 10, i10, i11 
    39 : arm_stp      i31, 12, i12, i13 
    40 : arm_stp      i31, 14, i14, i15 
    41 : arm_stp      i31, 16, i16, i17 
    42 : mov          i0, i2            
    43 : unspill      i9, 0             
    44 : add          i10, i31, 144     
    45 : vst_lane.u64 i10, v0           
    46 : vst_lane.u64 i10, v4           
    47 : vst_lane.u64 i10, v8           
    48 : vst_lane.u64 i10, v12          
    49 : call_noret   [i9]()            
    50 : add          i10, i31, 144     
    51 : vld_lane.u64 v0, i10           
    52 : vld_lane.u64 v4, i10           
    53 : vld_lane.u64 v8, i10           
    54 : vld_lane.u64 v12, i10          
    55 : unspill      i1, 1             
    56 : arm_ldp      i2, i3, i31, 2    
    57 : arm_ldp      i4, i5, i31, 4    
    58 : arm_ldp      i6, i7, i31, 6    
    59 : arm_ldp      i8, i9, i31, 8    
    60 : arm_ldp      i10, i11, i31, 10 
    61 : arm_ldp      i12, i13, i31, 12 
    62 : arm_ldp      i14, i15, i31, 14 
    63 : arm_ldp      i16, i17, i31, 16 
    64 : cmp          i0, 0             
    65 : jmp_eq 7                       
    66 : mov          i25, i3           
    67 : label 7:                       
    68 : add          i3, i3, 8         
    69 : jmp          3                 
    70 : label 5:                       
    71 : unspill      i26, 50           
    72 : cmp          i25, i26          
    73 : jmp_eq 9                       
    74 : unspill      i26, 53           
    75 : unspill      i27, 50           
    76 : load.fp64    i0, i26, i27      
    77 : unspill      i26, 53           
    78 : load.fp64    i1, i26, i25      
    79 : unspill      i26, 53           
    80 : store.fp64   i26, i25, i0      
    81 : unspill      i26, 53           
    82 : unspill      i27, 50           
    83 : store.fp64   i26, i27, i1      
    84 : label 9:                       
    85 : unspill      i26, 50           
    86 : add          i26, i26, 8       
    87 : spill        50, i26           
    88 : jmp          0                 
    89 : label 2:                       
    90 : arm_ldp      i29, i30, i31, 58 
    91 : unspill      i25, 54           
    92 : unspill      i26, 55           
    93 : unspill      i27, 56           
    94 : add          i31, i31, 480     
    95 : ret                            
