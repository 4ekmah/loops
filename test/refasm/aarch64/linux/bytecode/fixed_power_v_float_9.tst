fixed_power_v_float_9(i0, i1, i2)
     0 : sub      i31, i31, 16 
     1 : spill    0, i25       
     2 : mov      i3, 0        
     3 : mov      i25, 4       
     4 : mul      i2, i2, i25  
     5 : label 0:              
     6 : cmp      i3, i2       
     7 : jmp_ge 2              
     8 : vld.fp32 v0, i0, i3   
     9 : mul.fp32 v1, v0, v0   
    10 : mul.fp32 v1, v1, v1   
    11 : mul.fp32 v1, v1, v1   
    12 : mul.fp32 v0, v0, v1   
    13 : vst.fp32 i1, i3, v0   
    14 : add      i3, i3, 16   
    15 : jmp      0            
    16 : label 2:              
    17 : mov      i0, 0        
    18 : unspill  i25, 0       
    19 : add      i31, i31, 16 
    20 : ret                   
