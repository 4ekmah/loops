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
    33 : label 0:                   
    34 : unspill   i20, 2           
    35 : cmp       i20, i4          
    36 : jmp_ge    85               
    37 : mov       i19, 0           
    38 : cmp       i1, 0            
    39 : jmp_ne    2                
    40 : unspill   i20, 1           
    41 : load.u8   i19, 0, i0, i20  
    42 : jmp       42               
    43 : label 2:                   
    44 : cmp       i1, 1            
    45 : jmp_ne    5                
    46 : unspill   i20, 1           
    47 : load.i8   i19, 1, i0, i20  
    48 : jmp       41               
    49 : label 5:                   
    50 : cmp       i1, 2            
    51 : jmp_ne    9                
    52 : unspill   i20, 1           
    53 : load.u16  i19, 2, i0, i20  
    54 : jmp       40               
    55 : label 9:                   
    56 : cmp       i1, 3            
    57 : jmp_ne    14               
    58 : unspill   i20, 1           
    59 : load.i16  i19, 3, i0, i20  
    60 : jmp       39               
    61 : label 14:                  
    62 : cmp       i1, 4            
    63 : jmp_ne    20               
    64 : unspill   i20, 1           
    65 : load.u32  i19, 4, i0, i20  
    66 : jmp       38               
    67 : label 20:                  
    68 : cmp       i1, 5            
    69 : jmp_ne    27               
    70 : unspill   i20, 1           
    71 : load.i32  i19, 5, i0, i20  
    72 : jmp       37               
    73 : label 27:                  
    74 : cmp       i1, 6            
    75 : jmp_ne    35               
    76 : unspill   i20, 1           
    77 : load.u64  i19, 6, i0, i20  
    78 : jmp       36               
    79 : label 35:                  
    80 : unspill   i20, 1           
    81 : load.i64  i19, 7, i0, i20  
    82 : label 36:                  
    83 : label 37:                  
    84 : label 38:                  
    85 : label 39:                  
    86 : label 40:                  
    87 : label 41:                  
    88 : label 42:                  
    89 : cmp       i3, 0            
    90 : jmp_ne    44               
    91 : unspill   i20, 0           
    92 : store.u8  0, i2, i20, i19  
    93 : jmp       84               
    94 : label 44:                  
    95 : cmp       i3, 1            
    96 : jmp_ne    47               
    97 : unspill   i20, 0           
    98 : store.i8  1, i2, i20, i19  
    99 : jmp       83               
   100 : label 47:                  
   101 : cmp       i3, 2            
   102 : jmp_ne    51               
   103 : unspill   i20, 0           
   104 : store.u16 2, i2, i20, i19  
   105 : jmp       82               
   106 : label 51:                  
   107 : cmp       i3, 3            
   108 : jmp_ne    56               
   109 : unspill   i20, 0           
   110 : store.i16 3, i2, i20, i19  
   111 : jmp       81               
   112 : label 56:                  
   113 : cmp       i3, 4            
   114 : jmp_ne    62               
   115 : unspill   i20, 0           
   116 : store.u32 4, i2, i20, i19  
   117 : jmp       80               
   118 : label 62:                  
   119 : cmp       i3, 5            
   120 : jmp_ne    69               
   121 : unspill   i20, 0           
   122 : store.i32 5, i2, i20, i19  
   123 : jmp       79               
   124 : label 69:                  
   125 : cmp       i3, 6            
   126 : jmp_ne    77               
   127 : unspill   i20, 0           
   128 : store.u64 6, i2, i20, i19  
   129 : jmp       78               
   130 : label 77:                  
   131 : unspill   i20, 0           
   132 : store.i64 7, i2, i20, i19  
   133 : label 78:                  
   134 : label 79:                  
   135 : label 80:                  
   136 : label 81:                  
   137 : label 82:                  
   138 : label 83:                  
   139 : label 84:                  
   140 : unspill   i20, 1           
   141 : unspill   i21, 3           
   142 : add       i20, i20, i21    
   143 : spill     1, i20           
   144 : unspill   i20, 0           
   145 : add       i20, i20, i18    
   146 : spill     0, i20           
   147 : unspill   i20, 2           
   148 : add       i20, i20, 1      
   149 : spill     2, i20           
   150 : jmp       0                
   151 : label 85:                  
   152 : unspill   i18, 4           
   153 : unspill   i19, 5           
   154 : unspill   i20, 6           
   155 : unspill   i21, 7           
   156 : add       i31, i31, 64     
   157 : ret                        
