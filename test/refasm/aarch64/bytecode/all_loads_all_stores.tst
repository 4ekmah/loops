all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i31, i31, 80     
     1 : spill     5, i18           
     2 : spill     6, i19           
     3 : spill     7, i20           
     4 : spill     8, i21           
     5 : mov       i20, 0           
     6 : spill     2, i20           
     7 : mov       i20, 0           
     8 : spill     1, i20           
     9 : mov       i20, 0           
    10 : spill     0, i20           
    11 : cmp       i1, 1            
    12 : mov       i19, 2           
    13 : mov       i18, 1           
    14 : select    i18, 3, i19, i18 
    15 : cmp       i1, 3            
    16 : mov       i19, 4           
    17 : select    i18, 3, i19, i18 
    18 : cmp       i1, 5            
    19 : mov       i19, 8           
    20 : select    i18, 3, i19, i18 
    21 : mov       i20, i18         
    22 : spill     3, i20           
    23 : cmp       i3, 1            
    24 : mov       i19, 2           
    25 : mov       i18, 1           
    26 : select    i18, 3, i19, i18 
    27 : cmp       i3, 3            
    28 : mov       i19, 4           
    29 : select    i18, 3, i19, i18 
    30 : cmp       i3, 5            
    31 : mov       i19, 8           
    32 : select    i18, 3, i19, i18 
    33 : mov       i20, i18         
    34 : spill     4, i20           
    35 : label 0:                   
    36 : unspill   i20, 2           
    37 : cmp       i20, i4          
    38 : jmp_ge    85               
    39 : mov       i19, 0           
    40 : cmp       i1, 0            
    41 : jmp_ne    2                
    42 : unspill   i20, 1           
    43 : load.u8   i18, 0, i0, i20  
    44 : mov       i19, i18         
    45 : jmp       42               
    46 : label 2:                   
    47 : cmp       i1, 1            
    48 : jmp_ne    5                
    49 : unspill   i20, 1           
    50 : load.i8   i18, 1, i0, i20  
    51 : mov       i19, i18         
    52 : jmp       41               
    53 : label 5:                   
    54 : cmp       i1, 2            
    55 : jmp_ne    9                
    56 : unspill   i20, 1           
    57 : load.u16  i18, 2, i0, i20  
    58 : mov       i19, i18         
    59 : jmp       40               
    60 : label 9:                   
    61 : cmp       i1, 3            
    62 : jmp_ne    14               
    63 : unspill   i20, 1           
    64 : load.i16  i18, 3, i0, i20  
    65 : mov       i19, i18         
    66 : jmp       39               
    67 : label 14:                  
    68 : cmp       i1, 4            
    69 : jmp_ne    20               
    70 : unspill   i20, 1           
    71 : load.u32  i18, 4, i0, i20  
    72 : mov       i19, i18         
    73 : jmp       38               
    74 : label 20:                  
    75 : cmp       i1, 5            
    76 : jmp_ne    27               
    77 : unspill   i20, 1           
    78 : load.i32  i18, 5, i0, i20  
    79 : mov       i19, i18         
    80 : jmp       37               
    81 : label 27:                  
    82 : cmp       i1, 6            
    83 : jmp_ne    35               
    84 : unspill   i20, 1           
    85 : load.u64  i18, 6, i0, i20  
    86 : mov       i19, i18         
    87 : jmp       36               
    88 : label 35:                  
    89 : unspill   i20, 1           
    90 : load.i64  i18, 7, i0, i20  
    91 : mov       i19, i18         
    92 : label 36:                  
    93 : label 37:                  
    94 : label 38:                  
    95 : label 39:                  
    96 : label 40:                  
    97 : label 41:                  
    98 : label 42:                  
    99 : cmp       i3, 0            
   100 : jmp_ne    44               
   101 : unspill   i20, 0           
   102 : store.u8  0, i2, i20, i19  
   103 : jmp       84               
   104 : label 44:                  
   105 : cmp       i3, 1            
   106 : jmp_ne    47               
   107 : unspill   i20, 0           
   108 : store.i8  1, i2, i20, i19  
   109 : jmp       83               
   110 : label 47:                  
   111 : cmp       i3, 2            
   112 : jmp_ne    51               
   113 : unspill   i20, 0           
   114 : store.u16 2, i2, i20, i19  
   115 : jmp       82               
   116 : label 51:                  
   117 : cmp       i3, 3            
   118 : jmp_ne    56               
   119 : unspill   i20, 0           
   120 : store.i16 3, i2, i20, i19  
   121 : jmp       81               
   122 : label 56:                  
   123 : cmp       i3, 4            
   124 : jmp_ne    62               
   125 : unspill   i20, 0           
   126 : store.u32 4, i2, i20, i19  
   127 : jmp       80               
   128 : label 62:                  
   129 : cmp       i3, 5            
   130 : jmp_ne    69               
   131 : unspill   i20, 0           
   132 : store.i32 5, i2, i20, i19  
   133 : jmp       79               
   134 : label 69:                  
   135 : cmp       i3, 6            
   136 : jmp_ne    77               
   137 : unspill   i20, 0           
   138 : store.u64 6, i2, i20, i19  
   139 : jmp       78               
   140 : label 77:                  
   141 : unspill   i20, 0           
   142 : store.i64 7, i2, i20, i19  
   143 : label 78:                  
   144 : label 79:                  
   145 : label 80:                  
   146 : label 81:                  
   147 : label 82:                  
   148 : label 83:                  
   149 : label 84:                  
   150 : unspill   i20, 1           
   151 : unspill   i21, 3           
   152 : add       i20, i20, i21    
   153 : spill     1, i20           
   154 : unspill   i20, 0           
   155 : unspill   i21, 4           
   156 : add       i20, i20, i21    
   157 : spill     0, i20           
   158 : unspill   i20, 2           
   159 : add       i20, i20, 1      
   160 : spill     2, i20           
   161 : jmp       0                
   162 : label 85:                  
   163 : unspill   i18, 5           
   164 : unspill   i19, 6           
   165 : unspill   i20, 7           
   166 : unspill   i21, 8           
   167 : add       i31, i31, 80     
   168 : ret                        
