bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub  sp, sp, #0x50          ; ff 43 01 d1  
     1 : str  x25, [sp], #0x05       ; f9 17 00 f9  
     2 : str  x26, [sp], #0x06       ; fa 1b 00 f9  
     3 : str  x27, [sp], #0x07       ; fb 1f 00 f9  
     4 : str  x28, [sp], #0x08       ; fc 23 00 f9  
     5 : sub  x25, x4, x2            ; 99 00 02 cb  
     6 : cmp  x25, #0                ; 3f 03 00 f1  
     7 : cneg x26, x25, #0x0a        ; 3a a7 99 da  
     8 : str  x26, [sp], #0          ; fa 03 00 f9  
     9 : sub  x25, x4, x2            ; 99 00 02 cb  
    10 : cmp  x25, #0                ; 3f 03 00 f1  
    11 : asr  x26, x25, #0x3f        ; 3a ff 7f 93  
    12 : cinc x26, x26, #0x0d        ; 5a d7 9a 9a  
    13 : str  x26, [sp], #0x01       ; fa 07 00 f9  
    14 : sub  x25, x5, x3            ; b9 00 03 cb  
    15 : cmp  x25, #0                ; 3f 03 00 f1  
    16 : cneg x25, x25, #0x0a        ; 39 a7 99 da  
    17 : neg  x26, x25               ; fa 03 19 cb  
    18 : str  x26, [sp], #0x02       ; fa 0b 00 f9  
    19 : sub  x25, x5, x3            ; b9 00 03 cb  
    20 : cmp  x25, #0                ; 3f 03 00 f1  
    21 : asr  x26, x25, #0x3f        ; 3a ff 7f 93  
    22 : cinc x26, x26, #0x0d        ; 5a d7 9a 9a  
    23 : str  x26, [sp], #0x04       ; fa 13 00 f9  
    24 : ldr  x26, [sp], #0          ; fa 03 40 f9  
    25 : ldr  x27, [sp], #0x02       ; fb 0b 40 f9  
    26 : add  x28, x26, x27          ; 5c 03 1b 8b  
    27 : str  x28, [sp], #0x03       ; fc 0f 00 f9  
    28 : cmp  x0, #0                 ; 1f 00 00 f1  
    29 : b.eq [65]                   ; 80 04 00 54  
    30 : mul  x25, x3, x1            ; 79 7c 01 9b  
    31 : add  x25, x25, x2           ; 39 03 02 8b  
    32 : strb w6, x0, x25            ; 06 68 39 38  
    33 : cmp  x2, x4                 ; 5f 00 04 eb  
    34 : b.ne [38]                   ; 81 00 00 54  
    35 : cmp  x3, x5                 ; 7f 00 05 eb  
    36 : b.ne [38]                   ; 41 00 00 54  
    37 : b    [65]                   ; 1c 00 00 14  
    38 : ldr  x26, [sp], #0x03       ; fa 0f 40 f9  
    39 : lsl  x25, x26, #0x3f, #0x3e ; 59 fb 7f d3  
    40 : ldr  x26, [sp], #0x02       ; fa 0b 40 f9  
    41 : cmp  x25, x26               ; 3f 03 1a eb  
    42 : b.lt [52]                   ; 4b 01 00 54  
    43 : cmp  x2, x4                 ; 5f 00 04 eb  
    44 : b.ne [46]                   ; 41 00 00 54  
    45 : b    [65]                   ; 14 00 00 14  
    46 : ldr  x27, [sp], #0x02       ; fb 0b 40 f9  
    47 : ldr  x26, [sp], #0x03       ; fa 0f 40 f9  
    48 : add  x26, x26, x27          ; 5a 03 1b 8b  
    49 : str  x26, [sp], #0x03       ; fa 0f 00 f9  
    50 : ldr  x26, [sp], #0x01       ; fa 07 40 f9  
    51 : add  x2, x2, x26            ; 42 00 1a 8b  
    52 : ldr  x26, [sp], #0          ; fa 03 40 f9  
    53 : cmp  x25, x26               ; 3f 03 1a eb  
    54 : b.gt [64]                   ; 4c 01 00 54  
    55 : cmp  x3, x5                 ; 7f 00 05 eb  
    56 : b.ne [58]                   ; 41 00 00 54  
    57 : b    [65]                   ; 08 00 00 14  
    58 : ldr  x27, [sp], #0          ; fb 03 40 f9  
    59 : ldr  x26, [sp], #0x03       ; fa 0f 40 f9  
    60 : add  x26, x26, x27          ; 5a 03 1b 8b  
    61 : str  x26, [sp], #0x03       ; fa 0f 00 f9  
    62 : ldr  x26, [sp], #0x04       ; fa 13 40 f9  
    63 : add  x3, x3, x26            ; 63 00 1a 8b  
    64 : b    [28]                   ; dc ff ff 17  
    65 : ldr  x25, [sp], #0x05       ; f9 17 40 f9  
    66 : ldr  x26, [sp], #0x06       ; fa 1b 40 f9  
    67 : ldr  x27, [sp], #0x07       ; fb 1f 40 f9  
    68 : ldr  x28, [sp], #0x08       ; fc 23 40 f9  
    69 : add  sp, sp, #0x50          ; ff 43 01 91  
    70 : ret  x30                    ; c0 03 5f d6  
