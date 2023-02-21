nullify_msb_lsb(i0, i1, i2)
     0 : sub       i4, i4, 40 
     1 : mov       i1, i7     
     2 : shr       i1, i1, 1  
     3 : or        i1, i1, i7 
     4 : mov       i0, i1     
     5 : shr       i0, i0, 2  
     6 : or        i1, i1, i0 
     7 : mov       i0, i1     
     8 : shr       i0, i0, 4  
     9 : or        i1, i1, i0 
    10 : mov       i0, i1     
    11 : shr       i0, i0, 8  
    12 : or        i1, i1, i0 
    13 : mov       i0, i1     
    14 : shr       i0, i0, 16 
    15 : or        i1, i1, i0 
    16 : mov       i0, i1     
    17 : shr       i0, i0, 32 
    18 : or        i1, i1, i0 
    19 : add       i1, i1, 1  
    20 : shr       i1, i1, 1  
    21 : xor       i1, i1, i7 
    22 : store.u64 i2, i1     
    23 : mov       i2, i7     
    24 : sub       i2, i2, 1  
    25 : not       i2, i2     
    26 : and       i2, i2, i7 
    27 : xor       i2, i2, i7 
    28 : store.u64 i6, i2     
    29 : add       i4, i4, 40 
    30 : ret                  
