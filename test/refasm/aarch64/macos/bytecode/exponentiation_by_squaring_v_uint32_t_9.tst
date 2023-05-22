exponentiation_by_squaring_v_uint32_t_9(i0, i1, i2)
     0 : sub      i31, i31, 16 
     1 : spill    0, i18       
     2 : mov      i3, 0        
     3 : mov      i18, 4       
     4 : mul      i2, i2, i18  
     5 : label 0:              
     6 : cmp      i3, i2       
     7 : jmp_ge 2              
     8 : vld.u32  v0, i0, i3   
     9 : mul.u32  v1, v0, v0   
    10 : mul.u32  v1, v1, v1   
    11 : mul.u32  v1, v1, v1   
    12 : mul.u32  v0, v0, v1   
    13 : vst.u32  i1, i3, v0   
    14 : add      i3, i3, 16   
    15 : jmp      0            
    16 : label 2:              
    17 : mov      i0, 0        
    18 : unspill  i18, 0       
    19 : add      i31, i31, 16 
    20 : ret                   
