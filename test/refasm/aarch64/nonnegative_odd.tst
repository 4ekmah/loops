nonnegative_odd(i0, i1)
     0 : sub   sp, sp, #0x20    ; ff 83 00 d1  
     1 : str   x18, [sp], #0x01 ; f2 07 00 f9  
     2 : str   x19, [sp], #0x02 ; f3 0b 00 f9  
     3 : mov   x2, #0           ; 02 00 80 d2  
     4 : movn  x3, #0x03        ; 63 00 80 92  
     5 : mov   x18, #0x04       ; 92 00 80 d2  
     6 : mul   x1, x1, x18      ; 21 7c 12 9b  
     7 : cmp   x2, x1           ; 5f 00 01 eb  
     8 : b.ge  [27]             ; 6a 02 00 54  
     9 : ldrsw x18, [x0], x2    ; 12 68 a2 b8  
    10 : cmp   x18, #0          ; 5f 02 00 f1  
    11 : b.ge  [14]             ; 6a 00 00 54  
    12 : add   x2, x2, #0x04    ; 42 10 00 91  
    13 : b     [7]              ; fa ff ff 17  
    14 : mov   x19, #0x02       ; 53 00 80 d2  
    15 : str   x0, [sp], #0     ; e0 03 00 f9  
    16 : mov   x0, x18          ; e0 03 12 aa  
    17 : sdiv  x18, x0, x19     ; 12 0c d3 9a  
    18 : mul   x18, x18, x19    ; 52 7e 13 9b  
    19 : sub   x18, x0, x18     ; 12 00 12 cb  
    20 : ldr   x0, [sp], #0     ; e0 03 40 f9  
    21 : cmp   x18, #0          ; 5f 02 00 f1  
    22 : b.eq  [25]             ; 60 00 00 54  
    23 : mov   x3, x2           ; e3 03 02 aa  
    24 : b     [27]             ; 03 00 00 14  
    25 : add   x2, x2, #0x04    ; 42 10 00 91  
    26 : b     [7]              ; ed ff ff 17  
    27 : mov   x0, #0x04        ; 80 00 80 d2  
    28 : sdiv  x0, x3, x0       ; 60 0c c0 9a  
    29 : mov   x0, x0           ; e0 03 00 aa  
    30 : ldr   x18, [sp], #0x01 ; f2 07 40 f9  
    31 : ldr   x19, [sp], #0x02 ; f3 0b 40 f9  
    32 : add   sp, sp, #0x20    ; ff 83 00 91  
    33 : ret   x30              ; c0 03 5f d6  
