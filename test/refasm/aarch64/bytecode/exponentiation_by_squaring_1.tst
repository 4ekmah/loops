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
    10 : store.i32 5, i2, i19     
    11 : add       i18, i18, 1    
    12 : add       i3, i3, 4      
    13 : add       i2, i2, 4      
    14 : jmp       0              
    15 : label 1:                 
    16 : mov       i0, 0          
    17 : unspill   i18, 0         
    18 : unspill   i19, 1         
    19 : add       i31, i31, 16   
    20 : ret                      
