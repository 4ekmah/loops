helloworld_call()
     0 : sub        i4, i4, 72          
     1 : spill      8, i5               
     2 : mov        i5, i4              
     3 : add        i5, i5, 64          
     4 : mov        i1, 140702878850832 
     5 : spill      0, i0               
     6 : spill      1, i1               
     7 : spill      2, i2               
     8 : spill      3, i8               
     9 : spill      4, i9               
    10 : spill      5, i10              
    11 : spill      6, i11              
    12 : sub        i4, i4, 32          
    13 : call_noret [i1]()              
    14 : add        i4, i4, 32          
    15 : unspill    i0, 0               
    16 : unspill    i1, 1               
    17 : unspill    i2, 2               
    18 : unspill    i8, 3               
    19 : unspill    i9, 4               
    20 : unspill    i10, 5              
    21 : unspill    i11, 6              
    22 : unspill    i5, 8               
    23 : add        i4, i4, 72          
    24 : ret                            
