nullify_msb_lsb_v(i0, i1, i2, i3)
     0 : sub      i31, i31, 32 
     1 : spill    1, i25       
     2 : spill    2, i26       
     3 : mov      i26, 0       
     4 : spill    0, i26       
     5 : mov      i25, 4       
     6 : mul      i3, i3, i25  
     7 : mov      v0, 1        
     8 : label 0:              
     9 : unspill  i26, 0       
    10 : cmp      i26, i3      
    11 : jmp_ge 2              
    12 : unspill  i26, 0       
    13 : vld.u32  v1, i0, i26  
    14 : shr.u32  v2, v1, 1    
    15 : or       v2, v1, v2   
    16 : shr.u32  v3, v2, 2    
    17 : or       v2, v2, v3   
    18 : shr.u32  v3, v2, 4    
    19 : or       v2, v2, v3   
    20 : shr.u32  v3, v2, 8    
    21 : or       v2, v2, v3   
    22 : shr.u32  v3, v2, 16   
    23 : or       v2, v2, v3   
    24 : add.u32  v2, v2, v0   
    25 : shr.u32  v2, v2, 1    
    26 : xor      v2, v2, v1   
    27 : unspill  i26, 0       
    28 : vst.u32  i1, i26, v2  
    29 : sub.u32  v2, v1, v0   
    30 : not      v2, v2       
    31 : and      v2, v1, v2   
    32 : xor      v1, v2, v1   
    33 : unspill  i26, 0       
    34 : vst.u32  i2, i26, v1  
    35 : unspill  i26, 0       
    36 : add      i26, i26, 16 
    37 : spill    0, i26       
    38 : jmp      0            
    39 : label 2:              
    40 : unspill  i25, 1       
    41 : unspill  i26, 2       
    42 : add      i31, i31, 32 
    43 : ret                   
