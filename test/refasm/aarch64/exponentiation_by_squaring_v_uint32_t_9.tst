exponentiation_by_squaring_v_uint32_t_9(i0, i1, i2)
     0 : sub  sp, sp, #0x10       ; ff 43 00 d1  
     1 : str  x18, [sp], #0       ; f2 03 00 f9  
     2 : mov  x3, #0              ; 03 00 80 d2  
     3 : mov  x18, #0x04          ; 92 00 80 d2  
     4 : mul  x2, x2, x18         ; 42 7c 12 9b  
     5 : cmp  x3, x2              ; 7f 00 02 eb  
     6 : b.ge [17]                ; 6a 01 00 54  
     7 : ldr  v0.4s, x0, x3       ; 00 68 e3 3c  
     8 : mov  v0.4s, v0.4s        ; 00 1c a0 4e  
     9 : mov  v1.4s, v0.4s        ; 01 1c a0 4e  
    10 : mul  v0.4s, v0.4s, v0.4s ; 00 9c a0 4e  
    11 : mul  v0.4s, v0.4s, v0.4s ; 00 9c a0 4e  
    12 : mul  v0.4s, v0.4s, v0.4s ; 00 9c a0 4e  
    13 : mul  v0.4s, v1.4s, v0.4s ; 20 9c a0 4e  
    14 : str  v0.4s, x1, x3       ; 20 68 a3 3c  
    15 : add  x3, x3, #0x10       ; 63 40 00 91  
    16 : b    [5]                 ; f5 ff ff 17  
    17 : mov  x0, #0              ; 00 00 80 d2  
    18 : ldr  x18, [sp], #0       ; f2 03 40 f9  
    19 : add  sp, sp, #0x10       ; ff 43 00 91  
    20 : ret  x30                 ; c0 03 5f d6  