min_max_select(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x40         ; ff 03 01 d1  
     1 : str   x2, [sp], #0x01       ; e2 07 00 f9  
     2 : str   x3, [sp], #0          ; e3 03 00 f9  
     3 : str   x25, [sp], #0x05      ; f9 17 00 f9  
     4 : str   x26, [sp], #0x06      ; fa 1b 00 f9  
     5 : eor   x25, x25, x25         ; 39 03 19 ca  
     6 : eor   x26, x26, x26         ; 5a 03 1a ca  
     7 : str   x26, [sp], #0x03      ; fa 0f 00 f9  
     8 : eor   x26, x26, x26         ; 5a 03 1a ca  
     9 : str   x26, [sp], #0x02      ; fa 0b 00 f9  
    10 : ldrsw x2, [x0], #0          ; 02 00 80 b9  
    11 : mov   x3, x2                ; e3 03 02 aa  
    12 : lsl   x26, x1, #0x3e, #0x3d ; 3a f4 7e d3  
    13 : str   x26, [sp], #0x04      ; fa 13 00 f9  
    14 : ldr   x26, [sp], #0x04      ; fa 13 40 f9  
    15 : cmp   x25, x26              ; 3f 03 1a eb  
    16 : b.ge  [32]                  ; 0a 02 00 54  
    17 : ldrsw x1, [x0], x25         ; 01 68 b9 b8  
    18 : cmp   x1, x2                ; 3f 00 02 eb  
    19 : ldr   x26, [sp], #0x03      ; fa 0f 40 f9  
    20 : csel  x26, x25, x26, #0x0b  ; 3a b3 9a 9a  
    21 : str   x26, [sp], #0x03      ; fa 0f 00 f9  
    22 : cmp   x1, x2                ; 3f 00 02 eb  
    23 : csel  x2, x1, x2, #0x0b     ; 22 b0 82 9a  
    24 : cmp   x1, x3                ; 3f 00 03 eb  
    25 : ldr   x26, [sp], #0x02      ; fa 0b 40 f9  
    26 : csel  x26, x25, x26, #0x0c  ; 3a c3 9a 9a  
    27 : str   x26, [sp], #0x02      ; fa 0b 00 f9  
    28 : cmp   x1, x3                ; 3f 00 03 eb  
    29 : csel  x3, x1, x3, #0x0c     ; 23 c0 83 9a  
    30 : add   x25, x25, #0x04       ; 39 13 00 91  
    31 : b     [14]                  ; ef ff ff 17  
    32 : ldr   x26, [sp], #0x03      ; fa 0f 40 f9  
    33 : asr   x0, x26, #0x02        ; 40 ff 42 93  
    34 : ldr   x26, [sp], #0x02      ; fa 0b 40 f9  
    35 : asr   x1, x26, #0x02        ; 41 ff 42 93  
    36 : ldr   x26, [sp], #0x01      ; fa 07 40 f9  
    37 : str   w0, [x26], #0         ; 40 03 00 b9  
    38 : ldr   x26, [sp], #0         ; fa 03 40 f9  
    39 : str   w1, [x26], #0         ; 41 03 00 b9  
    40 : eor   x0, x0, x0            ; 00 00 00 ca  
    41 : ldr   x25, [sp], #0x05      ; f9 17 40 f9  
    42 : ldr   x26, [sp], #0x06      ; fa 1b 40 f9  
    43 : add   sp, sp, #0x40         ; ff 03 01 91  
    44 : ret   x30                   ; c0 03 5f d6  
