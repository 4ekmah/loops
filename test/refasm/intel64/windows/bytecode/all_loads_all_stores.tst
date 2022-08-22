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
    29 : label 0:                  
    30 : unspill   i13, 12         
    31 : cmp       s2, i13         
    32 : jmp_ge    85              
    33 : mov       i12, 0          
    34 : cmp       i2, 0           
    35 : jmp_ne    2               
    36 : unspill   i13, 0          
    37 : load.u8   i12, 0, i1, i13 
    38 : jmp       42              
    39 : label 2:                  
    40 : cmp       i2, 1           
    41 : jmp_ne    5               
    42 : unspill   i13, 0          
    43 : load.i8   i12, 1, i1, i13 
    44 : jmp       41              
    45 : label 5:                  
    46 : cmp       i2, 2           
    47 : jmp_ne    9               
    48 : unspill   i13, 0          
    49 : load.u16  i12, 2, i1, i13 
    50 : jmp       40              
    51 : label 9:                  
    52 : cmp       i2, 3           
    53 : jmp_ne    14              
    54 : unspill   i13, 0          
    55 : load.i16  i12, 3, i1, i13 
    56 : jmp       39              
    57 : label 14:                 
    58 : cmp       i2, 4           
    59 : jmp_ne    20              
    60 : unspill   i13, 0          
    61 : load.u32  i12, 4, i1, i13 
    62 : jmp       38              
    63 : label 20:                 
    64 : cmp       i2, 5           
    65 : jmp_ne    27              
    66 : unspill   i13, 0          
    67 : load.i32  i12, 5, i1, i13 
    68 : jmp       37              
    69 : label 27:                 
    70 : cmp       i2, 6           
    71 : jmp_ne    35              
    72 : unspill   i13, 0          
    73 : load.u64  i12, 6, i1, i13 
    74 : jmp       36              
    75 : label 35:                 
    76 : unspill   i13, 0          
    77 : load.i64  i12, 7, i1, i13 
    78 : label 36:                 
    79 : label 37:                 
    80 : label 38:                 
    81 : label 39:                 
    82 : label 40:                 
    83 : label 41:                 
    84 : label 42:                 
    85 : cmp       i9, 0           
    86 : jmp_ne    44              
    87 : unspill   i13, 1          
    88 : store.u8  0, i8, i13, i12 
    89 : jmp       84              
    90 : label 44:                 
    91 : cmp       i9, 1           
    92 : jmp_ne    47              
    93 : unspill   i13, 1          
    94 : store.i8  1, i8, i13, i12 
    95 : jmp       83              
    96 : label 47:                 
    97 : cmp       i9, 2           
    98 : jmp_ne    51              
    99 : unspill   i13, 1          
   100 : store.u16 2, i8, i13, i12 
   101 : jmp       82              
   102 : label 51:                 
   103 : cmp       i9, 3           
   104 : jmp_ne    56              
   105 : unspill   i13, 1          
   106 : store.i16 3, i8, i13, i12 
   107 : jmp       81              
   108 : label 56:                 
   109 : cmp       i9, 4           
   110 : jmp_ne    62              
   111 : unspill   i13, 1          
   112 : store.u32 4, i8, i13, i12 
   113 : jmp       80              
   114 : label 62:                 
   115 : cmp       i9, 5           
   116 : jmp_ne    69              
   117 : unspill   i13, 1          
   118 : store.i32 5, i8, i13, i12 
   119 : jmp       79              
   120 : label 69:                 
   121 : cmp       i9, 6           
   122 : jmp_ne    77              
   123 : unspill   i13, 1          
   124 : store.u64 6, i8, i13, i12 
   125 : jmp       78              
   126 : label 77:                 
   127 : unspill   i13, 1          
   128 : store.i64 7, i8, i13, i12 
   129 : label 78:                 
   130 : label 79:                 
   131 : label 80:                 
   132 : label 81:                 
   133 : label 82:                 
   134 : label 83:                 
   135 : label 84:                 
   136 : unspill   i13, 3          
   137 : add       s0, s0, i13     
   138 : add       s1, s1, i0      
   139 : add       s2, s2, 1       
   140 : jmp       0               
   141 : label 85:                 
   142 : unspill   i12, 4          
   143 : unspill   i13, 5          
   144 : add       i4, i4, 56      
   145 : ret                       
