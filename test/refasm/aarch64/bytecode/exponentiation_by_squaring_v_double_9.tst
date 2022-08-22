exponentiation_by_squaring_v_double_9(i0, i1, i2)
     0 : sub      i31, i31, 16 
     1 : spill    0, i18       
     2 : mov      i3, 0        
     3 : mov      i18, 8       
     4 : mul      i2, i2, i18  
     5 : label 0:              
     6 : cmp      i3, i2       
     7 : jmp_ge   1            
     8 : vld.fp64 v0, i0, i3   
     9 : mov      v1, v0       
    10 : mul.fp64 v0, v0, v0   
    11 : mul.fp64 v0, v0, v0   
    12 : mul.fp64 v0, v0, v0   
    13 : mul.fp64 v0, v1, v0   
    14 : vst.fp64 i1, i3, v0   
    15 : add      i3, i3, 16   
    16 : jmp      0            
    17 : label 1:              
    18 : mov      i0, 0        
    19 : unspill  i18, 0       
    20 : add      i31, i31, 16 
    21 : ret                   
