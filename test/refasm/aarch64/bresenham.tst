bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub  sp, sp, #0x40          ; ff 03 01 d1  
     1 : str  x18, [sp], #0x04       ; f2 13 00 f9  
     2 : str  x19, [sp], #0x05       ; f3 17 00 f9  
     3 : str  x20, [sp], #0x06       ; f4 1b 00 f9  
     4 : str  x21, [sp], #0x07       ; f5 1f 00 f9  
     5 : sub  x18, x4, x2            ; 92 00 02 cb  
     6 : cmp  x18, #0                ; 5f 02 00 f1  
     7 : cneg x18, x18, #0x0a        ; 52 a6 92 da  
     8 : sub  x19, x4, x2            ; 93 00 02 cb  
     9 : cmp  x19, #0                ; 7f 02 00 f1  
    10 : asr  x20, x19, #0x3f        ; 74 fe 7f 93  
    11 : cinc x20, x20, #0x0d        ; 94 d6 94 9a  
    12 : str  x20, [sp], #0          ; f4 03 00 f9  
    13 : sub  x19, x5, x3            ; b3 00 03 cb  
    14 : cmp  x19, #0                ; 7f 02 00 f1  
    15 : cneg x19, x19, #0x0a        ; 73 a6 93 da  
    16 : neg  x20, x19               ; f4 03 13 cb  
    17 : str  x20, [sp], #0x01       ; f4 07 00 f9  
    18 : sub  x19, x5, x3            ; b3 00 03 cb  
    19 : cmp  x19, #0                ; 7f 02 00 f1  
    20 : asr  x20, x19, #0x3f        ; 74 fe 7f 93  
    21 : cinc x20, x20, #0x0d        ; 94 d6 94 9a  
    22 : str  x20, [sp], #0x03       ; f4 0f 00 f9  
    23 : ldr  x20, [sp], #0x01       ; f4 07 40 f9  
    24 : add  x21, x18, x20          ; 55 02 14 8b  
    25 : str  x21, [sp], #0x02       ; f5 0b 00 f9  
    26 : cmp  x0, #0                 ; 1f 00 00 f1  
    27 : b.eq [61]                   ; 40 04 00 54  
    28 : mul  x19, x3, x1            ; 73 7c 01 9b  
    29 : add  x19, x19, x2           ; 73 02 02 8b  
    30 : strb w6, x0, x19            ; 06 68 33 38  
    31 : cmp  x2, x4                 ; 5f 00 04 eb  
    32 : b.ne [36]                   ; 81 00 00 54  
    33 : cmp  x3, x5                 ; 7f 00 05 eb  
    34 : b.ne [36]                   ; 41 00 00 54  
    35 : b    [61]                   ; 1a 00 00 14  
    36 : ldr  x20, [sp], #0x02       ; f4 0b 40 f9  
    37 : lsl  x19, x20, #0x3f, #0x3e ; 93 fa 7f d3  
    38 : ldr  x20, [sp], #0x01       ; f4 07 40 f9  
    39 : cmp  x19, x20               ; 7f 02 14 eb  
    40 : b.lt [50]                   ; 4b 01 00 54  
    41 : cmp  x2, x4                 ; 5f 00 04 eb  
    42 : b.ne [44]                   ; 41 00 00 54  
    43 : b    [61]                   ; 12 00 00 14  
    44 : ldr  x21, [sp], #0x01       ; f5 07 40 f9  
    45 : ldr  x20, [sp], #0x02       ; f4 0b 40 f9  
    46 : add  x20, x20, x21          ; 94 02 15 8b  
    47 : str  x20, [sp], #0x02       ; f4 0b 00 f9  
    48 : ldr  x20, [sp], #0          ; f4 03 40 f9  
    49 : add  x2, x2, x20            ; 42 00 14 8b  
    50 : cmp  x19, x18               ; 7f 02 12 eb  
    51 : b.gt [60]                   ; 2c 01 00 54  
    52 : cmp  x3, x5                 ; 7f 00 05 eb  
    53 : b.ne [55]                   ; 41 00 00 54  
    54 : b    [61]                   ; 07 00 00 14  
    55 : ldr  x20, [sp], #0x02       ; f4 0b 40 f9  
    56 : add  x20, x20, x18          ; 94 02 12 8b  
    57 : str  x20, [sp], #0x02       ; f4 0b 00 f9  
    58 : ldr  x20, [sp], #0x03       ; f4 0f 40 f9  
    59 : add  x3, x3, x20            ; 63 00 14 8b  
    60 : b    [26]                   ; de ff ff 17  
    61 : ldr  x18, [sp], #0x04       ; f2 13 40 f9  
    62 : ldr  x19, [sp], #0x05       ; f3 17 40 f9  
    63 : ldr  x20, [sp], #0x06       ; f4 1b 40 f9  
    64 : ldr  x21, [sp], #0x07       ; f5 1f 40 f9  
    65 : add  sp, sp, #0x40          ; ff 03 01 91  
    66 : ret  x30                    ; c0 03 5f d6  
