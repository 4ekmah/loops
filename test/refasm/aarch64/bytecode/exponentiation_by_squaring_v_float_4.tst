exponentiation_by_squaring_v_float_4(i0, i1, i2)
     0 : sub      i31, i31, 16 
     1 : spill    0, i18       
     2 : mov      i3, 0        
     3 : mov      i18, 4       
     4 : mul      i2, i2, i18  
     5 : label 0:              
     6 : cmp      i3, i2       
     7 : jmp_ge   1            
     8 : vld.fp32 v0, i0, i3   
     9 : mul.fp32 v0, v0, v0   
    10 : mul.fp32 v0, v0, v0   
    11 : vst.fp32 i1, i3, v0   
    12 : add      i3, i3, 16   
    13 : jmp      0            
    14 : label 1:              
    15 : mov      i0, 0        
    16 : unspill  i18, 0       
    17 : add      i31, i31, 16 
    18 : ret                   
