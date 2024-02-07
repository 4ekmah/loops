min_max_select(i0, i1, i2, i3)
     0 : sub       i4, i4, 56   
     1 : spill     1, i8        
     2 : spill     0, i9        
     3 : spill     5, i13       
     4 : mov       i0, 0        
     5 : mov       s3, 0        
     6 : mov       s2, 0        
     7 : load.i32  i8, i1       
     8 : mov       i9, i8       
     9 : mov       s4, i2       
    10 : shl       s4, s4, 2    
    11 : label 0:               
    12 : cmp       i0, s4       
    13 : jmp_ge 2               
    14 : load.i32  i2, i1, i0   
    15 : cmp       i2, i8       
    16 : unspill   i13, 3       
    17 : select_gt i13, i0, i13 
    18 : spill     3, i13       
    19 : cmp       i8, i2       
    20 : select_gt i8, i2, i8   
    21 : cmp       i2, i9       
    22 : unspill   i13, 2       
    23 : select_gt i13, i0, i13 
    24 : spill     2, i13       
    25 : cmp       i9, i2       
    26 : select_gt i9, i2, i9   
    27 : add       i0, i0, 4    
    28 : jmp       0            
    29 : label 2:               
    30 : unspill   i13, 3       
    31 : mov       i1, i13      
    32 : sar       i1, i1, 2    
    33 : unspill   i13, 2       
    34 : mov       i2, i13      
    35 : sar       i2, i2, 2    
    36 : unspill   i13, 1       
    37 : store.i32 i13, i1      
    38 : unspill   i13, 0       
    39 : store.i32 i13, i2      
    40 : mov       i0, 0        
    41 : unspill   i13, 5       
    42 : add       i4, i4, 56   
    43 : ret                    
