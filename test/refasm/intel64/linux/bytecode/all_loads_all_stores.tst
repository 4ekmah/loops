all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i4, i4, 56      
     1 : spill     4, i12          
     2 : spill     5, i13          
     3 : mov       s2, 0           
     4 : mov       s1, 0           
     5 : mov       s0, 0           
     6 : mov       i12, 1          
     7 : mov       i0, 2           
     8 : cmp       i6, 1           
     9 : select    i12, 3, i0, i12 
    10 : mov       i0, 4           
    11 : cmp       i6, 3           
    12 : select    i12, 3, i0, i12 
    13 : mov       i0, i12         
    14 : mov       i12, 8          
    15 : cmp       i6, 5           
    16 : select    i0, 3, i12, i0  
    17 : mov       s3, i0          
    18 : mov       i12, 1          
    19 : mov       i0, 2           
    20 : cmp       i1, 1           
    21 : select    i12, 3, i0, i12 
    22 : mov       i0, 4           
    23 : cmp       i1, 3           
    24 : select    i12, 3, i0, i12 
    25 : mov       i0, i12         
    26 : mov       i12, 8          
    27 : cmp       i1, 5           
    28 : select    i0, 3, i12, i0  
    29 : label 0:                  
    30 : cmp       s2, i8          
    31 : jmp_ge    85              
    32 : mov       i12, 0          
    33 : cmp       i6, 0           
    34 : jmp_ne    2               
    35 : unspill   i13, 1          
    36 : load.u8   i12, 0, i7, i13 
    37 : jmp       42              
    38 : label 2:                  
    39 : cmp       i6, 1           
    40 : jmp_ne    5               
    41 : unspill   i13, 1          
    42 : load.i8   i12, 1, i7, i13 
    43 : jmp       41              
    44 : label 5:                  
    45 : cmp       i6, 2           
    46 : jmp_ne    9               
    47 : unspill   i13, 1          
    48 : load.u16  i12, 2, i7, i13 
    49 : jmp       40              
    50 : label 9:                  
    51 : cmp       i6, 3           
    52 : jmp_ne    14              
    53 : unspill   i13, 1          
    54 : load.i16  i12, 3, i7, i13 
    55 : jmp       39              
    56 : label 14:                 
    57 : cmp       i6, 4           
    58 : jmp_ne    20              
    59 : unspill   i13, 1          
    60 : load.u32  i12, 4, i7, i13 
    61 : jmp       38              
    62 : label 20:                 
    63 : cmp       i6, 5           
    64 : jmp_ne    27              
    65 : unspill   i13, 1          
    66 : load.i32  i12, 5, i7, i13 
    67 : jmp       37              
    68 : label 27:                 
    69 : cmp       i6, 6           
    70 : jmp_ne    35              
    71 : unspill   i13, 1          
    72 : load.u64  i12, 6, i7, i13 
    73 : jmp       36              
    74 : label 35:                 
    75 : unspill   i13, 1          
    76 : load.i64  i12, 7, i7, i13 
    77 : label 36:                 
    78 : label 37:                 
    79 : label 38:                 
    80 : label 39:                 
    81 : label 40:                 
    82 : label 41:                 
    83 : label 42:                 
    84 : cmp       i1, 0           
    85 : jmp_ne    44              
    86 : unspill   i13, 0          
    87 : store.u8  0, i2, i13, i12 
    88 : jmp       84              
    89 : label 44:                 
    90 : cmp       i1, 1           
    91 : jmp_ne    47              
    92 : unspill   i13, 0          
    93 : store.i8  1, i2, i13, i12 
    94 : jmp       83              
    95 : label 47:                 
    96 : cmp       i1, 2           
    97 : jmp_ne    51              
    98 : unspill   i13, 0          
    99 : store.u16 2, i2, i13, i12 
   100 : jmp       82              
   101 : label 51:                 
   102 : cmp       i1, 3           
   103 : jmp_ne    56              
   104 : unspill   i13, 0          
   105 : store.i16 3, i2, i13, i12 
   106 : jmp       81              
   107 : label 56:                 
   108 : cmp       i1, 4           
   109 : jmp_ne    62              
   110 : unspill   i13, 0          
   111 : store.u32 4, i2, i13, i12 
   112 : jmp       80              
   113 : label 62:                 
   114 : cmp       i1, 5           
   115 : jmp_ne    69              
   116 : unspill   i13, 0          
   117 : store.i32 5, i2, i13, i12 
   118 : jmp       79              
   119 : label 69:                 
   120 : cmp       i1, 6           
   121 : jmp_ne    77              
   122 : unspill   i13, 0          
   123 : store.u64 6, i2, i13, i12 
   124 : jmp       78              
   125 : label 77:                 
   126 : unspill   i13, 0          
   127 : store.i64 7, i2, i13, i12 
   128 : label 78:                 
   129 : label 79:                 
   130 : label 80:                 
   131 : label 81:                 
   132 : label 82:                 
   133 : label 83:                 
   134 : label 84:                 
   135 : unspill   i13, 3          
   136 : add       s1, s1, i13     
   137 : add       s0, s0, i0      
   138 : add       s2, s2, 1       
   139 : jmp       0               
   140 : label 85:                 
   141 : unspill   i12, 4          
   142 : unspill   i13, 5          
   143 : add       i4, i4, 56      
   144 : ret                       
