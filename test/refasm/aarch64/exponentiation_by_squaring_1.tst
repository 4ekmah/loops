exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub   sp, sp, #0x20    ; ff 83 00 d1  
     1 : str   x18, [sp], #0x01 ; f2 07 00 f9  
     2 : str   x19, [sp], #0x02 ; f3 0b 00 f9  
     3 : str   x20, [sp], #0x03 ; f4 0f 00 f9  
     4 : mov   x3, #0x01        ; 23 00 80 d2  
     5 : mov   x3, #0           ; 03 00 80 d2  
     6 : mov   x20, #0          ; 14 00 80 d2  
     7 : str   x20, [sp], #0    ; f4 03 00 f9  
     8 : ldr   x20, [sp], #0    ; f4 03 40 f9  
     9 : cmp   x20, x1          ; 9f 02 01 eb  
    10 : b.ge  [22]             ; 8a 01 00 54  
    11 : ldrsw x19, [x0], x3    ; 13 68 a3 b8  
    12 : mov   x19, x19         ; f3 03 13 aa  
    13 : mov   x18, #0x01       ; 32 00 80 d2  
    14 : mul   x18, x18, x19    ; 52 7e 13 9b  
    15 : str   w18, [x2], #0    ; 52 00 00 b9  
    16 : ldr   x20, [sp], #0    ; f4 03 40 f9  
    17 : add   x20, x20, #0x01  ; 94 06 00 91  
    18 : str   x20, [sp], #0    ; f4 03 00 f9  
    19 : add   x3, x3, #0x04    ; 63 10 00 91  
    20 : add   x2, x2, #0x04    ; 42 10 00 91  
    21 : b     [8]              ; f3 ff ff 17  
    22 : mov   x0, #0           ; 00 00 80 d2  
    23 : ldr   x18, [sp], #0x01 ; f2 07 40 f9  
    24 : ldr   x19, [sp], #0x02 ; f3 0b 40 f9  
    25 : ldr   x20, [sp], #0x03 ; f4 0f 40 f9  
    26 : add   sp, sp, #0x20    ; ff 83 00 91  
    27 : ret   x30              ; c0 03 5f d6  
