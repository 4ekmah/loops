all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub       i4, i4, 56        
     1 : spill     4, i12            
     2 : spill     5, i13            
     3 : mov       s2, 0             
     4 : mov       s0, 0             
     5 : mov       s1, 0             
     6 : mov       i12, 2            
     7 : mov       i0, 1             
     8 : cmp       i2, 1             
     9 : select    i0, 28, i12, i0   
    10 : mov       i12, 4            
    11 : cmp       i2, 3             
    12 : select    i0, 28, i12, i0   
    13 : mov       i12, 8            
    14 : cmp       i2, 5             
    15 : mov       i13, i0           
    16 : select    i13, 28, i12, i13 
    17 : spill     3, i13            
    18 : mov       i12, 2            
    19 : mov       i0, 1             
    20 : cmp       i9, 1             
    21 : select    i0, 28, i12, i0   
    22 : mov       i12, 4            
    23 : cmp       i9, 3             
    24 : select    i0, 28, i12, i0   
    25 : mov       i12, 8            
    26 : cmp       i9, 5             
    27 : select    i0, 28, i12, i0   
    28 : label 0:                    
    29 : unspill   i13, 12           
    30 : cmp       s2, i13           
    31 : jmp_ge 2                    
    32 : mov       i12, 0            
    33 : cmp       i2, 0             
    34 : jmp_ne 4                    
    35 : unspill   i13, 0            
    36 : load.u8   i12, i1, i13      
    37 : jmp       5                 
    38 : label 4:                    
    39 : cmp       i2, 1             
    40 : jmp_ne 7                    
    41 : unspill   i13, 0            
    42 : load.i8   i12, i1, i13      
    43 : jmp       8                 
    44 : label 7:                    
    45 : cmp       i2, 2             
    46 : jmp_ne 10                   
    47 : unspill   i13, 0            
    48 : load.u16  i12, i1, i13      
    49 : jmp       11                
    50 : label 10:                   
    51 : cmp       i2, 3             
    52 : jmp_ne 13                   
    53 : unspill   i13, 0            
    54 : load.i16  i12, i1, i13      
    55 : jmp       14                
    56 : label 13:                   
    57 : cmp       i2, 4             
    58 : jmp_ne 16                   
    59 : unspill   i13, 0            
    60 : load.u32  i12, i1, i13      
    61 : jmp       17                
    62 : label 16:                   
    63 : cmp       i2, 5             
    64 : jmp_ne 19                   
    65 : unspill   i13, 0            
    66 : load.i32  i12, i1, i13      
    67 : jmp       20                
    68 : label 19:                   
    69 : cmp       i2, 6             
    70 : jmp_ne 22                   
    71 : unspill   i13, 0            
    72 : load.u64  i12, i1, i13      
    73 : jmp       23                
    74 : label 22:                   
    75 : unspill   i13, 0            
    76 : load.i64  i12, i1, i13      
    77 : label 5:                    
    78 : label 8:                    
    79 : label 11:                   
    80 : label 14:                   
    81 : label 17:                   
    82 : label 20:                   
    83 : label 23:                   
    84 : cmp       i9, 0             
    85 : jmp_ne 25                   
    86 : unspill   i13, 1            
    87 : store.u8  i8, i13, i12      
    88 : jmp       26                
    89 : label 25:                   
    90 : cmp       i9, 1             
    91 : jmp_ne 28                   
    92 : unspill   i13, 1            
    93 : store.i8  i8, i13, i12      
    94 : jmp       29                
    95 : label 28:                   
    96 : cmp       i9, 2             
    97 : jmp_ne 31                   
    98 : unspill   i13, 1            
    99 : store.u16 i8, i13, i12      
   100 : jmp       32                
   101 : label 31:                   
   102 : cmp       i9, 3             
   103 : jmp_ne 34                   
   104 : unspill   i13, 1            
   105 : store.i16 i8, i13, i12      
   106 : jmp       35                
   107 : label 34:                   
   108 : cmp       i9, 4             
   109 : jmp_ne 37                   
   110 : unspill   i13, 1            
   111 : store.u32 i8, i13, i12      
   112 : jmp       38                
   113 : label 37:                   
   114 : cmp       i9, 5             
   115 : jmp_ne 40                   
   116 : unspill   i13, 1            
   117 : store.i32 i8, i13, i12      
   118 : jmp       41                
   119 : label 40:                   
   120 : cmp       i9, 6             
   121 : jmp_ne 43                   
   122 : unspill   i13, 1            
   123 : store.u64 i8, i13, i12      
   124 : jmp       44                
   125 : label 43:                   
   126 : unspill   i13, 1            
   127 : store.i64 i8, i13, i12      
   128 : label 26:                   
   129 : label 29:                   
   130 : label 32:                   
   131 : label 35:                   
   132 : label 38:                   
   133 : label 41:                   
   134 : label 44:                   
   135 : unspill   i13, 3            
   136 : add       s0, s0, i13       
   137 : add       s1, s1, i0        
   138 : add       s2, s2, 1         
   139 : jmp       0                 
   140 : label 2:                    
   141 : unspill   i12, 4            
   142 : unspill   i13, 5            
   143 : add       i4, i4, 56        
   144 : ret                         
