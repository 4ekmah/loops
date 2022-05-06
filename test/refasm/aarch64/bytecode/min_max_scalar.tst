min_max_scalar(i0, i1, i2, i3)
     0 : sub       i31, i31, 80    
     1 : spill     5, i18          
     2 : spill     6, i19          
     3 : spill     7, i20          
     4 : spill     8, i21          
     5 : mov       i18, 0          
     6 : mov       i20, 0          
     7 : spill     1, i20          
     8 : mov       i20, 0          
     9 : spill     0, i20          
    10 : load.i32  i20, 5, i0      
    11 : spill     4, i20          
    12 : unspill   i20, 4          
    13 : mov       i21, i20        
    14 : spill     2, i21          
    15 : mov       i20, 4          
    16 : spill     3, i20          
    17 : unspill   i20, 3          
    18 : mul       i1, i1, i20     
    19 : label 0:                  
    20 : load.i32  i19, 5, i0, i18 
    21 : unspill   i20, 4          
    22 : cmp       i19, i20        
    23 : jmp_ge    1               
    24 : mov       i20, i19        
    25 : spill     4, i20          
    26 : mov       i20, i18        
    27 : spill     1, i20          
    28 : label 1:                  
    29 : unspill   i20, 2          
    30 : cmp       i19, i20        
    31 : jmp_le    2               
    32 : mov       i20, i19        
    33 : spill     2, i20          
    34 : mov       i20, i18        
    35 : spill     0, i20          
    36 : label 2:                  
    37 : unspill   i20, 3          
    38 : add       i18, i18, i20   
    39 : cmp       i18, i1         
    40 : jmp_gt    0               
    41 : unspill   i20, 1          
    42 : unspill   i21, 3          
    43 : div       i0, i20, i21    
    44 : unspill   i20, 0          
    45 : unspill   i21, 3          
    46 : div       i1, i20, i21    
    47 : store.i32 5, i2, i0       
    48 : store.i32 5, i3, i1       
    49 : mov       i3, 0           
    50 : mov       i0, i3          
    51 : unspill   i18, 5          
    52 : unspill   i19, 6          
    53 : unspill   i20, 7          
    54 : unspill   i21, 8          
    55 : add       i31, i31, 80    
    56 : ret                       
