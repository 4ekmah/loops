triangle_types(i0, i1, i2)
     0 : sub  sp, sp, #0x20    ; ff 83 00 d1  
     1 : str  x1, [sp], #0     ; e1 03 00 f9  
     2 : str  x25, [sp], #0x01 ; f9 07 00 f9  
     3 : str  x26, [sp], #0x02 ; fa 0b 00 f9  
     4 : cmp  x0, #0           ; 1f 00 00 f1  
     5 : b.le [11]             ; cd 00 00 54  
     6 : ldr  x26, [sp], #0    ; fa 03 40 f9  
     7 : cmp  x26, #0          ; 5f 03 00 f1  
     8 : b.le [11]             ; 6d 00 00 54  
     9 : cmp  x2, #0           ; 5f 00 00 f1  
    10 : b.gt [13]             ; 6c 00 00 54  
    11 : eor  x0, x0, x0       ; 00 00 00 ca  
    12 : b    [92]             ; 50 00 00 14  
    13 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    14 : add  x3, x26, x2      ; 43 03 02 8b  
    15 : cmp  x0, x3           ; 1f 00 03 eb  
    16 : b.gt [25]             ; 2c 01 00 54  
    17 : add  x3, x0, x2       ; 03 00 02 8b  
    18 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    19 : cmp  x26, x3          ; 5f 03 03 eb  
    20 : b.gt [25]             ; ac 00 00 54  
    21 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    22 : add  x3, x0, x26      ; 03 00 1a 8b  
    23 : cmp  x2, x3           ; 5f 00 03 eb  
    24 : b.le [27]             ; 6d 00 00 54  
    25 : eor  x0, x0, x0       ; 00 00 00 ca  
    26 : b    [92]             ; 42 00 00 14  
    27 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    28 : cmp  x0, x26          ; 1f 00 1a eb  
    29 : b.ne [34]             ; a1 00 00 54  
    30 : cmp  x0, x2           ; 1f 00 02 eb  
    31 : b.ne [34]             ; 61 00 00 54  
    32 : mov  x0, #0x02        ; 40 00 80 d2  
    33 : b    [92]             ; 3b 00 00 14  
    34 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    35 : cmp  x0, x26          ; 1f 00 1a eb  
    36 : b.eq [42]             ; c0 00 00 54  
    37 : cmp  x0, x2           ; 1f 00 02 eb  
    38 : b.eq [42]             ; 80 00 00 54  
    39 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    40 : cmp  x26, x2          ; 5f 03 02 eb  
    41 : b.ne [44]             ; 61 00 00 54  
    42 : mov  x0, #0x03        ; 60 00 80 d2  
    43 : b    [92]             ; 31 00 00 14  
    44 : mul  x3, x0, x0       ; 03 7c 00 9b  
    45 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    46 : mul  x25, x26, x26    ; 59 7f 1a 9b  
    47 : mul  x1, x2, x2       ; 41 7c 02 9b  
    48 : add  x1, x25, x1      ; 21 03 01 8b  
    49 : cmp  x3, x1           ; 7f 00 01 eb  
    50 : b.eq [65]             ; e0 01 00 54  
    51 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    52 : mul  x1, x26, x26     ; 41 7f 1a 9b  
    53 : mul  x3, x0, x0       ; 03 7c 00 9b  
    54 : mul  x25, x2, x2      ; 59 7c 02 9b  
    55 : add  x3, x3, x25      ; 63 00 19 8b  
    56 : cmp  x1, x3           ; 3f 00 03 eb  
    57 : b.eq [65]             ; 00 01 00 54  
    58 : mul  x1, x2, x2       ; 41 7c 02 9b  
    59 : mul  x3, x0, x0       ; 03 7c 00 9b  
    60 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    61 : mul  x25, x26, x26    ; 59 7f 1a 9b  
    62 : add  x3, x3, x25      ; 63 00 19 8b  
    63 : cmp  x1, x3           ; 3f 00 03 eb  
    64 : b.ne [67]             ; 61 00 00 54  
    65 : mov  x0, #0x01        ; 20 00 80 d2  
    66 : b    [92]             ; 1a 00 00 14  
    67 : mul  x1, x0, x0       ; 01 7c 00 9b  
    68 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    69 : mul  x3, x26, x26     ; 43 7f 1a 9b  
    70 : mul  x25, x2, x2      ; 59 7c 02 9b  
    71 : add  x3, x3, x25      ; 63 00 19 8b  
    72 : cmp  x1, x3           ; 3f 00 03 eb  
    73 : b.gt [88]             ; ec 01 00 54  
    74 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    75 : mul  x1, x26, x26     ; 41 7f 1a 9b  
    76 : mul  x3, x0, x0       ; 03 7c 00 9b  
    77 : mul  x25, x2, x2      ; 59 7c 02 9b  
    78 : add  x3, x3, x25      ; 63 00 19 8b  
    79 : cmp  x1, x3           ; 3f 00 03 eb  
    80 : b.gt [88]             ; 0c 01 00 54  
    81 : mul  x1, x2, x2       ; 41 7c 02 9b  
    82 : mul  x0, x0, x0       ; 00 7c 00 9b  
    83 : ldr  x26, [sp], #0    ; fa 03 40 f9  
    84 : mul  x2, x26, x26     ; 42 7f 1a 9b  
    85 : add  x0, x0, x2       ; 00 00 02 8b  
    86 : cmp  x1, x0           ; 3f 00 00 eb  
    87 : b.le [90]             ; 6d 00 00 54  
    88 : mov  x0, #0x05        ; a0 00 80 d2  
    89 : b    [92]             ; 03 00 00 14  
    90 : mov  x0, #0x04        ; 80 00 80 d2  
    91 : b    [92]             ; 01 00 00 14  
    92 : ldr  x25, [sp], #0x01 ; f9 07 40 f9  
    93 : ldr  x26, [sp], #0x02 ; fa 0b 40 f9  
    94 : add  sp, sp, #0x20    ; ff 83 00 91  
    95 : ret  x30              ; c0 03 5f d6  
