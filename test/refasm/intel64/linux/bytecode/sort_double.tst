sort_double(i0, i1)
     0 : sub        i4, i4, 152        
     1 : spill      15, i12            
     2 : spill      16, i13            
     3 : spill      18, i5             
     4 : mov        i5, i4             
     5 : add        i5, i5, 144        
     6 : mov        s14, i6            
     7 : shl        s14, s14, 3        
     8 : unspill    i13, 14            
     9 : mov        s13, i13           
    10 : sub        s13, s13, 8        
    11 : mov        s12, 0             
    12 : label 0:                      
    13 : unspill    i13, 13            
    14 : cmp        s12, i13           
    15 : jmp_ge 2                      
    16 : mov        i0, s12            
    17 : mov        i12, i0            
    18 : add        i12, i12, 8        
    19 : label 3:                      
    20 : cmp        i12, s14           
    21 : jmp_ge 5                      
    22 : load.fp64  i1, i7, i12        
    23 : load.fp64  i2, i7, i0         
    24 : mov        i6, 94826573738795 
    25 : spill      0, i0              
    26 : spill      1, i1              
    27 : spill      2, i2              
    28 : spill      3, i6              
    29 : spill      4, i7              
    30 : spill      5, i8              
    31 : spill      6, i9              
    32 : spill      7, i10             
    33 : spill      8, i11             
    34 : mov        i7, i1             
    35 : mov        i6, i2             
    36 : unspill    i10, 3             
    37 : call_noret [i10]()            
    38 : mov        i6, i0             
    39 : unspill    i0, 0              
    40 : unspill    i1, 1              
    41 : unspill    i2, 2              
    42 : unspill    i7, 4              
    43 : unspill    i8, 5              
    44 : unspill    i9, 6              
    45 : unspill    i10, 7             
    46 : unspill    i11, 8             
    47 : cmp        i6, 0              
    48 : jmp_eq 7                      
    49 : mov        i0, i12            
    50 : label 7:                      
    51 : add        i12, i12, 8        
    52 : jmp        3                  
    53 : label 5:                      
    54 : cmp        i0, s12            
    55 : jmp_eq 9                      
    56 : unspill    i13, 12            
    57 : load.fp64  i6, i7, i13        
    58 : load.fp64  i2, i7, i0         
    59 : store.fp64 i7, i0, i6         
    60 : unspill    i13, 12            
    61 : store.fp64 i7, i13, i2        
    62 : label 9:                      
    63 : add        s12, s12, 8        
    64 : jmp        0                  
    65 : label 2:                      
    66 : unspill    i5, 18             
    67 : unspill    i12, 15            
    68 : unspill    i13, 16            
    69 : add        i4, i4, 152        
    70 : ret                           
