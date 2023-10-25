exponentiation_by_squaring_0(i0, i1, i2)
     0 : sub   sp, sp, #0x10    ; ff 43 00 d1  
     1 : str   x18, [sp], #0    ; f2 03 00 f9  
     2 : str   x19, [sp], #0x01 ; f3 07 00 f9  
     3 : eor   x3, x3, x3       ; 63 00 03 ca  
     4 : eor   x18, x18, x18    ; 52 02 12 ca  
     5 : cmp   x18, x1          ; 5f 02 01 eb  
     6 : b.ge  [14]             ; 0a 01 00 54  
     7 : ldrsw x19, [x0], x3    ; 13 68 a3 b8  
     8 : mov   x19, #0x01       ; 33 00 80 d2  
     9 : str   w19, [x2], #0    ; 53 00 00 b9  
    10 : add   x18, x18, #0x01  ; 52 06 00 91  
    11 : add   x3, x3, #0x04    ; 63 10 00 91  
    12 : add   x2, x2, #0x04    ; 42 10 00 91  
    13 : b     [5]              ; f8 ff ff 17  
    14 : eor   x0, x0, x0       ; 00 00 00 ca  
    15 : ldr   x18, [sp], #0    ; f2 03 40 f9  
    16 : ldr   x19, [sp], #0x01 ; f3 07 40 f9  
    17 : add   sp, sp, #0x10    ; ff 43 00 91  
    18 : ret   x30              ; c0 03 5f d6  
