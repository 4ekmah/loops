exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub       i31, i31, 16   
     1 : spill     0, i18         
     2 : spill     1, i19         
     3 : mov       i3, 1          
     4 : mov       i3, 0          
     5 : mov       i18, 0         
     6 : label 0:                 
     7 : cmp       i18, i1        
     8 : jmp_ge    1              
     9 : load.i32  i19, 5, i0, i3 
    10 : mov       i19, i19       
    11 : mov       i19, i19       
    12 : store.i32 5, i2, i19     
    13 : add       i18, i18, 1    
    14 : add       i3, i3, 4      
    15 : add       i2, i2, 4      
    16 : jmp       0              
    17 : label 1:                 
    18 : mov       i0, 0          
    19 : unspill   i18, 0         
    20 : unspill   i19, 1         
    21 : add       i31, i31, 16   
    22 : ret                      
