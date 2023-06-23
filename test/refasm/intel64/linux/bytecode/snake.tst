snake(i0, i1, i2)
     0 : sub        i4, i4, 184        
     1 : spill      18, i6             
     2 : spill      17, i2             
     3 : spill      19, i12            
     4 : spill      20, i13            
     5 : spill      22, i5             
     6 : mov        i5, i4             
     7 : add        i5, i5, 176        
     8 : unspill    i13, 18            
     9 : mov        i1, i13            
    10 : add        i1, i1, s17        
    11 : mov        s16, i1            
    12 : sub        s16, s16, 1        
    13 : mov        s15, 0             
    14 : mov        s14, 1             
    15 : mov        i13, s14           
    16 : neg        i13, i13           
    17 : spill      12, i13            
    18 : mov        s13, 0             
    19 : label 0:                      
    20 : unspill    i13, 16            
    21 : cmp        s13, i13           
    22 : jmp_ge 2                      
    23 : mov        i12, 0             
    24 : mov        i0, 0              
    25 : unspill    i13, 13            
    26 : mov        i1, i13            
    27 : and        i1, i1, 1          
    28 : cmp        i1, 0              
    29 : jmp_eq 4                      
    30 : unspill    i13, 17            
    31 : mov        i1, i13            
    32 : sub        i1, i1, 1          
    33 : unspill    i13, 13            
    34 : mov        i12, i13           
    35 : cmp        i12, i1            
    36 : select_gt  i12, i1, i12       
    37 : unspill    i13, 13            
    38 : mov        i2, i13            
    39 : sub        i2, i2, i1         
    40 : mov        i6, 0              
    41 : cmp        s13, i1            
    42 : mov        i0, i6             
    43 : select_gt  i0, i2, i0         
    44 : jmp        5                  
    45 : label 4:                      
    46 : unspill    i13, 18            
    47 : mov        i6, i13            
    48 : sub        i6, i6, 1          
    49 : unspill    i13, 13            
    50 : mov        i2, i13            
    51 : sub        i2, i2, i6         
    52 : mov        i1, 0              
    53 : cmp        s13, i6            
    54 : mov        i12, i1            
    55 : select_gt  i12, i2, i12       
    56 : unspill    i13, 13            
    57 : mov        i0, i13            
    58 : cmp        i0, i6             
    59 : select_gt  i0, i6, i0         
    60 : label 5:                      
    61 : label 6:                      
    62 : cmp        i12, 0             
    63 : jmp_gt 8                      
    64 : cmp        i12, s17           
    65 : jmp_ge 8                      
    66 : cmp        i0, 0              
    67 : jmp_gt 8                      
    68 : cmp        i0, s18            
    69 : jmp_ge 8                      
    70 : mov        i6, 94826573737671 
    71 : spill      0, i0              
    72 : spill      1, i1              
    73 : spill      2, i2              
    74 : spill      3, i6              
    75 : spill      4, i7              
    76 : spill      5, i8              
    77 : spill      6, i9              
    78 : spill      7, i10             
    79 : spill      8, i11             
    80 : mov        i7, i12            
    81 : mov        i6, i0             
    82 : unspill    i10, 3             
    83 : call_noret [i10]()            
    84 : unspill    i0, 0              
    85 : unspill    i1, 1              
    86 : unspill    i2, 2              
    87 : unspill    i6, 3              
    88 : unspill    i7, 4              
    89 : unspill    i8, 5              
    90 : unspill    i9, 6              
    91 : unspill    i10, 7             
    92 : unspill    i11, 8             
    93 : mov        i6, i0             
    94 : mul        i6, i6, s17        
    95 : add        i6, i6, i12        
    96 : unspill    i13, 15            
    97 : store.u8   i7, i6, i13        
    98 : add        s15, s15, 1        
    99 : add        i12, i12, s14      
   100 : add        i0, i0, s12        
   101 : jmp        6                  
   102 : label 8:                      
   103 : neg        s14, s14           
   104 : neg        s12, s12           
   105 : add        s13, s13, 1        
   106 : jmp        0                  
   107 : label 2:                      
   108 : unspill    i5, 22             
   109 : unspill    i12, 19            
   110 : unspill    i13, 20            
   111 : add        i4, i4, 184        
   112 : ret                           
