bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 88    
     1 : spill     9, i13        
     2 : spill     10, i14       
     3 : unspill   i13, 16       
     4 : mov       i0, i13       
     5 : sub       i0, i0, i8    
     6 : mov       i13, i0       
     7 : neg       i13, i13      
     8 : select_s  i13, i0, i13  
     9 : spill     4, i13        
    10 : unspill   i13, 16       
    11 : mov       i0, i13       
    12 : sub       i0, i0, i8    
    13 : spill     0, i1         
    14 : mov       i13, i0       
    15 : mov       i1, i13       
    16 : sar       i13, i13, 63  
    17 : neg       i1, i1        
    18 : x86_adc   i13, i13, i13 
    19 : unspill   i1, 0         
    20 : spill     5, i13        
    21 : unspill   i13, 17       
    22 : mov       i0, i13       
    23 : sub       i0, i0, i9    
    24 : spill     0, i0         
    25 : neg       i0, i0        
    26 : select_s  i0, s0, i0    
    27 : mov       s6, i0        
    28 : neg       s6, s6        
    29 : unspill   i13, 17       
    30 : mov       i0, i13       
    31 : sub       i0, i0, i9    
    32 : spill     0, i1         
    33 : mov       i13, i0       
    34 : mov       i1, i13       
    35 : sar       i13, i13, 63  
    36 : neg       i1, i1        
    37 : x86_adc   i13, i13, i13 
    38 : unspill   i1, 0         
    39 : spill     8, i13        
    40 : unspill   i13, 4        
    41 : unspill   i14, 6        
    42 : mov       s7, i13       
    43 : add       s7, s7, i14   
    44 : label 0:                
    45 : cmp       i1, 0         
    46 : jmp_eq 2                
    47 : mov       i0, i9        
    48 : mul       i0, i0, i2    
    49 : add       i0, i0, i8    
    50 : unspill   i13, 18       
    51 : store.u8  i1, i0, i13   
    52 : cmp       i8, s16       
    53 : jmp_ne 4                
    54 : cmp       i9, s17       
    55 : jmp_ne 4                
    56 : jmp       2             
    57 : label 4:                
    58 : unspill   i13, 7        
    59 : mov       i0, i13       
    60 : shl       i0, i0, 1     
    61 : cmp       i0, s6        
    62 : jmp_gt 6                
    63 : cmp       i8, s16       
    64 : jmp_ne 8                
    65 : jmp       2             
    66 : label 8:                
    67 : unspill   i13, 6        
    68 : add       s7, s7, i13   
    69 : add       i8, i8, s5    
    70 : label 6:                
    71 : cmp       i0, s4        
    72 : jmp_gt 10               
    73 : cmp       i9, s17       
    74 : jmp_ne 12               
    75 : jmp       2             
    76 : label 12:               
    77 : unspill   i13, 4        
    78 : add       s7, s7, i13   
    79 : add       i9, i9, s8    
    80 : label 10:               
    81 : jmp       0             
    82 : label 2:                
    83 : unspill   i13, 9        
    84 : unspill   i14, 10       
    85 : add       i4, i4, 88    
    86 : ret                     
