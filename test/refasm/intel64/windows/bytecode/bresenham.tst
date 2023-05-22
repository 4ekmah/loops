bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 104   
     1 : spill     9, i12        
     2 : spill     10, i13       
     3 : spill     11, i14       
     4 : unspill   i0, 18        
     5 : mov       i12, i0       
     6 : sub       i12, i12, i8  
     7 : mov       i13, i12      
     8 : neg       i13, i13      
     9 : select_s  i13, i12, i13 
    10 : spill     4, i13        
    11 : mov       i12, i0       
    12 : sub       i12, i12, i8  
    13 : spill     0, i1         
    14 : mov       i13, i12      
    15 : mov       i1, i13       
    16 : sar       i13, i13, 63  
    17 : neg       i1, i1        
    18 : x86_adc   i13, i13, i13 
    19 : unspill   i1, 0         
    20 : spill     5, i13        
    21 : unspill   i13, 19       
    22 : mov       i12, i13      
    23 : sub       i12, i12, i9  
    24 : spill     0, i12        
    25 : neg       i12, i12      
    26 : select_s  i12, s0, i12  
    27 : mov       s6, i12       
    28 : neg       s6, s6        
    29 : unspill   i13, 19       
    30 : mov       i12, i13      
    31 : sub       i12, i12, i9  
    32 : spill     0, i1         
    33 : mov       i13, i12      
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
    47 : mov       i12, i9       
    48 : mul       i12, i12, i2  
    49 : add       i12, i12, i8  
    50 : unspill   i13, 20       
    51 : store.u8  i1, i12, i13  
    52 : cmp       i8, i0        
    53 : jmp_ne 4                
    54 : cmp       i9, s19       
    55 : jmp_ne 4                
    56 : jmp       2             
    57 : label 4:                
    58 : unspill   i13, 7        
    59 : mov       i12, i13      
    60 : shl       i12, i12, 1   
    61 : cmp       i12, s6       
    62 : jmp_gt 6                
    63 : cmp       i8, i0        
    64 : jmp_ne 8                
    65 : jmp       2             
    66 : label 8:                
    67 : unspill   i13, 6        
    68 : add       s7, s7, i13   
    69 : add       i8, i8, s5    
    70 : label 6:                
    71 : cmp       i12, s4       
    72 : jmp_gt 10               
    73 : cmp       i9, s19       
    74 : jmp_ne 12               
    75 : jmp       2             
    76 : label 12:               
    77 : unspill   i13, 4        
    78 : add       s7, s7, i13   
    79 : add       i9, i9, s8    
    80 : label 10:               
    81 : jmp       0             
    82 : label 2:                
    83 : unspill   i12, 9        
    84 : unspill   i13, 10       
    85 : unspill   i14, 11       
    86 : add       i4, i4, 104   
    87 : ret                     
