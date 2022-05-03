arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : unspill   i0, 5           
     1 : unspill   i10, 6          
     2 : sub       i4, i4, 56      
     3 : spill     1, i3           
     4 : spill     2, i5           
     5 : spill     3, i6           
     6 : spill     4, i7           
     7 : spill     5, i12          
     8 : mov       i11, 0          
     9 : mov       i3, 0           
    10 : mov       i6, 4           
    11 : mov       s0, 1           
    12 : label 0:                  
    13 : load.i32  i5, 5, i1, i11  
    14 : load.i32  i12, 5, i2, i11 
    15 : mov       i7, i5          
    16 : add       i7, i7, i12     
    17 : store.i32 5, i9, i7       
    18 : mov       i7, i5          
    19 : sub       i7, i7, i12     
    20 : store.i32 5, i0, i7       
    21 : mul       i5, i5, i12     
    22 : store.i32 5, i10, i5      
    23 : add       i3, i3, s0      
    24 : add       i11, i11, i6    
    25 : add       i9, i9, i6      
    26 : add       i0, i0, i6      
    27 : add       i10, i10, i6    
    28 : cmp       i3, i8          
    29 : jmp_gt    0               
    30 : mov       i9, 0           
    31 : mov       i0, i9          
    32 : unspill   i3, 1           
    33 : unspill   i5, 2           
    34 : unspill   i6, 3           
    35 : unspill   i7, 4           
    36 : unspill   i12, 5          
    37 : add       i4, i4, 56      
    38 : ret                       
