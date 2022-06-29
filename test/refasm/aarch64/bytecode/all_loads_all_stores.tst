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
    11 : cmp       i1, 1            
    12 : mov       i19, 2           
    13 : mov       i18, 1           
    14 : select    i18, 3, i19, i18 
    15 : cmp       i1, 3            
    16 : mov       i19, 4           
    17 : select    i18, 3, i19, i18 
    18 : mov       i18, i18         
    19 : cmp       i1, 5            
    20 : mov       i19, 8           
    21 : select    i18, 3, i19, i18 
    22 : mov       i20, i18         
    23 : spill     3, i20           
    24 : cmp       i3, 1            
    25 : mov       i19, 2           
    26 : mov       i18, 1           
    27 : select    i18, 3, i19, i18 
    28 : cmp       i3, 3            
    29 : mov       i19, 4           
    30 : select    i18, 3, i19, i18 
    31 : mov       i18, i18         
    32 : cmp       i3, 5            
    33 : mov       i19, 8           
    34 : select    i18, 3, i19, i18 
    35 : mov       i18, i18         
    36 : label 0:                   
    37 : unspill   i20, 2           
    38 : cmp       i20, i4          
    39 : jmp_ge    85               
    40 : mov       i19, 0           
    41 : cmp       i1, 0            
    42 : jmp_ne    2                
    43 : unspill   i20, 1           
    44 : load.u8   i19, 0, i0, i20  
    45 : mov       i19, i19         
    46 : jmp       42               
    47 : label 2:                   
    48 : cmp       i1, 1            
    49 : jmp_ne    5                
    50 : unspill   i20, 1           
    51 : load.i8   i19, 1, i0, i20  
    52 : mov       i19, i19         
    53 : jmp       41               
    54 : label 5:                   
    55 : cmp       i1, 2            
    56 : jmp_ne    9                
    57 : unspill   i20, 1           
    58 : load.u16  i19, 2, i0, i20  
    59 : mov       i19, i19         
    60 : jmp       40               
    61 : label 9:                   
    62 : cmp       i1, 3            
    63 : jmp_ne    14               
    64 : unspill   i20, 1           
    65 : load.i16  i19, 3, i0, i20  
    66 : mov       i19, i19         
    67 : jmp       39               
    68 : label 14:                  
    69 : cmp       i1, 4            
    70 : jmp_ne    20               
    71 : unspill   i20, 1           
    72 : load.u32  i19, 4, i0, i20  
    73 : mov       i19, i19         
    74 : jmp       38               
    75 : label 20:                  
    76 : cmp       i1, 5            
    77 : jmp_ne    27               
    78 : unspill   i20, 1           
    79 : load.i32  i19, 5, i0, i20  
    80 : mov       i19, i19         
    81 : jmp       37               
    82 : label 27:                  
    83 : cmp       i1, 6            
    84 : jmp_ne    35               
    85 : unspill   i20, 1           
    86 : load.u64  i19, 6, i0, i20  
    87 : mov       i19, i19         
    88 : jmp       36               
    89 : label 35:                  
    90 : unspill   i20, 1           
    91 : load.i64  i19, 7, i0, i20  
    92 : mov       i19, i19         
    93 : label 36:                  
    94 : label 37:                  
    95 : label 38:                  
    96 : label 39:                  
    97 : label 40:                  
    98 : label 41:                  
    99 : label 42:                  
   100 : cmp       i3, 0            
   101 : jmp_ne    44               
   102 : unspill   i20, 0           
   103 : store.u8  0, i2, i20, i19  
   104 : jmp       84               
   105 : label 44:                  
   106 : cmp       i3, 1            
   107 : jmp_ne    47               
   108 : unspill   i20, 0           
   109 : store.i8  1, i2, i20, i19  
   110 : jmp       83               
   111 : label 47:                  
   112 : cmp       i3, 2            
   113 : jmp_ne    51               
   114 : unspill   i20, 0           
   115 : store.u16 2, i2, i20, i19  
   116 : jmp       82               
   117 : label 51:                  
   118 : cmp       i3, 3            
   119 : jmp_ne    56               
   120 : unspill   i20, 0           
   121 : store.i16 3, i2, i20, i19  
   122 : jmp       81               
   123 : label 56:                  
   124 : cmp       i3, 4            
   125 : jmp_ne    62               
   126 : unspill   i20, 0           
   127 : store.u32 4, i2, i20, i19  
   128 : jmp       80               
   129 : label 62:                  
   130 : cmp       i3, 5            
   131 : jmp_ne    69               
   132 : unspill   i20, 0           
   133 : store.i32 5, i2, i20, i19  
   134 : jmp       79               
   135 : label 69:                  
   136 : cmp       i3, 6            
   137 : jmp_ne    77               
   138 : unspill   i20, 0           
   139 : store.u64 6, i2, i20, i19  
   140 : jmp       78               
   141 : label 77:                  
   142 : unspill   i20, 0           
   143 : store.i64 7, i2, i20, i19  
   144 : label 78:                  
   145 : label 79:                  
   146 : label 80:                  
   147 : label 81:                  
   148 : label 82:                  
   149 : label 83:                  
   150 : label 84:                  
   151 : unspill   i20, 1           
   152 : unspill   i21, 3           
   153 : add       i20, i20, i21    
   154 : spill     1, i20           
   155 : unspill   i20, 0           
   156 : add       i20, i20, i18    
   157 : spill     0, i20           
   158 : unspill   i20, 2           
   159 : add       i20, i20, 1      
   160 : spill     2, i20           
   161 : jmp       0                
   162 : label 85:                  
   163 : unspill   i18, 4           
   164 : unspill   i19, 5           
   165 : unspill   i20, 6           
   166 : unspill   i21, 7           
   167 : add       i31, i31, 64     
   168 : ret                        
