ten_args_to_sum(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9)
     0 : sub sp, sp, #0x10    ; ff 43 00 d1  
     1 : str x25, [sp], #0    ; f9 03 00 f9  
     2 : str x26, [sp], #0x01 ; fa 07 00 f9  
     3 : mov x25, #0x01       ; 39 00 80 d2  
     4 : mul x0, x0, x25      ; 00 7c 19 9b  
     5 : mov x25, #0x02       ; 59 00 80 d2  
     6 : mul x1, x1, x25      ; 21 7c 19 9b  
     7 : add x0, x0, x1       ; 00 00 01 8b  
     8 : mov x1, #0x03        ; 61 00 80 d2  
     9 : mul x1, x2, x1       ; 41 7c 01 9b  
    10 : add x0, x0, x1       ; 00 00 01 8b  
    11 : mov x1, #0x04        ; 81 00 80 d2  
    12 : mul x1, x3, x1       ; 61 7c 01 9b  
    13 : add x0, x0, x1       ; 00 00 01 8b  
    14 : mov x1, #0x05        ; a1 00 80 d2  
    15 : mul x1, x4, x1       ; 81 7c 01 9b  
    16 : add x0, x0, x1       ; 00 00 01 8b  
    17 : mov x1, #0x06        ; c1 00 80 d2  
    18 : mul x1, x5, x1       ; a1 7c 01 9b  
    19 : add x0, x0, x1       ; 00 00 01 8b  
    20 : mov x1, #0x07        ; e1 00 80 d2  
    21 : mul x1, x6, x1       ; c1 7c 01 9b  
    22 : add x0, x0, x1       ; 00 00 01 8b  
    23 : mov x1, #0x08        ; 01 01 80 d2  
    24 : mul x1, x7, x1       ; e1 7c 01 9b  
    25 : add x0, x0, x1       ; 00 00 01 8b  
    26 : mov x1, #0x03        ; 61 00 80 d2  
    27 : ldr x26, [sp], #0x02 ; fa 0b 40 f9  
    28 : mul x1, x26, x1      ; 41 7f 01 9b  
    29 : add x0, x0, x1       ; 00 00 01 8b  
    30 : mov x1, #0x02        ; 41 00 80 d2  
    31 : ldr x26, [sp], #0x03 ; fa 0f 40 f9  
    32 : mul x1, x26, x1      ; 41 7f 01 9b  
    33 : add x0, x0, x1       ; 00 00 01 8b  
    34 : ldr x25, [sp], #0    ; f9 03 40 f9  
    35 : ldr x26, [sp], #0x01 ; fa 07 40 f9  
    36 : add sp, sp, #0x10    ; ff 43 00 91  
    37 : ret x30              ; c0 03 5f d6  
