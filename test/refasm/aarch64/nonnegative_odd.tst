nonnegative_odd(i0, i1)
     0 : sub   sp, sp, #0x40    ; ff 03 01 d1  
     1 : str   x18, [sp], #0x03 ; f2 0f 00 f9  
     2 : str   x19, [sp], #0x04 ; f3 13 00 f9  
     3 : str   x20, [sp], #0x05 ; f4 17 00 f9  
     4 : str   x21, [sp], #0x06 ; f5 1b 00 f9  
     5 : mov   x2, #0           ; 02 00 80 d2  
     6 : mov   x3, #0           ; 03 00 80 d2  
     7 : mov   x20, #0x02       ; 54 00 80 d2  
     8 : str   x20, [sp], #0x02 ; f4 0b 00 f9  
     9 : mov   x20, #0x04       ; 94 00 80 d2  
    10 : str   x20, [sp], #0x01 ; f4 07 00 f9  
    11 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    12 : sub   x21, x2, x20     ; 55 00 14 cb  
    13 : str   x21, [sp], #0    ; f5 03 00 f9  
    14 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    15 : mul   x1, x1, x20      ; 21 7c 14 9b  
    16 : cmp   x2, x1           ; 5f 00 01 eb  
    17 : b.ge  [38]             ; aa 02 00 54  
    18 : ldrsw x19, [x0], x2    ; 13 68 a2 b8  
    19 : cmp   x19, x3          ; 7f 02 03 eb  
    20 : b.ge  [24]             ; 8a 00 00 54  
    21 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    22 : add   x2, x2, x20      ; 42 00 14 8b  
    23 : b     [16]             ; f9 ff ff 17  
    24 : ldr   x20, [sp], #0x02 ; f4 0b 40 f9  
    25 : sdiv  x18, x19, x20    ; 72 0e d4 9a  
    26 : ldr   x20, [sp], #0x02 ; f4 0b 40 f9  
    27 : mul   x18, x18, x20    ; 52 7e 14 9b  
    28 : sub   x18, x19, x18    ; 72 02 12 cb  
    29 : mov   x18, x18         ; f2 03 12 aa  
    30 : cmp   x18, x3          ; 5f 02 03 eb  
    31 : b.eq  [35]             ; 80 00 00 54  
    32 : mov   x20, x2          ; f4 03 02 aa  
    33 : str   x20, [sp], #0    ; f4 03 00 f9  
    34 : b     [38]             ; 04 00 00 14  
    35 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    36 : add   x2, x2, x20      ; 42 00 14 8b  
    37 : b     [16]             ; eb ff ff 17  
    38 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    39 : ldr   x21, [sp], #0    ; f5 03 40 f9  
    40 : sdiv  x1, x21, x20     ; a1 0e d4 9a  
    41 : mov   x0, x1           ; e0 03 01 aa  
    42 : ldr   x18, [sp], #0x03 ; f2 0f 40 f9  
    43 : ldr   x19, [sp], #0x04 ; f3 13 40 f9  
    44 : ldr   x20, [sp], #0x05 ; f4 17 40 f9  
    45 : ldr   x21, [sp], #0x06 ; f5 1b 40 f9  
    46 : add   sp, sp, #0x40    ; ff 03 01 91  
    47 : ret   x30              ; c0 03 5f d6  
