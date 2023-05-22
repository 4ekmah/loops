exp_f32(i0, i1, i2)
     0 : sub            i31, i31, 272   
     1 : spill          30, v29         
     2 : spill          32, v30         
     3 : mov            i3, 49317       
     4 : arm_movk       i3, 49840, 16   
     5 : broadcast.fp32 v29, i3         
     6 : spill          28, v29         
     7 : mov            i3, 49317       
     8 : arm_movk       i3, 17072, 16   
     9 : broadcast.fp32 v29, i3         
    10 : spill          26, v29         
    11 : mov            i3, 0           
    12 : arm_movk       i3, 16128, 16   
    13 : broadcast.fp32 v29, i3         
    14 : spill          24, v29         
    15 : mov            i3, 0           
    16 : arm_movk       i3, 16256, 16   
    17 : broadcast.fp32 v29, i3         
    18 : spill          22, v29         
    19 : mov            i3, 43579       
    20 : arm_movk       i3, 16312, 16   
    21 : broadcast.fp32 v29, i3         
    22 : spill          2, v29          
    23 : mov            i3, 32768       
    24 : arm_movk       i3, 48945, 16   
    25 : broadcast.fp32 v29, i3         
    26 : spill          4, v29          
    27 : mov            i3, 32899       
    28 : arm_movk       i3, 14686, 16   
    29 : broadcast.fp32 v29, i3         
    30 : spill          6, v29          
    31 : mov            i3, 26983       
    32 : arm_movk       i3, 14672, 16   
    33 : broadcast.fp32 v29, i3         
    34 : spill          8, v29          
    35 : mov            i3, 17358       
    36 : arm_movk       i3, 15031, 16   
    37 : broadcast.fp32 v29, i3         
    38 : spill          10, v29         
    39 : mov            i3, 35080       
    40 : arm_movk       i3, 15368, 16   
    41 : broadcast.fp32 v29, i3         
    42 : spill          12, v29         
    43 : mov            i3, 43457       
    44 : arm_movk       i3, 15658, 16   
    45 : broadcast.fp32 v29, i3         
    46 : spill          14, v29         
    47 : mov            i3, 43690       
    48 : arm_movk       i3, 15914, 16   
    49 : broadcast.fp32 v29, i3         
    50 : spill          16, v29         
    51 : mov            i3, 0           
    52 : arm_movk       i3, 16128, 16   
    53 : broadcast.fp32 v29, i3         
    54 : spill          18, v29         
    55 : mov            v29, 127        
    56 : spill          20, v29         
    57 : mov            i3, 0           
    58 : label 0:                       
    59 : cmp            i2, 0           
    60 : jmp_le 2                       
    61 : vld.fp32       v3, i1, i3      
    62 : unspill        v29, 26         
    63 : min.fp32       v3, v3, v29     
    64 : unspill        v29, 28         
    65 : max.fp32       v3, v3, v29     
    66 : unspill        v29, 24         
    67 : unspill        v30, 2          
    68 : mov            v2, v29         
    69 : fma.fp32       v2, v2, v3, v30 
    70 : floor.fp32_i32 v2, v2          
    71 : cast.i32_fp32  v1, v2          
    72 : unspill        v29, 4          
    73 : fma.fp32       v3, v3, v1, v29 
    74 : unspill        v29, 6          
    75 : fma.fp32       v3, v3, v1, v29 
    76 : unspill        v30, 8          
    77 : unspill        v29, 10         
    78 : mov            v1, v29         
    79 : fma.fp32       v1, v1, v3, v30 
    80 : unspill        v29, 12         
    81 : spill          0, v0           
    82 : mov            v0, v1          
    83 : mov            v1, v29         
    84 : fma.fp32       v1, v1, v0, v3  
    85 : unspill        v0, 0           
    86 : unspill        v29, 14         
    87 : spill          0, v0           
    88 : mov            v0, v1          
    89 : mov            v1, v29         
    90 : fma.fp32       v1, v1, v0, v3  
    91 : unspill        v0, 0           
    92 : unspill        v29, 16         
    93 : spill          0, v0           
    94 : mov            v0, v1          
    95 : mov            v1, v29         
    96 : fma.fp32       v1, v1, v0, v3  
    97 : unspill        v0, 0           
    98 : unspill        v29, 18         
    99 : spill          0, v0           
   100 : mov            v0, v1          
   101 : mov            v1, v29         
   102 : fma.fp32       v1, v1, v0, v3  
   103 : unspill        v0, 0           
   104 : mul.fp32       v0, v3, v3      
   105 : fma.fp32       v3, v3, v1, v0  
   106 : unspill        v29, 22         
   107 : add.fp32       v0, v3, v29     
   108 : unspill        v29, 20         
   109 : add.i32        v1, v2, v29     
   110 : sal.i32        v1, v1, 23      
   111 : mul.fp32       v0, v0, v1      
   112 : vst.fp32       i0, i3, v0      
   113 : add            i3, i3, 16      
   114 : sub            i2, i2, 4       
   115 : jmp            0               
   116 : label 2:                       
   117 : unspill        v29, 30         
   118 : unspill        v30, 32         
   119 : add            i31, i31, 272   
   120 : ret                            
