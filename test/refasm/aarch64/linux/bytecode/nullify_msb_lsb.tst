nullify_msb_lsb(i0, i1, i2)
     0 : sub       i31, i31, 16 
     1 : spill     0, i18       
     2 : shr       i3, i0, 1    
     3 : or        i3, i0, i3   
     4 : shr       i18, i3, 2   
     5 : or        i3, i3, i18  
     6 : shr       i18, i3, 4   
     7 : or        i3, i3, i18  
     8 : shr       i18, i3, 8   
     9 : or        i3, i3, i18  
    10 : shr       i18, i3, 16  
    11 : or        i3, i3, i18  
    12 : shr       i18, i3, 32  
    13 : or        i3, i3, i18  
    14 : add       i3, i3, 1    
    15 : shr       i3, i3, 1    
    16 : xor       i3, i3, i0   
    17 : store.u64 i2, i3       
    18 : sub       i2, i0, 1    
    19 : not       i2, i2       
    20 : and       i2, i0, i2   
    21 : xor       i0, i2, i0   
    22 : store.u64 i1, i0       
    23 : unspill   i18, 0       
    24 : add       i31, i31, 16 
    25 : ret                    
