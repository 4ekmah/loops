helloworld_call()
     0 : sub        i4, i4, 104        
     1 : spill      12, i5             
     2 : mov        i5, i4             
     3 : add        i5, i5, 96         
     4 : mov        i7, 94826573737272 
     5 : spill      0, i0              
     6 : spill      1, i1              
     7 : spill      2, i2              
     8 : spill      3, i6              
     9 : spill      4, i7              
    10 : spill      5, i8              
    11 : spill      6, i9              
    12 : spill      7, i10             
    13 : spill      8, i11             
    14 : call_noret [i7]()             
    15 : unspill    i0, 0              
    16 : unspill    i1, 1              
    17 : unspill    i2, 2              
    18 : unspill    i6, 3              
    19 : unspill    i7, 4              
    20 : unspill    i8, 5              
    21 : unspill    i9, 6              
    22 : unspill    i10, 7             
    23 : unspill    i11, 8             
    24 : unspill    i5, 12             
    25 : add        i4, i4, 104        
    26 : ret                           
