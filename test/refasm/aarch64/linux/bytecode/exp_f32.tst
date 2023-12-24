exp_f32(i0, i1, i2)
     0 : sub            i31, i31, 176      
     1 : spill          2, v8              
     2 : spill          4, v9              
     3 : spill          6, v10             
     4 : spill          8, v11             
     5 : spill          10, v12            
     6 : spill          12, v13            
     7 : spill          14, v14            
     8 : spill          16, v15            
     9 : spill          18, v16            
    10 : spill          20, v17            
    11 : mov            i3, 49317          
    12 : arm_movk       i3, 49840, 16      
    13 : broadcast.fp32 v0, i3             
    14 : mov            i3, 49317          
    15 : arm_movk       i3, 17072, 16      
    16 : broadcast.fp32 v1, i3             
    17 : mov            i3, 0              
    18 : arm_movk       i3, 16128, 16      
    19 : broadcast.fp32 v2, i3             
    20 : mov            i3, 0              
    21 : arm_movk       i3, 16256, 16      
    22 : broadcast.fp32 v3, i3             
    23 : mov            i3, 43579          
    24 : arm_movk       i3, 16312, 16      
    25 : broadcast.fp32 v4, i3             
    26 : mov            i3, 32768          
    27 : arm_movk       i3, 48945, 16      
    28 : broadcast.fp32 v5, i3             
    29 : mov            i3, 32899          
    30 : arm_movk       i3, 14686, 16      
    31 : broadcast.fp32 v6, i3             
    32 : mov            i3, 26983          
    33 : arm_movk       i3, 14672, 16      
    34 : broadcast.fp32 v7, i3             
    35 : mov            i3, 17358          
    36 : arm_movk       i3, 15031, 16      
    37 : broadcast.fp32 v8, i3             
    38 : mov            i3, 35080          
    39 : arm_movk       i3, 15368, 16      
    40 : broadcast.fp32 v9, i3             
    41 : mov            i3, 43457          
    42 : arm_movk       i3, 15658, 16      
    43 : broadcast.fp32 v10, i3            
    44 : mov            i3, 43690          
    45 : arm_movk       i3, 15914, 16      
    46 : broadcast.fp32 v11, i3            
    47 : mov            i3, 0              
    48 : arm_movk       i3, 16128, 16      
    49 : broadcast.fp32 v12, i3            
    50 : mov            v13, 127           
    51 : mov            i3, 0              
    52 : label 0:                          
    53 : cmp            i2, 0              
    54 : jmp_le 2                          
    55 : vld.fp32       v14, i1, i3        
    56 : min.fp32       v14, v14, v1       
    57 : max.fp32       v14, v14, v0       
    58 : mov            v15, v2            
    59 : fma.fp32       v15, v15, v14, v4  
    60 : floor.fp32_i32 v15, v15           
    61 : cast.i32_fp32  v16, v15           
    62 : fma.fp32       v14, v14, v16, v5  
    63 : fma.fp32       v14, v14, v16, v6  
    64 : mov            v16, v8            
    65 : fma.fp32       v16, v16, v14, v7  
    66 : spill          0, v0              
    67 : mov            v0, v16            
    68 : mov            v16, v9            
    69 : fma.fp32       v16, v16, v0, v14  
    70 : unspill        v0, 0              
    71 : spill          0, v0              
    72 : mov            v0, v16            
    73 : mov            v16, v10           
    74 : fma.fp32       v16, v16, v0, v14  
    75 : unspill        v0, 0              
    76 : spill          0, v0              
    77 : mov            v0, v16            
    78 : mov            v16, v11           
    79 : fma.fp32       v16, v16, v0, v14  
    80 : unspill        v0, 0              
    81 : spill          0, v0              
    82 : mov            v0, v16            
    83 : mov            v16, v12           
    84 : fma.fp32       v16, v16, v0, v14  
    85 : unspill        v0, 0              
    86 : mul.fp32       v17, v14, v14      
    87 : fma.fp32       v14, v14, v16, v17 
    88 : add.fp32       v14, v14, v3       
    89 : add.i32        v15, v15, v13      
    90 : sal.i32        v15, v15, 23       
    91 : mul.fp32       v14, v14, v15      
    92 : vst.fp32       i0, i3, v14        
    93 : add            i3, i3, 16         
    94 : sub            i2, i2, 4          
    95 : jmp            0                  
    96 : label 2:                          
    97 : unspill        v8, 2              
    98 : unspill        v9, 4              
    99 : unspill        v10, 6             
   100 : unspill        v11, 8             
   101 : unspill        v12, 10            
   102 : unspill        v13, 12            
   103 : unspill        v14, 14            
   104 : unspill        v15, 16            
   105 : unspill        v16, 18            
   106 : unspill        v17, 20            
   107 : add            i31, i31, 176      
   108 : ret                               
