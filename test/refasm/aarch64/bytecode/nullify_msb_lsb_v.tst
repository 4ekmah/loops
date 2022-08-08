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
    23 : mov      v2, v2       
    24 : xor      v2, v2, v1   
    25 : vst.u32  i1, i18, v2  
    26 : sub.u32  v2, v1, v0   
    27 : not      v2, v2       
    28 : and      v2, v1, v2   
    29 : xor      v1, v2, v1   
    30 : vst.u32  i2, i18, v1  
    31 : add      i18, i18, 16 
    32 : jmp      0            
    33 : label 1:              
    34 : unspill  i18, 0       
    35 : unspill  i19, 1       
    36 : add      i31, i31, 16 
    37 : ret                   
