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
    16 : unspill   i20, 4          
    17 : cmp       i18, i20        
    18 : jmp_ge    1               
    19 : load.i32  i19, 5, i0, i18 
    20 : cmp       i19, i2         
    21 : unspill   i20, 3          
    22 : select    i1, 2, i18, i20 
    23 : mov       i20, i1         
    24 : spill     3, i20          
    25 : cmp       i19, i2         
    26 : select    i1, 2, i19, i2  
    27 : mov       i2, i1          
    28 : cmp       i19, i3         
    29 : unspill   i20, 2          
    30 : select    i1, 3, i18, i20 
    31 : mov       i20, i1         
    32 : spill     2, i20          
    33 : cmp       i19, i3         
    34 : select    i1, 3, i19, i3  
    35 : mov       i3, i1          
    36 : add       i18, i18, 4     
    37 : jmp       0               
    38 : label 1:                  
    39 : mov       i0, 4           
    40 : unspill   i20, 3          
    41 : sar       i0, i20, 2      
    42 : unspill   i20, 2          
    43 : sar       i1, i20, 2      
    44 : unspill   i20, 1          
    45 : store.i32 5, i20, i0      
    46 : unspill   i20, 0          
    47 : store.i32 5, i20, i1      
    48 : mov       i0, 0           
    49 : unspill   i18, 5          
    50 : unspill   i19, 6          
    51 : unspill   i20, 7          
    52 : add       i31, i31, 64    
    53 : ret                       
