all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub   sp, sp, #0x40        ; ff 03 01 d1  
     1 : str   x18, [sp], #0x04     ; f2 13 00 f9  
     2 : str   x19, [sp], #0x05     ; f3 17 00 f9  
     3 : str   x20, [sp], #0x06     ; f4 1b 00 f9  
     4 : str   x21, [sp], #0x07     ; f5 1f 00 f9  
     5 : mov   x20, #0              ; 14 00 80 d2  
     6 : str   x20, [sp], #0x02     ; f4 0b 00 f9  
     7 : mov   x20, #0              ; 14 00 80 d2  
     8 : str   x20, [sp], #0x01     ; f4 07 00 f9  
     9 : mov   x20, #0              ; 14 00 80 d2  
    10 : str   x20, [sp], #0        ; f4 03 00 f9  
    11 : mov   x19, #0x02           ; 53 00 80 d2  
    12 : mov   x18, #0x01           ; 32 00 80 d2  
    13 : cmp   x1, #0x01            ; 3f 04 00 f1  
    14 : csel  x18, x19, x18, #0x0c ; 72 c2 92 9a  
    15 : mov   x19, #0x04           ; 93 00 80 d2  
    16 : cmp   x1, #0x03            ; 3f 0c 00 f1  
    17 : csel  x18, x19, x18, #0x0c ; 72 c2 92 9a  
    18 : mov   x19, #0x08           ; 13 01 80 d2  
    19 : cmp   x1, #0x05            ; 3f 14 00 f1  
    20 : csel  x20, x19, x18, #0x0c ; 74 c2 92 9a  
    21 : str   x20, [sp], #0x03     ; f4 0f 00 f9  
    22 : mov   x19, #0x02           ; 53 00 80 d2  
    23 : mov   x18, #0x01           ; 32 00 80 d2  
    24 : cmp   x3, #0x01            ; 7f 04 00 f1  
    25 : csel  x18, x19, x18, #0x0c ; 72 c2 92 9a  
    26 : mov   x19, #0x04           ; 93 00 80 d2  
    27 : cmp   x3, #0x03            ; 7f 0c 00 f1  
    28 : csel  x18, x19, x18, #0x0c ; 72 c2 92 9a  
    29 : mov   x19, #0x08           ; 13 01 80 d2  
    30 : cmp   x3, #0x05            ; 7f 14 00 f1  
    31 : csel  x18, x19, x18, #0x0c ; 72 c2 92 9a  
    32 : ldr   x20, [sp], #0x02     ; f4 0b 40 f9  
    33 : cmp   x20, x4              ; 9f 02 04 eb  
    34 : b.ge  [121]                ; ea 0a 00 54  
    35 : mov   x19, #0              ; 13 00 80 d2  
    36 : cmp   x1, #0               ; 3f 00 00 f1  
    37 : b.ne  [41]                 ; 81 00 00 54  
    38 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    39 : ldrb  x19, [x0], x20       ; 13 68 74 38  
    40 : b     [73]                 ; 21 00 00 14  
    41 : cmp   x1, #0x01            ; 3f 04 00 f1  
    42 : b.ne  [46]                 ; 81 00 00 54  
    43 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    44 : ldrsb x19, [x0], x20       ; 13 68 b4 38  
    45 : b     [73]                 ; 1c 00 00 14  
    46 : cmp   x1, #0x02            ; 3f 08 00 f1  
    47 : b.ne  [51]                 ; 81 00 00 54  
    48 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    49 : ldrh  x19, [x0], x20       ; 13 68 74 78  
    50 : b     [73]                 ; 17 00 00 14  
    51 : cmp   x1, #0x03            ; 3f 0c 00 f1  
    52 : b.ne  [56]                 ; 81 00 00 54  
    53 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    54 : ldrsh x19, [x0], x20       ; 13 68 b4 78  
    55 : b     [73]                 ; 12 00 00 14  
    56 : cmp   x1, #0x04            ; 3f 10 00 f1  
    57 : b.ne  [61]                 ; 81 00 00 54  
    58 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    59 : ldr   x19, [x0], x20       ; 13 68 74 b8  
    60 : b     [73]                 ; 0d 00 00 14  
    61 : cmp   x1, #0x05            ; 3f 14 00 f1  
    62 : b.ne  [66]                 ; 81 00 00 54  
    63 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    64 : ldrsw x19, [x0], x20       ; 13 68 b4 b8  
    65 : b     [73]                 ; 08 00 00 14  
    66 : cmp   x1, #0x06            ; 3f 18 00 f1  
    67 : b.ne  [71]                 ; 81 00 00 54  
    68 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    69 : ldr   x19, [x0], x20       ; 13 68 74 f8  
    70 : b     [73]                 ; 03 00 00 14  
    71 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
    72 : ldr   x19, [x0], x20       ; 13 68 74 f8  
    73 : cmp   x3, #0               ; 7f 00 00 f1  
    74 : b.ne  [78]                 ; 81 00 00 54  
    75 : ldr   x20, [sp], #0        ; f4 03 40 f9  
    76 : strb  w19, x2, x20         ; 53 68 34 38  
    77 : b     [110]                ; 21 00 00 14  
    78 : cmp   x3, #0x01            ; 7f 04 00 f1  
    79 : b.ne  [83]                 ; 81 00 00 54  
    80 : ldr   x20, [sp], #0        ; f4 03 40 f9  
    81 : strb  w19, x2, x20         ; 53 68 34 38  
    82 : b     [110]                ; 1c 00 00 14  
    83 : cmp   x3, #0x02            ; 7f 08 00 f1  
    84 : b.ne  [88]                 ; 81 00 00 54  
    85 : ldr   x20, [sp], #0        ; f4 03 40 f9  
    86 : strh  w19, x2, x20         ; 53 68 34 78  
    87 : b     [110]                ; 17 00 00 14  
    88 : cmp   x3, #0x03            ; 7f 0c 00 f1  
    89 : b.ne  [93]                 ; 81 00 00 54  
    90 : ldr   x20, [sp], #0        ; f4 03 40 f9  
    91 : strh  w19, x2, x20         ; 53 68 34 78  
    92 : b     [110]                ; 12 00 00 14  
    93 : cmp   x3, #0x04            ; 7f 10 00 f1  
    94 : b.ne  [98]                 ; 81 00 00 54  
    95 : ldr   x20, [sp], #0        ; f4 03 40 f9  
    96 : str   #0, w19, x2, x20     ; 53 68 34 b8  
    97 : b     [110]                ; 0d 00 00 14  
    98 : cmp   x3, #0x05            ; 7f 14 00 f1  
    99 : b.ne  [103]                ; 81 00 00 54  
   100 : ldr   x20, [sp], #0        ; f4 03 40 f9  
   101 : str   #0, w19, x2, x20     ; 53 68 34 b8  
   102 : b     [110]                ; 08 00 00 14  
   103 : cmp   x3, #0x06            ; 7f 18 00 f1  
   104 : b.ne  [108]                ; 81 00 00 54  
   105 : ldr   x20, [sp], #0        ; f4 03 40 f9  
   106 : str   #0x01, x13, x2, x14  ; 53 68 34 f8  
   107 : b     [110]                ; 03 00 00 14  
   108 : ldr   x20, [sp], #0        ; f4 03 40 f9  
   109 : str   #0x01, x13, x2, x14  ; 53 68 34 f8  
   110 : ldr   x20, [sp], #0x01     ; f4 07 40 f9  
   111 : ldr   x21, [sp], #0x03     ; f5 0f 40 f9  
   112 : add   x20, x20, x21        ; 94 02 15 8b  
   113 : str   x20, [sp], #0x01     ; f4 07 00 f9  
   114 : ldr   x20, [sp], #0        ; f4 03 40 f9  
   115 : add   x20, x20, x18        ; 94 02 12 8b  
   116 : str   x20, [sp], #0        ; f4 03 00 f9  
   117 : ldr   x20, [sp], #0x02     ; f4 0b 40 f9  
   118 : add   x20, x20, #0x01      ; 94 06 00 91  
   119 : str   x20, [sp], #0x02     ; f4 0b 00 f9  
   120 : b     [32]                 ; a8 ff ff 17  
   121 : ldr   x18, [sp], #0x04     ; f2 13 40 f9  
   122 : ldr   x19, [sp], #0x05     ; f3 17 40 f9  
   123 : ldr   x20, [sp], #0x06     ; f4 1b 40 f9  
   124 : ldr   x21, [sp], #0x07     ; f5 1f 40 f9  
   125 : add   sp, sp, #0x40        ; ff 03 01 91  
   126 : ret   x30                  ; c0 03 5f d6  
