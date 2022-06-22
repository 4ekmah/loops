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
    37 : mov       i19, 0           
    38 : cmp       i1, 0            
    39 : jmp_ne    1                
    40 : unspill   i20, 1           
    41 : load.u8   i19, 0, i0, i20  
    42 : mov       i19, i19         
    43 : jmp       14               
    44 : label 1:                   
    45 : cmp       i1, 1            
    46 : jmp_ne    2                
    47 : unspill   i20, 1           
    48 : load.i8   i19, 1, i0, i20  
    49 : mov       i19, i19         
    50 : jmp       13               
    51 : label 2:                   
    52 : cmp       i1, 2            
    53 : jmp_ne    3                
    54 : unspill   i20, 1           
    55 : load.u16  i19, 2, i0, i20  
    56 : mov       i19, i19         
    57 : jmp       12               
    58 : label 3:                   
    59 : cmp       i1, 3            
    60 : jmp_ne    4                
    61 : unspill   i20, 1           
    62 : load.i16  i19, 3, i0, i20  
    63 : mov       i19, i19         
    64 : jmp       11               
    65 : label 4:                   
    66 : cmp       i1, 4            
    67 : jmp_ne    5                
    68 : unspill   i20, 1           
    69 : load.u32  i19, 4, i0, i20  
    70 : mov       i19, i19         
    71 : jmp       10               
    72 : label 5:                   
    73 : cmp       i1, 5            
    74 : jmp_ne    6                
    75 : unspill   i20, 1           
    76 : load.i32  i19, 5, i0, i20  
    77 : mov       i19, i19         
    78 : jmp       9                
    79 : label 6:                   
    80 : cmp       i1, 6            
    81 : jmp_ne    7                
    82 : unspill   i20, 1           
    83 : load.u64  i19, 6, i0, i20  
    84 : mov       i19, i19         
    85 : jmp       8                
    86 : label 7:                   
    87 : unspill   i20, 1           
    88 : load.i64  i19, 7, i0, i20  
    89 : mov       i19, i19         
    90 : label 8:                   
    91 : label 9:                   
    92 : label 10:                  
    93 : label 11:                  
    94 : label 12:                  
    95 : label 13:                  
    96 : label 14:                  
    97 : cmp       i3, 0            
    98 : jmp_ne    15               
    99 : unspill   i20, 0           
   100 : store.u8  0, i2, i20, i19  
   101 : jmp       28               
   102 : label 15:                  
   103 : cmp       i3, 1            
   104 : jmp_ne    16               
   105 : unspill   i20, 0           
   106 : store.i8  1, i2, i20, i19  
   107 : jmp       27               
   108 : label 16:                  
   109 : cmp       i3, 2            
   110 : jmp_ne    17               
   111 : unspill   i20, 0           
   112 : store.u16 2, i2, i20, i19  
   113 : jmp       26               
   114 : label 17:                  
   115 : cmp       i3, 3            
   116 : jmp_ne    18               
   117 : unspill   i20, 0           
   118 : store.i16 3, i2, i20, i19  
   119 : jmp       25               
   120 : label 18:                  
   121 : cmp       i3, 4            
   122 : jmp_ne    19               
   123 : unspill   i20, 0           
   124 : store.u32 4, i2, i20, i19  
   125 : jmp       24               
   126 : label 19:                  
   127 : cmp       i3, 5            
   128 : jmp_ne    20               
   129 : unspill   i20, 0           
   130 : store.i32 5, i2, i20, i19  
   131 : jmp       23               
   132 : label 20:                  
   133 : cmp       i3, 6            
   134 : jmp_ne    21               
   135 : unspill   i20, 0           
   136 : store.u64 6, i2, i20, i19  
   137 : jmp       22               
   138 : label 21:                  
   139 : unspill   i20, 0           
   140 : store.i64 7, i2, i20, i19  
   141 : label 22:                  
   142 : label 23:                  
   143 : label 24:                  
   144 : label 25:                  
   145 : label 26:                  
   146 : label 27:                  
   147 : label 28:                  
   148 : unspill   i20, 1           
   149 : unspill   i21, 3           
   150 : add       i20, i20, i21    
   151 : spill     1, i20           
   152 : unspill   i20, 0           
   153 : add       i20, i20, i18    
   154 : spill     0, i20           
   155 : unspill   i20, 2           
   156 : add       i20, i20, 1      
   157 : spill     2, i20           
   158 : unspill   i20, 2           
   159 : cmp       i20, i4          
   160 : jmp_gt    0                
   161 : unspill   i18, 4           
   162 : unspill   i19, 5           
   163 : unspill   i20, 6           
   164 : unspill   i21, 7           
   165 : add       i31, i31, 64     
   166 : ret                        
