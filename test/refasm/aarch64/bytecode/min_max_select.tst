min_max_select(i0, i1, i2, i3)
     0 : sub       i31, i31, 64    
     1 : spill     1, i2           
     2 : spill     0, i3           
     3 : spill     5, i18          
     4 : spill     6, i19          
     5 : spill     7, i20          
     6 : mov       i18, 0          
     7 : mov       i20, 0          
     8 : spill     3, i20          
     9 : mov       i20, 0          
    10 : spill     2, i20          
    11 : load.i32  i2, 5, i0       
    12 : mov       i3, i2          
    13 : shl       i20, i1, 2      
    14 : spill     4, i20          
    15 : label 0:                  
    16 : load.i32  i19, 5, i0, i18 
    17 : cmp       i19, i2         
    18 : unspill   i20, 3          
    19 : select    i1, 2, i18, i20 
    20 : mov       i20, i1         
    21 : spill     3, i20          
    22 : cmp       i19, i2         
    23 : select    i1, 2, i19, i2  
    24 : mov       i2, i1          
    25 : cmp       i19, i3         
    26 : unspill   i20, 2          
    27 : select    i1, 3, i18, i20 
    28 : mov       i20, i1         
    29 : spill     2, i20          
    30 : cmp       i19, i3         
    31 : select    i1, 3, i19, i3  
    32 : mov       i3, i1          
    33 : add       i18, i18, 4     
    34 : unspill   i20, 4          
    35 : cmp       i18, i20        
    36 : jmp_gt    0               
    37 : mov       i0, 4           
    38 : unspill   i20, 3          
    39 : sar       i0, i20, 2      
    40 : unspill   i20, 2          
    41 : sar       i1, i20, 2      
    42 : unspill   i20, 1          
    43 : store.i32 5, i20, i0      
    44 : unspill   i20, 0          
    45 : store.i32 5, i20, i1      
    46 : mov       i0, 0           
    47 : unspill   i18, 5          
    48 : unspill   i19, 6          
    49 : unspill   i20, 7          
    50 : add       i31, i31, 64    
    51 : ret                       
