min_max_scalar(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x50    ; ff 43 01 d1  
     1 : str   x18, [sp], #0x05 ; f2 17 00 f9  
     2 : str   x19, [sp], #0x06 ; f3 1b 00 f9  
     3 : str   x20, [sp], #0x07 ; f4 1f 00 f9  
     4 : str   x21, [sp], #0x08 ; f5 23 00 f9  
     5 : mov   x18, #0          ; 12 00 80 d2  
     6 : mov   x20, #0          ; 14 00 80 d2  
     7 : str   x20, [sp], #0x01 ; f4 07 00 f9  
     8 : mov   x20, #0          ; 14 00 80 d2  
     9 : str   x20, [sp], #0    ; f4 03 00 f9  
    10 : ldrsw x20, [x0], #0    ; 14 04 80 b8  
    11 : str   x20, [sp], #0x04 ; f4 13 00 f9  
    12 : ldr   x20, [sp], #0x04 ; f4 13 40 f9  
    13 : mov   x21, x20         ; f5 03 14 aa  
    14 : str   x21, [sp], #0x02 ; f5 0b 00 f9  
    15 : mov   x20, #0x04       ; 94 00 80 d2  
    16 : str   x20, [sp], #0x03 ; f4 0f 00 f9  
    17 : ldr   x20, [sp], #0x03 ; f4 0f 40 f9  
    18 : mul   x1, x1, x20      ; 21 7c 14 9b  
    19 : ldrsw x19, [x0], x18   ; 13 68 b2 b8  
    20 : ldr   x20, [sp], #0x04 ; f4 13 40 f9  
    21 : cmp   x19, x20         ; 7f 02 14 eb  
    22 : b.ge  [27]             ; aa 00 00 54  
    23 : mov   x20, x19         ; f4 03 13 aa  
    24 : str   x20, [sp], #0x04 ; f4 13 00 f9  
    25 : mov   x20, x18         ; f4 03 12 aa  
    26 : str   x20, [sp], #0x01 ; f4 07 00 f9  
    27 : ldr   x20, [sp], #0x02 ; f4 0b 40 f9  
    28 : cmp   x19, x20         ; 7f 02 14 eb  
    29 : b.le  [34]             ; ad 00 00 54  
    30 : mov   x20, x19         ; f4 03 13 aa  
    31 : str   x20, [sp], #0x02 ; f4 0b 00 f9  
    32 : mov   x20, x18         ; f4 03 12 aa  
    33 : str   x20, [sp], #0    ; f4 03 00 f9  
    34 : ldr   x20, [sp], #0x03 ; f4 0f 40 f9  
    35 : add   x18, x18, x20    ; 52 02 14 8b  
    36 : cmp   x18, x1          ; 5f 02 01 eb  
    37 : b.lt  [19]             ; cb fd ff 54  
    38 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    39 : ldr   x21, [sp], #0x03 ; f5 0f 40 f9  
    40 : sdiv  x0, x20, x21     ; 80 0e d5 9a  
    41 : ldr   x20, [sp], #0    ; f4 03 40 f9  
    42 : ldr   x21, [sp], #0x03 ; f5 0f 40 f9  
    43 : sdiv  x1, x20, x21     ; 81 0e d5 9a  
    44 : str   w0, [x2], #0     ; 40 00 00 b9  
    45 : str   w1, [x3], #0     ; 61 00 00 b9  
    46 : mov   x3, #0           ; 03 00 80 d2  
    47 : mov   x0, x3           ; e0 03 03 aa  
    48 : ldr   x18, [sp], #0x05 ; f2 17 40 f9  
    49 : ldr   x19, [sp], #0x06 ; f3 1b 40 f9  
    50 : ldr   x20, [sp], #0x07 ; f4 1f 40 f9  
    51 : ldr   x21, [sp], #0x08 ; f5 23 40 f9  
    52 : add   sp, sp, #0x50    ; ff 43 01 91  
    53 : ret   x30              ; c0 03 5f d6  
