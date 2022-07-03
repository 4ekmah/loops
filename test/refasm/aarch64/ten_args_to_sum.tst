ten_args_to_sum(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9)
     0 : sub sp, sp, #0x20    ; ff 83 00 d1  
     1 : str x18, [sp], #0    ; f2 03 00 f9  
     2 : str x19, [sp], #0x01 ; f3 07 00 f9  
     3 : str x20, [sp], #0x02 ; f4 0b 00 f9  
     4 : ldr x18, [sp], #0x04 ; f2 13 40 f9  
     5 : mov x19, #0x01       ; 33 00 80 d2  
     6 : mul x0, x0, x19      ; 00 7c 13 9b  
     7 : mov x19, #0x02       ; 53 00 80 d2  
     8 : mul x1, x1, x19      ; 21 7c 13 9b  
     9 : add x0, x0, x1       ; 00 00 01 8b  
    10 : mov x1, #0x03        ; 61 00 80 d2  
    11 : mul x1, x2, x1       ; 41 7c 01 9b  
    12 : add x0, x0, x1       ; 00 00 01 8b  
    13 : mov x1, #0x04        ; 81 00 80 d2  
    14 : mul x1, x3, x1       ; 61 7c 01 9b  
    15 : add x0, x0, x1       ; 00 00 01 8b  
    16 : mov x1, #0x05        ; a1 00 80 d2  
    17 : mul x1, x4, x1       ; 81 7c 01 9b  
    18 : add x0, x0, x1       ; 00 00 01 8b  
    19 : mov x1, #0x06        ; c1 00 80 d2  
    20 : mul x1, x5, x1       ; a1 7c 01 9b  
    21 : add x0, x0, x1       ; 00 00 01 8b  
    22 : mov x1, #0x07        ; e1 00 80 d2  
    23 : mul x1, x6, x1       ; c1 7c 01 9b  
    24 : add x0, x0, x1       ; 00 00 01 8b  
    25 : mov x1, #0x08        ; 01 01 80 d2  
    26 : mul x1, x7, x1       ; e1 7c 01 9b  
    27 : add x0, x0, x1       ; 00 00 01 8b  
    28 : mov x1, #0x03        ; 61 00 80 d2  
    29 : mul x1, x18, x1      ; 41 7e 01 9b  
    30 : add x0, x0, x1       ; 00 00 01 8b  
    31 : mov x1, #0x02        ; 41 00 80 d2  
    32 : ldr x20, [sp], #0x05 ; f4 17 40 f9  
    33 : mul x1, x20, x1      ; 81 7e 01 9b  
    34 : add x0, x0, x1       ; 00 00 01 8b  
    35 : mov x0, x0           ; e0 03 00 aa  
    36 : ldr x18, [sp], #0    ; f2 03 40 f9  
    37 : ldr x19, [sp], #0x01 ; f3 07 40 f9  
    38 : ldr x20, [sp], #0x02 ; f4 0b 40 f9  
    39 : add sp, sp, #0x20    ; ff 83 00 91  
    40 : ret x30              ; c0 03 5f d6  
