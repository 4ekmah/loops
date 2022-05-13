min_max_scalar(i0, i1, i2, i3)
     0 : sub       i31, i31, 80    
     1 : spill     1, i2           
     2 : spill     0, i3           
     3 : spill     5, i18          
     4 : spill     6, i19          
     5 : spill     7, i20          
     6 : spill     8, i21          
     7 : mov       i18, 0          
     8 : mov       i20, 0          
     9 : spill     4, i20          
    10 : mov       i20, 0          
    11 : spill     2, i20          
    12 : load.i32  i2, 5, i0       
    13 : mov       i3, i2          
    14 : mov       i20, 4          
    15 : spill     3, i20          
    16 : unspill   i20, 3          
    17 : mul       i1, i1, i20     
    18 : label 0:                  
    19 : load.i32  i19, 5, i0, i18 
    20 : cmp       i19, i2         
    21 : jmp_ge    1               
    22 : mov       i2, i19         
    23 : mov       i20, i18        
    24 : spill     4, i20          
    25 : label 1:                  
    26 : cmp       i19, i3         
    27 : jmp_le    2               
    28 : mov       i3, i19         
    29 : mov       i20, i18        
    30 : spill     2, i20          
    31 : label 2:                  
    32 : unspill   i20, 3          
    33 : add       i18, i18, i20   
    34 : cmp       i18, i1         
    35 : jmp_gt    0               
    36 : unspill   i20, 4          
    37 : unspill   i21, 3          
    38 : div       i0, i20, i21    
    39 : unspill   i20, 2          
    40 : unspill   i21, 3          
    41 : div       i1, i20, i21    
    42 : unspill   i20, 1          
    43 : store.i32 5, i20, i0      
    44 : unspill   i20, 0          
    45 : store.i32 5, i20, i1      
    46 : mov       i0, 0           
    47 : mov       i0, i0          
    48 : unspill   i18, 5          
    49 : unspill   i19, 6          
    50 : unspill   i20, 7          
    51 : unspill   i21, 8          
    52 : add       i31, i31, 80    
    53 : ret                       
