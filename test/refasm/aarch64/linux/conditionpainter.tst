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
    10 : b.gt [143]                ; ac 10 00 54  
    11 : movn x2, #0x04            ; 82 00 80 92  
    12 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    13 : sub  x2, x20, x2          ; 82 02 02 cb  
    14 : mov  x3, #0x0b            ; 63 01 80 d2  
    15 : mul  x2, x2, x3           ; 42 7c 03 9b  
    16 : mov  x3, #0x08            ; 03 01 80 d2  
    17 : mul  x20, x2, x3          ; 54 7c 03 9b  
    18 : str  x20, [sp], #0x03     ; f4 0f 00 f9  
    19 : movn x20, #0x04           ; 94 00 80 92  
    20 : str  x20, [sp], #0x02     ; f4 0b 00 f9  
    21 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    22 : cmp  x20, #0x05           ; 9f 16 00 f1  
    23 : b.gt [139]                ; 8c 0e 00 54  
    24 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    25 : add  x18, x20, #0x03      ; 92 0e 00 91  
    26 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    27 : cmp  x20, x18             ; 9f 02 12 eb  
    28 : cset x18, #0x0b           ; f2 b7 9f 9a  
    29 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    30 : cmp  x20, #0x04           ; 9f 12 00 f1  
    31 : cset x19, #0x0c           ; f3 c7 9f 9a  
    32 : and  x18, x18, x19        ; 52 02 13 8a  
    33 : movn x19, #0x01           ; 33 00 80 92  
    34 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    35 : cmp  x20, x19             ; 9f 02 13 eb  
    36 : cset x19, #0x0b           ; f3 b7 9f 9a  
    37 : and  x18, x18, x19        ; 52 02 13 8a  
    38 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    39 : cmp  x20, #0              ; 9f 02 00 f1  
    40 : cset x19, #0x0c           ; f3 c7 9f 9a  
    41 : and  x18, x18, x19        ; 52 02 13 8a  
    42 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    43 : sub  x19, x20, #0x01      ; 93 06 00 d1  
    44 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    45 : cmp  x20, x19             ; 9f 02 13 eb  
    46 : cset x19, #0x0c           ; f3 c7 9f 9a  
    47 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    48 : cmp  x20, #0              ; 9f 02 00 f1  
    49 : cset x1, #0x0b            ; e1 b7 9f 9a  
    50 : and  x1, x19, x1          ; 61 02 01 8a  
    51 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    52 : cmp  x20, #0              ; 9f 02 00 f1  
    53 : cset x19, #0x0c           ; f3 c7 9f 9a  
    54 : and  x1, x1, x19          ; 21 00 13 8a  
    55 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    56 : mul  x19, x20, x20        ; 93 7e 14 9b  
    57 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    58 : mul  x0, x20, x20         ; 80 7e 14 9b  
    59 : add  x0, x19, x0          ; 60 02 00 8b  
    60 : cmp  x0, #0x09            ; 1f 24 00 f1  
    61 : cset x0, #0x0c            ; e0 c7 9f 9a  
    62 : and  x0, x1, x0           ; 20 00 00 8a  
    63 : orr  x0, x18, x0          ; 40 02 00 aa  
    64 : mov  x1, #0x02            ; 41 00 80 d2  
    65 : eor  x18, x18, x18        ; 52 02 12 ca  
    66 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    67 : cmp  x20, #0x02           ; 9f 0a 00 f1  
    68 : cset x19, #0x0b           ; f3 b7 9f 9a  
    69 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    70 : cmp  x20, #0x04           ; 9f 12 00 f1  
    71 : cset x3, #0x0c            ; e3 c7 9f 9a  
    72 : and  x3, x19, x3          ; 63 02 03 8a  
    73 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    74 : cmp  x20, #0x01           ; 9f 06 00 f1  
    75 : cset x19, #0x0b           ; f3 b7 9f 9a  
    76 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    77 : cmp  x20, #0x03           ; 9f 0e 00 f1  
    78 : cset x2, #0x0c            ; e2 c7 9f 9a  
    79 : and  x2, x19, x2          ; 62 02 02 8a  
    80 : orr  x2, x3, x2           ; 62 00 02 aa  
    81 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    82 : mul  x3, x20, x20         ; 83 7e 14 9b  
    83 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    84 : mul  x19, x20, x20        ; 93 7e 14 9b  
    85 : add  x3, x3, x19          ; 63 00 13 8b  
    86 : cmp  x3, #0x10            ; 7f 40 00 f1  
    87 : cset x3, #0x0c            ; e3 c7 9f 9a  
    88 : and  x2, x2, x3           ; 42 00 03 8a  
    89 : cmp  x2, #0               ; 5f 00 00 f1  
    90 : csel x1, x1, x18, #0x01   ; 21 10 92 9a  
    91 : add  x0, x0, x1           ; 00 00 01 8b  
    92 : mov  x1, #0x02            ; 41 00 80 d2  
    93 : ldr  x20, [sp], #0        ; f4 03 40 f9  
    94 : mul  x1, x20, x1          ; 81 7e 01 9b  
    95 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    96 : cmp  x1, x20              ; 3f 00 14 eb  
    97 : b.lt [107]                ; 4b 01 00 54  
    98 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
    99 : add  x1, x20, #0x03       ; 81 0e 00 91  
   100 : neg  x1, x1               ; e1 03 01 cb  
   101 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   102 : add  x2, x20, #0x03       ; 82 0e 00 91  
   103 : mul  x1, x1, x2           ; 21 7c 02 9b  
   104 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   105 : cmp  x20, x1              ; 9f 02 01 eb  
   106 : b.le [122]                ; 0d 02 00 54  
   107 : mov  x1, #0x02            ; 41 00 80 d2  
   108 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   109 : mul  x1, x20, x1          ; 81 7e 01 9b  
   110 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   111 : cmp  x20, x1              ; 9f 02 01 eb  
   112 : b.gt [126]                ; cc 01 00 54  
   113 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   114 : add  x1, x20, #0x03       ; 81 0e 00 91  
   115 : neg  x1, x1               ; e1 03 01 cb  
   116 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   117 : add  x2, x20, #0x03       ; 82 0e 00 91  
   118 : mul  x1, x1, x2           ; 21 7c 02 9b  
   119 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   120 : cmp  x20, x1              ; 9f 02 01 eb  
   121 : b.lt [126]                ; ab 00 00 54  
   122 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   123 : cmp  x20, #0              ; 9f 02 00 f1  
   124 : b.gt [126]                ; 4c 00 00 54  
   125 : add  x0, x0, #0x03        ; 00 0c 00 91  
   126 : ldr  x20, [sp], #0x01     ; f4 07 40 f9  
   127 : ldr  x21, [sp], #0x03     ; f5 0f 40 f9  
   128 : add  x1, x20, x21         ; 81 02 15 8b  
   129 : movn x2, #0x04            ; 82 00 80 92  
   130 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   131 : sub  x2, x20, x2          ; 82 02 02 cb  
   132 : lsl  x2, x2, #0x3d, #0x3c ; 42 f0 7d d3  
   133 : add  x1, x1, x2           ; 21 00 02 8b  
   134 : str  x0, [x1], #0         ; 20 00 00 f9  
   135 : ldr  x20, [sp], #0x02     ; f4 0b 40 f9  
   136 : add  x20, x20, #0x01      ; 94 06 00 91  
   137 : str  x20, [sp], #0x02     ; f4 0b 00 f9  
   138 : b    [21]                 ; 8b ff ff 17  
   139 : ldr  x20, [sp], #0        ; f4 03 40 f9  
   140 : add  x20, x20, #0x01      ; 94 06 00 91  
   141 : str  x20, [sp], #0        ; f4 03 00 f9  
   142 : b    [8]                  ; 7a ff ff 17  
   143 : ldr  x18, [sp], #0x04     ; f2 13 40 f9  
   144 : ldr  x19, [sp], #0x05     ; f3 17 40 f9  
   145 : ldr  x20, [sp], #0x06     ; f4 1b 40 f9  
   146 : ldr  x21, [sp], #0x07     ; f5 1f 40 f9  
   147 : add  sp, sp, #0x40        ; ff 03 01 91  
   148 : ret  x30                  ; c0 03 5f d6  
