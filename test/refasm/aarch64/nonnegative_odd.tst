nonnegative_odd(i0, i1)
     0 : sub   sp, sp, #0x30   ; ff c3 00 d1  
     1 : mov   x4, #0          ; 04 00 80 d2  
     2 : str   x4, [sp], #0x04 ; e4 13 00 f9  
     3 : mov   x4, #0          ; 04 00 80 d2  
     4 : str   x4, [sp], #0x03 ; e4 0f 00 f9  
     5 : mov   x4, #0x02       ; 44 00 80 d2  
     6 : str   x4, [sp], #0    ; e4 03 00 f9  
     7 : mov   x4, #0x04       ; 84 00 80 d2  
     8 : str   x4, [sp], #0x01 ; e4 07 00 f9  
     9 : ldr   x4, [sp], #0x04 ; e4 13 40 f9  
    10 : ldr   x5, [sp], #0x01 ; e5 07 40 f9  
    11 : sub   x6, x4, x5      ; 86 00 05 cb  
    12 : str   x6, [sp], #0x02 ; e6 0b 00 f9  
    13 : ldr   x4, [sp], #0x01 ; e4 07 40 f9  
    14 : mul   x1, x1, x4      ; 21 7c 04 9b  
    15 : ldr   x4, [sp], #0x04 ; e4 13 40 f9  
    16 : cmp   x4, x1          ; 9f 00 01 eb  
    17 : b.ge  [46]            ; aa 03 00 54  
    18 : ldr   x4, [sp], #0x04 ; e4 13 40 f9  
    19 : ldrsw x3, [x0], x4    ; 03 68 a4 b8  
    20 : ldr   x4, [sp], #0x03 ; e4 0f 40 f9  
    21 : cmp   x3, x4          ; 7f 00 04 eb  
    22 : b.ge  [28]            ; ca 00 00 54  
    23 : ldr   x4, [sp], #0x04 ; e4 13 40 f9  
    24 : ldr   x5, [sp], #0x01 ; e5 07 40 f9  
    25 : add   x4, x4, x5      ; 84 00 05 8b  
    26 : str   x4, [sp], #0x04 ; e4 13 00 f9  
    27 : b     [16]            ; f5 ff ff 17  
    28 : ldr   x4, [sp], #0    ; e4 03 40 f9  
    29 : sdiv  x2, x3, x4      ; 62 0c c4 9a  
    30 : ldr   x4, [sp], #0    ; e4 03 40 f9  
    31 : mul   x2, x2, x4      ; 42 7c 04 9b  
    32 : sub   x2, x3, x2      ; 62 00 02 cb  
    33 : mov   x2, x2          ; e2 03 02 aa  
    34 : ldr   x4, [sp], #0x03 ; e4 0f 40 f9  
    35 : cmp   x2, x4          ; 5f 00 04 eb  
    36 : b.eq  [41]            ; a0 00 00 54  
    37 : ldr   x4, [sp], #0x04 ; e4 13 40 f9  
    38 : mov   x5, x4          ; e5 03 04 aa  
    39 : str   x5, [sp], #0x02 ; e5 0b 00 f9  
    40 : b     [46]            ; 06 00 00 14  
    41 : ldr   x4, [sp], #0x04 ; e4 13 40 f9  
    42 : ldr   x5, [sp], #0x01 ; e5 07 40 f9  
    43 : add   x4, x4, x5      ; 84 00 05 8b  
    44 : str   x4, [sp], #0x04 ; e4 13 00 f9  
    45 : b     [16]            ; e3 ff ff 17  
    46 : ldr   x4, [sp], #0x01 ; e4 07 40 f9  
    47 : ldr   x5, [sp], #0x02 ; e5 0b 40 f9  
    48 : sdiv  x0, x5, x4      ; a0 0c c4 9a  
    49 : mov   x0, x0          ; e0 03 00 aa  
    50 : add   sp, sp, #0x30   ; ff c3 00 91  
    51 : ret   x30             ; c0 03 5f d6  
