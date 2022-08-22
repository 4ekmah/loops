exponentiation_by_squaring_v_float_9(i0, i1, i2)
     0 : sub  sp, sp, #0x10       ; ff 43 00 d1  
     1 : str  x18, [sp], #0       ; f2 03 00 f9  
     2 : mov  x3, #0              ; 03 00 80 d2  
     3 : mov  x18, #0x04          ; 92 00 80 d2  
     4 : mul  x2, x2, x18         ; 42 7c 12 9b  
     5 : cmp  x3, x2              ; 7f 00 02 eb  
     6 : b.ge [16]                ; 4a 01 00 54  
     7 : ldr  v0.4s, x0, x3       ; 00 68 e3 3c  
     8 : mov  v1.4s, v0.4s        ; 01 1c a0 4e  
     9 : fmul v0.4s, v0.4s, v0.4s ; 00 dc 20 6e  
    10 : fmul v0.4s, v0.4s, v0.4s ; 00 dc 20 6e  
    11 : fmul v0.4s, v0.4s, v0.4s ; 00 dc 20 6e  
    12 : fmul v0.4s, v1.4s, v0.4s ; 20 dc 20 6e  
    13 : str  v0.4s, x1, x3       ; 20 68 a3 3c  
    14 : add  x3, x3, #0x10       ; 63 40 00 91  
    15 : b    [5]                 ; f6 ff ff 17  
    16 : mov  x0, #0              ; 00 00 80 d2  
    17 : ldr  x18, [sp], #0       ; f2 03 40 f9  
    18 : add  sp, sp, #0x10       ; ff 43 00 91  
    19 : ret  x30                 ; c0 03 5f d6  
