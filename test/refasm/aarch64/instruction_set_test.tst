instruction_set_test()
     0 : str   #0x01, x0, x0, #0x20        ; 00 80 00 f9  
     1 : str   #0x01, x0, x0, #0x20        ; 00 80 00 f9  
     2 : str   #0x01, x0, x0, x0           ; 00 68 20 f8  
     3 : str   #0x01, x0, x0, x0           ; 00 68 20 f8  
     4 : str   #0, w0, x0, #0x40           ; 00 00 01 b9  
     5 : str   #0, w0, x0, #0x40           ; 00 00 01 b9  
     6 : str   #0, w0, x0, x0              ; 00 68 20 b8  
     7 : str   #0, w0, x0, x0              ; 00 68 20 b8  
     8 : strh  w0, x0, #0x80               ; 00 00 02 79  
     9 : strh  w0, x0, #0x80               ; 00 00 02 79  
    10 : strh  w0, x0, x0                  ; 00 68 20 78  
    11 : strh  w0, x0, x0                  ; 00 68 20 78  
    12 : strb  w0, x0, #0x100              ; 00 00 04 39  
    13 : strb  w0, x0, #0x100              ; 00 00 04 39  
    14 : strb  w0, x0, x0                  ; 00 68 20 38  
    15 : strb  w0, x0, x0                  ; 00 68 20 38  
    16 : ldr   x0, [x0], #0                ; 00 00 40 f9  
    17 : ldr   x0, [x0], #0x20             ; 00 80 40 f9  
    18 : ldr   x15, [x0], #0x20            ; 0f 80 40 f9  
    19 : ldr   x0, [x15], #0x20            ; e0 81 40 f9  
    20 : ldr   x0, [x0], x0                ; 00 68 60 f8  
    21 : ldr   x15, [x0], x0               ; 0f 68 60 f8  
    22 : ldr   x0, [x15], x0               ; e0 69 60 f8  
    23 : ldr   x0, [x0], x15               ; 00 68 6f f8  
    24 : ldr   x0, [x0], #0                ; 00 00 40 f9  
    25 : ldr   x0, [x0], #0x20             ; 00 80 40 f9  
    26 : ldr   x15, [x0], #0x20            ; 0f 80 40 f9  
    27 : ldr   x0, [x15], #0x20            ; e0 81 40 f9  
    28 : ldr   x0, [x0], x0                ; 00 68 60 f8  
    29 : ldr   x15, [x0], x0               ; 0f 68 60 f8  
    30 : ldr   x0, [x15], x0               ; e0 69 60 f8  
    31 : ldr   x0, [x0], x15               ; 00 68 6f f8  
    32 : ldr   x0, [x0], #0                ; 00 00 40 b9  
    33 : ldr   x0, [x0], #0x40             ; 00 00 41 b9  
    34 : ldr   x15, [x0], #0x40            ; 0f 00 41 b9  
    35 : ldr   x0, [x15], #0x40            ; e0 01 41 b9  
    36 : ldr   x0, [x0], x0                ; 00 68 60 b8  
    37 : ldr   x15, [x0], x0               ; 0f 68 60 b8  
    38 : ldr   x0, [x15], x0               ; e0 69 60 b8  
    39 : ldr   x0, [x0], x15               ; 00 68 6f b8  
    40 : ldrsw x0, [x0], #0                ; 00 00 80 b9  
    41 : ldrsw x0, [x0], #0x40             ; 00 00 81 b9  
    42 : ldrsw x15, [x0], #0x40            ; 0f 00 81 b9  
    43 : ldrsw x0, [x15], #0x40            ; e0 01 81 b9  
    44 : ldrsw x0, [x0], x0                ; 00 68 a0 b8  
    45 : ldrsw x15, [x0], x0               ; 0f 68 a0 b8  
    46 : ldrsw x0, [x15], x0               ; e0 69 a0 b8  
    47 : ldrsw x0, [x0], x15               ; 00 68 af b8  
    48 : ldrh  x0, [x0], #0                ; 00 00 40 79  
    49 : ldrh  x0, [x0], #0x80             ; 00 00 42 79  
    50 : ldrh  x15, [x0], #0x80            ; 0f 00 42 79  
    51 : ldrh  x0, [x15], #0x80            ; e0 01 42 79  
    52 : ldrh  x0, [x0], x0                ; 00 68 60 78  
    53 : ldrh  x15, [x0], x0               ; 0f 68 60 78  
    54 : ldrh  x0, [x15], x0               ; e0 69 60 78  
    55 : ldrh  x0, [x0], x15               ; 00 68 6f 78  
    56 : ldrsh x0, [x0], #0                ; 00 00 80 79  
    57 : ldrsh x0, [x0], #0x80             ; 00 00 82 79  
    58 : ldrsh x15, [x0], #0x80            ; 0f 00 82 79  
    59 : ldrsh x0, [x15], #0x80            ; e0 01 82 79  
    60 : ldrsh x0, [x0], x0                ; 00 68 a0 78  
    61 : ldrsh x15, [x0], x0               ; 0f 68 a0 78  
    62 : ldrsh x0, [x15], x0               ; e0 69 a0 78  
    63 : ldrsh x0, [x0], x15               ; 00 68 af 78  
    64 : ldrb  x0, [x0], #0                ; 00 00 40 39  
    65 : ldrb  x0, [x0], #0x100            ; 00 00 44 39  
    66 : ldrb  x15, [x0], #0x100           ; 0f 00 44 39  
    67 : ldrb  x0, [x15], #0x100           ; e0 01 44 39  
    68 : ldrb  x0, [x0], x0                ; 00 68 60 38  
    69 : ldrb  x15, [x0], x0               ; 0f 68 60 38  
    70 : ldrb  x0, [x15], x0               ; e0 69 60 38  
    71 : ldrb  x0, [x0], x15               ; 00 68 6f 38  
    72 : ldrsb x0, [x0], #0                ; 00 00 80 39  
    73 : ldrsb x0, [x0], #0x100            ; 00 00 84 39  
    74 : ldrsb x15, [x0], #0x100           ; 0f 00 84 39  
    75 : ldrsb x0, [x15], #0x100           ; e0 01 84 39  
    76 : ldrsb x0, [x0], x0                ; 00 68 a0 38  
    77 : ldrsb x15, [x0], x0               ; 0f 68 a0 38  
    78 : ldrsb x0, [x15], x0               ; e0 69 a0 38  
    79 : ldrsb x0, [x0], x15               ; 00 68 af 38  
    80 : csel  x0, x0, x0, #0              ; 00 00 80 9a  
    81 : csel  x0, x0, x0, #0x01           ; 00 10 80 9a  
    82 : csel  x0, x0, x0, #0x0b           ; 00 b0 80 9a  
    83 : csel  x0, x0, x0, #0x0c           ; 00 c0 80 9a  
    84 : csel  x0, x0, x0, #0x0d           ; 00 d0 80 9a  
    85 : csel  x0, x0, x0, #0x0a           ; 00 a0 80 9a  
    86 : csel  x0, x0, x0, #0x04           ; 00 40 80 9a  
    87 : csel  x0, x0, x0, #0x05           ; 00 50 80 9a  
    88 : csel  x15, x0, x0, #0             ; 0f 00 80 9a  
    89 : csel  x0, x15, x0, #0             ; e0 01 80 9a  
    90 : csel  x0, x0, x15, #0             ; 00 00 8f 9a  
    91 : cinc  x0, x0, #0x01               ; 00 14 80 9a  
    92 : cinc  x0, x0, #0                  ; 00 04 80 9a  
    93 : cinc  x0, x0, #0x0a               ; 00 a4 80 9a  
    94 : cinc  x0, x0, #0x0d               ; 00 d4 80 9a  
    95 : cinc  x0, x0, #0x0c               ; 00 c4 80 9a  
    96 : cinc  x0, x0, #0x0b               ; 00 b4 80 9a  
    97 : cinc  x0, x0, #0x05               ; 00 54 80 9a  
    98 : cinc  x0, x0, #0x04               ; 00 44 80 9a  
    99 : cinc  x15, x0, #0x01              ; 0f 14 80 9a  
   100 : cinc  x0, x15, #0x01              ; e0 15 8f 9a  
   101 : cneg  x0, x0, #0x01               ; 00 14 80 da  
   102 : cneg  x0, x0, #0                  ; 00 04 80 da  
   103 : cneg  x0, x0, #0x0a               ; 00 a4 80 da  
   104 : cneg  x0, x0, #0x0d               ; 00 d4 80 da  
   105 : cneg  x0, x0, #0x0c               ; 00 c4 80 da  
   106 : cneg  x0, x0, #0x0b               ; 00 b4 80 da  
   107 : cneg  x0, x0, #0x05               ; 00 54 80 da  
   108 : cneg  x0, x0, #0x04               ; 00 44 80 da  
   109 : cneg  x15, x0, #0x01              ; 0f 14 80 da  
   110 : cneg  x0, x15, #0x01              ; e0 15 8f da  
   111 : asr   x0, x0, #0x3f               ; 00 fc 7f 93  
   112 : asr   x15, x0, #0x3f              ; 0f fc 7f 93  
   113 : asr   x0, x15, #0x3f              ; e0 fd 7f 93  
   114 : asr   x0, x0, x0                  ; 00 28 c0 9a  
   115 : asr   x15, x0, x0                 ; 0f 28 c0 9a  
   116 : asr   x0, x15, x0                 ; e0 29 c0 9a  
   117 : asr   x0, x0, x15                 ; 00 28 cf 9a  
   118 : lsr   x0, x0, #0x3f               ; 00 fc 7f d3  
   119 : lsr   x15, x0, #0x3f              ; 0f fc 7f d3  
   120 : lsr   x0, x15, #0x3f              ; e0 fd 7f d3  
   121 : lsr   x0, x0, x0                  ; 00 24 c0 9a  
   122 : lsr   x15, x0, x0                 ; 0f 24 c0 9a  
   123 : lsr   x0, x15, x0                 ; e0 25 c0 9a  
   124 : lsr   x0, x0, x15                 ; 00 24 cf 9a  
   125 : lsl   x0, x0, #0x01, #0           ; 00 00 41 d3  
   126 : lsl   x0, x0, #0x21, #0x20        ; 00 80 61 d3  
   127 : lsl   x0, x0, #0x31, #0x30        ; 00 c0 71 d3  
   128 : lsl   x0, x0, #0x39, #0x38        ; 00 e0 79 d3  
   129 : lsl   x0, x0, #0x3f, #0x3e        ; 00 f8 7f d3  
   130 : lsl   x0, x0, #0, #0x3f           ; 00 fc 40 d3  
   131 : lsl   x15, x0, #0x01, #0          ; 0f 00 41 d3  
   132 : lsl   x0, x15, #0x01, #0          ; e0 01 41 d3  
   133 : lsl   x0, x0, x0                  ; 00 20 c0 9a  
   134 : lsl   x15, x0, x0                 ; 0f 20 c0 9a  
   135 : lsl   x0, x15, x0                 ; e0 21 c0 9a  
   136 : lsl   x0, x0, x15                 ; 00 20 cf 9a  
   137 : and   x0, x0, x0                  ; 00 00 00 8a  
   138 : and   x15, x0, x0                 ; 0f 00 00 8a  
   139 : and   x0, x15, x0                 ; e0 01 00 8a  
   140 : and   x0, x0, x15                 ; 00 00 0f 8a  
   141 : and   x0, x0, #0x01, #0, #0x07    ; 00 1c 40 92  
   142 : and   x0, x0, #0x01, #0x38, #0x07 ; 00 1c 78 92  
   143 : and   x0, x0, #0x01, #0x08, #0x07 ; 00 1c 48 92  
   144 : and   x15, x0, #0x01, #0, #0x07   ; 0f 1c 40 92  
   145 : and   x0, x15, #0x01, #0, #0x07   ; e0 1d 40 92  
   146 : orr   x0, x0, x0                  ; 00 00 00 aa  
   147 : orr   x15, x0, x0                 ; 0f 00 00 aa  
   148 : orr   x0, x15, x0                 ; e0 01 00 aa  
   149 : orr   x0, x0, x15                 ; 00 00 0f aa  
   150 : orr   x0, x0, #0x01, #0, #0x07    ; 00 1c 40 b2  
   151 : orr   x0, x0, #0x01, #0x38, #0x07 ; 00 1c 78 b2  
   152 : orr   x0, x0, #0x01, #0x08, #0x07 ; 00 1c 48 b2  
   153 : orr   x15, x0, #0x01, #0, #0x07   ; 0f 1c 40 b2  
   154 : orr   x0, x15, #0x01, #0, #0x07   ; e0 1d 40 b2  
   155 : eor   x0, x0, x0                  ; 00 00 00 ca  
   156 : eor   x15, x0, x0                 ; 0f 00 00 ca  
   157 : eor   x0, x15, x0                 ; e0 01 00 ca  
   158 : eor   x0, x0, x15                 ; 00 00 0f ca  
   159 : eor   x0, x0, #0x01, #0, #0x07    ; 00 1c 40 d2  
   160 : eor   x0, x0, #0x01, #0x38, #0x07 ; 00 1c 78 d2  
   161 : eor   x0, x0, #0x01, #0x08, #0x07 ; 00 1c 48 d2  
   162 : eor   x15, x0, #0x01, #0, #0x07   ; 0f 1c 40 d2  
   163 : eor   x0, x15, #0x01, #0, #0x07   ; e0 1d 40 d2  
   164 : mvn   x0, x0                      ; e0 03 20 aa  
   165 : mvn   x15, x0                     ; ef 03 20 aa  
   166 : mvn   x0, x15                     ; e0 03 2f aa  
   167 : neg   x0, x0                      ; e0 03 00 cb  
   168 : neg   x15, x0                     ; ef 03 00 cb  
   169 : neg   x0, x15                     ; e0 03 0f cb  
