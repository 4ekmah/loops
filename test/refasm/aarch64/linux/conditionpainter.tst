conditionpainter(i0)
     0 : sub  sp, sp, #0x40        ; ff 03 01 d1  
     1 : str  x0, [sp], #0x01      ; e0 07 00 f9  
     2 : str  x25, [sp], #0x05     ; f9 17 00 f9  
     3 : str  x26, [sp], #0x06     ; fa 1b 00 f9  
     4 : str  x27, [sp], #0x07     ; fb 1f 00 f9  
     5 : movn x26, #0x04           ; 9a 00 80 92  
     6 : str  x26, [sp], #0        ; fa 03 00 f9  
     7 : ldr  x26, [sp], #0        ; fa 03 40 f9  
     8 : cmp  x26, #0x05           ; 5f 17 00 f1  
     9 : b.gt [142]                ; ac 10 00 54  
    10 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    11 : add  x2, x26, #0x05       ; 42 17 00 91  
    12 : mov  x3, #0x0b            ; 63 01 80 d2  
    13 : mul  x2, x2, x3           ; 42 7c 03 9b  
    14 : mov  x3, #0x08            ; 03 01 80 d2  
    15 : mul  x26, x2, x3          ; 5a 7c 03 9b  
    16 : str  x26, [sp], #0x03     ; fa 0f 00 f9  
    17 : movn x26, #0x04           ; 9a 00 80 92  
    18 : str  x26, [sp], #0x02     ; fa 0b 00 f9  
    19 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    20 : cmp  x26, #0x05           ; 5f 17 00 f1  
    21 : b.gt [138]                ; ac 0e 00 54  
    22 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    23 : add  x25, x26, #0x03      ; 59 0f 00 91  
    24 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    25 : cmp  x26, x25             ; 5f 03 19 eb  
    26 : cset x25, #0x0b           ; f9 b7 9f 9a  
    27 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    28 : cmp  x26, #0x04           ; 5f 13 00 f1  
    29 : cset x1, #0x0c            ; e1 c7 9f 9a  
    30 : and  x1, x25, x1          ; 21 03 01 8a  
    31 : movn x25, #0x01           ; 39 00 80 92  
    32 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    33 : cmp  x26, x25             ; 5f 03 19 eb  
    34 : cset x25, #0x0b           ; f9 b7 9f 9a  
    35 : and  x1, x1, x25          ; 21 00 19 8a  
    36 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    37 : cmp  x26, #0              ; 5f 03 00 f1  
    38 : cset x25, #0x0c           ; f9 c7 9f 9a  
    39 : and  x1, x1, x25          ; 21 00 19 8a  
    40 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    41 : sub  x25, x26, #0x01      ; 59 07 00 d1  
    42 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    43 : cmp  x26, x25             ; 5f 03 19 eb  
    44 : cset x25, #0x0c           ; f9 c7 9f 9a  
    45 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    46 : cmp  x26, #0              ; 5f 03 00 f1  
    47 : cset x0, #0x0b            ; e0 b7 9f 9a  
    48 : and  x0, x25, x0          ; 20 03 00 8a  
    49 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    50 : cmp  x26, #0              ; 5f 03 00 f1  
    51 : cset x25, #0x0c           ; f9 c7 9f 9a  
    52 : and  x0, x0, x25          ; 00 00 19 8a  
    53 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    54 : mul  x25, x26, x26        ; 59 7f 1a 9b  
    55 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    56 : mul  x3, x26, x26         ; 43 7f 1a 9b  
    57 : add  x3, x25, x3          ; 23 03 03 8b  
    58 : cmp  x3, #0x09            ; 7f 24 00 f1  
    59 : cset x3, #0x0c            ; e3 c7 9f 9a  
    60 : and  x0, x0, x3           ; 00 00 03 8a  
    61 : orr  x26, x1, x0          ; 3a 00 00 aa  
    62 : str  x26, [sp], #0x04     ; fa 13 00 f9  
    63 : mov  x1, #0x02            ; 41 00 80 d2  
    64 : eor  x3, x3, x3           ; 63 00 03 ca  
    65 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    66 : cmp  x26, #0x02           ; 5f 0b 00 f1  
    67 : cset x25, #0x0b           ; f9 b7 9f 9a  
    68 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    69 : cmp  x26, #0x04           ; 5f 13 00 f1  
    70 : cset x2, #0x0c            ; e2 c7 9f 9a  
    71 : and  x2, x25, x2          ; 22 03 02 8a  
    72 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    73 : cmp  x26, #0x01           ; 5f 07 00 f1  
    74 : cset x25, #0x0b           ; f9 b7 9f 9a  
    75 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    76 : cmp  x26, #0x03           ; 5f 0f 00 f1  
    77 : cset x0, #0x0c            ; e0 c7 9f 9a  
    78 : and  x0, x25, x0          ; 20 03 00 8a  
    79 : orr  x0, x2, x0           ; 40 00 00 aa  
    80 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    81 : mul  x2, x26, x26         ; 42 7f 1a 9b  
    82 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    83 : mul  x25, x26, x26        ; 59 7f 1a 9b  
    84 : add  x2, x2, x25          ; 42 00 19 8b  
    85 : cmp  x2, #0x10            ; 5f 40 00 f1  
    86 : cset x2, #0x0c            ; e2 c7 9f 9a  
    87 : and  x0, x0, x2           ; 00 00 02 8a  
    88 : cmp  x0, #0               ; 1f 00 00 f1  
    89 : csel x0, x1, x3, #0x01    ; 20 10 83 9a  
    90 : ldr  x26, [sp], #0x04     ; fa 13 40 f9  
    91 : add  x0, x26, x0          ; 40 03 00 8b  
    92 : mov  x1, #0x02            ; 41 00 80 d2  
    93 : ldr  x26, [sp], #0        ; fa 03 40 f9  
    94 : mul  x1, x26, x1          ; 41 7f 01 9b  
    95 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    96 : cmp  x1, x26              ; 3f 00 1a eb  
    97 : b.lt [107]                ; 4b 01 00 54  
    98 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
    99 : add  x1, x26, #0x03       ; 41 0f 00 91  
   100 : neg  x1, x1               ; e1 03 01 cb  
   101 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
   102 : add  x2, x26, #0x03       ; 42 0f 00 91  
   103 : mul  x1, x1, x2           ; 21 7c 02 9b  
   104 : ldr  x26, [sp], #0        ; fa 03 40 f9  
   105 : cmp  x26, x1              ; 5f 03 01 eb  
   106 : b.le [122]                ; 0d 02 00 54  
   107 : mov  x1, #0x02            ; 41 00 80 d2  
   108 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
   109 : mul  x1, x26, x1          ; 41 7f 01 9b  
   110 : ldr  x26, [sp], #0        ; fa 03 40 f9  
   111 : cmp  x26, x1              ; 5f 03 01 eb  
   112 : b.gt [126]                ; cc 01 00 54  
   113 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
   114 : add  x1, x26, #0x03       ; 41 0f 00 91  
   115 : neg  x1, x1               ; e1 03 01 cb  
   116 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
   117 : add  x2, x26, #0x03       ; 42 0f 00 91  
   118 : mul  x1, x1, x2           ; 21 7c 02 9b  
   119 : ldr  x26, [sp], #0        ; fa 03 40 f9  
   120 : cmp  x26, x1              ; 5f 03 01 eb  
   121 : b.lt [126]                ; ab 00 00 54  
   122 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
   123 : cmp  x26, #0              ; 5f 03 00 f1  
   124 : b.gt [126]                ; 4c 00 00 54  
   125 : add  x0, x0, #0x03        ; 00 0c 00 91  
   126 : ldr  x26, [sp], #0x01     ; fa 07 40 f9  
   127 : ldr  x27, [sp], #0x03     ; fb 0f 40 f9  
   128 : add  x1, x26, x27         ; 41 03 1b 8b  
   129 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
   130 : add  x2, x26, #0x05       ; 42 17 00 91  
   131 : lsl  x2, x2, #0x3d, #0x3c ; 42 f0 7d d3  
   132 : add  x1, x1, x2           ; 21 00 02 8b  
   133 : str  x0, [x1], #0         ; 20 00 00 f9  
   134 : ldr  x26, [sp], #0x02     ; fa 0b 40 f9  
   135 : add  x26, x26, #0x01      ; 5a 07 00 91  
   136 : str  x26, [sp], #0x02     ; fa 0b 00 f9  
   137 : b    [19]                 ; 8a ff ff 17  
   138 : ldr  x26, [sp], #0        ; fa 03 40 f9  
   139 : add  x26, x26, #0x01      ; 5a 07 00 91  
   140 : str  x26, [sp], #0        ; fa 03 00 f9  
   141 : b    [7]                  ; 7a ff ff 17  
   142 : ldr  x25, [sp], #0x05     ; f9 17 40 f9  
   143 : ldr  x26, [sp], #0x06     ; fa 1b 40 f9  
   144 : ldr  x27, [sp], #0x07     ; fb 1f 40 f9  
   145 : add  sp, sp, #0x40        ; ff 03 01 91  
   146 : ret  x30                  ; c0 03 5f d6  
