arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : sub       i4, i4, 56      
     1 : spill     4, i9           
     2 : spill     5, i12          
     3 : spill     6, i13          
     4 : mov       s0, 0           
     5 : mov       s1, 0           
     6 : mov       s2, 4           
     7 : mov       s3, 1           
     8 : label 0:                  
     9 : unspill   i13, 0          
    10 : load.i32  i12, 5, i1, i13 
    11 : unspill   i13, 0          
    12 : load.i32  i0, 5, i2, i13  
    13 : mov       i9, i12         
    14 : add       i9, i9, i0      
    15 : unspill   i13, 4          
    16 : store.i32 5, i13, i9      
    17 : mov       i9, i12         
    18 : sub       i9, i9, i0      
    19 : unspill   i13, 12         
    20 : store.i32 5, i13, i9      
    21 : mul       i12, i12, i0    
    22 : unspill   i13, 13         
    23 : store.i32 5, i13, i12     
    24 : unspill   i13, 3          
    25 : add       s1, s1, i13     
    26 : unspill   i13, 2          
    27 : add       s0, s0, i13     
    28 : unspill   i13, 2          
    29 : add       s4, s4, i13     
    30 : unspill   i13, 2          
    31 : add       s12, s12, i13   
    32 : unspill   i13, 2          
    33 : add       s13, s13, i13   
    34 : cmp       s1, i8          
    35 : jmp_gt    0               
    36 : mov       i1, 0           
    37 : mov       i0, i1          
    38 : unspill   i12, 5          
    39 : unspill   i13, 6          
    40 : add       i4, i4, 56      
    41 : ret                       
