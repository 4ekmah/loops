triangle_types(i0, i1, i2)
     0 : sub       i31, i31, 32  
     1 : spill     0, i1         
     2 : spill     1, i25        
     3 : spill     2, i26        
     4 : cmp       i0, 0         
     5 : jmp_le 0                
     6 : unspill   i26, 0        
     7 : cmp       i26, 0        
     8 : jmp_le 0                
     9 : cmp       i2, 0         
    10 : jmp_gt 1                
    11 : label 0:                
    12 : mov       i0, 0         
    13 : jmp       18            
    14 : label 1:                
    15 : unspill   i26, 0        
    16 : add       i3, i26, i2   
    17 : cmp       i0, i3        
    18 : jmp_gt 3                
    19 : add       i3, i0, i2    
    20 : unspill   i26, 0        
    21 : cmp       i26, i3       
    22 : jmp_gt 3                
    23 : unspill   i26, 0        
    24 : add       i3, i0, i26   
    25 : cmp       i2, i3        
    26 : jmp_le 4                
    27 : label 3:                
    28 : mov       i0, 0         
    29 : jmp       18            
    30 : label 4:                
    31 : unspill   i26, 0        
    32 : cmp       i0, i26       
    33 : jmp_ne 7                
    34 : cmp       i0, i2        
    35 : jmp_ne 7                
    36 : mov       i0, 2         
    37 : jmp       18            
    38 : label 7:                
    39 : unspill   i26, 0        
    40 : cmp       i0, i26       
    41 : jmp_eq 9                
    42 : cmp       i0, i2        
    43 : jmp_eq 9                
    44 : unspill   i26, 0        
    45 : cmp       i26, i2       
    46 : jmp_ne 10               
    47 : label 9:                
    48 : mov       i0, 3         
    49 : jmp       18            
    50 : label 10:               
    51 : mul       i3, i0, i0    
    52 : unspill   i26, 0        
    53 : mul       i25, i26, i26 
    54 : mul       i1, i2, i2    
    55 : add       i1, i25, i1   
    56 : cmp       i3, i1        
    57 : jmp_eq 12               
    58 : unspill   i26, 0        
    59 : mul       i1, i26, i26  
    60 : mul       i3, i0, i0    
    61 : mul       i25, i2, i2   
    62 : add       i3, i3, i25   
    63 : cmp       i1, i3        
    64 : jmp_eq 12               
    65 : mul       i1, i2, i2    
    66 : mul       i3, i0, i0    
    67 : unspill   i26, 0        
    68 : mul       i25, i26, i26 
    69 : add       i3, i3, i25   
    70 : cmp       i1, i3        
    71 : jmp_ne 13               
    72 : label 12:               
    73 : mov       i0, 1         
    74 : jmp       18            
    75 : label 13:               
    76 : mul       i1, i0, i0    
    77 : unspill   i26, 0        
    78 : mul       i3, i26, i26  
    79 : mul       i25, i2, i2   
    80 : add       i3, i3, i25   
    81 : cmp       i1, i3        
    82 : jmp_gt 15               
    83 : unspill   i26, 0        
    84 : mul       i1, i26, i26  
    85 : mul       i3, i0, i0    
    86 : mul       i25, i2, i2   
    87 : add       i3, i3, i25   
    88 : cmp       i1, i3        
    89 : jmp_gt 15               
    90 : mul       i1, i2, i2    
    91 : mul       i0, i0, i0    
    92 : unspill   i26, 0        
    93 : mul       i2, i26, i26  
    94 : add       i0, i0, i2    
    95 : cmp       i1, i0        
    96 : jmp_le 16               
    97 : label 15:               
    98 : mov       i0, 5         
    99 : jmp       18            
   100 : label 16:               
   101 : mov       i0, 4         
   102 : jmp       18            
   103 : label 2:                
   104 : label 5:                
   105 : label 8:                
   106 : label 11:               
   107 : label 14:               
   108 : label 17:               
   109 : label 18:               
   110 : unspill   i25, 1        
   111 : unspill   i26, 2        
   112 : add       i31, i31, 32  
   113 : ret                     
