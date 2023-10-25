nullify_msb_lsb(i0, i1, i2)
     0 : mov       i1, i7     
     1 : shr       i1, i1, 1  
     2 : or        i1, i1, i7 
     3 : mov       i0, i1     
     4 : shr       i0, i0, 2  
     5 : or        i1, i1, i0 
     6 : mov       i0, i1     
     7 : shr       i0, i0, 4  
     8 : or        i1, i1, i0 
     9 : mov       i0, i1     
    10 : shr       i0, i0, 8  
    11 : or        i1, i1, i0 
    12 : mov       i0, i1     
    13 : shr       i0, i0, 16 
    14 : or        i1, i1, i0 
    15 : mov       i0, i1     
    16 : shr       i0, i0, 32 
    17 : or        i1, i1, i0 
    18 : add       i1, i1, 1  
    19 : shr       i1, i1, 1  
    20 : xor       i1, i1, i7 
    21 : store.u64 i2, i1     
    22 : mov       i2, i7     
    23 : sub       i2, i2, 1  
    24 : not       i2, i2     
    25 : and       i2, i2, i7 
    26 : xor       i2, i2, i7 
    27 : store.u64 i6, i2     
    28 : ret                  
