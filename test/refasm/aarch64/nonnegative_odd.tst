nonnegative_odd(i0, i1)
     0 : sub   sp, sp, #0x30            ; ff c3 00 d1  
     1 : str   x18, [sp], #0x02         ; f2 0b 00 f9  
     2 : str   x19, [sp], #0x03         ; f3 0f 00 f9  
     3 : str   x20, [sp], #0x04         ; f4 13 00 f9  
     4 : mov   x2, #0                   ; 02 00 80 d2  
     5 : mov   x20, #0xfffffffffffffffc ; 74 00 80 92  
     6 : str   x20, [sp], #0            ; f4 03 00 f9  
     7 : mov   x20, #0x04               ; 94 00 80 d2  
     8 : str   x20, [sp], #0x01         ; f4 07 00 f9  
     9 : ldr   x20, [sp], #0x01         ; f4 07 40 f9  
    10 : mul   x1, x1, x20              ; 21 7c 14 9b  
    11 : cmp   x2, x1                   ; 5f 00 01 eb  
    12 : b.ge  [33]                     ; aa 02 00 54  
    13 : ldrsw x19, [x0], x2            ; 13 68 a2 b8  
    14 : mov   x3, #0                   ; 03 00 80 d2  
    15 : cmp   x19, x3                  ; 7f 02 03 eb  
    16 : b.ge  [19]                     ; 6a 00 00 54  
    17 : add   x2, x2, #0x04            ; 42 10 00 91  
    18 : b     [11]                     ; f9 ff ff 17  
    19 : mov   x3, #0x02                ; 43 00 80 d2  
    20 : sdiv  x3, x19, x3              ; 63 0e c3 9a  
    21 : mov   x18, #0x02               ; 52 00 80 d2  
    22 : mul   x3, x3, x18              ; 63 7c 12 9b  
    23 : sub   x3, x19, x3              ; 63 02 03 cb  
    24 : mov   x3, x3                   ; e3 03 03 aa  
    25 : mov   x18, #0                  ; 12 00 80 d2  
    26 : cmp   x3, x18                  ; 7f 00 12 eb  
    27 : b.eq  [31]                     ; 80 00 00 54  
    28 : mov   x20, x2                  ; f4 03 02 aa  
    29 : str   x20, [sp], #0            ; f4 03 00 f9  
    30 : b     [33]                     ; 03 00 00 14  
    31 : add   x2, x2, #0x04            ; 42 10 00 91  
    32 : b     [11]                     ; eb ff ff 17  
    33 : mov   x0, #0x04                ; 80 00 80 d2  
    34 : ldr   x20, [sp], #0            ; f4 03 40 f9  
    35 : sdiv  x0, x20, x0              ; 80 0e c0 9a  
    36 : mov   x0, x0                   ; e0 03 00 aa  
    37 : ldr   x18, [sp], #0x02         ; f2 0b 40 f9  
    38 : ldr   x19, [sp], #0x03         ; f3 0f 40 f9  
    39 : ldr   x20, [sp], #0x04         ; f4 13 40 f9  
    40 : add   sp, sp, #0x30            ; ff c3 00 91  
    41 : ret   x30                      ; c0 03 5f d6  
