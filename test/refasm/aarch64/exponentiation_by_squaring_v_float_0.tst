exponentiation_by_squaring_v_float_0(i0, i1, i2)
     0 : sub  sp, sp, #0x10       ; ff 43 00 d1  
     1 : str  x18, [sp], #0       ; f2 03 00 f9  
     2 : mov  x3, #0              ; 03 00 80 d2  
     3 : mov  x18, #0x04          ; 92 00 80 d2  
     4 : mul  x2, x2, x18         ; 42 7c 12 9b  
     5 : cmp  x3, x2              ; 7f 00 02 eb  
     6 : b.ge [14]                ; 0a 01 00 54  
     7 : ldr  v0.4s, x0, x3       ; 00 68 e3 3c  
     8 : mov  x18, #0             ; 12 00 80 d2  
     9 : movk x18, #0x3f80, #0x01 ; 12 f0 a7 f2  
    10 : dup  v0.4s, x18          ; 40 0e 04 4e  
    11 : str  v0.4s, x1, x3       ; 20 68 a3 3c  
    12 : add  x3, x3, #0x10       ; 63 40 00 91  
    13 : b    [5]                 ; f8 ff ff 17  
    14 : mov  x0, #0              ; 00 00 80 d2  
    15 : ldr  x18, [sp], #0       ; f2 03 40 f9  
    16 : add  sp, sp, #0x10       ; ff 43 00 91  
    17 : ret  x30                 ; c0 03 5f d6  
