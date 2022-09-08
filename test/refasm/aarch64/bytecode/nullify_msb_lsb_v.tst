nullify_msb_lsb_v(i0, i1, i2, i3)
     0 : sub      i31, i31, 16 
     1 : spill    0, i18       
     2 : spill    1, i19       
     3 : mov      i18, 0       
     4 : mov      i19, 4       
     5 : mul      i3, i3, i19  
     6 : mov      v0, 1        
     7 : label 0:              
     8 : cmp      i18, i3      
     9 : jmp_ge   1            
    10 : vld.u32  v1, i0, i18  
    11 : shr.u32  v2, v1, 1    
    12 : or       v2, v1, v2   
    13 : shr.u32  v3, v2, 2    
    14 : or       v2, v2, v3   
    15 : shr.u32  v3, v2, 4    
    16 : or       v2, v2, v3   
    17 : shr.u32  v3, v2, 8    
    18 : or       v2, v2, v3   
    19 : shr.u32  v3, v2, 16   
    20 : or       v2, v2, v3   
    21 : add.u32  v2, v2, v0   
    22 : shr.u32  v2, v2, 1    
    23 : xor      v2, v2, v1   
    24 : vst.u32  i1, i18, v2  
    25 : sub.u32  v2, v1, v0   
    26 : not      v2, v2       
    27 : and      v2, v1, v2   
    28 : xor      v1, v2, v1   
    29 : vst.u32  i2, i18, v1  
    30 : add      i18, i18, 16 
    31 : jmp      0            
    32 : label 1:              
    33 : unspill  i18, 0       
    34 : unspill  i19, 1       
    35 : add      i31, i31, 16 
    36 : ret                   
