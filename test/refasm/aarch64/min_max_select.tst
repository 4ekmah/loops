min_max_select(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x40         ; ff 03 01 d1  
     1 : str   x2, [sp], #0x01       ; e2 07 00 f9  
     2 : str   x3, [sp], #0          ; e3 03 00 f9  
     3 : str   x18, [sp], #0x05      ; f2 17 00 f9  
     4 : str   x19, [sp], #0x06      ; f3 1b 00 f9  
     5 : str   x20, [sp], #0x07      ; f4 1f 00 f9  
     6 : mov   x18, #0               ; 12 00 80 d2  
     7 : mov   x20, #0               ; 14 00 80 d2  
     8 : str   x20, [sp], #0x03      ; f4 0f 00 f9  
     9 : mov   x20, #0               ; 14 00 80 d2  
    10 : str   x20, [sp], #0x02      ; f4 0b 00 f9  
    11 : ldrsw x2, [x0], #0          ; 02 00 80 b9  
    12 : mov   x3, x2                ; e3 03 02 aa  
    13 : lsl   x20, x1, #0x3e, #0x3d ; 34 f4 7e d3  
    14 : str   x20, [sp], #0x04      ; f4 13 00 f9  
    15 : ldrsw x19, [x0], x18        ; 13 68 b2 b8  
    16 : cmp   x19, x2               ; 7f 02 02 eb  
    17 : ldr   x20, [sp], #0x03      ; f4 0f 40 f9  
    18 : csel  x1, x18, x20, #0x0b   ; 41 b2 94 9a  
    19 : mov   x20, x1               ; f4 03 01 aa  
    20 : str   x20, [sp], #0x03      ; f4 0f 00 f9  
    21 : cmp   x19, x2               ; 7f 02 02 eb  
    22 : csel  x1, x19, x2, #0x0b    ; 61 b2 82 9a  
    23 : mov   x2, x1                ; e2 03 01 aa  
    24 : cmp   x19, x3               ; 7f 02 03 eb  
    25 : ldr   x20, [sp], #0x02      ; f4 0b 40 f9  
    26 : csel  x1, x18, x20, #0x0c   ; 41 c2 94 9a  
    27 : mov   x20, x1               ; f4 03 01 aa  
    28 : str   x20, [sp], #0x02      ; f4 0b 00 f9  
    29 : cmp   x19, x3               ; 7f 02 03 eb  
    30 : csel  x1, x19, x3, #0x0c    ; 61 c2 83 9a  
    31 : mov   x3, x1                ; e3 03 01 aa  
    32 : add   x18, x18, #0x04       ; 52 12 00 91  
    33 : ldr   x20, [sp], #0x04      ; f4 13 40 f9  
    34 : cmp   x18, x20              ; 5f 02 14 eb  
    35 : b.lt  [15]                  ; 8b fd ff 54  
    36 : mov   x0, #0x04             ; 80 00 80 d2  
    37 : ldr   x20, [sp], #0x03      ; f4 0f 40 f9  
    38 : asr   x0, x20, #0x02        ; 80 fe 42 93  
    39 : ldr   x20, [sp], #0x02      ; f4 0b 40 f9  
    40 : asr   x1, x20, #0x02        ; 81 fe 42 93  
    41 : ldr   x20, [sp], #0x01      ; f4 07 40 f9  
    42 : str   w0, [x20], #0         ; 80 02 00 b9  
    43 : ldr   x20, [sp], #0         ; f4 03 40 f9  
    44 : str   w1, [x20], #0         ; 81 02 00 b9  
    45 : mov   x0, #0                ; 00 00 80 d2  
    46 : ldr   x18, [sp], #0x05      ; f2 17 40 f9  
    47 : ldr   x19, [sp], #0x06      ; f3 1b 40 f9  
    48 : ldr   x20, [sp], #0x07      ; f4 1f 40 f9  
    49 : add   sp, sp, #0x40         ; ff 03 01 91  
    50 : ret   x30                   ; c0 03 5f d6  
