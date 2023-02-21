all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i4, i4, 56        
     1 : spill     4, i12            
     2 : spill     5, i13            
     3 : mov       s2, 0             
     4 : mov       s1, 0             
     5 : mov       s0, 0             
     6 : mov       i12, 2            
     7 : mov       i0, 1             
     8 : cmp       i6, 1             
     9 : select    i0, 28, i12, i0   
    10 : mov       i12, 4            
    11 : cmp       i6, 3             
    12 : select    i0, 28, i12, i0   
    13 : mov       i12, 8            
    14 : cmp       i6, 5             
    15 : mov       i13, i0           
    16 : select    i13, 28, i12, i13 
    17 : spill     3, i13            
    18 : mov       i12, 2            
    19 : mov       i0, 1             
    20 : cmp       i1, 1             
    21 : select    i0, 28, i12, i0   
    22 : mov       i12, 4            
    23 : cmp       i1, 3             
    24 : select    i0, 28, i12, i0   
    25 : mov       i12, 8            
    26 : cmp       i1, 5             
    27 : select    i0, 28, i12, i0   
    28 : label 0:                    
    29 : cmp       s2, i8            
    30 : jmp_ge    85                
    31 : mov       i12, 0            
    32 : cmp       i6, 0             
    33 : jmp_ne    2                 
    34 : unspill   i13, 1            
    35 : load.u8   i12, i7, i13      
    36 : jmp       42                
    37 : label 2:                    
    38 : cmp       i6, 1             
    39 : jmp_ne    5                 
    40 : unspill   i13, 1            
    41 : load.i8   i12, i7, i13      
    42 : jmp       41                
    43 : label 5:                    
    44 : cmp       i6, 2             
    45 : jmp_ne    9                 
    46 : unspill   i13, 1            
    47 : load.u16  i12, i7, i13      
    48 : jmp       40                
    49 : label 9:                    
    50 : cmp       i6, 3             
    51 : jmp_ne    14                
    52 : unspill   i13, 1            
    53 : load.i16  i12, i7, i13      
    54 : jmp       39                
    55 : label 14:                   
    56 : cmp       i6, 4             
    57 : jmp_ne    20                
    58 : unspill   i13, 1            
    59 : load.u32  i12, i7, i13      
    60 : jmp       38                
    61 : label 20:                   
    62 : cmp       i6, 5             
    63 : jmp_ne    27                
    64 : unspill   i13, 1            
    65 : load.i32  i12, i7, i13      
    66 : jmp       37                
    67 : label 27:                   
    68 : cmp       i6, 6             
    69 : jmp_ne    35                
    70 : unspill   i13, 1            
    71 : load.u64  i12, i7, i13      
    72 : jmp       36                
    73 : label 35:                   
    74 : unspill   i13, 1            
    75 : load.i64  i12, i7, i13      
    76 : label 36:                   
    77 : label 37:                   
    78 : label 38:                   
    79 : label 39:                   
    80 : label 40:                   
    81 : label 41:                   
    82 : label 42:                   
    83 : cmp       i1, 0             
    84 : jmp_ne    44                
    85 : unspill   i13, 0            
    86 : store.u8  i2, i13, i12      
    87 : jmp       84                
    88 : label 44:                   
    89 : cmp       i1, 1             
    90 : jmp_ne    47                
    91 : unspill   i13, 0            
    92 : store.i8  i2, i13, i12      
    93 : jmp       83                
    94 : label 47:                   
    95 : cmp       i1, 2             
    96 : jmp_ne    51                
    97 : unspill   i13, 0            
    98 : store.u16 i2, i13, i12      
    99 : jmp       82                
   100 : label 51:                   
   101 : cmp       i1, 3             
   102 : jmp_ne    56                
   103 : unspill   i13, 0            
   104 : store.i16 i2, i13, i12      
   105 : jmp       81                
   106 : label 56:                   
   107 : cmp       i1, 4             
   108 : jmp_ne    62                
   109 : unspill   i13, 0            
   110 : store.u32 i2, i13, i12      
   111 : jmp       80                
   112 : label 62:                   
   113 : cmp       i1, 5             
   114 : jmp_ne    69                
   115 : unspill   i13, 0            
   116 : store.i32 i2, i13, i12      
   117 : jmp       79                
   118 : label 69:                   
   119 : cmp       i1, 6             
   120 : jmp_ne    77                
   121 : unspill   i13, 0            
   122 : store.u64 i2, i13, i12      
   123 : jmp       78                
   124 : label 77:                   
   125 : unspill   i13, 0            
   126 : store.i64 i2, i13, i12      
   127 : label 78:                   
   128 : label 79:                   
   129 : label 80:                   
   130 : label 81:                   
   131 : label 82:                   
   132 : label 83:                   
   133 : label 84:                   
   134 : unspill   i13, 3            
   135 : add       s1, s1, i13       
   136 : add       s0, s0, i0        
   137 : add       s2, s2, 1         
   138 : jmp       0                 
   139 : label 85:                   
   140 : unspill   i12, 4            
   141 : unspill   i13, 5            
   142 : add       i4, i4, 56        
   143 : ret                         
