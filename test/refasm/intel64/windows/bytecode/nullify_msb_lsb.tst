nullify_msb_lsb(i0, i1, i2)
     0 : mov       i9, i1     
     1 : shr       i9, i9, 1  
     2 : or        i9, i9, i1 
     3 : mov       i0, i9     
     4 : shr       i0, i0, 2  
     5 : or        i9, i9, i0 
     6 : mov       i0, i9     
     7 : shr       i0, i0, 4  
     8 : or        i9, i9, i0 
     9 : mov       i0, i9     
    10 : shr       i0, i0, 8  
    11 : or        i9, i9, i0 
    12 : mov       i0, i9     
    13 : shr       i0, i0, 16 
    14 : or        i9, i9, i0 
    15 : mov       i0, i9     
    16 : shr       i0, i0, 32 
    17 : or        i9, i9, i0 
    18 : add       i9, i9, 1  
    19 : shr       i9, i9, 1  
    20 : xor       i9, i9, i1 
    21 : store.u64 i8, i9     
    22 : mov       i8, i1     
    23 : sub       i8, i8, 1  
    24 : not       i8, i8     
    25 : and       i8, i8, i1 
    26 : xor       i8, i8, i1 
    27 : store.u64 i2, i8     
    28 : ret                  
