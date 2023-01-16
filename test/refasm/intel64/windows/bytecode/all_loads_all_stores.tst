all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i4, i4, 56      
     1 : spill     5, i12          
     2 : spill     6, i13          
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
    29 : mov       s4, i0          
    30 : label 0:                  
    31 : unspill   i13, 12         
    32 : cmp       s2, i13         
    33 : jmp_ge    85              
    34 : mov       i12, 0          
    35 : cmp       i2, 0           
    36 : jmp_ne    2               
    37 : unspill   i13, 0          
    38 : load.u8   i0, 0, i1, i13  
    39 : mov       i12, i0         
    40 : jmp       42              
    41 : label 2:                  
    42 : cmp       i2, 1           
    43 : jmp_ne    5               
    44 : unspill   i13, 0          
    45 : load.i8   i0, 1, i1, i13  
    46 : mov       i12, i0         
    47 : jmp       41              
    48 : label 5:                  
    49 : cmp       i2, 2           
    50 : jmp_ne    9               
    51 : unspill   i13, 0          
    52 : load.u16  i0, 2, i1, i13  
    53 : mov       i12, i0         
    54 : jmp       40              
    55 : label 9:                  
    56 : cmp       i2, 3           
    57 : jmp_ne    14              
    58 : unspill   i13, 0          
    59 : load.i16  i0, 3, i1, i13  
    60 : mov       i12, i0         
    61 : jmp       39              
    62 : label 14:                 
    63 : cmp       i2, 4           
    64 : jmp_ne    20              
    65 : unspill   i13, 0          
    66 : load.u32  i0, 4, i1, i13  
    67 : mov       i12, i0         
    68 : jmp       38              
    69 : label 20:                 
    70 : cmp       i2, 5           
    71 : jmp_ne    27              
    72 : unspill   i13, 0          
    73 : load.i32  i0, 5, i1, i13  
    74 : mov       i12, i0         
    75 : jmp       37              
    76 : label 27:                 
    77 : cmp       i2, 6           
    78 : jmp_ne    35              
    79 : unspill   i13, 0          
    80 : load.u64  i0, 6, i1, i13  
    81 : mov       i12, i0         
    82 : jmp       36              
    83 : label 35:                 
    84 : unspill   i13, 0          
    85 : load.i64  i0, 7, i1, i13  
    86 : mov       i12, i0         
    87 : label 36:                 
    88 : label 37:                 
    89 : label 38:                 
    90 : label 39:                 
    91 : label 40:                 
    92 : label 41:                 
    93 : label 42:                 
    94 : cmp       i9, 0           
    95 : jmp_ne    44              
    96 : unspill   i13, 1          
    97 : store.u8  0, i8, i13, i12 
    98 : jmp       84              
    99 : label 44:                 
   100 : cmp       i9, 1           
   101 : jmp_ne    47              
   102 : unspill   i13, 1          
   103 : store.i8  1, i8, i13, i12 
   104 : jmp       83              
   105 : label 47:                 
   106 : cmp       i9, 2           
   107 : jmp_ne    51              
   108 : unspill   i13, 1          
   109 : store.u16 2, i8, i13, i12 
   110 : jmp       82              
   111 : label 51:                 
   112 : cmp       i9, 3           
   113 : jmp_ne    56              
   114 : unspill   i13, 1          
   115 : store.i16 3, i8, i13, i12 
   116 : jmp       81              
   117 : label 56:                 
   118 : cmp       i9, 4           
   119 : jmp_ne    62              
   120 : unspill   i13, 1          
   121 : store.u32 4, i8, i13, i12 
   122 : jmp       80              
   123 : label 62:                 
   124 : cmp       i9, 5           
   125 : jmp_ne    69              
   126 : unspill   i13, 1          
   127 : store.i32 5, i8, i13, i12 
   128 : jmp       79              
   129 : label 69:                 
   130 : cmp       i9, 6           
   131 : jmp_ne    77              
   132 : unspill   i13, 1          
   133 : store.u64 6, i8, i13, i12 
   134 : jmp       78              
   135 : label 77:                 
   136 : unspill   i13, 1          
   137 : store.i64 7, i8, i13, i12 
   138 : label 78:                 
   139 : label 79:                 
   140 : label 80:                 
   141 : label 81:                 
   142 : label 82:                 
   143 : label 83:                 
   144 : label 84:                 
   145 : unspill   i13, 3          
   146 : add       s0, s0, i13     
   147 : unspill   i13, 4          
   148 : add       s1, s1, i13     
   149 : add       s2, s2, 1       
   150 : jmp       0               
   151 : label 85:                 
   152 : unspill   i12, 5          
   153 : unspill   i13, 6          
   154 : add       i4, i4, 56      
   155 : ret                       
