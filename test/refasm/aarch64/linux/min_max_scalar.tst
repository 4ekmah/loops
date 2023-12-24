min_max_scalar(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x40    ; ff 03 01 d1  
     1 : str   x2, [sp], #0x01  ; e2 07 00 f9  
     2 : str   x3, [sp], #0     ; e3 03 00 f9  
     3 : str   x25, [sp], #0x05 ; f9 17 00 f9  
     4 : str   x26, [sp], #0x06 ; fa 1b 00 f9  
     5 : eor   x25, x25, x25    ; 39 03 19 ca  
     6 : eor   x26, x26, x26    ; 5a 03 1a ca  
     7 : str   x26, [sp], #0x03 ; fa 0f 00 f9  
     8 : eor   x26, x26, x26    ; 5a 03 1a ca  
     9 : str   x26, [sp], #0x02 ; fa 0b 00 f9  
    10 : ldrsw x2, [x0], #0     ; 02 00 80 b9  
    11 : mov   x26, x2          ; fa 03 02 aa  
    12 : str   x26, [sp], #0x04 ; fa 13 00 f9  
    13 : mov   x3, #0x04        ; 83 00 80 d2  
    14 : mul   x1, x1, x3       ; 21 7c 03 9b  
    15 : cmp   x25, x1          ; 3f 03 01 eb  
    16 : b.ge  [32]             ; 0a 02 00 54  
    17 : ldrsw x3, [x0], x25    ; 03 68 b9 b8  
    18 : cmp   x3, x2           ; 7f 00 02 eb  
    19 : b.ge  [23]             ; 8a 00 00 54  
    20 : mov   x2, x3           ; e2 03 03 aa  
    21 : mov   x26, x25         ; fa 03 19 aa  
    22 : str   x26, [sp], #0x03 ; fa 0f 00 f9  
    23 : ldr   x26, [sp], #0x04 ; fa 13 40 f9  
    24 : cmp   x3, x26          ; 7f 00 1a eb  
    25 : b.le  [30]             ; ad 00 00 54  
    26 : mov   x26, x3          ; fa 03 03 aa  
    27 : str   x26, [sp], #0x04 ; fa 13 00 f9  
    28 : mov   x26, x25         ; fa 03 19 aa  
    29 : str   x26, [sp], #0x02 ; fa 0b 00 f9  
    30 : add   x25, x25, #0x04  ; 39 13 00 91  
    31 : b     [15]             ; f0 ff ff 17  
    32 : mov   x0, #0x04        ; 80 00 80 d2  
    33 : ldr   x26, [sp], #0x03 ; fa 0f 40 f9  
    34 : sdiv  x1, x26, x0      ; 41 0f c0 9a  
    35 : ldr   x26, [sp], #0x02 ; fa 0b 40 f9  
    36 : sdiv  x0, x26, x0      ; 40 0f c0 9a  
    37 : ldr   x26, [sp], #0x01 ; fa 07 40 f9  
    38 : str   w1, [x26], #0    ; 41 03 00 b9  
    39 : ldr   x26, [sp], #0    ; fa 03 40 f9  
    40 : str   w0, [x26], #0    ; 40 03 00 b9  
    41 : eor   x0, x0, x0       ; 00 00 00 ca  
    42 : ldr   x25, [sp], #0x05 ; f9 17 40 f9  
    43 : ldr   x26, [sp], #0x06 ; fa 1b 40 f9  
    44 : add   sp, sp, #0x40    ; ff 03 01 91  
    45 : ret   x30              ; c0 03 5f d6  
