min_max_select(i0, i1, i2, i3)
     0 : sub       i4, i4, 56   
     1 : spill     1, i2        
     2 : spill     0, i1        
     3 : spill     4, i12       
     4 : spill     5, i13       
     5 : mov       i0, 0        
     6 : mov       s3, 0        
     7 : mov       s2, 0        
     8 : load.i32  i2, i7       
     9 : mov       i1, i2       
    10 : shl       i6, i6, 2    
    11 : label 0:               
    12 : cmp       i0, i6       
    13 : jmp_ge 2               
    14 : load.i32  i12, i7, i0  
    15 : cmp       i12, i2      
    16 : unspill   i13, 3       
    17 : select_gt i13, i0, i13 
    18 : spill     3, i13       
    19 : cmp       i2, i12      
    20 : select_gt i2, i12, i2  
    21 : cmp       i12, i1      
    22 : unspill   i13, 2       
    23 : select_gt i13, i0, i13 
    24 : spill     2, i13       
    25 : cmp       i1, i12      
    26 : select_gt i1, i12, i1  
    27 : add       i0, i0, 4    
    28 : jmp       0            
    29 : label 2:               
    30 : unspill   i13, 3       
    31 : mov       i7, i13      
    32 : sar       i7, i7, 2    
    33 : unspill   i13, 2       
    34 : mov       i6, i13      
    35 : sar       i6, i6, 2    
    36 : unspill   i13, 1       
    37 : store.i32 i13, i7      
    38 : unspill   i13, 0       
    39 : store.i32 i13, i6      
    40 : mov       i0, 0        
    41 : unspill   i12, 4       
    42 : unspill   i13, 5       
    43 : add       i4, i4, 56   
    44 : ret                    
