exponentiation_by_squaring_v_double_9(i0, i1, i2)
     0 : sub  sp, sp, #0x10       ; ff 43 00 d1  
     1 : str  x18, [sp], #0       ; f2 03 00 f9  
     2 : mov  x3, #0              ; 03 00 80 d2  
     3 : mov  x18, #0x08          ; 12 01 80 d2  
     4 : mul  x2, x2, x18         ; 42 7c 12 9b  
     5 : cmp  x3, x2              ; 7f 00 02 eb  
     6 : b.ge [15]                ; 2a 01 00 54  
     7 : ldr  v0.2d, x0, x3       ; 00 68 e3 3c  
     8 : fmul v1.2d, v0.2d, v0.2d ; 01 dc 60 6e  
     9 : fmul v1.2d, v1.2d, v1.2d ; 21 dc 61 6e  
    10 : fmul v1.2d, v1.2d, v1.2d ; 21 dc 61 6e  
    11 : fmul v0.2d, v0.2d, v1.2d ; 00 dc 61 6e  
    12 : str  v0.2d, x1, x3       ; 20 68 a3 3c  
    13 : add  x3, x3, #0x10       ; 63 40 00 91  
    14 : b    [5]                 ; f7 ff ff 17  
    15 : mov  x0, #0              ; 00 00 80 d2  
    16 : ldr  x18, [sp], #0       ; f2 03 40 f9  
    17 : add  sp, sp, #0x10       ; ff 43 00 91  
    18 : ret  x30                 ; c0 03 5f d6  
