nullify_msb_lsb(i0, i1, i2)
     0 : sub       i4, i4, 40 
     1 : mov       i9, i1     
     2 : shr       i9, i9, 1  
     3 : or        i9, i9, i1 
     4 : mov       i0, i9     
     5 : shr       i0, i0, 2  
     6 : or        i9, i9, i0 
     7 : mov       i0, i9     
     8 : shr       i0, i0, 4  
     9 : or        i9, i9, i0 
    10 : mov       i0, i9     
    11 : shr       i0, i0, 8  
    12 : or        i9, i9, i0 
    13 : mov       i0, i9     
    14 : shr       i0, i0, 16 
    15 : or        i9, i9, i0 
    16 : mov       i0, i9     
    17 : shr       i0, i0, 32 
    18 : or        i9, i9, i0 
    19 : add       i9, i9, 1  
    20 : shr       i9, i9, 1  
    21 : xor       i9, i9, i1 
    22 : store.u64 i8, i9     
    23 : mov       i8, i1     
    24 : sub       i8, i8, 1  
    25 : not       i8, i8     
    26 : and       i8, i8, i1 
    27 : xor       i8, i8, i1 
    28 : store.u64 i2, i8     
    29 : add       i4, i4, 40 
    30 : ret                  
