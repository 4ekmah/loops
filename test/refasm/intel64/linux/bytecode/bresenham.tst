bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 88    
     1 : spill     9, i13        
     2 : spill     10, i14       
     3 : mov       i0, i8        
     4 : sub       i0, i0, i2    
     5 : mov       i13, i0       
     6 : neg       i13, i13      
     7 : select_s  i13, i0, i13  
     8 : spill     4, i13        
     9 : mov       i0, i8        
    10 : sub       i0, i0, i2    
    11 : spill     0, i1         
    12 : mov       i13, i0       
    13 : mov       i1, i13       
    14 : sar       i13, i13, 63  
    15 : neg       i1, i1        
    16 : x86_adc   i13, i13, i13 
    17 : unspill   i1, 0         
    18 : spill     5, i13        
    19 : mov       i0, i9        
    20 : sub       i0, i0, i1    
    21 : spill     0, i0         
    22 : neg       i0, i0        
    23 : select_s  i0, s0, i0    
    24 : mov       s6, i0        
    25 : neg       s6, s6        
    26 : mov       i0, i9        
    27 : sub       i0, i0, i1    
    28 : spill     0, i1         
    29 : mov       i13, i0       
    30 : mov       i1, i13       
    31 : sar       i13, i13, 63  
    32 : neg       i1, i1        
    33 : x86_adc   i13, i13, i13 
    34 : unspill   i1, 0         
    35 : spill     8, i13        
    36 : unspill   i13, 4        
    37 : unspill   i14, 6        
    38 : mov       s7, i13       
    39 : add       s7, s7, i14   
    40 : label 0:                
    41 : cmp       i7, 0         
    42 : jmp_eq 2                
    43 : mov       i0, i1        
    44 : mul       i0, i0, i6    
    45 : add       i0, i0, i2    
    46 : unspill   i13, 12       
    47 : store.u8  i7, i0, i13   
    48 : cmp       i2, i8        
    49 : jmp_ne 4                
    50 : cmp       i1, i9        
    51 : jmp_ne 4                
    52 : jmp       2             
    53 : label 4:                
    54 : unspill   i13, 7        
    55 : mov       i0, i13       
    56 : shl       i0, i0, 1     
    57 : cmp       i0, s6        
    58 : jmp_gt 6                
    59 : cmp       i2, i8        
    60 : jmp_ne 8                
    61 : jmp       2             
    62 : label 8:                
    63 : unspill   i13, 6        
    64 : add       s7, s7, i13   
    65 : add       i2, i2, s5    
    66 : label 6:                
    67 : cmp       i0, s4        
    68 : jmp_gt 10               
    69 : cmp       i1, i9        
    70 : jmp_ne 12               
    71 : jmp       2             
    72 : label 12:               
    73 : unspill   i13, 4        
    74 : add       s7, s7, i13   
    75 : add       i1, i1, s8    
    76 : label 10:               
    77 : jmp       0             
    78 : label 2:                
    79 : unspill   i13, 9        
    80 : unspill   i14, 10       
    81 : add       i4, i4, 88    
    82 : ret                     
