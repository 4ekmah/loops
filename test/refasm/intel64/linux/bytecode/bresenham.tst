bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 104   
     1 : spill     9, i12        
     2 : spill     10, i13       
     3 : spill     11, i14       
     4 : unspill   i0, 14        
     5 : mov       i12, i8       
     6 : sub       i12, i12, i2  
     7 : mov       i13, i12      
     8 : neg       i13, i13      
     9 : select_s  i13, i12, i13 
    10 : spill     4, i13        
    11 : mov       i12, i8       
    12 : sub       i12, i12, i2  
    13 : spill     0, i1         
    14 : mov       i13, i12      
    15 : mov       i1, i13       
    16 : sar       i13, i13, 63  
    17 : neg       i1, i1        
    18 : x86_adc   i13, i13, i13 
    19 : unspill   i1, 0         
    20 : spill     5, i13        
    21 : mov       i12, i9       
    22 : sub       i12, i12, i1  
    23 : spill     0, i12        
    24 : neg       i12, i12      
    25 : select_s  i12, s0, i12  
    26 : mov       s6, i12       
    27 : neg       s6, s6        
    28 : mov       i12, i9       
    29 : sub       i12, i12, i1  
    30 : spill     0, i1         
    31 : mov       i13, i12      
    32 : mov       i1, i13       
    33 : sar       i13, i13, 63  
    34 : neg       i1, i1        
    35 : x86_adc   i13, i13, i13 
    36 : unspill   i1, 0         
    37 : spill     8, i13        
    38 : unspill   i13, 4        
    39 : unspill   i14, 6        
    40 : mov       s7, i13       
    41 : add       s7, s7, i14   
    42 : label 0:                
    43 : cmp       i7, 0         
    44 : jmp_eq 2                
    45 : mov       i12, i1       
    46 : mul       i12, i12, i6  
    47 : add       i12, i12, i2  
    48 : store.u8  i7, i12, i0   
    49 : cmp       i2, i8        
    50 : jmp_ne 4                
    51 : cmp       i1, i9        
    52 : jmp_ne 4                
    53 : jmp       2             
    54 : label 4:                
    55 : unspill   i13, 7        
    56 : mov       i12, i13      
    57 : shl       i12, i12, 1   
    58 : cmp       i12, s6       
    59 : jmp_gt 6                
    60 : cmp       i2, i8        
    61 : jmp_ne 8                
    62 : jmp       2             
    63 : label 8:                
    64 : unspill   i13, 6        
    65 : add       s7, s7, i13   
    66 : add       i2, i2, s5    
    67 : label 6:                
    68 : cmp       i12, s4       
    69 : jmp_gt 10               
    70 : cmp       i1, i9        
    71 : jmp_ne 12               
    72 : jmp       2             
    73 : label 12:               
    74 : unspill   i13, 4        
    75 : add       s7, s7, i13   
    76 : add       i1, i1, s8    
    77 : label 10:               
    78 : jmp       0             
    79 : label 2:                
    80 : unspill   i12, 9        
    81 : unspill   i13, 10       
    82 : unspill   i14, 11       
    83 : add       i4, i4, 104   
    84 : ret                     
