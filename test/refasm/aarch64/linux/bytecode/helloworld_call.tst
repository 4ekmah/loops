helloworld_call()
     0 : sub          i31, i31, 416     
     1 : arm_stp      i31, 50, i29, i30 
     2 : mov          i29, i31          
     3 : mov          i0, 42256         
     4 : arm_movk     i0, 48117, 16     
     5 : arm_movk     i0, 43690, 32     
     6 : arm_stp      i31, 0, i0, i1    
     7 : arm_stp      i31, 2, i2, i3    
     8 : arm_stp      i31, 4, i4, i5    
     9 : arm_stp      i31, 6, i6, i7    
    10 : arm_stp      i31, 8, i8, i9    
    11 : arm_stp      i31, 10, i10, i11 
    12 : arm_stp      i31, 12, i12, i13 
    13 : arm_stp      i31, 14, i14, i15 
    14 : arm_stp      i31, 16, i16, i17 
    15 : add          i10, i31, 144     
    16 : vst_lane.u64 i10, v0           
    17 : vst_lane.u64 i10, v4           
    18 : vst_lane.u64 i10, v8           
    19 : vst_lane.u64 i10, v12          
    20 : call_noret   [i0]()            
    21 : add          i10, i31, 144     
    22 : vld_lane.u64 v0, i10           
    23 : vld_lane.u64 v4, i10           
    24 : vld_lane.u64 v8, i10           
    25 : vld_lane.u64 v12, i10          
    26 : arm_ldp      i0, i1, i31, 0    
    27 : arm_ldp      i2, i3, i31, 2    
    28 : arm_ldp      i4, i5, i31, 4    
    29 : arm_ldp      i6, i7, i31, 6    
    30 : arm_ldp      i8, i9, i31, 8    
    31 : arm_ldp      i10, i11, i31, 10 
    32 : arm_ldp      i12, i13, i31, 12 
    33 : arm_ldp      i14, i15, i31, 14 
    34 : arm_ldp      i16, i17, i31, 16 
    35 : arm_ldp      i29, i30, i31, 50 
    36 : add          i31, i31, 416     
    37 : ret                            
