sort_double(i0, i1)
     0 : sub        i4, i4, 120         
     1 : spill      11, i12             
     2 : spill      12, i13             
     3 : spill      14, i5              
     4 : mov        i5, i4              
     5 : add        i5, i5, 112         
     6 : mov        s10, i2             
     7 : shl        s10, s10, 3         
     8 : unspill    i13, 10             
     9 : mov        s9, i13             
    10 : sub        s9, s9, 8           
    11 : mov        s8, 0               
    12 : label 0:                       
    13 : unspill    i13, 9              
    14 : cmp        s8, i13             
    15 : jmp_ge 2                       
    16 : mov        i0, s8              
    17 : mov        i12, i0             
    18 : add        i12, i12, 8         
    19 : label 3:                       
    20 : cmp        i12, s10            
    21 : jmp_ge 5                       
    22 : load.fp64  i9, i1, i12         
    23 : load.fp64  i8, i1, i0          
    24 : mov        i2, 140702878852848 
    25 : spill      0, i0               
    26 : spill      1, i1               
    27 : spill      2, i2               
    28 : spill      3, i8               
    29 : spill      4, i9               
    30 : spill      5, i10              
    31 : spill      6, i11              
    32 : mov        i1, i9              
    33 : mov        i2, i8              
    34 : unspill    i10, 2              
    35 : sub        i4, i4, 32          
    36 : call_noret [i10]()             
    37 : add        i4, i4, 32          
    38 : mov        i2, i0              
    39 : unspill    i0, 0               
    40 : unspill    i1, 1               
    41 : unspill    i8, 3               
    42 : unspill    i9, 4               
    43 : unspill    i10, 5              
    44 : unspill    i11, 6              
    45 : cmp        i2, 0               
    46 : jmp_eq 7                       
    47 : mov        i0, i12             
    48 : label 7:                       
    49 : add        i12, i12, 8         
    50 : jmp        3                   
    51 : label 5:                       
    52 : cmp        i0, s8              
    53 : jmp_eq 9                       
    54 : unspill    i13, 8              
    55 : load.fp64  i2, i1, i13         
    56 : load.fp64  i8, i1, i0          
    57 : store.fp64 i1, i0, i2          
    58 : unspill    i13, 8              
    59 : store.fp64 i1, i13, i8         
    60 : label 9:                       
    61 : add        s8, s8, 8           
    62 : jmp        0                   
    63 : label 2:                       
    64 : unspill    i5, 14              
    65 : unspill    i12, 11             
    66 : unspill    i13, 12             
    67 : add        i4, i4, 120         
    68 : ret                            
