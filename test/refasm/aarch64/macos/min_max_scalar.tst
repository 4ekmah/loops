min_max_scalar(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x40    ; ff 03 01 d1  
     1 : str   x2, [sp], #0x01  ; e2 07 00 f9  
     2 : str   x3, [sp], #0     ; e3 03 00 f9  
     3 : str   x18, [sp], #0x04 ; f2 13 00 f9  
     4 : str   x19, [sp], #0x05 ; f3 17 00 f9  
     5 : str   x20, [sp], #0x06 ; f4 1b 00 f9  
     6 : eor   x18, x18, x18    ; 52 02 12 ca  
     7 : eor   x20, x20, x20    ; 94 02 14 ca  
     8 : str   x20, [sp], #0x03 ; f4 0f 00 f9  
     9 : eor   x20, x20, x20    ; 94 02 14 ca  
    10 : str   x20, [sp], #0x02 ; f4 0b 00 f9  
    11 : ldrsw x2, [x0], #0     ; 02 00 80 b9  
    12 : mov   x3, x2           ; e3 03 02 aa  
    13 : mov   x19, #0x04       ; 93 00 80 d2  
    14 : mul   x1, x1, x19      ; 21 7c 13 9b  
    15 : cmp   x18, x1          ; 5f 02 01 eb  
    16 : b.ge  [30]             ; ca 01 00 54  
    17 : ldrsw x19, [x0], x18   ; 13 68 b2 b8  
    18 : cmp   x19, x2          ; 7f 02 02 eb  
    19 : b.ge  [23]             ; 8a 00 00 54  
    20 : mov   x2, x19          ; e2 03 13 aa  
    21 : mov   x20, x18         ; f4 03 12 aa  
    22 : str   x20, [sp], #0x03 ; f4 0f 00 f9  
    23 : cmp   x19, x3          ; 7f 02 03 eb  
    24 : b.le  [28]             ; 8d 00 00 54  
    25 : mov   x3, x19          ; e3 03 13 aa  
    26 : mov   x20, x18         ; f4 03 12 aa  
    27 : str   x20, [sp], #0x02 ; f4 0b 00 f9  
    28 : add   x18, x18, #0x04  ; 52 12 00 91  
    29 : b     [15]             ; f2 ff ff 17  
    30 : mov   x0, #0x04        ; 80 00 80 d2  
    31 : ldr   x20, [sp], #0x03 ; f4 0f 40 f9  
    32 : sdiv  x1, x20, x0      ; 81 0e c0 9a  
    33 : ldr   x20, [sp], #0x02 ; f4 0b 40 f9  
    34 : sdiv  x0, x20, x0      ; 80 0e c0 9a  
    35 : ldr   x20, [sp], #0x01 ; f4 07 40 f9  
    36 : str   w1, [x20], #0    ; 81 02 00 b9  
    37 : ldr   x20, [sp], #0    ; f4 03 40 f9  
    38 : str   w0, [x20], #0    ; 80 02 00 b9  
    39 : eor   x0, x0, x0       ; 00 00 00 ca  
    40 : ldr   x18, [sp], #0x04 ; f2 13 40 f9  
    41 : ldr   x19, [sp], #0x05 ; f3 17 40 f9  
    42 : ldr   x20, [sp], #0x06 ; f4 1b 40 f9  
    43 : add   sp, sp, #0x40    ; ff 03 01 91  
    44 : ret   x30              ; c0 03 5f d6  
