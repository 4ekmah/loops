arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub       i4, i4, 56      
     1 : spill     4, i9           
     2 : spill     5, i12          
     3 : spill     6, i13          
     4 : mov       s2, 0           
     5 : mov       s3, 0           
     6 : label 0:                  
     7 : cmp       s3, i8          
     8 : jmp_ge    1               
     9 : unspill   i13, 2          
    10 : load.i32  i12, 5, i1, i13 
    11 : unspill   i13, 2          
    12 : load.i32  i0, 5, i2, i13  
    13 : mov       i9, i12         
    14 : add       i9, i9, i0      
    15 : unspill   i13, 4          
    16 : store.i32 5, i13, i9      
    17 : mov       i9, i12         
    18 : sub       i9, i9, i0      
    19 : unspill   i13, 12         
    20 : store.i32 5, i13, i9      
    21 : mov       i9, i12         
    22 : mul       i9, i9, i0      
    23 : unspill   i13, 13         
    24 : store.i32 5, i13, i9      
    25 : spill     0, i0           
    26 : spill     1, i2           
    27 : mov       i0, i12         
    28 : x86_cqo                   
    29 : div       i0, i0, s0      
    30 : mov       i9, i0          
    31 : unspill   i0, 0           
    32 : unspill   i2, 1           
    33 : unspill   i13, 14         
    34 : store.i32 5, i13, i9      
    35 : add       s3, s3, 1       
    36 : add       s2, s2, 4       
    37 : add       s4, s4, 4       
    38 : add       s12, s12, 4     
    39 : add       s13, s13, 4     
    40 : add       s14, s14, 4     
    41 : jmp       0               
    42 : label 1:                  
    43 : mov       i0, 0           
    44 : unspill   i12, 5          
    45 : unspill   i13, 6          
    46 : add       i4, i4, 56      
    47 : ret                       
