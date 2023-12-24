min_max_select(i0, i1, i2, i3)
     0 : sub       i31, i31, 64  
     1 : spill     1, i2         
     2 : spill     0, i3         
     3 : spill     5, i25        
     4 : spill     6, i26        
     5 : mov       i25, 0        
     6 : mov       i26, 0        
     7 : spill     3, i26        
     8 : mov       i26, 0        
     9 : spill     2, i26        
    10 : load.i32  i2, i0        
    11 : mov       i3, i2        
    12 : shl       i26, i1, 2    
    13 : spill     4, i26        
    14 : label 0:                
    15 : unspill   i26, 4        
    16 : cmp       i25, i26      
    17 : jmp_ge 2                
    18 : load.i32  i1, i0, i25   
    19 : cmp       i1, i2        
    20 : unspill   i26, 3        
    21 : select_gt i26, i25, i26 
    22 : spill     3, i26        
    23 : cmp       i1, i2        
    24 : select_gt i2, i1, i2    
    25 : cmp       i1, i3        
    26 : unspill   i26, 2        
    27 : select_gt i26, i25, i26 
    28 : spill     2, i26        
    29 : cmp       i1, i3        
    30 : select_gt i3, i1, i3    
    31 : add       i25, i25, 4   
    32 : jmp       0             
    33 : label 2:                
    34 : unspill   i26, 3        
    35 : sar       i0, i26, 2    
    36 : unspill   i26, 2        
    37 : sar       i1, i26, 2    
    38 : unspill   i26, 1        
    39 : store.i32 i26, i0       
    40 : unspill   i26, 0        
    41 : store.i32 i26, i1       
    42 : mov       i0, 0         
    43 : unspill   i25, 5        
    44 : unspill   i26, 6        
    45 : add       i31, i31, 64  
    46 : ret                     
