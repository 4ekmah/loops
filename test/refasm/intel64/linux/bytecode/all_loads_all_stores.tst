all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i4, i4, 56      
     1 : spill     5, i12          
     2 : spill     6, i13          
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
    29 : mov       s4, i0          
    30 : label 0:                  
    31 : cmp       s2, i8          
    32 : jmp_ge    85              
    33 : mov       i12, 0          
    34 : cmp       i6, 0           
    35 : jmp_ne    2               
    36 : unspill   i13, 1          
    37 : load.u8   i0, 0, i7, i13  
    38 : mov       i12, i0         
    39 : jmp       42              
    40 : label 2:                  
    41 : cmp       i6, 1           
    42 : jmp_ne    5               
    43 : unspill   i13, 1          
    44 : load.i8   i0, 1, i7, i13  
    45 : mov       i12, i0         
    46 : jmp       41              
    47 : label 5:                  
    48 : cmp       i6, 2           
    49 : jmp_ne    9               
    50 : unspill   i13, 1          
    51 : load.u16  i0, 2, i7, i13  
    52 : mov       i12, i0         
    53 : jmp       40              
    54 : label 9:                  
    55 : cmp       i6, 3           
    56 : jmp_ne    14              
    57 : unspill   i13, 1          
    58 : load.i16  i0, 3, i7, i13  
    59 : mov       i12, i0         
    60 : jmp       39              
    61 : label 14:                 
    62 : cmp       i6, 4           
    63 : jmp_ne    20              
    64 : unspill   i13, 1          
    65 : load.u32  i0, 4, i7, i13  
    66 : mov       i12, i0         
    67 : jmp       38              
    68 : label 20:                 
    69 : cmp       i6, 5           
    70 : jmp_ne    27              
    71 : unspill   i13, 1          
    72 : load.i32  i0, 5, i7, i13  
    73 : mov       i12, i0         
    74 : jmp       37              
    75 : label 27:                 
    76 : cmp       i6, 6           
    77 : jmp_ne    35              
    78 : unspill   i13, 1          
    79 : load.u64  i0, 6, i7, i13  
    80 : mov       i12, i0         
    81 : jmp       36              
    82 : label 35:                 
    83 : unspill   i13, 1          
    84 : load.i64  i0, 7, i7, i13  
    85 : mov       i12, i0         
    86 : label 36:                 
    87 : label 37:                 
    88 : label 38:                 
    89 : label 39:                 
    90 : label 40:                 
    91 : label 41:                 
    92 : label 42:                 
    93 : cmp       i1, 0           
    94 : jmp_ne    44              
    95 : unspill   i13, 0          
    96 : store.u8  0, i2, i13, i12 
    97 : jmp       84              
    98 : label 44:                 
    99 : cmp       i1, 1           
   100 : jmp_ne    47              
   101 : unspill   i13, 0          
   102 : store.i8  1, i2, i13, i12 
   103 : jmp       83              
   104 : label 47:                 
   105 : cmp       i1, 2           
   106 : jmp_ne    51              
   107 : unspill   i13, 0          
   108 : store.u16 2, i2, i13, i12 
   109 : jmp       82              
   110 : label 51:                 
   111 : cmp       i1, 3           
   112 : jmp_ne    56              
   113 : unspill   i13, 0          
   114 : store.i16 3, i2, i13, i12 
   115 : jmp       81              
   116 : label 56:                 
   117 : cmp       i1, 4           
   118 : jmp_ne    62              
   119 : unspill   i13, 0          
   120 : store.u32 4, i2, i13, i12 
   121 : jmp       80              
   122 : label 62:                 
   123 : cmp       i1, 5           
   124 : jmp_ne    69              
   125 : unspill   i13, 0          
   126 : store.i32 5, i2, i13, i12 
   127 : jmp       79              
   128 : label 69:                 
   129 : cmp       i1, 6           
   130 : jmp_ne    77              
   131 : unspill   i13, 0          
   132 : store.u64 6, i2, i13, i12 
   133 : jmp       78              
   134 : label 77:                 
   135 : unspill   i13, 0          
   136 : store.i64 7, i2, i13, i12 
   137 : label 78:                 
   138 : label 79:                 
   139 : label 80:                 
   140 : label 81:                 
   141 : label 82:                 
   142 : label 83:                 
   143 : label 84:                 
   144 : unspill   i13, 3          
   145 : add       s1, s1, i13     
   146 : unspill   i13, 4          
   147 : add       s0, s0, i13     
   148 : add       s2, s2, 1       
   149 : jmp       0               
   150 : label 85:                 
   151 : unspill   i12, 5          
   152 : unspill   i13, 6          
   153 : add       i4, i4, 56      
   154 : ret                       
