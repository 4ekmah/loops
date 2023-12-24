nonnegative_odd(i0, i1)
     0 : sub   sp, sp, #0x30    ; ff c3 00 d1  
     1 : str   x25, [sp], #0x03 ; f9 0f 00 f9  
     2 : str   x26, [sp], #0x04 ; fa 13 00 f9  
     3 : eor   x2, x2, x2       ; 42 00 02 ca  
     4 : movn  x26, #0x03       ; 7a 00 80 92  
     5 : str   x26, [sp], #0x02 ; fa 0b 00 f9  
     6 : mov   x25, #0x04       ; 99 00 80 d2  
     7 : mul   x1, x1, x25      ; 21 7c 19 9b  
     8 : cmp   x2, x1           ; 5f 00 01 eb  
     9 : b.ge  [29]             ; 8a 02 00 54  
    10 : ldrsw x25, [x0], x2    ; 19 68 a2 b8  
    11 : cmp   x25, #0          ; 3f 03 00 f1  
    12 : b.ge  [15]             ; 6a 00 00 54  
    13 : add   x2, x2, #0x04    ; 42 10 00 91  
    14 : b     [8]              ; fa ff ff 17  
    15 : mov   x3, #0x02        ; 43 00 80 d2  
    16 : str   x0, [sp], #0     ; e0 03 00 f9  
    17 : mov   x0, x3           ; e0 03 03 aa  
    18 : sdiv  x3, x25, x0      ; 23 0f c0 9a  
    19 : mul   x3, x3, x0       ; 63 7c 00 9b  
    20 : sub   x3, x25, x3      ; 23 03 03 cb  
    21 : ldr   x0, [sp], #0     ; e0 03 40 f9  
    22 : cmp   x3, #0           ; 7f 00 00 f1  
    23 : b.eq  [27]             ; 80 00 00 54  
    24 : mov   x26, x2          ; fa 03 02 aa  
    25 : str   x26, [sp], #0x02 ; fa 0b 00 f9  
    26 : b     [29]             ; 03 00 00 14  
    27 : add   x2, x2, #0x04    ; 42 10 00 91  
    28 : b     [8]              ; ec ff ff 17  
    29 : mov   x0, #0x04        ; 80 00 80 d2  
    30 : ldr   x26, [sp], #0x02 ; fa 0b 40 f9  
    31 : sdiv  x0, x26, x0      ; 40 0f c0 9a  
    32 : ldr   x25, [sp], #0x03 ; f9 0f 40 f9  
    33 : ldr   x26, [sp], #0x04 ; fa 13 40 f9  
    34 : add   sp, sp, #0x30    ; ff c3 00 91  
    35 : ret   x30              ; c0 03 5f d6  
