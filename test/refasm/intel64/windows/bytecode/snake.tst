snake(i0, i1, i2)
     0 : sub        i4, i4, 152         
     1 : spill      14, i2              
     2 : spill      13, i8              
     3 : spill      15, i12             
     4 : spill      16, i13             
     5 : spill      18, i5              
     6 : mov        i5, i4              
     7 : add        i5, i5, 144         
     8 : unspill    i13, 14             
     9 : mov        i9, i13             
    10 : add        i9, i9, s13         
    11 : mov        s12, i9             
    12 : sub        s12, s12, 1         
    13 : mov        s11, 0              
    14 : mov        s10, 1              
    15 : mov        i13, s10            
    16 : neg        i13, i13            
    17 : spill      8, i13              
    18 : mov        s9, 0               
    19 : label 0:                       
    20 : unspill    i13, 12             
    21 : cmp        s9, i13             
    22 : jmp_ge 2                       
    23 : mov        i12, 0              
    24 : mov        i0, 0               
    25 : unspill    i13, 9              
    26 : mov        i9, i13             
    27 : and        i9, i9, 1           
    28 : cmp        i9, 0               
    29 : jmp_eq 4                       
    30 : unspill    i13, 13             
    31 : mov        i9, i13             
    32 : sub        i9, i9, 1           
    33 : unspill    i13, 9              
    34 : mov        i12, i13            
    35 : cmp        i12, i9             
    36 : select_gt  i12, i9, i12        
    37 : unspill    i13, 9              
    38 : mov        i8, i13             
    39 : sub        i8, i8, i9          
    40 : mov        i2, 0               
    41 : cmp        s9, i9              
    42 : mov        i0, i2              
    43 : select_gt  i0, i8, i0          
    44 : jmp        5                   
    45 : label 4:                       
    46 : unspill    i13, 14             
    47 : mov        i2, i13             
    48 : sub        i2, i2, 1           
    49 : unspill    i13, 9              
    50 : mov        i8, i13             
    51 : sub        i8, i8, i2          
    52 : mov        i9, 0               
    53 : cmp        s9, i2              
    54 : mov        i12, i9             
    55 : select_gt  i12, i8, i12        
    56 : unspill    i13, 9              
    57 : mov        i0, i13             
    58 : cmp        i0, i2              
    59 : select_gt  i0, i2, i0          
    60 : label 5:                       
    61 : label 6:                       
    62 : cmp        i12, 0              
    63 : jmp_gt 8                       
    64 : cmp        i12, s13            
    65 : jmp_ge 8                       
    66 : cmp        i0, 0               
    67 : jmp_gt 8                       
    68 : cmp        i0, s14             
    69 : jmp_ge 8                       
    70 : mov        i2, 140702878851312 
    71 : spill      0, i0               
    72 : spill      1, i1               
    73 : spill      2, i2               
    74 : spill      3, i8               
    75 : spill      4, i9               
    76 : spill      5, i10              
    77 : spill      6, i11              
    78 : mov        i1, i12             
    79 : mov        i2, i0              
    80 : unspill    i10, 2              
    81 : sub        i4, i4, 32          
    82 : call_noret [i10]()             
    83 : add        i4, i4, 32          
    84 : unspill    i0, 0               
    85 : unspill    i1, 1               
    86 : unspill    i2, 2               
    87 : unspill    i8, 3               
    88 : unspill    i9, 4               
    89 : unspill    i10, 5              
    90 : unspill    i11, 6              
    91 : mov        i2, i0              
    92 : mul        i2, i2, s13         
    93 : add        i2, i2, i12         
    94 : unspill    i13, 11             
    95 : store.u8   i1, i2, i13         
    96 : add        s11, s11, 1         
    97 : add        i12, i12, s10       
    98 : add        i0, i0, s8          
    99 : jmp        6                   
   100 : label 8:                       
   101 : neg        s10, s10            
   102 : neg        s8, s8              
   103 : add        s9, s9, 1           
   104 : jmp        0                   
   105 : label 2:                       
   106 : unspill    i5, 18              
   107 : unspill    i12, 15             
   108 : unspill    i13, 16             
   109 : add        i4, i4, 152         
   110 : ret                            
