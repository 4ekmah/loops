bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub  sp, sp, #0x50          ; ff 43 01 d1  
     1 : str  x18, [sp], #0x05       ; f2 17 00 f9  
     2 : str  x19, [sp], #0x06       ; f3 1b 00 f9  
     3 : str  x20, [sp], #0x07       ; f4 1f 00 f9  
     4 : str  x21, [sp], #0x08       ; f5 23 00 f9  
     5 : str  x22, [sp], #0x09       ; f6 27 00 f9  
     6 : sub  x18, x4, x2            ; 92 00 02 cb  
     7 : cmp  x18, #0                ; 5f 02 00 f1  
     8 : cneg x20, x18, #0x0a        ; 54 a6 92 da  
     9 : str  x20, [sp], #0x04       ; f4 13 00 f9  
    10 : sub  x19, x4, x2            ; 93 00 02 cb  
    11 : cmp  x19, #0                ; 7f 02 00 f1  
    12 : asr  x20, x19, #0x3f        ; 74 fe 7f 93  
    13 : cinc x20, x20, #0x0d        ; 94 d6 94 9a  
    14 : str  x20, [sp], #0          ; f4 03 00 f9  
    15 : sub  x19, x5, x3            ; b3 00 03 cb  
    16 : cmp  x19, #0                ; 7f 02 00 f1  
    17 : cneg x19, x19, #0x0a        ; 73 a6 93 da  
    18 : neg  x20, x19               ; f4 03 13 cb  
    19 : str  x20, [sp], #0x01       ; f4 07 00 f9  
    20 : sub  x19, x5, x3            ; b3 00 03 cb  
    21 : cmp  x19, #0                ; 7f 02 00 f1  
    22 : asr  x20, x19, #0x3f        ; 74 fe 7f 93  
    23 : cinc x20, x20, #0x0d        ; 94 d6 94 9a  
    24 : str  x20, [sp], #0x03       ; f4 0f 00 f9  
    25 : ldr  x20, [sp], #0x04       ; f4 13 40 f9  
    26 : ldr  x21, [sp], #0x01       ; f5 07 40 f9  
    27 : add  x22, x20, x21          ; 96 02 15 8b  
    28 : str  x22, [sp], #0x02       ; f6 0b 00 f9  
    29 : cmp  x0, #0                 ; 1f 00 00 f1  
    30 : b.eq [70]                   ; 00 05 00 54  
    31 : mul  x19, x3, x1            ; 73 7c 01 9b  
    32 : add  x19, x19, x2           ; 73 02 02 8b  
    33 : strb w6, x0, x19            ; 06 68 33 38  
    34 : cmp  x2, x4                 ; 5f 00 04 eb  
    35 : b.ne [39]                   ; 81 00 00 54  
    36 : cmp  x3, x5                 ; 7f 00 05 eb  
    37 : b.ne [39]                   ; 41 00 00 54  
    38 : b    [70]                   ; 20 00 00 14  
    39 : ldr  x20, [sp], #0x02       ; f4 0b 40 f9  
    40 : lsl  x19, x20, #0x3f, #0x3e ; 93 fa 7f d3  
    41 : ldr  x20, [sp], #0x01       ; f4 07 40 f9  
    42 : cmp  x19, x20               ; 7f 02 14 eb  
    43 : b.lt [55]                   ; 8b 01 00 54  
    44 : cmp  x2, x4                 ; 5f 00 04 eb  
    45 : b.ne [47]                   ; 41 00 00 54  
    46 : b    [70]                   ; 18 00 00 14  
    47 : ldr  x20, [sp], #0x01       ; f4 07 40 f9  
    48 : ldr  x21, [sp], #0x02       ; f5 0b 40 f9  
    49 : add  x18, x21, x20          ; b2 02 14 8b  
    50 : mov  x20, x18               ; f4 03 12 aa  
    51 : str  x20, [sp], #0x02       ; f4 0b 00 f9  
    52 : ldr  x20, [sp], #0          ; f4 03 40 f9  
    53 : add  x18, x2, x20           ; 52 00 14 8b  
    54 : mov  x2, x18                ; e2 03 12 aa  
    55 : ldr  x20, [sp], #0x04       ; f4 13 40 f9  
    56 : cmp  x19, x20               ; 7f 02 14 eb  
    57 : b.gt [69]                   ; 8c 01 00 54  
    58 : cmp  x3, x5                 ; 7f 00 05 eb  
    59 : b.ne [61]                   ; 41 00 00 54  
    60 : b    [70]                   ; 0a 00 00 14  
    61 : ldr  x20, [sp], #0x04       ; f4 13 40 f9  
    62 : ldr  x21, [sp], #0x02       ; f5 0b 40 f9  
    63 : add  x18, x21, x20          ; b2 02 14 8b  
    64 : mov  x20, x18               ; f4 03 12 aa  
    65 : str  x20, [sp], #0x02       ; f4 0b 00 f9  
    66 : ldr  x20, [sp], #0x03       ; f4 0f 40 f9  
    67 : add  x18, x3, x20           ; 72 00 14 8b  
    68 : mov  x3, x18                ; e3 03 12 aa  
    69 : b    [29]                   ; d8 ff ff 17  
    70 : ldr  x18, [sp], #0x05       ; f2 17 40 f9  
    71 : ldr  x19, [sp], #0x06       ; f3 1b 40 f9  
    72 : ldr  x20, [sp], #0x07       ; f4 1f 40 f9  
    73 : ldr  x21, [sp], #0x08       ; f5 23 40 f9  
    74 : ldr  x22, [sp], #0x09       ; f6 27 40 f9  
    75 : add  sp, sp, #0x50          ; ff 43 01 91  
    76 : ret  x30                    ; c0 03 5f d6  
