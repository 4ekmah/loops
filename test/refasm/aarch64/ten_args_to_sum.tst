ten_args_to_sum(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9)
     0 : sub sp, sp, #0x30    ; ff c3 00 d1  
     1 : str x18, [sp], #0x02 ; f2 0b 00 f9  
     2 : str x19, [sp], #0x03 ; f3 0f 00 f9  
     3 : str x20, [sp], #0x04 ; f4 13 00 f9  
     4 : str x21, [sp], #0x05 ; f5 17 00 f9  
     5 : ldr x18, [sp], #0x06 ; f2 1b 40 f9  
     6 : ldr x19, [sp], #0x07 ; f3 1f 40 f9  
     7 : mov x20, #0x01       ; 34 00 80 d2  
     8 : str x20, [sp], #0    ; f4 03 00 f9  
     9 : ldr x20, [sp], #0    ; f4 03 40 f9  
    10 : mul x21, x0, x20     ; 15 7c 14 9b  
    11 : str x21, [sp], #0x01 ; f5 07 00 f9  
    12 : mov x0, #0x02        ; 40 00 80 d2  
    13 : mul x1, x1, x0       ; 21 7c 00 9b  
    14 : ldr x20, [sp], #0x01 ; f4 07 40 f9  
    15 : add x1, x20, x1      ; 81 02 01 8b  
    16 : mov x0, #0x03        ; 60 00 80 d2  
    17 : mul x2, x2, x0       ; 42 7c 00 9b  
    18 : add x2, x1, x2       ; 22 00 02 8b  
    19 : mov x1, #0x04        ; 81 00 80 d2  
    20 : mul x3, x3, x1       ; 63 7c 01 9b  
    21 : add x3, x2, x3       ; 43 00 03 8b  
    22 : mov x2, #0x05        ; a2 00 80 d2  
    23 : mul x4, x4, x2       ; 84 7c 02 9b  
    24 : add x4, x3, x4       ; 64 00 04 8b  
    25 : mov x3, #0x06        ; c3 00 80 d2  
    26 : mul x5, x5, x3       ; a5 7c 03 9b  
    27 : add x5, x4, x5       ; 85 00 05 8b  
    28 : mov x4, #0x07        ; e4 00 80 d2  
    29 : mul x6, x6, x4       ; c6 7c 04 9b  
    30 : add x6, x5, x6       ; a6 00 06 8b  
    31 : mov x5, #0x08        ; 05 01 80 d2  
    32 : mul x7, x7, x5       ; e7 7c 05 9b  
    33 : add x7, x6, x7       ; c7 00 07 8b  
    34 : mov x6, #0x03        ; 66 00 80 d2  
    35 : mul x6, x18, x6      ; 46 7e 06 9b  
    36 : add x6, x7, x6       ; e6 00 06 8b  
    37 : mov x7, #0x02        ; 47 00 80 d2  
    38 : mul x7, x19, x7      ; 67 7e 07 9b  
    39 : add x7, x6, x7       ; c7 00 07 8b  
    40 : mov x0, x7           ; e0 03 07 aa  
    41 : ldr x18, [sp], #0x02 ; f2 0b 40 f9  
    42 : ldr x19, [sp], #0x03 ; f3 0f 40 f9  
    43 : ldr x20, [sp], #0x04 ; f4 13 40 f9  
    44 : ldr x21, [sp], #0x05 ; f5 17 40 f9  
    45 : add sp, sp, #0x30    ; ff c3 00 91  
    46 : ret x30              ; c0 03 5f d6  
