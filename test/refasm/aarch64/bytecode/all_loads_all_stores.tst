all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i31, i31, 64      
     1 : spill     4, i18            
     2 : spill     5, i19            
     3 : spill     6, i20            
     4 : spill     7, i21            
     5 : mov       i20, 0            
     6 : spill     2, i20            
     7 : mov       i20, 0            
     8 : spill     1, i20            
     9 : mov       i20, 0            
    10 : spill     0, i20            
    11 : mov       i19, 2            
    12 : mov       i18, 1            
    13 : cmp       i1, 1             
    14 : select    i18, 28, i19, i18 
    15 : mov       i19, 4            
    16 : cmp       i1, 3             
    17 : select    i18, 28, i19, i18 
    18 : mov       i19, 8            
    19 : cmp       i1, 5             
    20 : select    i20, 28, i19, i18 
    21 : spill     3, i20            
    22 : mov       i19, 2            
    23 : mov       i18, 1            
    24 : cmp       i3, 1             
    25 : select    i18, 28, i19, i18 
    26 : mov       i19, 4            
    27 : cmp       i3, 3             
    28 : select    i18, 28, i19, i18 
    29 : mov       i19, 8            
    30 : cmp       i3, 5             
    31 : select    i18, 28, i19, i18 
    32 : label 0:                    
    33 : unspill   i20, 2            
    34 : cmp       i20, i4           
    35 : jmp_ge    85                
    36 : mov       i19, 0            
    37 : cmp       i1, 0             
    38 : jmp_ne    2                 
    39 : unspill   i20, 1            
    40 : load.u8   i19, i0, i20      
    41 : jmp       42                
    42 : label 2:                    
    43 : cmp       i1, 1             
    44 : jmp_ne    5                 
    45 : unspill   i20, 1            
    46 : load.i8   i19, i0, i20      
    47 : jmp       41                
    48 : label 5:                    
    49 : cmp       i1, 2             
    50 : jmp_ne    9                 
    51 : unspill   i20, 1            
    52 : load.u16  i19, i0, i20      
    53 : jmp       40                
    54 : label 9:                    
    55 : cmp       i1, 3             
    56 : jmp_ne    14                
    57 : unspill   i20, 1            
    58 : load.i16  i19, i0, i20      
    59 : jmp       39                
    60 : label 14:                   
    61 : cmp       i1, 4             
    62 : jmp_ne    20                
    63 : unspill   i20, 1            
    64 : load.u32  i19, i0, i20      
    65 : jmp       38                
    66 : label 20:                   
    67 : cmp       i1, 5             
    68 : jmp_ne    27                
    69 : unspill   i20, 1            
    70 : load.i32  i19, i0, i20      
    71 : jmp       37                
    72 : label 27:                   
    73 : cmp       i1, 6             
    74 : jmp_ne    35                
    75 : unspill   i20, 1            
    76 : load.u64  i19, i0, i20      
    77 : jmp       36                
    78 : label 35:                   
    79 : unspill   i20, 1            
    80 : load.i64  i19, i0, i20      
    81 : label 36:                   
    82 : label 37:                   
    83 : label 38:                   
    84 : label 39:                   
    85 : label 40:                   
    86 : label 41:                   
    87 : label 42:                   
    88 : cmp       i3, 0             
    89 : jmp_ne    44                
    90 : unspill   i20, 0            
    91 : store.u8  i2, i20, i19      
    92 : jmp       84                
    93 : label 44:                   
    94 : cmp       i3, 1             
    95 : jmp_ne    47                
    96 : unspill   i20, 0            
    97 : store.i8  i2, i20, i19      
    98 : jmp       83                
    99 : label 47:                   
   100 : cmp       i3, 2             
   101 : jmp_ne    51                
   102 : unspill   i20, 0            
   103 : store.u16 i2, i20, i19      
   104 : jmp       82                
   105 : label 51:                   
   106 : cmp       i3, 3             
   107 : jmp_ne    56                
   108 : unspill   i20, 0            
   109 : store.i16 i2, i20, i19      
   110 : jmp       81                
   111 : label 56:                   
   112 : cmp       i3, 4             
   113 : jmp_ne    62                
   114 : unspill   i20, 0            
   115 : store.u32 i2, i20, i19      
   116 : jmp       80                
   117 : label 62:                   
   118 : cmp       i3, 5             
   119 : jmp_ne    69                
   120 : unspill   i20, 0            
   121 : store.i32 i2, i20, i19      
   122 : jmp       79                
   123 : label 69:                   
   124 : cmp       i3, 6             
   125 : jmp_ne    77                
   126 : unspill   i20, 0            
   127 : store.u64 i2, i20, i19      
   128 : jmp       78                
   129 : label 77:                   
   130 : unspill   i20, 0            
   131 : store.i64 i2, i20, i19      
   132 : label 78:                   
   133 : label 79:                   
   134 : label 80:                   
   135 : label 81:                   
   136 : label 82:                   
   137 : label 83:                   
   138 : label 84:                   
   139 : unspill   i20, 1            
   140 : unspill   i21, 3            
   141 : add       i20, i20, i21     
   142 : spill     1, i20            
   143 : unspill   i20, 0            
   144 : add       i20, i20, i18     
   145 : spill     0, i20            
   146 : unspill   i20, 2            
   147 : add       i20, i20, 1       
   148 : spill     2, i20            
   149 : jmp       0                 
   150 : label 85:                   
   151 : unspill   i18, 4            
   152 : unspill   i19, 5            
   153 : unspill   i20, 6            
   154 : unspill   i21, 7            
   155 : add       i31, i31, 64      
   156 : ret                         
