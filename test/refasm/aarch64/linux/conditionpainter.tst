conditionpainter(i0)
     0 : sub  sp, sp, #0x40        ; ff 03 01 d1  
     1 : str  x0, [sp], #0x01      ; e0 07 00 f9  
     2 : str  x18, [sp], #0x04     ; f2 13 00 f9  
     3 : str  x19, [sp], #0x05     ; f3 17 00 f9  
     4 : str  x20, [sp], #0x06     ; f4 1b 00 f9  
     5 : str  x21, [sp], #0x07     ; f5 1f 00 f9  
     6 : movn x20, #0x04           ; 94 00 80 92  
     7 : str  x20, [sp], #0        ; f4 03 00 f9  
     8 : ldr  x20, [sp], #0        ; f4 03 40 f9  
     9 : cmp  x20, #0x05           ; 9f 16 00 f1  
    10 : b.gt [141]                ; 6c 10 00 54  
    11 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    12 : add  x2, x20, #0x05       ; 82 16 00 91  
    13 : mov  x3, #0x0b            ; 63 01 80 d2  
    14 : mul  x2, x2, x3           ; 42 7c 03 9b  
    15 : mov  x3, #0x08            ; 03 01 80 d2  
    16 : mul  x20, x2, x3          ; 54 7c 03 9b  
    17 : str  x20, [sp], #0x03     ; f4 0f 00 f9  
    18 : movn x20, #0x04           ; 94 00 80 92  
    19 : str  x20, [sp], #0x02     ; f4 0b 00 f9  
    20 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    21 : cmp  x20, #0x05           ; 9f 16 00 f1  
    22 : b.gt [137]                ; 6c 0e 00 54  
    23 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    24 : add  x18, x20, #0x03      ; 92 0e 00 91  
    25 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    26 : cmp  x20, x18             ; 9f 02 12 eb  
    27 : cset x18, #0x0b           ; f2 b7 9f 9a  
    28 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    29 : cmp  x20, #0x04           ; 9f 12 00 f1  
    30 : cset x19, #0x0c           ; f3 c7 9f 9a  
    31 : and  x18, x18, x19        ; 52 02 13 8a  
    32 : movn x19, #0x01           ; 33 00 80 92  
    33 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    34 : cmp  x20, x19             ; 9f 02 13 eb  
    35 : cset x19, #0x0b           ; f3 b7 9f 9a  
    36 : and  x18, x18, x19        ; 52 02 13 8a  
    37 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    38 : cmp  x20, #0              ; 9f 02 00 f1  
    39 : cset x19, #0x0c           ; f3 c7 9f 9a  
    40 : and  x18, x18, x19        ; 52 02 13 8a  
    41 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    42 : sub  x19, x20, #0x01      ; 93 06 00 d1  
    43 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    44 : cmp  x20, x19             ; 9f 02 13 eb  
    45 : cset x19, #0x0c           ; f3 c7 9f 9a  
    46 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    47 : cmp  x20, #0              ; 9f 02 00 f1  
    48 : cset x1, #0x0b            ; e1 b7 9f 9a  
    49 : and  x1, x19, x1          ; 61 02 01 8a  
    50 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    51 : cmp  x20, #0              ; 9f 02 00 f1  
    52 : cset x19, #0x0c           ; f3 c7 9f 9a  
    53 : and  x1, x1, x19          ; 21 00 13 8a  
    54 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    55 : mul  x19, x20, x20        ; 93 7e 14 9b  
    56 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    57 : mul  x0, x20, x20         ; 80 7e 14 9b  
    58 : add  x0, x19, x0          ; 60 02 00 8b  
    59 : cmp  x0, #0x09            ; 1f 24 00 f1  
    60 : cset x0, #0x0c            ; e0 c7 9f 9a  
    61 : and  x0, x1, x0           ; 20 00 00 8a  
    62 : orr  x0, x18, x0          ; 40 02 00 aa  
    63 : mov  x1, #0x02            ; 41 00 80 d2  
    64 : eor  x18, x18, x18        ; 52 02 12 ca  
    65 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    66 : cmp  x20, #0x02           ; 9f 0a 00 f1  
    67 : cset x19, #0x0b           ; f3 b7 9f 9a  
    68 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    69 : cmp  x20, #0x04           ; 9f 12 00 f1  
    70 : cset x3, #0x0c            ; e3 c7 9f 9a  
    71 : and  x3, x19, x3          ; 63 02 03 8a  
    72 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    73 : cmp  x20, #0x01           ; 9f 06 00 f1  
    74 : cset x19, #0x0b           ; f3 b7 9f 9a  
    75 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    76 : cmp  x20, #0x03           ; 9f 0e 00 f1  
    77 : cset x2, #0x0c            ; e2 c7 9f 9a  
    78 : and  x2, x19, x2          ; 62 02 02 8a  
    79 : orr  x2, x3, x2           ; 62 00 02 aa  
    80 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    81 : mul  x3, x20, x20         ; 83 7e 14 9b  
    82 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    83 : mul  x19, x20, x20        ; 93 7e 14 9b  
    84 : add  x3, x3, x19          ; 63 00 13 8b  
    85 : cmp  x3, #0x10            ; 7f 40 00 f1  
    86 : cset x3, #0x0c            ; e3 c7 9f 9a  
    87 : and  x2, x2, x3           ; 42 00 03 8a  
    88 : cmp  x2, #0               ; 5f 00 00 f1  
    89 : csel x1, x1, x18, #0x01   ; 21 10 92 9a  
    90 : add  x0, x0, x1           ; 00 00 01 8b  
    91 : mov  x1, #0x02            ; 41 00 80 d2  
    92 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    93 : mul  x1, x20, x1          ; 81 7e 01 9b  
    94 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    95 : cmp  x1, x20              ; 3f 00 14 eb  
    96 : b.lt [106]                ; 4b 01 00 54  
    97 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    98 : add  x1, x20, #0x03       ; 81 0e 00 91  
    99 : neg  x1, x1               ; e1 03 01 cb  
   100 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   101 : add  x2, x20, #0x03       ; 82 0e 00 91  
   102 : mul  x1, x1, x2           ; 21 7c 02 9b  
   103 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   104 : cmp  x20, x1              ; 9f 02 01 eb  
   105 : b.le [121]                ; 0d 02 00 54  
   106 : mov  x1, #0x02            ; 41 00 80 d2  
   107 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   108 : mul  x1, x20, x1          ; 81 7e 01 9b  
   109 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   110 : cmp  x20, x1              ; 9f 02 01 eb  
   111 : b.gt [125]                ; cc 01 00 54  
   112 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   113 : add  x1, x20, #0x03       ; 81 0e 00 91  
   114 : neg  x1, x1               ; e1 03 01 cb  
   115 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   116 : add  x2, x20, #0x03       ; 82 0e 00 91  
   117 : mul  x1, x1, x2           ; 21 7c 02 9b  
   118 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   119 : cmp  x20, x1              ; 9f 02 01 eb  
   120 : b.lt [125]                ; ab 00 00 54  
   121 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   122 : cmp  x20, #0              ; 9f 02 00 f1  
   123 : b.gt [125]                ; 4c 00 00 54  
   124 : add  x0, x0, #0x03        ; 00 0c 00 91  
   125 : ldr  x20, [sp], #0x01     ; f4 07 40 f9  
   126 : ldr  x21, [sp], #0x03     ; f5 0f 40 f9  
   127 : add  x1, x20, x21         ; 81 02 15 8b  
   128 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   129 : add  x2, x20, #0x05       ; 82 16 00 91  
   130 : lsl  x2, x2, #0x3d, #0x3c ; 42 f0 7d d3  
   131 : add  x1, x1, x2           ; 21 00 02 8b  
   132 : str  x0, [x1], #0         ; 20 00 00 f9  
   133 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   134 : add  x20, x20, #0x01      ; 94 06 00 91  
   135 : str  x20, [sp], #0x02     ; f4 0b 00 f9  
   136 : b    [20]                 ; 8c ff ff 17  
   137 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   138 : add  x20, x20, #0x01      ; 94 06 00 91  
   139 : str  x20, [sp], #0        ; f4 03 00 f9  
   140 : b    [8]                  ; 7c ff ff 17  
   141 : ldr  x18, [sp], #0x04     ; f2 13 40 f9  
   142 : ldr  x19, [sp], #0x05     ; f3 17 40 f9  
   143 : ldr  x20, [sp], #0x06     ; f4 1b 40 f9  
   144 : ldr  x21, [sp], #0x07     ; f5 1f 40 f9  
   145 : add  sp, sp, #0x40        ; ff 03 01 91  
   146 : ret  x30                  ; c0 03 5f d6  
