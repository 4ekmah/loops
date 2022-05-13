min_max_scalar(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x50    ; ff 43 01 d1  
     1 : str   x2, [sp], #0x01  ; e2 07 00 f9  
     2 : str   x3, [sp], #0     ; e3 03 00 f9  
     3 : str   x18, [sp], #0x05 ; f2 17 00 f9  
     4 : str   x19, [sp], #0x06 ; f3 1b 00 f9  
     5 : str   x20, [sp], #0x07 ; f4 1f 00 f9  
     6 : str   x21, [sp], #0x08 ; f5 23 00 f9  
     7 : mov   x18, #0          ; 12 00 80 d2  
     8 : mov   x20, #0          ; 14 00 80 d2  
     9 : str   x20, [sp], #0x04 ; f4 13 00 f9  
    10 : mov   x20, #0          ; 14 00 80 d2  
    11 : str   x20, [sp], #0x02 ; f4 0b 00 f9  
    12 : ldrsw x2, [x0], #0     ; 02 04 80 b8  
    13 : mov   x3, x2           ; e3 03 02 aa  
    14 : mov   x20, #0x04       ; 94 00 80 d2  
    15 : str   x20, [sp], #0x03 ; f4 0f 00 f9  
    16 : ldr   x20, [sp], #0x03 ; f4 0f 40 f9  
    17 : mul   x1, x1, x20      ; 21 7c 14 9b  
    18 : ldrsw x19, [x0], x18   ; 13 68 b2 b8  
    19 : cmp   x19, x2          ; 7f 02 02 eb  
    20 : b.ge  [24]             ; 8a 00 00 54  
    21 : mov   x2, x19          ; e2 03 13 aa  
    22 : mov   x20, x18         ; f4 03 12 aa  
    23 : str   x20, [sp], #0x04 ; f4 13 00 f9  
    24 : cmp   x19, x3          ; 7f 02 03 eb  
    25 : b.le  [29]             ; 8d 00 00 54  
    26 : mov   x3, x19          ; e3 03 13 aa  
    27 : mov   x20, x18         ; f4 03 12 aa  
    28 : str   x20, [sp], #0x02 ; f4 0b 00 f9  
    29 : ldr   x20, [sp], #0x03 ; f4 0f 40 f9  
    30 : add   x18, x18, x20    ; 52 02 14 8b  
    31 : cmp   x18, x1          ; 5f 02 01 eb  
    32 : b.lt  [18]             ; 4b fe ff 54  
    33 : ldr   x20, [sp], #0x04 ; f4 13 40 f9  
    34 : ldr   x21, [sp], #0x03 ; f5 0f 40 f9  
    35 : sdiv  x0, x20, x21     ; 80 0e d5 9a  
    36 : ldr   x20, [sp], #0x02 ; f4 0b 40 f9  
    37 : ldr   x21, [sp], #0x03 ; f5 0f 40 f9  
    38 : sdiv  x1, x20, x21     ; 81 0e d5 9a  
    39 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    40 : str   w0, [x20], #0    ; 80 02 00 b9  
    41 : ldr   x20, [sp], #0    ; f4 03 40 f9  
    42 : str   w1, [x20], #0    ; 81 02 00 b9  
    43 : mov   x0, #0           ; 00 00 80 d2  
    44 : mov   x0, x0           ; e0 03 00 aa  
    45 : ldr   x18, [sp], #0x05 ; f2 17 40 f9  
    46 : ldr   x19, [sp], #0x06 ; f3 1b 40 f9  
    47 : ldr   x20, [sp], #0x07 ; f4 1f 40 f9  
    48 : ldr   x21, [sp], #0x08 ; f5 23 40 f9  
    49 : add   sp, sp, #0x50    ; ff 43 01 91  
    50 : ret   x30              ; c0 03 5f d6  
