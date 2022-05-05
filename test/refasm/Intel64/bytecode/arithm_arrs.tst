arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : unspill   i0, 5           
     1 : unspill   i12, 6          
     2 : sub       i4, i4, 104     
     3 : spill     9, i12          
     4 : spill     10, i13         
     5 : spill     11, i14         
     6 : mov       s0, 0           
     7 : mov       s1, 0           
     8 : mov       s2, 4           
     9 : mov       s3, 1           
    10 : label 0:                  
    11 : unspill   i13, 0          
    12 : load.i32  i14, 5, i1, i13 
    13 : spill     4, i14          
    14 : unspill   i13, 0          
    15 : load.i32  i14, 5, i2, i13 
    16 : spill     5, i14          
    17 : unspill   i13, 4          
    18 : unspill   i14, 5          
    19 : mov       s6, i13         
    20 : add       s6, s6, i14     
    21 : unspill   i13, 6          
    22 : store.i32 5, i9, i13      
    23 : unspill   i13, 4          
    24 : unspill   i14, 5          
    25 : mov       s7, i13         
    26 : sub       s7, s7, i14     
    27 : unspill   i13, 7          
    28 : store.i32 5, i0, i13      
    29 : unspill   i13, 4          
    30 : mov       i14, i13        
    31 : mul       i14, i14, s5    
    32 : spill     8, i14          
    33 : unspill   i13, 8          
    34 : store.i32 5, i12, i13     
    35 : unspill   i13, 3          
    36 : add       s1, s1, i13     
    37 : unspill   i13, 2          
    38 : add       s0, s0, i13     
    39 : add       i9, i9, s2      
    40 : add       i0, i0, s2      
    41 : add       i12, i12, s2    
    42 : cmp       s1, i8          
    43 : jmp_gt    0               
    44 : mov       i9, 0           
    45 : mov       i0, i9          
    46 : unspill   i12, 9          
    47 : unspill   i13, 10         
    48 : unspill   i14, 11         
    49 : add       i4, i4, 104     
    50 : ret                       
