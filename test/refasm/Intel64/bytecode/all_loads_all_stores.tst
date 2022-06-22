all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i4, i4, 56      
     1 : spill     4, i12          
     2 : spill     5, i13          
     3 : mov       s2, 0           
     4 : mov       s0, 0           
     5 : mov       s1, 0           
     6 : mov       i12, 1          
     7 : mov       i0, 2           
     8 : cmp       i2, 1           
     9 : select    i12, 3, i0, i12 
    10 : mov       i0, 4           
    11 : cmp       i2, 3           
    12 : select    i12, 3, i0, i12 
    13 : mov       i0, i12         
    14 : mov       i12, 8          
    15 : cmp       i2, 5           
    16 : select    i0, 3, i12, i0  
    17 : mov       s3, i0          
    18 : mov       i12, 1          
    19 : mov       i0, 2           
    20 : cmp       i9, 1           
    21 : select    i12, 3, i0, i12 
    22 : mov       i0, 4           
    23 : cmp       i9, 3           
    24 : select    i12, 3, i0, i12 
    25 : mov       i0, i12         
    26 : mov       i12, 8          
    27 : cmp       i9, 5           
    28 : select    i0, 3, i12, i0  
    29 : mov       i0, i0          
    30 : label 0:                  
    31 : mov       i12, 0          
    32 : cmp       i2, 0           
    33 : jmp_ne    1               
    34 : unspill   i13, 0          
    35 : load.u8   i12, 0, i1, i13 
    36 : mov       i12, i12        
    37 : jmp       14              
    38 : label 1:                  
    39 : cmp       i2, 1           
    40 : jmp_ne    2               
    41 : unspill   i13, 0          
    42 : load.i8   i12, 1, i1, i13 
    43 : mov       i12, i12        
    44 : jmp       13              
    45 : label 2:                  
    46 : cmp       i2, 2           
    47 : jmp_ne    3               
    48 : unspill   i13, 0          
    49 : load.u16  i12, 2, i1, i13 
    50 : mov       i12, i12        
    51 : jmp       12              
    52 : label 3:                  
    53 : cmp       i2, 3           
    54 : jmp_ne    4               
    55 : unspill   i13, 0          
    56 : load.i16  i12, 3, i1, i13 
    57 : mov       i12, i12        
    58 : jmp       11              
    59 : label 4:                  
    60 : cmp       i2, 4           
    61 : jmp_ne    5               
    62 : unspill   i13, 0          
    63 : load.u32  i12, 4, i1, i13 
    64 : mov       i12, i12        
    65 : jmp       10              
    66 : label 5:                  
    67 : cmp       i2, 5           
    68 : jmp_ne    6               
    69 : unspill   i13, 0          
    70 : load.i32  i12, 5, i1, i13 
    71 : mov       i12, i12        
    72 : jmp       9               
    73 : label 6:                  
    74 : cmp       i2, 6           
    75 : jmp_ne    7               
    76 : unspill   i13, 0          
    77 : load.u64  i12, 6, i1, i13 
    78 : mov       i12, i12        
    79 : jmp       8               
    80 : label 7:                  
    81 : unspill   i13, 0          
    82 : load.i64  i12, 7, i1, i13 
    83 : mov       i12, i12        
    84 : label 8:                  
    85 : label 9:                  
    86 : label 10:                 
    87 : label 11:                 
    88 : label 12:                 
    89 : label 13:                 
    90 : label 14:                 
    91 : cmp       i9, 0           
    92 : jmp_ne    15              
    93 : unspill   i13, 1          
    94 : store.u8  0, i8, i13, i12 
    95 : jmp       28              
    96 : label 15:                 
    97 : cmp       i9, 1           
    98 : jmp_ne    16              
    99 : unspill   i13, 1          
   100 : store.i8  1, i8, i13, i12 
   101 : jmp       27              
   102 : label 16:                 
   103 : cmp       i9, 2           
   104 : jmp_ne    17              
   105 : unspill   i13, 1          
   106 : store.u16 2, i8, i13, i12 
   107 : jmp       26              
   108 : label 17:                 
   109 : cmp       i9, 3           
   110 : jmp_ne    18              
   111 : unspill   i13, 1          
   112 : store.i16 3, i8, i13, i12 
   113 : jmp       25              
   114 : label 18:                 
   115 : cmp       i9, 4           
   116 : jmp_ne    19              
   117 : unspill   i13, 1          
   118 : store.u32 4, i8, i13, i12 
   119 : jmp       24              
   120 : label 19:                 
   121 : cmp       i9, 5           
   122 : jmp_ne    20              
   123 : unspill   i13, 1          
   124 : store.i32 5, i8, i13, i12 
   125 : jmp       23              
   126 : label 20:                 
   127 : cmp       i9, 6           
   128 : jmp_ne    21              
   129 : unspill   i13, 1          
   130 : store.u64 6, i8, i13, i12 
   131 : jmp       22              
   132 : label 21:                 
   133 : unspill   i13, 1          
   134 : store.i64 7, i8, i13, i12 
   135 : label 22:                 
   136 : label 23:                 
   137 : label 24:                 
   138 : label 25:                 
   139 : label 26:                 
   140 : label 27:                 
   141 : label 28:                 
   142 : unspill   i13, 3          
   143 : add       s0, s0, i13     
   144 : add       s1, s1, i0      
   145 : add       s2, s2, 1       
   146 : unspill   i13, 12         
   147 : cmp       s2, i13         
   148 : jmp_gt    0               
   149 : unspill   i12, 4          
   150 : unspill   i13, 5          
   151 : add       i4, i4, 56      
   152 : ret                       
