fixed_power_v_uint32_t_0(i0, i1, i2)
     0 : sub      i31, i31, 16 
     1 : spill    0, i25       
     2 : mov      i3, 0        
     3 : mov      i25, 4       
     4 : mul      i2, i2, i25  
     5 : label 0:              
     6 : cmp      i3, i2       
     7 : jmp_ge 2              
     8 : vld.u32  v0, i0, i3   
     9 : mov      v0, 1        
    10 : vst.u32  i1, i3, v0   
    11 : add      i3, i3, 16   
    12 : jmp      0            
    13 : label 2:              
    14 : mov      i0, 0        
    15 : unspill  i25, 0       
    16 : add      i31, i31, 16 
    17 : ret                   
