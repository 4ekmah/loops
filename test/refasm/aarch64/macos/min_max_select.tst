min_max_select(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x40        ; ff 03 01 d1  
     1 : str   x2, [sp], #0x01      ; e2 07 00 f9  
     2 : str   x3, [sp], #0         ; e3 03 00 f9  
     3 : str   x18, [sp], #0x04     ; f2 13 00 f9  
     4 : str   x19, [sp], #0x05     ; f3 17 00 f9  
     5 : str   x20, [sp], #0x06     ; f4 1b 00 f9  
     6 : eor   x18, x18, x18        ; 52 02 12 ca  
     7 : eor   x20, x20, x20        ; 94 02 14 ca  
     8 : str   x20, [sp], #0x03     ; f4 0f 00 f9  
     9 : eor   x20, x20, x20        ; 94 02 14 ca  
    10 : str   x20, [sp], #0x02     ; f4 0b 00 f9  
    11 : ldrsw x2, [x0], #0         ; 02 00 80 b9  
    12 : mov   x3, x2               ; e3 03 02 aa  
    13 : lsl   x1, x1, #0x3e, #0x3d ; 21 f4 7e d3  
    14 : cmp   x18, x1              ; 5f 02 01 eb  
    15 : b.ge  [31]                 ; 0a 02 00 54  
    16 : ldrsw x19, [x0], x18       ; 13 68 b2 b8  
    17 : cmp   x19, x2              ; 7f 02 02 eb  
    18 : ldr   x20, [sp], #0x03     ; f4 0f 40 f9  
    19 : csel  x20, x18, x20, #0x0b ; 54 b2 94 9a  
    20 : str   x20, [sp], #0x03     ; f4 0f 00 f9  
    21 : cmp   x19, x2              ; 7f 02 02 eb  
    22 : csel  x2, x19, x2, #0x0b   ; 62 b2 82 9a  
    23 : cmp   x19, x3              ; 7f 02 03 eb  
    24 : ldr   x20, [sp], #0x02     ; f4 0b 40 f9  
    25 : csel  x20, x18, x20, #0x0c ; 54 c2 94 9a  
    26 : str   x20, [sp], #0x02     ; f4 0b 00 f9  
    27 : cmp   x19, x3              ; 7f 02 03 eb  
    28 : csel  x3, x19, x3, #0x0c   ; 63 c2 83 9a  
    29 : add   x18, x18, #0x04      ; 52 12 00 91  
    30 : b     [14]                 ; f0 ff ff 17  
    31 : mov   x0, #0x04            ; 80 00 80 d2  
    32 : ldr   x20, [sp], #0x03     ; f4 0f 40 f9  
    33 : asr   x0, x20, #0x02       ; 80 fe 42 93  
    34 : ldr   x20, [sp], #0x02     ; f4 0b 40 f9  
    35 : asr   x1, x20, #0x02       ; 81 fe 42 93  
    36 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    37 : str   w0, [x20], #0        ; 80 02 00 b9  
    38 : ldr   x20, [sp], #0        ; f4 03 40 f9  
    39 : str   w1, [x20], #0        ; 81 02 00 b9  
    40 : eor   x0, x0, x0           ; 00 00 00 ca  
    41 : ldr   x18, [sp], #0x04     ; f2 13 40 f9  
    42 : ldr   x19, [sp], #0x05     ; f3 17 40 f9  
    43 : ldr   x20, [sp], #0x06     ; f4 1b 40 f9  
    44 : add   sp, sp, #0x40        ; ff 03 01 91  
    45 : ret   x30                  ; c0 03 5f d6  
