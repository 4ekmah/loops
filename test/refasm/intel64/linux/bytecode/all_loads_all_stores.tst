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
    30 : jmp_ge 2                    
    31 : mov       i12, 0            
    32 : cmp       i6, 0             
    33 : jmp_ne 4                    
    34 : unspill   i13, 1            
    35 : load.u8   i12, i7, i13      
    36 : jmp       5                 
    37 : label 4:                    
    38 : cmp       i6, 1             
    39 : jmp_ne 7                    
    40 : unspill   i13, 1            
    41 : load.i8   i12, i7, i13      
    42 : jmp       8                 
    43 : label 7:                    
    44 : cmp       i6, 2             
    45 : jmp_ne 10                   
    46 : unspill   i13, 1            
    47 : load.u16  i12, i7, i13      
    48 : jmp       11                
    49 : label 10:                   
    50 : cmp       i6, 3             
    51 : jmp_ne 13                   
    52 : unspill   i13, 1            
    53 : load.i16  i12, i7, i13      
    54 : jmp       14                
    55 : label 13:                   
    56 : cmp       i6, 4             
    57 : jmp_ne 16                   
    58 : unspill   i13, 1            
    59 : load.u32  i12, i7, i13      
    60 : jmp       17                
    61 : label 16:                   
    62 : cmp       i6, 5             
    63 : jmp_ne 19                   
    64 : unspill   i13, 1            
    65 : load.i32  i12, i7, i13      
    66 : jmp       20                
    67 : label 19:                   
    68 : cmp       i6, 6             
    69 : jmp_ne 22                   
    70 : unspill   i13, 1            
    71 : load.u64  i12, i7, i13      
    72 : jmp       23                
    73 : label 22:                   
    74 : unspill   i13, 1            
    75 : load.i64  i12, i7, i13      
    76 : label 5:                    
    77 : label 8:                    
    78 : label 11:                   
    79 : label 14:                   
    80 : label 17:                   
    81 : label 20:                   
    82 : label 23:                   
    83 : cmp       i1, 0             
    84 : jmp_ne 25                   
    85 : unspill   i13, 0            
    86 : store.u8  i2, i13, i12      
    87 : jmp       26                
    88 : label 25:                   
    89 : cmp       i1, 1             
    90 : jmp_ne 28                   
    91 : unspill   i13, 0            
    92 : store.i8  i2, i13, i12      
    93 : jmp       29                
    94 : label 28:                   
    95 : cmp       i1, 2             
    96 : jmp_ne 31                   
    97 : unspill   i13, 0            
    98 : store.u16 i2, i13, i12      
    99 : jmp       32                
   100 : label 31:                   
   101 : cmp       i1, 3             
   102 : jmp_ne 34                   
   103 : unspill   i13, 0            
   104 : store.i16 i2, i13, i12      
   105 : jmp       35                
   106 : label 34:                   
   107 : cmp       i1, 4             
   108 : jmp_ne 37                   
   109 : unspill   i13, 0            
   110 : store.u32 i2, i13, i12      
   111 : jmp       38                
   112 : label 37:                   
   113 : cmp       i1, 5             
   114 : jmp_ne 40                   
   115 : unspill   i13, 0            
   116 : store.i32 i2, i13, i12      
   117 : jmp       41                
   118 : label 40:                   
   119 : cmp       i1, 6             
   120 : jmp_ne 43                   
   121 : unspill   i13, 0            
   122 : store.u64 i2, i13, i12      
   123 : jmp       44                
   124 : label 43:                   
   125 : unspill   i13, 0            
   126 : store.i64 i2, i13, i12      
   127 : label 26:                   
   128 : label 29:                   
   129 : label 32:                   
   130 : label 35:                   
   131 : label 38:                   
   132 : label 41:                   
   133 : label 44:                   
   134 : unspill   i13, 3            
   135 : add       s1, s1, i13       
   136 : add       s0, s0, i0        
   137 : add       s2, s2, 1         
   138 : jmp       0                 
   139 : label 2:                    
   140 : unspill   i12, 4            
   141 : unspill   i13, 5            
   142 : add       i4, i4, 56        
   143 : ret                         
