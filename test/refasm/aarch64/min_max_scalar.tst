min_max_scalar(i0, i1, i2, i3)
     0 : sub   sp, sp, #0x40   ; ff 03 01 d1  
     1 : mov   x4, #0          ; 04 00 80 d2  
     2 : str   x4, [sp], #0    ; e4 03 00 f9  
     3 : mov   x4, #0          ; 04 00 80 d2  
     4 : str   x4, [sp], #0x01 ; e4 07 00 f9  
     5 : mov   x4, #0          ; 04 00 80 d2  
     6 : str   x4, [sp], #0x02 ; e4 0b 00 f9  
     7 : ldrsw x4, [x0], #0    ; 04 04 80 b8  
     8 : str   x4, [sp], #0x03 ; e4 0f 00 f9  
     9 : ldr   x4, [sp], #0x03 ; e4 0f 40 f9  
    10 : mov   x5, x4          ; e5 03 04 aa  
    11 : str   x5, [sp], #0x04 ; e5 13 00 f9  
    12 : mov   x4, #0x04       ; 84 00 80 d2  
    13 : str   x4, [sp], #0x05 ; e4 17 00 f9  
    14 : ldr   x4, [sp], #0x05 ; e4 17 40 f9  
    15 : mul   x5, x1, x4      ; 25 7c 04 9b  
    16 : str   x5, [sp], #0x06 ; e5 1b 00 f9  
    17 : ldr   x4, [sp], #0    ; e4 03 40 f9  
    18 : ldrsw x1, [x0], x4    ; 01 68 a4 b8  
    19 : ldr   x4, [sp], #0x03 ; e4 0f 40 f9  
    20 : cmp   x1, x4          ; 3f 00 04 eb  
    21 : b.ge  [27]            ; ca 00 00 54  
    22 : mov   x4, x1          ; e4 03 01 aa  
    23 : str   x4, [sp], #0x03 ; e4 0f 00 f9  
    24 : ldr   x4, [sp], #0    ; e4 03 40 f9  
    25 : mov   x5, x4          ; e5 03 04 aa  
    26 : str   x5, [sp], #0x01 ; e5 07 00 f9  
    27 : ldr   x4, [sp], #0x04 ; e4 13 40 f9  
    28 : cmp   x1, x4          ; 3f 00 04 eb  
    29 : b.le  [35]            ; cd 00 00 54  
    30 : mov   x4, x1          ; e4 03 01 aa  
    31 : str   x4, [sp], #0x04 ; e4 13 00 f9  
    32 : ldr   x4, [sp], #0    ; e4 03 40 f9  
    33 : mov   x5, x4          ; e5 03 04 aa  
    34 : str   x5, [sp], #0x02 ; e5 0b 00 f9  
    35 : ldr   x4, [sp], #0    ; e4 03 40 f9  
    36 : ldr   x5, [sp], #0x05 ; e5 17 40 f9  
    37 : add   x4, x4, x5      ; 84 00 05 8b  
    38 : str   x4, [sp], #0    ; e4 03 00 f9  
    39 : ldr   x4, [sp], #0x06 ; e4 1b 40 f9  
    40 : ldr   x5, [sp], #0    ; e5 03 40 f9  
    41 : cmp   x5, x4          ; bf 00 04 eb  
    42 : b.lt  [17]            ; eb fc ff 54  
    43 : ldr   x4, [sp], #0x01 ; e4 07 40 f9  
    44 : ldr   x5, [sp], #0x05 ; e5 17 40 f9  
    45 : sdiv  x0, x4, x5      ; 80 0c c5 9a  
    46 : ldr   x4, [sp], #0x02 ; e4 0b 40 f9  
    47 : ldr   x5, [sp], #0x05 ; e5 17 40 f9  
    48 : sdiv  x1, x4, x5      ; 81 0c c5 9a  
    49 : str   w0, [x2], #0    ; 40 00 00 b9  
    50 : str   w1, [x3], #0    ; 61 00 00 b9  
    51 : mov   x0, #0          ; 00 00 80 d2  
    52 : mov   x0, x0          ; e0 03 00 aa  
    53 : add   sp, sp, #0x40   ; ff 03 01 91  
    54 : ret   x30             ; c0 03 5f d6  
