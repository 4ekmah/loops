min_max_select(i0, i1, i2, i3)
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
    11 : load.i32  i2, i0            
    12 : mov       i3, i2            
    13 : shl       i1, i1, 2         
    14 : label 0:                    
    15 : cmp       i18, i1           
    16 : jmp_ge 2                    
    17 : load.i32  i19, i0, i18      
    18 : cmp       i19, i2           
    19 : unspill   i20, 3            
    20 : select    i20, 31, i18, i20 
    21 : spill     3, i20            
    22 : cmp       i19, i2           
    23 : select    i2, 31, i19, i2   
    24 : cmp       i19, i3           
    25 : unspill   i20, 2            
    26 : select    i20, 28, i18, i20 
    27 : spill     2, i20            
    28 : cmp       i19, i3           
    29 : select    i3, 28, i19, i3   
    30 : add       i18, i18, 4       
    31 : jmp       0                 
    32 : label 2:                    
    33 : mov       i0, 4             
    34 : unspill   i20, 3            
    35 : sar       i0, i20, 2        
    36 : unspill   i20, 2            
    37 : sar       i1, i20, 2        
    38 : unspill   i20, 1            
    39 : store.i32 i20, i0           
    40 : unspill   i20, 0            
    41 : store.i32 i20, i1           
    42 : mov       i0, 0             
    43 : unspill   i18, 4            
    44 : unspill   i19, 5            
    45 : unspill   i20, 6            
    46 : add       i31, i31, 64      
    47 : ret                         
