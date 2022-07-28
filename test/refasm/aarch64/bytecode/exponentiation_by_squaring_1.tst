exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub       i31, i31, 32   
     1 : spill     1, i18         
     2 : spill     2, i19         
     3 : spill     3, i20         
     4 : mov       i3, 1          
     5 : mov       i3, 0          
     6 : mov       i20, 0         
     7 : spill     0, i20         
     8 : label 0:                 
     9 : unspill   i20, 0         
    10 : cmp       i20, i1        
    11 : jmp_ge    1              
    12 : load.i32  i19, 5, i0, i3 
    13 : mov       i19, i19       
    14 : mov       i18, 1         
    15 : mul       i18, i18, i19  
    16 : store.i32 5, i2, i18     
    17 : unspill   i20, 0         
    18 : add       i20, i20, 1    
    19 : spill     0, i20         
    20 : add       i3, i3, 4      
    21 : add       i2, i2, 4      
    22 : jmp       0              
    23 : label 1:                 
    24 : mov       i0, 0          
    25 : unspill   i18, 1         
    26 : unspill   i19, 2         
    27 : unspill   i20, 3         
    28 : add       i31, i31, 32   
    29 : ret                      
