min_max_scalar(i0, i1, i2, i3)
     0 : sub       i31, i31, 64    
     1 : spill     1, i2           
     2 : spill     0, i3           
     3 : spill     4, i18          
     4 : spill     5, i19          
     5 : spill     6, i20          
     6 : mov       i18, 0          
     7 : mov       i20, 0          
     8 : spill     3, i20          
     9 : mov       i20, 0          
    10 : spill     2, i20          
    11 : load.i32  i2, 5, i0       
    12 : mov       i3, i2          
    13 : mov       i19, 4          
    14 : mul       i1, i1, i19     
    15 : label 0:                  
    16 : load.i32  i19, 5, i0, i18 
    17 : cmp       i19, i2         
    18 : jmp_ge    1               
    19 : mov       i2, i19         
    20 : mov       i20, i18        
    21 : spill     3, i20          
    22 : label 1:                  
    23 : cmp       i19, i3         
    24 : jmp_le    2               
    25 : mov       i3, i19         
    26 : mov       i20, i18        
    27 : spill     2, i20          
    28 : label 2:                  
    29 : add       i18, i18, 4     
    30 : cmp       i18, i1         
    31 : jmp_gt    0               
    32 : mov       i0, 4           
    33 : unspill   i20, 3          
    34 : div       i1, i20, i0     
    35 : unspill   i20, 2          
    36 : div       i0, i20, i0     
    37 : unspill   i20, 1          
    38 : store.i32 5, i20, i1      
    39 : unspill   i20, 0          
    40 : store.i32 5, i20, i0      
    41 : mov       i0, 0           
    42 : unspill   i18, 4          
    43 : unspill   i19, 5          
    44 : unspill   i20, 6          
    45 : add       i31, i31, 64    
    46 : ret                       
