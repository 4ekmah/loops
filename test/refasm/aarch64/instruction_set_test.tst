instruction_set_test()
     0 : movn   x0, #0                        ; 00 00 80 92  
     1 : movn   x15, #0                       ; 0f 00 80 92  
     2 : movn   x0, #0xffff                   ; e0 ff 9f 92  
     3 : str    #0x01, x0, x0, #0x20          ; 00 80 00 f9  
     4 : str    #0x01, x0, x0, #0x20          ; 00 80 00 f9  
     5 : str    #0x01, x0, x0, x0             ; 00 68 20 f8  
     6 : str    #0x01, x0, x0, x0             ; 00 68 20 f8  
     7 : str    #0, w0, x0, #0x40             ; 00 00 01 b9  
     8 : str    #0, w0, x0, #0x40             ; 00 00 01 b9  
     9 : str    #0, w0, x0, x0                ; 00 68 20 b8  
    10 : str    #0, w0, x0, x0                ; 00 68 20 b8  
    11 : strh   w0, x0, #0x80                 ; 00 00 02 79  
    12 : strh   w0, x0, #0x80                 ; 00 00 02 79  
    13 : strh   w0, x0, x0                    ; 00 68 20 78  
    14 : strh   w0, x0, x0                    ; 00 68 20 78  
    15 : strb   w0, x0, #0x100                ; 00 00 04 39  
    16 : strb   w0, x0, #0x100                ; 00 00 04 39  
    17 : strb   w0, x0, x0                    ; 00 68 20 38  
    18 : strb   w0, x0, x0                    ; 00 68 20 38  
    19 : ldr    x0, [x0], #0                  ; 00 00 40 f9  
    20 : ldr    x0, [x0], #0x20               ; 00 80 40 f9  
    21 : ldr    x15, [x0], #0x20              ; 0f 80 40 f9  
    22 : ldr    x0, [x15], #0x20              ; e0 81 40 f9  
    23 : ldr    x0, [x0], x0                  ; 00 68 60 f8  
    24 : ldr    x15, [x0], x0                 ; 0f 68 60 f8  
    25 : ldr    x0, [x15], x0                 ; e0 69 60 f8  
    26 : ldr    x0, [x0], x15                 ; 00 68 6f f8  
    27 : ldr    x0, [x0], #0                  ; 00 00 40 f9  
    28 : ldr    x0, [x0], #0x20               ; 00 80 40 f9  
    29 : ldr    x15, [x0], #0x20              ; 0f 80 40 f9  
    30 : ldr    x0, [x15], #0x20              ; e0 81 40 f9  
    31 : ldr    x0, [x0], x0                  ; 00 68 60 f8  
    32 : ldr    x15, [x0], x0                 ; 0f 68 60 f8  
    33 : ldr    x0, [x15], x0                 ; e0 69 60 f8  
    34 : ldr    x0, [x0], x15                 ; 00 68 6f f8  
    35 : ldr    x0, [x0], #0                  ; 00 00 40 b9  
    36 : ldr    x0, [x0], #0x40               ; 00 00 41 b9  
    37 : ldr    x15, [x0], #0x40              ; 0f 00 41 b9  
    38 : ldr    x0, [x15], #0x40              ; e0 01 41 b9  
    39 : ldr    x0, [x0], x0                  ; 00 68 60 b8  
    40 : ldr    x15, [x0], x0                 ; 0f 68 60 b8  
    41 : ldr    x0, [x15], x0                 ; e0 69 60 b8  
    42 : ldr    x0, [x0], x15                 ; 00 68 6f b8  
    43 : ldrsw  x0, [x0], #0                  ; 00 00 80 b9  
    44 : ldrsw  x0, [x0], #0x40               ; 00 00 81 b9  
    45 : ldrsw  x15, [x0], #0x40              ; 0f 00 81 b9  
    46 : ldrsw  x0, [x15], #0x40              ; e0 01 81 b9  
    47 : ldrsw  x0, [x0], x0                  ; 00 68 a0 b8  
    48 : ldrsw  x15, [x0], x0                 ; 0f 68 a0 b8  
    49 : ldrsw  x0, [x15], x0                 ; e0 69 a0 b8  
    50 : ldrsw  x0, [x0], x15                 ; 00 68 af b8  
    51 : ldrh   x0, [x0], #0                  ; 00 00 40 79  
    52 : ldrh   x0, [x0], #0x80               ; 00 00 42 79  
    53 : ldrh   x15, [x0], #0x80              ; 0f 00 42 79  
    54 : ldrh   x0, [x15], #0x80              ; e0 01 42 79  
    55 : ldrh   x0, [x0], x0                  ; 00 68 60 78  
    56 : ldrh   x15, [x0], x0                 ; 0f 68 60 78  
    57 : ldrh   x0, [x15], x0                 ; e0 69 60 78  
    58 : ldrh   x0, [x0], x15                 ; 00 68 6f 78  
    59 : ldrsh  x0, [x0], #0                  ; 00 00 80 79  
    60 : ldrsh  x0, [x0], #0x80               ; 00 00 82 79  
    61 : ldrsh  x15, [x0], #0x80              ; 0f 00 82 79  
    62 : ldrsh  x0, [x15], #0x80              ; e0 01 82 79  
    63 : ldrsh  x0, [x0], x0                  ; 00 68 a0 78  
    64 : ldrsh  x15, [x0], x0                 ; 0f 68 a0 78  
    65 : ldrsh  x0, [x15], x0                 ; e0 69 a0 78  
    66 : ldrsh  x0, [x0], x15                 ; 00 68 af 78  
    67 : ldrb   x0, [x0], #0                  ; 00 00 40 39  
    68 : ldrb   x0, [x0], #0x100              ; 00 00 44 39  
    69 : ldrb   x15, [x0], #0x100             ; 0f 00 44 39  
    70 : ldrb   x0, [x15], #0x100             ; e0 01 44 39  
    71 : ldrb   x0, [x0], x0                  ; 00 68 60 38  
    72 : ldrb   x15, [x0], x0                 ; 0f 68 60 38  
    73 : ldrb   x0, [x15], x0                 ; e0 69 60 38  
    74 : ldrb   x0, [x0], x15                 ; 00 68 6f 38  
    75 : ldrsb  x0, [x0], #0                  ; 00 00 80 39  
    76 : ldrsb  x0, [x0], #0x100              ; 00 00 84 39  
    77 : ldrsb  x15, [x0], #0x100             ; 0f 00 84 39  
    78 : ldrsb  x0, [x15], #0x100             ; e0 01 84 39  
    79 : ldrsb  x0, [x0], x0                  ; 00 68 a0 38  
    80 : ldrsb  x15, [x0], x0                 ; 0f 68 a0 38  
    81 : ldrsb  x0, [x15], x0                 ; e0 69 a0 38  
    82 : ldrsb  x0, [x0], x15                 ; 00 68 af 38  
    83 : csel   x0, x0, x0, #0                ; 00 00 80 9a  
    84 : csel   x0, x0, x0, #0x01             ; 00 10 80 9a  
    85 : csel   x0, x0, x0, #0x0b             ; 00 b0 80 9a  
    86 : csel   x0, x0, x0, #0x0c             ; 00 c0 80 9a  
    87 : csel   x0, x0, x0, #0x0d             ; 00 d0 80 9a  
    88 : csel   x0, x0, x0, #0x0a             ; 00 a0 80 9a  
    89 : csel   x0, x0, x0, #0x04             ; 00 40 80 9a  
    90 : csel   x0, x0, x0, #0x05             ; 00 50 80 9a  
    91 : csel   x15, x0, x0, #0               ; 0f 00 80 9a  
    92 : csel   x0, x15, x0, #0               ; e0 01 80 9a  
    93 : csel   x0, x0, x15, #0               ; 00 00 8f 9a  
    94 : cinc   x0, x0, #0x01                 ; 00 14 80 9a  
    95 : cinc   x0, x0, #0                    ; 00 04 80 9a  
    96 : cinc   x0, x0, #0x0a                 ; 00 a4 80 9a  
    97 : cinc   x0, x0, #0x0d                 ; 00 d4 80 9a  
    98 : cinc   x0, x0, #0x0c                 ; 00 c4 80 9a  
    99 : cinc   x0, x0, #0x0b                 ; 00 b4 80 9a  
   100 : cinc   x0, x0, #0x05                 ; 00 54 80 9a  
   101 : cinc   x0, x0, #0x04                 ; 00 44 80 9a  
   102 : cinc   x15, x0, #0x01                ; 0f 14 80 9a  
   103 : cinc   x0, x15, #0x01                ; e0 15 8f 9a  
   104 : cneg   x0, x0, #0x01                 ; 00 14 80 da  
   105 : cneg   x0, x0, #0                    ; 00 04 80 da  
   106 : cneg   x0, x0, #0x0a                 ; 00 a4 80 da  
   107 : cneg   x0, x0, #0x0d                 ; 00 d4 80 da  
   108 : cneg   x0, x0, #0x0c                 ; 00 c4 80 da  
   109 : cneg   x0, x0, #0x0b                 ; 00 b4 80 da  
   110 : cneg   x0, x0, #0x05                 ; 00 54 80 da  
   111 : cneg   x0, x0, #0x04                 ; 00 44 80 da  
   112 : cneg   x15, x0, #0x01                ; 0f 14 80 da  
   113 : cneg   x0, x15, #0x01                ; e0 15 8f da  
   114 : asr    x0, x0, #0x3f                 ; 00 fc 7f 93  
   115 : asr    x15, x0, #0x3f                ; 0f fc 7f 93  
   116 : asr    x0, x15, #0x3f                ; e0 fd 7f 93  
   117 : asr    x0, x0, x0                    ; 00 28 c0 9a  
   118 : asr    x15, x0, x0                   ; 0f 28 c0 9a  
   119 : asr    x0, x15, x0                   ; e0 29 c0 9a  
   120 : asr    x0, x0, x15                   ; 00 28 cf 9a  
   121 : lsr    x0, x0, #0x3f                 ; 00 fc 7f d3  
   122 : lsr    x15, x0, #0x3f                ; 0f fc 7f d3  
   123 : lsr    x0, x15, #0x3f                ; e0 fd 7f d3  
   124 : lsr    x0, x0, x0                    ; 00 24 c0 9a  
   125 : lsr    x15, x0, x0                   ; 0f 24 c0 9a  
   126 : lsr    x0, x15, x0                   ; e0 25 c0 9a  
   127 : lsr    x0, x0, x15                   ; 00 24 cf 9a  
   128 : lsl    x0, x0, #0x01, #0             ; 00 00 41 d3  
   129 : lsl    x0, x0, #0x21, #0x20          ; 00 80 61 d3  
   130 : lsl    x0, x0, #0x31, #0x30          ; 00 c0 71 d3  
   131 : lsl    x0, x0, #0x39, #0x38          ; 00 e0 79 d3  
   132 : lsl    x0, x0, #0x3f, #0x3e          ; 00 f8 7f d3  
   133 : lsl    x0, x0, #0, #0x3f             ; 00 fc 40 d3  
   134 : lsl    x15, x0, #0x01, #0            ; 0f 00 41 d3  
   135 : lsl    x0, x15, #0x01, #0            ; e0 01 41 d3  
   136 : lsl    x0, x0, x0                    ; 00 20 c0 9a  
   137 : lsl    x15, x0, x0                   ; 0f 20 c0 9a  
   138 : lsl    x0, x15, x0                   ; e0 21 c0 9a  
   139 : lsl    x0, x0, x15                   ; 00 20 cf 9a  
   140 : and    x0, x0, x0                    ; 00 00 00 8a  
   141 : and    x15, x0, x0                   ; 0f 00 00 8a  
   142 : and    x0, x15, x0                   ; e0 01 00 8a  
   143 : and    x0, x0, x15                   ; 00 00 0f 8a  
   144 : and    x0, x0, #0x01, #0, #0x07      ; 00 1c 40 92  
   145 : and    x0, x0, #0x01, #0x38, #0x07   ; 00 1c 78 92  
   146 : and    x0, x0, #0x01, #0x08, #0x07   ; 00 1c 48 92  
   147 : and    x15, x0, #0x01, #0, #0x07     ; 0f 1c 40 92  
   148 : and    x0, x15, #0x01, #0, #0x07     ; e0 1d 40 92  
   149 : orr    x0, x0, x0                    ; 00 00 00 aa  
   150 : orr    x15, x0, x0                   ; 0f 00 00 aa  
   151 : orr    x0, x15, x0                   ; e0 01 00 aa  
   152 : orr    x0, x0, x15                   ; 00 00 0f aa  
   153 : orr    x0, x0, #0x01, #0, #0x07      ; 00 1c 40 b2  
   154 : orr    x0, x0, #0x01, #0x38, #0x07   ; 00 1c 78 b2  
   155 : orr    x0, x0, #0x01, #0x08, #0x07   ; 00 1c 48 b2  
   156 : orr    x15, x0, #0x01, #0, #0x07     ; 0f 1c 40 b2  
   157 : orr    x0, x15, #0x01, #0, #0x07     ; e0 1d 40 b2  
   158 : eor    x0, x0, x0                    ; 00 00 00 ca  
   159 : eor    x15, x0, x0                   ; 0f 00 00 ca  
   160 : eor    x0, x15, x0                   ; e0 01 00 ca  
   161 : eor    x0, x0, x15                   ; 00 00 0f ca  
   162 : eor    x0, x0, #0x01, #0, #0x07      ; 00 1c 40 d2  
   163 : eor    x0, x0, #0x01, #0x38, #0x07   ; 00 1c 78 d2  
   164 : eor    x0, x0, #0x01, #0x08, #0x07   ; 00 1c 48 d2  
   165 : eor    x15, x0, #0x01, #0, #0x07     ; 0f 1c 40 d2  
   166 : eor    x0, x15, #0x01, #0, #0x07     ; e0 1d 40 d2  
   167 : mvn    x0, x0                        ; e0 03 20 aa  
   168 : mvn    x15, x0                       ; ef 03 20 aa  
   169 : mvn    x0, x15                       ; e0 03 2f aa  
   170 : neg    x0, x0                        ; e0 03 00 cb  
   171 : neg    x15, x0                       ; ef 03 00 cb  
   172 : neg    x0, x15                       ; e0 03 0f cb  
   173 : ldr    v0.2d, x0, #0                 ; 00 00 c0 3d  
   174 : ldr    v0.2d, x0, #0x801             ; 00 04 e0 3d  
   175 : ldr    v31.2d, x0, #0                ; 1f 00 c0 3d  
   176 : ldr    v0.2d, x15, #0                ; e0 01 c0 3d  
   177 : ldr    v0.2d, x0, x0                 ; 00 68 e0 3c  
   178 : ldr    v31.2d, x0, x0                ; 1f 68 e0 3c  
   179 : ldr    v0.2d, x15, x0                ; e0 69 e0 3c  
   180 : ldr    v0.2d, x0, x15                ; 00 68 ef 3c  
   181 : str    v0.2d, x0, #0                 ; 00 00 80 3d  
   182 : str    v0.2d, x0, #0x801             ; 00 04 a0 3d  
   183 : str    v31.2d, x0, #0                ; 1f 00 80 3d  
   184 : str    v0.2d, x15, #0                ; e0 01 80 3d  
   185 : str    v0.2d, x0, x0                 ; 00 68 a0 3c  
   186 : str    v31.2d, x0, x0                ; 1f 68 a0 3c  
   187 : str    v0.2d, x15, x0                ; e0 69 a0 3c  
   188 : str    v0.2d, x0, x15                ; 00 68 af 3c  
   189 : add    v0.16b, v0.16b, v0.16b        ; 00 84 20 4e  
   190 : add    v31.16b, v0.16b, v0.16b       ; 1f 84 20 4e  
   191 : add    v0.16b, v31.16b, v0.16b       ; e0 87 20 4e  
   192 : add    v0.16b, v0.16b, v31.16b       ; 00 84 3f 4e  
   193 : add    v0.8h, v0.8h, v0.8h           ; 00 84 60 4e  
   194 : add    v31.8h, v0.8h, v0.8h          ; 1f 84 60 4e  
   195 : add    v0.8h, v31.8h, v0.8h          ; e0 87 60 4e  
   196 : add    v0.8h, v0.8h, v31.8h          ; 00 84 7f 4e  
   197 : add    v0.4s, v0.4s, v0.4s           ; 00 84 a0 4e  
   198 : add    v31.4s, v0.4s, v0.4s          ; 1f 84 a0 4e  
   199 : add    v0.4s, v31.4s, v0.4s          ; e0 87 a0 4e  
   200 : add    v0.4s, v0.4s, v31.4s          ; 00 84 bf 4e  
   201 : add    v0.2d, v0.2d, v0.2d           ; 00 84 e0 4e  
   202 : add    v31.2d, v0.2d, v0.2d          ; 1f 84 e0 4e  
   203 : add    v0.2d, v31.2d, v0.2d          ; e0 87 e0 4e  
   204 : add    v0.2d, v0.2d, v31.2d          ; 00 84 ff 4e  
   205 : fadd   v0.8h, v0.8h, v0.8h           ; 00 14 40 4e  
   206 : fadd   v31.8h, v0.8h, v0.8h          ; 1f 14 40 4e  
   207 : fadd   v0.8h, v31.8h, v0.8h          ; e0 17 40 4e  
   208 : fadd   v0.8h, v0.8h, v31.8h          ; 00 14 5f 4e  
   209 : fadd   v0.4s, v0.4s, v0.4s           ; 00 d4 20 4e  
   210 : fadd   v31.4s, v0.4s, v0.4s          ; 1f d4 20 4e  
   211 : fadd   v0.4s, v31.4s, v0.4s          ; e0 d7 20 4e  
   212 : fadd   v0.4s, v0.4s, v31.4s          ; 00 d4 3f 4e  
   213 : fadd   v0.2d, v0.2d, v0.2d           ; 00 d4 60 4e  
   214 : fadd   v31.2d, v0.2d, v0.2d          ; 1f d4 60 4e  
   215 : fadd   v0.2d, v31.2d, v0.2d          ; e0 d7 60 4e  
   216 : fadd   v0.2d, v0.2d, v31.2d          ; 00 d4 7f 4e  
   217 : sub    v0.16b, v0.16b, v0.16b        ; 00 84 20 6e  
   218 : sub    v31.16b, v0.16b, v0.16b       ; 1f 84 20 6e  
   219 : sub    v0.16b, v31.16b, v0.16b       ; e0 87 20 6e  
   220 : sub    v0.16b, v0.16b, v31.16b       ; 00 84 3f 6e  
   221 : sub    v0.8h, v0.8h, v0.8h           ; 00 84 60 6e  
   222 : sub    v31.8h, v0.8h, v0.8h          ; 1f 84 60 6e  
   223 : sub    v0.8h, v31.8h, v0.8h          ; e0 87 60 6e  
   224 : sub    v0.8h, v0.8h, v31.8h          ; 00 84 7f 6e  
   225 : sub    v0.4s, v0.4s, v0.4s           ; 00 84 a0 6e  
   226 : sub    v31.4s, v0.4s, v0.4s          ; 1f 84 a0 6e  
   227 : sub    v0.4s, v31.4s, v0.4s          ; e0 87 a0 6e  
   228 : sub    v0.4s, v0.4s, v31.4s          ; 00 84 bf 6e  
   229 : sub    v0.2d, v0.2d, v0.2d           ; 00 84 e0 6e  
   230 : sub    v31.2d, v0.2d, v0.2d          ; 1f 84 e0 6e  
   231 : sub    v0.2d, v31.2d, v0.2d          ; e0 87 e0 6e  
   232 : sub    v0.2d, v0.2d, v31.2d          ; 00 84 ff 6e  
   233 : fsub   v0.8h, v0.8h, v0.8h           ; 00 14 c0 4e  
   234 : fsub   v31.8h, v0.8h, v0.8h          ; 1f 14 c0 4e  
   235 : fsub   v0.8h, v31.8h, v0.8h          ; e0 17 c0 4e  
   236 : fsub   v0.8h, v0.8h, v31.8h          ; 00 14 df 4e  
   237 : fsub   v0.4s, v0.4s, v0.4s           ; 00 d4 a0 4e  
   238 : fsub   v31.4s, v0.4s, v0.4s          ; 1f d4 a0 4e  
   239 : fsub   v0.4s, v31.4s, v0.4s          ; e0 d7 a0 4e  
   240 : fsub   v0.4s, v0.4s, v31.4s          ; 00 d4 bf 4e  
   241 : fsub   v0.2d, v0.2d, v0.2d           ; 00 d4 e0 4e  
   242 : fsub   v31.2d, v0.2d, v0.2d          ; 1f d4 e0 4e  
   243 : fsub   v0.2d, v31.2d, v0.2d          ; e0 d7 e0 4e  
   244 : fsub   v0.2d, v0.2d, v31.2d          ; 00 d4 ff 4e  
   245 : mul    v0.16b, v0.16b, v0.16b        ; 00 9c 20 4e  
   246 : mul    v31.16b, v0.16b, v0.16b       ; 1f 9c 20 4e  
   247 : mul    v0.16b, v31.16b, v0.16b       ; e0 9f 20 4e  
   248 : mul    v0.16b, v0.16b, v31.16b       ; 00 9c 3f 4e  
   249 : mul    v0.8h, v0.8h, v0.8h           ; 00 9c 60 4e  
   250 : mul    v31.8h, v0.8h, v0.8h          ; 1f 9c 60 4e  
   251 : mul    v0.8h, v31.8h, v0.8h          ; e0 9f 60 4e  
   252 : mul    v0.8h, v0.8h, v31.8h          ; 00 9c 7f 4e  
   253 : mul    v0.4s, v0.4s, v0.4s           ; 00 9c a0 4e  
   254 : mul    v31.4s, v0.4s, v0.4s          ; 1f 9c a0 4e  
   255 : mul    v0.4s, v31.4s, v0.4s          ; e0 9f a0 4e  
   256 : mul    v0.4s, v0.4s, v31.4s          ; 00 9c bf 4e  
   257 : fmul   v0.8h, v0.8h, v0.8h           ; 00 1c 40 6e  
   258 : fmul   v31.8h, v0.8h, v0.8h          ; 1f 1c 40 6e  
   259 : fmul   v0.8h, v31.8h, v0.8h          ; e0 1f 40 6e  
   260 : fmul   v0.8h, v0.8h, v31.8h          ; 00 1c 5f 6e  
   261 : fmul   v0.4s, v0.4s, v0.4s           ; 00 dc 20 6e  
   262 : fmul   v31.4s, v0.4s, v0.4s          ; 1f dc 20 6e  
   263 : fmul   v0.4s, v31.4s, v0.4s          ; e0 df 20 6e  
   264 : fmul   v0.4s, v0.4s, v31.4s          ; 00 dc 3f 6e  
   265 : fmul   v0.2d, v0.2d, v0.2d           ; 00 dc 60 6e  
   266 : fmul   v31.2d, v0.2d, v0.2d          ; 1f dc 60 6e  
   267 : fmul   v0.2d, v31.2d, v0.2d          ; e0 df 60 6e  
   268 : fmul   v0.2d, v0.2d, v31.2d          ; 00 dc 7f 6e  
   269 : fdiv   v0.4s, v0.4s, v0.4s           ; 00 fc 20 6e  
   270 : fdiv   v31.4s, v0.4s, v0.4s          ; 1f fc 20 6e  
   271 : fdiv   v0.4s, v31.4s, v0.4s          ; e0 ff 20 6e  
   272 : fdiv   v0.4s, v0.4s, v31.4s          ; 00 fc 3f 6e  
   273 : fdiv   v0.2d, v0.2d, v0.2d           ; 00 fc 60 6e  
   274 : fdiv   v31.2d, v0.2d, v0.2d          ; 1f fc 60 6e  
   275 : fdiv   v0.2d, v31.2d, v0.2d          ; e0 ff 60 6e  
   276 : fdiv   v0.2d, v0.2d, v31.2d          ; 00 fc 7f 6e  
   277 : fdiv   v0.8h, v0.8h, v0.8h           ; 00 3c 40 6e  
   278 : fdiv   v31.8h, v0.8h, v0.8h          ; 1f 3c 40 6e  
   279 : fdiv   v0.8h, v31.8h, v0.8h          ; e0 3f 40 6e  
   280 : fdiv   v0.8h, v0.8h, v31.8h          ; 00 3c 5f 6e  
   281 : shl    v0.16b, v0.16b, #0x01, #0x01  ; 00 54 09 4f  
   282 : shl    v31.16b, v0.16b, #0x01, #0x01 ; 1f 54 09 4f  
   283 : shl    v0.16b, v31.16b, #0x01, #0x01 ; e0 57 09 4f  
   284 : shl    v0.16b, v0.16b, #0x01, #0x07  ; 00 54 0f 4f  
   285 : shl    v0.8h, v0.8h, #0x02, #0x01    ; 00 54 11 4f  
   286 : shl    v31.8h, v0.8h, #0x02, #0x01   ; 1f 54 11 4f  
   287 : shl    v0.8h, v31.8h, #0x02, #0x01   ; e0 57 11 4f  
   288 : shl    v0.8h, v0.8h, #0x03, #0x07    ; 00 54 1f 4f  
   289 : shl    v0.4s, v0.4s, #0x04, #0x01    ; 00 54 21 4f  
   290 : shl    v31.4s, v0.4s, #0x04, #0x01   ; 1f 54 21 4f  
   291 : shl    v0.4s, v31.4s, #0x04, #0x01   ; e0 57 21 4f  
   292 : shl    v0.4s, v0.4s, #0x07, #0x07    ; 00 54 3f 4f  
   293 : shl    v0.2d, v0.2d, #0x08, #0x01    ; 00 54 41 4f  
   294 : shl    v31.2d, v0.2d, #0x08, #0x01   ; 1f 54 41 4f  
   295 : shl    v0.2d, v31.2d, #0x08, #0x01   ; e0 57 41 4f  
   296 : shl    v0.2d, v0.2d, #0x0f, #0x07    ; 00 54 7f 4f  
   297 : ushl   v0.16b, v0.16b, v0.16b        ; 00 44 20 6e  
   298 : ushl   v31.16b, v0.16b, v0.16b       ; 1f 44 20 6e  
   299 : ushl   v0.16b, v31.16b, v0.16b       ; e0 47 20 6e  
   300 : ushl   v0.16b, v0.16b, v31.16b       ; 00 44 3f 6e  
   301 : ushl   v0.8h, v0.8h, v0.8h           ; 00 44 60 6e  
   302 : ushl   v31.8h, v0.8h, v0.8h          ; 1f 44 60 6e  
   303 : ushl   v0.8h, v31.8h, v0.8h          ; e0 47 60 6e  
   304 : ushl   v0.8h, v0.8h, v31.8h          ; 00 44 7f 6e  
   305 : ushl   v0.4s, v0.4s, v0.4s           ; 00 44 a0 6e  
   306 : ushl   v31.4s, v0.4s, v0.4s          ; 1f 44 a0 6e  
   307 : ushl   v0.4s, v31.4s, v0.4s          ; e0 47 a0 6e  
   308 : ushl   v0.4s, v0.4s, v31.4s          ; 00 44 bf 6e  
   309 : ushl   v0.2d, v0.2d, v0.2d           ; 00 44 e0 6e  
   310 : ushl   v31.2d, v0.2d, v0.2d          ; 1f 44 e0 6e  
   311 : ushl   v0.2d, v31.2d, v0.2d          ; e0 47 e0 6e  
   312 : ushl   v0.2d, v0.2d, v31.2d          ; 00 44 ff 6e  
   313 : sshl   v0.16b, v0.16b, v0.16b        ; 00 44 20 4e  
   314 : sshl   v31.16b, v0.16b, v0.16b       ; 1f 44 20 4e  
   315 : sshl   v0.16b, v31.16b, v0.16b       ; e0 47 20 4e  
   316 : sshl   v0.16b, v0.16b, v31.16b       ; 00 44 3f 4e  
   317 : sshl   v0.8h, v0.8h, v0.8h           ; 00 44 60 4e  
   318 : sshl   v31.8h, v0.8h, v0.8h          ; 1f 44 60 4e  
   319 : sshl   v0.8h, v31.8h, v0.8h          ; e0 47 60 4e  
   320 : sshl   v0.8h, v0.8h, v31.8h          ; 00 44 7f 4e  
   321 : sshl   v0.4s, v0.4s, v0.4s           ; 00 44 a0 4e  
   322 : sshl   v31.4s, v0.4s, v0.4s          ; 1f 44 a0 4e  
   323 : sshl   v0.4s, v31.4s, v0.4s          ; e0 47 a0 4e  
   324 : sshl   v0.4s, v0.4s, v31.4s          ; 00 44 bf 4e  
   325 : sshl   v0.2d, v0.2d, v0.2d           ; 00 44 e0 4e  
   326 : sshl   v31.2d, v0.2d, v0.2d          ; 1f 44 e0 4e  
   327 : sshl   v0.2d, v31.2d, v0.2d          ; e0 47 e0 4e  
   328 : sshl   v0.2d, v0.2d, v31.2d          ; 00 44 ff 4e  
   329 : ushr   v0.16b, v0.16b, #0x01, #0x07  ; 00 04 0f 6f  
   330 : ushr   v31.16b, v0.16b, #0x01, #0x07 ; 1f 04 0f 6f  
   331 : ushr   v0.16b, v31.16b, #0x01, #0x07 ; e0 07 0f 6f  
   332 : ushr   v0.16b, v0.16b, #0x01, #0x01  ; 00 04 09 6f  
   333 : ushr   v0.8h, v0.8h, #0x03, #0x07    ; 00 04 1f 6f  
   334 : ushr   v31.8h, v0.8h, #0x03, #0x07   ; 1f 04 1f 6f  
   335 : ushr   v0.8h, v31.8h, #0x03, #0x07   ; e0 07 1f 6f  
   336 : ushr   v0.8h, v0.8h, #0x02, #0x01    ; 00 04 11 6f  
   337 : ushr   v0.4s, v0.4s, #0x07, #0x07    ; 00 04 3f 6f  
   338 : ushr   v31.4s, v0.4s, #0x07, #0x07   ; 1f 04 3f 6f  
   339 : ushr   v0.4s, v31.4s, #0x07, #0x07   ; e0 07 3f 6f  
   340 : ushr   v0.4s, v0.4s, #0x04, #0x01    ; 00 04 21 6f  
   341 : ushr   v0.2d, v0.2d, #0x0f, #0x07    ; 00 04 7f 6f  
   342 : ushr   v31.2d, v0.2d, #0x0f, #0x07   ; 1f 04 7f 6f  
   343 : ushr   v0.2d, v31.2d, #0x0f, #0x07   ; e0 07 7f 6f  
   344 : ushr   v0.2d, v0.2d, #0x08, #0x01    ; 00 04 41 6f  
   345 : sshr   v0.16b, v0.16b, #0x01, #0x07  ; 00 04 0f 4f  
   346 : sshr   v31.16b, v0.16b, #0x01, #0x07 ; 1f 04 0f 4f  
   347 : sshr   v0.16b, v31.16b, #0x01, #0x07 ; e0 07 0f 4f  
   348 : sshr   v0.16b, v0.16b, #0x01, #0x01  ; 00 04 09 4f  
   349 : sshr   v0.8h, v0.8h, #0x03, #0x07    ; 00 04 1f 4f  
   350 : sshr   v31.8h, v0.8h, #0x03, #0x07   ; 1f 04 1f 4f  
   351 : sshr   v0.8h, v31.8h, #0x03, #0x07   ; e0 07 1f 4f  
   352 : sshr   v0.8h, v0.8h, #0x02, #0x01    ; 00 04 11 4f  
   353 : sshr   v0.4s, v0.4s, #0x07, #0x07    ; 00 04 3f 4f  
   354 : sshr   v31.4s, v0.4s, #0x07, #0x07   ; 1f 04 3f 4f  
   355 : sshr   v0.4s, v31.4s, #0x07, #0x07   ; e0 07 3f 4f  
   356 : sshr   v0.4s, v0.4s, #0x04, #0x01    ; 00 04 21 4f  
   357 : sshr   v0.2d, v0.2d, #0x0f, #0x07    ; 00 04 7f 4f  
   358 : sshr   v31.2d, v0.2d, #0x0f, #0x07   ; 1f 04 7f 4f  
   359 : sshr   v0.2d, v31.2d, #0x0f, #0x07   ; e0 07 7f 4f  
   360 : sshr   v0.2d, v0.2d, #0x08, #0x01    ; 00 04 41 4f  
   361 : and    v0.16b, v0.16b, v0.16b        ; 00 1c 20 4e  
   362 : and    v31.16b, v0.16b, v0.16b       ; 1f 1c 20 4e  
   363 : and    v0.16b, v31.16b, v0.16b       ; e0 1f 20 4e  
   364 : and    v0.16b, v0.16b, v31.16b       ; 00 1c 3f 4e  
   365 : orr    v0.16b, v0.16b, v0.16b        ; 00 1c a0 4e  
   366 : orr    v31.16b, v0.16b, v0.16b       ; 1f 1c a0 4e  
   367 : orr    v0.16b, v31.16b, v0.16b       ; e0 1f a0 4e  
   368 : orr    v0.16b, v0.16b, v31.16b       ; 00 1c bf 4e  
   369 : eor    v0.16b, v0.16b, v0.16b        ; 00 1c 20 6e  
   370 : eor    v31.16b, v0.16b, v0.16b       ; 1f 1c 20 6e  
   371 : eor    v0.16b, v31.16b, v0.16b       ; e0 1f 20 6e  
   372 : eor    v0.16b, v0.16b, v31.16b       ; 00 1c 3f 6e  
   373 : mvn    v0.16b, v0.16b                ; 00 58 20 6e  
   374 : mvn    v31.16b, v0.16b               ; 1f 58 20 6e  
   375 : mvn    v0.16b, v31.16b               ; e0 5b 20 6e  
   376 : movi   v0.16b, #0, #0                ; 00 e4 00 4f  
   377 : movi   v31.16b, #0, #0               ; 1f e4 00 4f  
   378 : movi   v0.16b, #0x1f, #0x07          ; e0 e7 07 4f  
   379 : movi   v0.8h, #0, #0                 ; 00 84 00 4f  
   380 : movi   v31.8h, #0, #0                ; 1f 84 00 4f  
   381 : movi   v0.8h, #0x1f, #0x07           ; e0 87 07 4f  
   382 : movi   v0.4s, #0, #0                 ; 00 04 00 4f  
   383 : movi   v31.4s, #0, #0                ; 1f 04 00 4f  
   384 : movi   v0.4s, #0x1f, #0x07           ; e0 07 07 4f  
   385 : mvni   v0.8h, #0, #0                 ; 00 84 00 6f  
   386 : mvni   v31.8h, #0, #0                ; 1f 84 00 6f  
   387 : mvni   v0.8h, #0x1f, #0x07           ; e0 87 07 6f  
   388 : mvni   v0.4s, #0, #0                 ; 00 04 00 6f  
   389 : mvni   v31.4s, #0, #0                ; 1f 04 00 6f  
   390 : mvni   v0.4s, #0x1f, #0x07           ; e0 07 07 6f  
   391 : fmin   v0.8h, v0.8h, v0.8h           ; 00 34 c0 4e  
   392 : fmin   v31.8h, v0.8h, v0.8h          ; 1f 34 c0 4e  
   393 : fmin   v0.8h, v31.8h, v0.8h          ; e0 37 c0 4e  
   394 : fmin   v0.8h, v0.8h, v31.8h          ; 00 34 df 4e  
   395 : fmin   v0.4s, v0.4s, v0.4s           ; 00 f4 a0 4e  
   396 : fmin   v31.4s, v0.4s, v0.4s          ; 1f f4 a0 4e  
   397 : fmin   v0.4s, v31.4s, v0.4s          ; e0 f7 a0 4e  
   398 : fmin   v0.4s, v0.4s, v31.4s          ; 00 f4 bf 4e  
   399 : fmin   v0.2d, v0.2d, v0.2d           ; 00 f4 e0 4e  
   400 : fmin   v31.2d, v0.2d, v0.2d          ; 1f f4 e0 4e  
   401 : fmin   v0.2d, v31.2d, v0.2d          ; e0 f7 e0 4e  
   402 : fmin   v0.2d, v0.2d, v31.2d          ; 00 f4 ff 4e  
   403 : fmax   v0.8h, v0.8h, v0.8h           ; 00 34 40 4e  
   404 : fmax   v31.8h, v0.8h, v0.8h          ; 1f 34 40 4e  
   405 : fmax   v0.8h, v31.8h, v0.8h          ; e0 37 40 4e  
   406 : fmax   v0.8h, v0.8h, v31.8h          ; 00 34 5f 4e  
   407 : fmax   v0.4s, v0.4s, v0.4s           ; 00 f4 20 4e  
   408 : fmax   v31.4s, v0.4s, v0.4s          ; 1f f4 20 4e  
   409 : fmax   v0.4s, v31.4s, v0.4s          ; e0 f7 20 4e  
   410 : fmax   v0.4s, v0.4s, v31.4s          ; 00 f4 3f 4e  
   411 : fmax   v0.2d, v0.2d, v0.2d           ; 00 f4 60 4e  
   412 : fmax   v31.2d, v0.2d, v0.2d          ; 1f f4 60 4e  
   413 : fmax   v0.2d, v31.2d, v0.2d          ; e0 f7 60 4e  
   414 : fmax   v0.2d, v0.2d, v31.2d          ; 00 f4 7f 4e  
   415 : fmla   v0.8h, v0.8h, v0.8h           ; 00 0c 40 4e  
   416 : fmla   v31.8h, v0.8h, v0.8h          ; 1f 0c 40 4e  
   417 : fmla   v0.8h, v31.8h, v0.8h          ; e0 0f 40 4e  
   418 : fmla   v0.8h, v0.8h, v31.8h          ; 00 0c 5f 4e  
   419 : fmla   v0.4s, v0.4s, v0.4s           ; 00 cc 20 4e  
   420 : fmla   v31.4s, v0.4s, v0.4s          ; 1f cc 20 4e  
   421 : fmla   v0.4s, v31.4s, v0.4s          ; e0 cf 20 4e  
   422 : fmla   v0.4s, v0.4s, v31.4s          ; 00 cc 3f 4e  
   423 : fmla   v0.2d, v0.2d, v0.2d           ; 00 cc 60 4e  
   424 : fmla   v31.2d, v0.2d, v0.2d          ; 1f cc 60 4e  
   425 : fmla   v0.2d, v31.2d, v0.2d          ; e0 cf 60 4e  
   426 : fmla   v0.2d, v0.2d, v31.2d          ; 00 cc 7f 4e  
   427 : fcvtzs v0.8h, v0.8h                  ; 00 b8 f9 4e  
   428 : fcvtzs v31.8h, v0.8h                 ; 1f b8 f9 4e  
   429 : fcvtzs v0.8h, v31.8h                 ; e0 bb f9 4e  
   430 : fcvtzs v0.4s, v0.4s                  ; 00 b8 a1 4e  
   431 : fcvtzs v31.4s, v0.4s                 ; 1f b8 a1 4e  
   432 : fcvtzs v0.4s, v31.4s                 ; e0 bb a1 4e  
   433 : fcvtzs v0.2d, v0.2d                  ; 00 b8 e1 4e  
   434 : fcvtzs v31.2d, v0.2d                 ; 1f b8 e1 4e  
   435 : fcvtzs v0.2d, v31.2d                 ; e0 bb e1 4e  
   436 : fcvtzu v0.8h, v0.8h                  ; 00 b8 f9 6e  
   437 : fcvtzu v31.8h, v0.8h                 ; 1f b8 f9 6e  
   438 : fcvtzu v0.8h, v31.8h                 ; e0 bb f9 6e  
   439 : fcvtzu v0.4s, v0.4s                  ; 00 b8 a1 6e  
   440 : fcvtzu v31.4s, v0.4s                 ; 1f b8 a1 6e  
   441 : fcvtzu v0.4s, v31.4s                 ; e0 bb a1 6e  
   442 : fcvtzu v0.2d, v0.2d                  ; 00 b8 e1 6e  
   443 : fcvtzu v31.2d, v0.2d                 ; 1f b8 e1 6e  
   444 : fcvtzu v0.2d, v31.2d                 ; e0 bb e1 6e  
   445 : fcvtms v0.8h, v0.8h                  ; 00 b8 79 4e  
   446 : fcvtms v31.8h, v0.8h                 ; 1f b8 79 4e  
   447 : fcvtms v0.8h, v31.8h                 ; e0 bb 79 4e  
   448 : fcvtms v0.4s, v0.4s                  ; 00 b8 21 4e  
   449 : fcvtms v31.4s, v0.4s                 ; 1f b8 21 4e  
   450 : fcvtms v0.4s, v31.4s                 ; e0 bb 21 4e  
   451 : fcvtms v0.2d, v0.2d                  ; 00 b8 61 4e  
   452 : fcvtms v31.2d, v0.2d                 ; 1f b8 61 4e  
   453 : fcvtms v0.2d, v31.2d                 ; e0 bb 61 4e  
   454 : fcvtmu v0.8h, v0.8h                  ; 00 b8 79 6e  
   455 : fcvtmu v31.8h, v0.8h                 ; 1f b8 79 6e  
   456 : fcvtmu v0.8h, v31.8h                 ; e0 bb 79 6e  
   457 : fcvtmu v0.4s, v0.4s                  ; 00 b8 21 6e  
   458 : fcvtmu v31.4s, v0.4s                 ; 1f b8 21 6e  
   459 : fcvtmu v0.4s, v31.4s                 ; e0 bb 21 6e  
   460 : fcvtmu v0.2d, v0.2d                  ; 00 b8 61 6e  
   461 : fcvtmu v31.2d, v0.2d                 ; 1f b8 61 6e  
   462 : fcvtmu v0.2d, v31.2d                 ; e0 bb 61 6e  
   463 : scvtf  v0.8h, v0.8h                  ; 00 d8 79 4e  
   464 : scvtf  v31.8h, v0.8h                 ; 1f d8 79 4e  
   465 : scvtf  v0.8h, v31.8h                 ; e0 db 79 4e  
   466 : scvtf  v0.4s, v0.4s                  ; 00 d8 21 4e  
   467 : scvtf  v31.4s, v0.4s                 ; 1f d8 21 4e  
   468 : scvtf  v0.4s, v31.4s                 ; e0 db 21 4e  
   469 : scvtf  v0.2d, v0.2d                  ; 00 d8 61 4e  
   470 : scvtf  v31.2d, v0.2d                 ; 1f d8 61 4e  
   471 : scvtf  v0.2d, v31.2d                 ; e0 db 61 4e  
   472 : ucvtf  v0.8h, v0.8h                  ; 00 d8 79 6e  
   473 : ucvtf  v31.8h, v0.8h                 ; 1f d8 79 6e  
   474 : ucvtf  v0.8h, v31.8h                 ; e0 db 79 6e  
   475 : ucvtf  v0.4s, v0.4s                  ; 00 d8 21 6e  
   476 : ucvtf  v31.4s, v0.4s                 ; 1f d8 21 6e  
   477 : ucvtf  v0.4s, v31.4s                 ; e0 db 21 6e  
   478 : ucvtf  v0.2d, v0.2d                  ; 00 d8 61 6e  
   479 : ucvtf  v31.2d, v0.2d                 ; 1f d8 61 6e  
   480 : ucvtf  v0.2d, v31.2d                 ; e0 db 61 6e  
   481 : fcmgt  v0.8h, v0.8h, v0.8h           ; 00 24 c0 6e  
   482 : fcmgt  v31.8h, v0.8h, v0.8h          ; 1f 24 c0 6e  
   483 : fcmgt  v0.8h, v31.8h, v0.8h          ; e0 27 c0 6e  
   484 : fcmgt  v0.8h, v0.8h, v31.8h          ; 00 24 df 6e  
   485 : fcmgt  v0.4s, v0.4s, v0.4s           ; 00 e4 a0 6e  
   486 : fcmgt  v31.4s, v0.4s, v0.4s          ; 1f e4 a0 6e  
   487 : fcmgt  v0.4s, v31.4s, v0.4s          ; e0 e7 a0 6e  
   488 : fcmgt  v0.4s, v0.4s, v31.4s          ; 00 e4 bf 6e  
   489 : fcmgt  v0.2d, v0.2d, v0.2d           ; 00 e4 e0 6e  
   490 : fcmgt  v31.2d, v0.2d, v0.2d          ; 1f e4 e0 6e  
   491 : fcmgt  v0.2d, v31.2d, v0.2d          ; e0 e7 e0 6e  
   492 : fcmgt  v0.2d, v0.2d, v31.2d          ; 00 e4 ff 6e  
   493 : fcmge  v0.8h, v0.8h, v0.8h           ; 00 24 40 6e  
   494 : fcmge  v31.8h, v0.8h, v0.8h          ; 1f 24 40 6e  
   495 : fcmge  v0.8h, v31.8h, v0.8h          ; e0 27 40 6e  
   496 : fcmge  v0.8h, v0.8h, v31.8h          ; 00 24 5f 6e  
   497 : fcmge  v0.4s, v0.4s, v0.4s           ; 00 e4 20 6e  
   498 : fcmge  v31.4s, v0.4s, v0.4s          ; 1f e4 20 6e  
   499 : fcmge  v0.4s, v31.4s, v0.4s          ; e0 e7 20 6e  
   500 : fcmge  v0.4s, v0.4s, v31.4s          ; 00 e4 3f 6e  
   501 : fcmge  v0.2d, v0.2d, v0.2d           ; 00 e4 60 6e  
   502 : fcmge  v31.2d, v0.2d, v0.2d          ; 1f e4 60 6e  
   503 : fcmge  v0.2d, v31.2d, v0.2d          ; e0 e7 60 6e  
   504 : fcmge  v0.2d, v0.2d, v31.2d          ; 00 e4 7f 6e  
   505 : fcmeq  v0.8h, v0.8h, v0.8h           ; 00 24 40 4e  
   506 : fcmeq  v31.8h, v0.8h, v0.8h          ; 1f 24 40 4e  
   507 : fcmeq  v0.8h, v31.8h, v0.8h          ; e0 27 40 4e  
   508 : fcmeq  v0.8h, v0.8h, v31.8h          ; 00 24 5f 4e  
   509 : fcmeq  v0.4s, v0.4s, v0.4s           ; 00 e4 20 4e  
   510 : fcmeq  v31.4s, v0.4s, v0.4s          ; 1f e4 20 4e  
   511 : fcmeq  v0.4s, v31.4s, v0.4s          ; e0 e7 20 4e  
   512 : fcmeq  v0.4s, v0.4s, v31.4s          ; 00 e4 3f 4e  
   513 : fcmeq  v0.2d, v0.2d, v0.2d           ; 00 e4 60 4e  
   514 : fcmeq  v31.2d, v0.2d, v0.2d          ; 1f e4 60 4e  
   515 : fcmeq  v0.2d, v31.2d, v0.2d          ; e0 e7 60 4e  
   516 : fcmeq  v0.2d, v0.2d, v31.2d          ; 00 e4 7f 4e  
   517 : neg    v0.16b, v0.16b                ; 00 b8 20 6e  
   518 : neg    v31.16b, v0.16b               ; 1f b8 20 6e  
   519 : neg    v0.16b, v31.16b               ; e0 bb 20 6e  
   520 : neg    v0.8h, v0.8h                  ; 00 b8 60 6e  
   521 : neg    v31.8h, v0.8h                 ; 1f b8 60 6e  
   522 : neg    v0.8h, v31.8h                 ; e0 bb 60 6e  
   523 : neg    v0.4s, v0.4s                  ; 00 b8 a0 6e  
   524 : neg    v31.4s, v0.4s                 ; 1f b8 a0 6e  
   525 : neg    v0.4s, v31.4s                 ; e0 bb a0 6e  
   526 : neg    v0.2d, v0.2d                  ; 00 b8 e0 6e  
   527 : neg    v31.2d, v0.2d                 ; 1f b8 e0 6e  
   528 : neg    v0.2d, v31.2d                 ; e0 bb e0 6e  
   529 : fneg   v0.8h, v0.8h                  ; 00 f8 f8 6e  
   530 : fneg   v31.8h, v0.8h                 ; 1f f8 f8 6e  
   531 : fneg   v0.8h, v31.8h                 ; e0 fb f8 6e  
   532 : fneg   v0.4s, v0.4s                  ; 00 f8 a0 6e  
   533 : fneg   v31.4s, v0.4s                 ; 1f f8 a0 6e  
   534 : fneg   v0.4s, v31.4s                 ; e0 fb a0 6e  
   535 : fneg   v0.2d, v0.2d                  ; 00 f8 e0 6e  
   536 : fneg   v31.2d, v0.2d                 ; 1f f8 e0 6e  
   537 : fneg   v0.2d, v31.2d                 ; e0 fb e0 6e  
   538 : movk   x0, #0, #0x01                 ; 00 00 a0 f2  
   539 : movk   x15, #0, #0x01                ; 0f 00 a0 f2  
   540 : movk   x0, #0xffff, #0x01            ; e0 ff bf f2  
   541 : movk   x0, #0, #0x02                 ; 00 00 c0 f2  
   542 : movk   x15, #0, #0x02                ; 0f 00 c0 f2  
   543 : movk   x0, #0xffff, #0x02            ; e0 ff df f2  
   544 : movk   x0, #0, #0x03                 ; 00 00 e0 f2  
   545 : movk   x15, #0, #0x03                ; 0f 00 e0 f2  
   546 : movk   x0, #0xffff, #0x03            ; e0 ff ff f2  
   547 : dup    v0.16b, x0                    ; 00 0c 01 4e  
   548 : dup    v31.16b, x0                   ; 1f 0c 01 4e  
   549 : dup    v0.16b, x15                   ; e0 0d 01 4e  
   550 : dup    v0.8h, x0                     ; 00 0c 02 4e  
   551 : dup    v31.8h, x0                    ; 1f 0c 02 4e  
   552 : dup    v0.8h, x15                    ; e0 0d 02 4e  
   553 : dup    v0.4s, x0                     ; 00 0c 04 4e  
   554 : dup    v31.4s, x0                    ; 1f 0c 04 4e  
   555 : dup    v0.4s, x15                    ; e0 0d 04 4e  
   556 : dup    v0.2d, x0                     ; 00 0c 08 4e  
   557 : dup    v31.2d, x0                    ; 1f 0c 08 4e  
   558 : dup    v0.2d, x15                    ; e0 0d 08 4e  
   559 : dup    v0.16b, v0.16b, #0            ; 00 04 01 4e  
   560 : dup    v31.16b, v0.16b, #0           ; 1f 04 01 4e  
   561 : dup    v0.16b, v31.16b, #0           ; e0 07 01 4e  
   562 : dup    v0.16b, v0.16b, #0x0f         ; 00 04 1f 4e  
   563 : dup    v0.8h, v0.8h, #0              ; 00 04 02 4e  
   564 : dup    v31.8h, v0.8h, #0             ; 1f 04 02 4e  
   565 : dup    v0.8h, v31.8h, #0             ; e0 07 02 4e  
   566 : dup    v0.8h, v0.8h, #0x07           ; 00 04 1e 4e  
   567 : dup    v0.4s, v0.4s, #0              ; 00 04 04 4e  
   568 : dup    v31.4s, v0.4s, #0             ; 1f 04 04 4e  
   569 : dup    v0.4s, v31.4s, #0             ; e0 07 04 4e  
   570 : dup    v0.4s, v0.4s, #0x03           ; 00 04 1c 4e  
   571 : dup    v0.2d, v0.2d, #0              ; 00 04 08 4e  
   572 : dup    v31.2d, v0.2d, #0             ; 1f 04 08 4e  
   573 : dup    v0.2d, v31.2d, #0             ; e0 07 08 4e  
   574 : dup    v0.2d, v0.2d, #0x01           ; 00 04 18 4e  
   575 : sshll  v0.8h, v0.16b, #0             ; 00 a4 08 0f  
   576 : sshll  v31.8h, v0.16b, #0            ; 1f a4 08 0f  
   577 : sshll  v0.8h, v31.16b, #0            ; e0 a7 08 0f  
   578 : sshll  v0.4s, v0.8h, #0              ; 00 a4 10 0f  
   579 : sshll  v31.4s, v0.8h, #0             ; 1f a4 10 0f  
   580 : sshll  v0.4s, v31.8h, #0             ; e0 a7 10 0f  
   581 : sshll  v0.2d, v0.4s, #0              ; 00 a4 20 0f  
   582 : sshll  v31.2d, v0.4s, #0             ; 1f a4 20 0f  
   583 : sshll  v0.2d, v31.4s, #0             ; e0 a7 20 0f  
   584 : ushll  v0.8h, v0.16b, #0             ; 00 a4 08 2f  
   585 : ushll  v31.8h, v0.16b, #0            ; 1f a4 08 2f  
   586 : ushll  v0.8h, v31.16b, #0            ; e0 a7 08 2f  
   587 : ushll  v0.4s, v0.8h, #0              ; 00 a4 10 2f  
   588 : ushll  v31.4s, v0.8h, #0             ; 1f a4 10 2f  
   589 : ushll  v0.4s, v31.8h, #0             ; e0 a7 10 2f  
   590 : ushll  v0.2d, v0.4s, #0              ; 00 a4 20 2f  
   591 : ushll  v31.2d, v0.4s, #0             ; 1f a4 20 2f  
   592 : ushll  v0.2d, v31.4s, #0             ; e0 a7 20 2f  
   593 : sshll2 v0.8h, v0.16b, #0             ; 00 a4 08 4f  
   594 : sshll2 v31.8h, v0.16b, #0            ; 1f a4 08 4f  
   595 : sshll2 v0.8h, v31.16b, #0            ; e0 a7 08 4f  
   596 : sshll2 v0.4s, v0.8h, #0              ; 00 a4 10 4f  
   597 : sshll2 v31.4s, v0.8h, #0             ; 1f a4 10 4f  
   598 : sshll2 v0.4s, v31.8h, #0             ; e0 a7 10 4f  
   599 : sshll2 v0.2d, v0.4s, #0              ; 00 a4 20 4f  
   600 : sshll2 v31.2d, v0.4s, #0             ; 1f a4 20 4f  
   601 : sshll2 v0.2d, v31.4s, #0             ; e0 a7 20 4f  
   602 : ushll2 v0.8h, v0.16b, #0             ; 00 a4 08 6f  
   603 : ushll2 v31.8h, v0.16b, #0            ; 1f a4 08 6f  
   604 : ushll2 v0.8h, v31.16b, #0            ; e0 a7 08 6f  
   605 : ushll2 v0.4s, v0.8h, #0              ; 00 a4 10 6f  
   606 : ushll2 v31.4s, v0.8h, #0             ; 1f a4 10 6f  
   607 : ushll2 v0.4s, v31.8h, #0             ; e0 a7 10 6f  
   608 : ushll2 v0.2d, v0.4s, #0              ; 00 a4 20 6f  
   609 : ushll2 v31.2d, v0.4s, #0             ; 1f a4 20 6f  
   610 : ushll2 v0.2d, v31.4s, #0             ; e0 a7 20 6f  
   611 : xtn    v0.16b, v0.8h                 ; 00 28 21 0e  
   612 : xtn    v31.16b, v0.8h                ; 1f 28 21 0e  
   613 : xtn    v0.16b, v31.8h                ; e0 2b 21 0e  
   614 : xtn    v0.8h, v0.4s                  ; 00 28 61 0e  
   615 : xtn    v31.8h, v0.4s                 ; 1f 28 61 0e  
   616 : xtn    v0.8h, v31.4s                 ; e0 2b 61 0e  
   617 : xtn    v0.4s, v0.2d                  ; 00 28 a1 0e  
   618 : xtn    v31.4s, v0.2d                 ; 1f 28 a1 0e  
   619 : xtn    v0.4s, v31.2d                 ; e0 2b a1 0e  
   620 : xtn2   v0.16b, v0.8h                 ; 00 28 21 4e  
   621 : xtn2   v31.16b, v0.8h                ; 1f 28 21 4e  
   622 : xtn2   v0.16b, v31.8h                ; e0 2b 21 4e  
   623 : xtn2   v0.8h, v0.4s                  ; 00 28 61 4e  
   624 : xtn2   v31.8h, v0.4s                 ; 1f 28 61 4e  
   625 : xtn2   v0.8h, v31.4s                 ; e0 2b 61 4e  
   626 : xtn2   v0.4s, v0.2d                  ; 00 28 a1 4e  
   627 : xtn2   v31.4s, v0.2d                 ; 1f 28 a1 4e  
   628 : xtn2   v0.4s, v31.2d                 ; e0 2b a1 4e  
   629 : smaxv  v0.16b, v0.16b                ; 00 a8 30 4e  
   630 : smaxv  v31.16b, v0.16b               ; 1f a8 30 4e  
   631 : smaxv  v0.16b, v31.16b               ; e0 ab 30 4e  
   632 : smaxv  v0.8h, v0.8h                  ; 00 a8 70 4e  
   633 : smaxv  v31.8h, v0.8h                 ; 1f a8 70 4e  
   634 : smaxv  v0.8h, v31.8h                 ; e0 ab 70 4e  
   635 : smaxv  v0.4s, v0.4s                  ; 00 a8 b0 4e  
   636 : smaxv  v31.4s, v0.4s                 ; 1f a8 b0 4e  
   637 : smaxv  v0.4s, v31.4s                 ; e0 ab b0 4e  
   638 : umaxv  v0.16b, v0.16b                ; 00 a8 30 6e  
   639 : umaxv  v31.16b, v0.16b               ; 1f a8 30 6e  
   640 : umaxv  v0.16b, v31.16b               ; e0 ab 30 6e  
   641 : umaxv  v0.8h, v0.8h                  ; 00 a8 70 6e  
   642 : umaxv  v31.8h, v0.8h                 ; 1f a8 70 6e  
   643 : umaxv  v0.8h, v31.8h                 ; e0 ab 70 6e  
   644 : umaxv  v0.4s, v0.4s                  ; 00 a8 b0 6e  
   645 : umaxv  v31.4s, v0.4s                 ; 1f a8 b0 6e  
   646 : umaxv  v0.4s, v31.4s                 ; e0 ab b0 6e  
   647 : fmaxv  v0.8h, v0.8h                  ; 00 f8 30 4e  
   648 : fmaxv  v31.8h, v0.8h                 ; 1f f8 30 4e  
   649 : fmaxv  v0.8h, v31.8h                 ; e0 fb 30 4e  
   650 : fmaxv  v0.4s, v0.4s                  ; 00 f8 30 6e  
   651 : fmaxv  v31.4s, v0.4s                 ; 1f f8 30 6e  
   652 : fmaxv  v0.4s, v31.4s                 ; e0 fb 30 6e  
   653 : sminv  v0.16b, v0.16b                ; 00 a8 31 4e  
   654 : sminv  v31.16b, v0.16b               ; 1f a8 31 4e  
   655 : sminv  v0.16b, v31.16b               ; e0 ab 31 4e  
   656 : sminv  v0.8h, v0.8h                  ; 00 a8 71 4e  
   657 : sminv  v31.8h, v0.8h                 ; 1f a8 71 4e  
   658 : sminv  v0.8h, v31.8h                 ; e0 ab 71 4e  
   659 : sminv  v0.4s, v0.4s                  ; 00 a8 b1 4e  
   660 : sminv  v31.4s, v0.4s                 ; 1f a8 b1 4e  
   661 : sminv  v0.4s, v31.4s                 ; e0 ab b1 4e  
   662 : uminv  v0.16b, v0.16b                ; 00 a8 31 6e  
   663 : uminv  v31.16b, v0.16b               ; 1f a8 31 6e  
   664 : uminv  v0.16b, v31.16b               ; e0 ab 31 6e  
   665 : uminv  v0.8h, v0.8h                  ; 00 a8 71 6e  
   666 : uminv  v31.8h, v0.8h                 ; 1f a8 71 6e  
   667 : uminv  v0.8h, v31.8h                 ; e0 ab 71 6e  
   668 : uminv  v0.4s, v0.4s                  ; 00 a8 b1 6e  
   669 : uminv  v31.4s, v0.4s                 ; 1f a8 b1 6e  
   670 : uminv  v0.4s, v31.4s                 ; e0 ab b1 6e  
   671 : fminv  v0.8h, v0.8h                  ; 00 f8 b0 4e  
   672 : fminv  v31.8h, v0.8h                 ; 1f f8 b0 4e  
   673 : fminv  v0.8h, v31.8h                 ; e0 fb b0 4e  
   674 : fminv  v0.4s, v0.4s                  ; 00 f8 b0 6e  
   675 : fminv  v31.4s, v0.4s                 ; 1f f8 b0 6e  
   676 : fminv  v0.4s, v31.4s                 ; e0 fb b0 6e  
   677 : ld1    v0.16b, x0, #0                ; 00 00 40 0d  
   678 : ld1    v31.16b, x0, #0               ; 1f 00 40 0d  
   679 : ld1    v0.16b, x0, #0x0f             ; 00 1c 40 4d  
   680 : ld1    v0.16b, x15, #0               ; e0 01 40 0d  
   681 : ld1    v0.8h, x0, #0                 ; 00 40 40 0d  
   682 : ld1    v31.8h, x0, #0                ; 1f 40 40 0d  
   683 : ld1    v0.8h, x0, #0x07              ; 00 58 40 4d  
   684 : ld1    v0.8h, x15, #0                ; e0 41 40 0d  
   685 : ld1    v0.4s, x0, #0                 ; 00 80 40 0d  
   686 : ld1    v31.4s, x0, #0                ; 1f 80 40 0d  
   687 : ld1    v0.4s, x0, #0x03              ; 00 90 40 4d  
   688 : ld1    v0.4s, x15, #0                ; e0 81 40 0d  
   689 : ld1    v0.2d, x0, #0                 ; 00 84 40 0d  
   690 : ld1    v31.2d, x0, #0                ; 1f 84 40 0d  
   691 : ld1    v0.2d, x0, #0x01              ; 00 84 40 4d  
   692 : ld1    v0.2d, x15, #0                ; e0 85 40 0d  
   693 : ld2    v0.16b, v1.16b, x0            ; 00 80 40 4c  
   694 : ld2    v31.16b, v0.16b, x0           ; 1f 80 40 4c  
   695 : ld2    v0.16b, v1.16b, x15           ; e0 81 40 4c  
   696 : ld2    v0.8h, v1.8h, x0              ; 00 84 40 4c  
   697 : ld2    v31.8h, v0.8h, x0             ; 1f 84 40 4c  
   698 : ld2    v0.8h, v1.8h, x15             ; e0 85 40 4c  
   699 : ld2    v0.4s, v1.4s, x0              ; 00 88 40 4c  
   700 : ld2    v31.4s, v0.4s, x0             ; 1f 88 40 4c  
   701 : ld2    v0.4s, v1.4s, x15             ; e0 89 40 4c  
   702 : ld2    v0.2d, v1.2d, x0              ; 00 8c 40 4c  
   703 : ld2    v31.2d, v0.2d, x0             ; 1f 8c 40 4c  
   704 : ld2    v0.2d, v1.2d, x15             ; e0 8d 40 4c  
   705 : st1    v0.16b, #0, x0                ; 00 00 00 0d  
   706 : st1    v31.16b, #0, x0               ; 1f 00 00 0d  
   707 : st1    v0.16b, #0x0f, x0             ; 00 1c 00 4d  
   708 : st1    v0.16b, #0, x15               ; e0 01 00 0d  
   709 : st1    v0.8h, #0, x0                 ; 00 40 00 0d  
   710 : st1    v31.8h, #0, x0                ; 1f 40 00 0d  
   711 : st1    v0.8h, #0x07, x0              ; 00 58 00 4d  
   712 : st1    v0.8h, #0, x15                ; e0 41 00 0d  
   713 : st1    v0.4s, #0, x0                 ; 00 80 00 0d  
   714 : st1    v31.4s, #0, x0                ; 1f 80 00 0d  
   715 : st1    v0.4s, #0x03, x0              ; 00 90 00 4d  
   716 : st1    v0.4s, #0, x15                ; e0 81 00 0d  
   717 : st1    v0.2d, #0, x0                 ; 00 84 00 0d  
   718 : st1    v31.2d, #0, x0                ; 1f 84 00 0d  
   719 : st1    v0.2d, #0x01, x0              ; 00 84 00 4d  
   720 : st1    v0.2d, #0, x15                ; e0 85 00 0d  
   721 : ext    v0.16b, v0.16b, v0.16b, #0    ; 00 00 00 6e  
   722 : ext    v31.16b, v0.16b, v0.16b, #0   ; 1f 00 00 6e  
   723 : ext    v0.16b, v31.16b, v0.16b, #0   ; e0 03 00 6e  
   724 : ext    v0.16b, v0.16b, v31.16b, #0   ; 00 00 1f 6e  
   725 : ext    v0.16b, v0.16b, v0.16b, #0x0f ; 00 78 00 6e  
   726 : ext    v0.8h, v0.8h, v0.8h, #0       ; 00 00 00 6e  
   727 : ext    v31.8h, v0.8h, v0.8h, #0      ; 1f 00 00 6e  
   728 : ext    v0.8h, v31.8h, v0.8h, #0      ; e0 03 00 6e  
   729 : ext    v0.8h, v0.8h, v31.8h, #0      ; 00 00 1f 6e  
   730 : ext    v0.8h, v0.8h, v0.8h, #0x0e    ; 00 70 00 6e  
   731 : ext    v0.4s, v0.4s, v0.4s, #0       ; 00 00 00 6e  
   732 : ext    v31.4s, v0.4s, v0.4s, #0      ; 1f 00 00 6e  
   733 : ext    v0.4s, v31.4s, v0.4s, #0      ; e0 03 00 6e  
   734 : ext    v0.4s, v0.4s, v31.4s, #0      ; 00 00 1f 6e  
   735 : ext    v0.4s, v0.4s, v0.4s, #0x0c    ; 00 60 00 6e  
   736 : ext    v0.2d, v0.2d, v0.2d, #0       ; 00 00 00 6e  
   737 : ext    v31.2d, v0.2d, v0.2d, #0      ; 1f 00 00 6e  
   738 : ext    v0.2d, v31.2d, v0.2d, #0      ; e0 03 00 6e  
   739 : ext    v0.2d, v0.2d, v31.2d, #0      ; 00 00 1f 6e  
   740 : ext    v0.2d, v0.2d, v0.2d, #0x10    ; 00 00 00 6e  
   741 : fmla   v0.8h, v0.8h, v0.8h, #0       ; 00 10 00 4f  
   742 : fmla   v31.8h, v0.8h, v0.8h, #0      ; 1f 10 00 4f  
   743 : fmla   v0.8h, v31.8h, v0.8h, #0      ; e0 13 00 4f  
   744 : fmla   v0.8h, v0.8h, v15.8h, #0      ; 00 10 0f 4f  
   745 : fmla   v0.8h, v0.8h, v0.8h, #0x07    ; 00 18 30 4f  
   746 : fmla   v0.4s, v0.4s, v0.4s, #0       ; 00 10 80 4f  
   747 : fmla   v31.4s, v0.4s, v0.4s, #0      ; 1f 10 80 4f  
   748 : fmla   v0.4s, v31.4s, v0.4s, #0      ; e0 13 80 4f  
   749 : fmla   v0.4s, v0.4s, v31.4s, #0      ; 00 10 9f 4f  
   750 : fmla   v0.4s, v0.4s, v0.4s, #0x03    ; 00 18 a0 4f  
   751 : fmla   v0.2d, v0.2d, v0.2d, #0       ; 00 10 c0 4f  
   752 : fmla   v31.2d, v0.2d, v0.2d, #0      ; 1f 10 c0 4f  
   753 : fmla   v0.2d, v31.2d, v0.2d, #0      ; e0 13 c0 4f  
   754 : fmla   v0.2d, v0.2d, v31.2d, #0      ; 00 10 df 4f  
   755 : fmla   v0.2d, v0.2d, v0.2d, #0x01    ; 00 18 c0 4f  
   756 : cmhi   v0.16b, v0.16b, v0.16b        ; 00 34 20 6e  
   757 : cmhi   v31.16b, v0.16b, v0.16b       ; 1f 34 20 6e  
   758 : cmhi   v0.16b, v31.16b, v0.16b       ; e0 37 20 6e  
   759 : cmhi   v0.16b, v0.16b, v31.16b       ; 00 34 3f 6e  
   760 : cmhi   v0.8h, v0.8h, v0.8h           ; 00 34 60 6e  
   761 : cmhi   v31.8h, v0.8h, v0.8h          ; 1f 34 60 6e  
   762 : cmhi   v0.8h, v31.8h, v0.8h          ; e0 37 60 6e  
   763 : cmhi   v0.8h, v0.8h, v31.8h          ; 00 34 7f 6e  
   764 : cmhi   v0.4s, v0.4s, v0.4s           ; 00 34 a0 6e  
   765 : cmhi   v31.4s, v0.4s, v0.4s          ; 1f 34 a0 6e  
   766 : cmhi   v0.4s, v31.4s, v0.4s          ; e0 37 a0 6e  
   767 : cmhi   v0.4s, v0.4s, v31.4s          ; 00 34 bf 6e  
   768 : cmhi   v0.2d, v0.2d, v0.2d           ; 00 34 e0 6e  
   769 : cmhi   v31.2d, v0.2d, v0.2d          ; 1f 34 e0 6e  
   770 : cmhi   v0.2d, v31.2d, v0.2d          ; e0 37 e0 6e  
   771 : cmhi   v0.2d, v0.2d, v31.2d          ; 00 34 ff 6e  
   772 : cmhs   v0.16b, v0.16b, v0.16b        ; 00 3c 20 6e  
   773 : cmhs   v31.16b, v0.16b, v0.16b       ; 1f 3c 20 6e  
   774 : cmhs   v0.16b, v31.16b, v0.16b       ; e0 3f 20 6e  
   775 : cmhs   v0.16b, v0.16b, v31.16b       ; 00 3c 3f 6e  
   776 : cmhs   v0.8h, v0.8h, v0.8h           ; 00 3c 60 6e  
   777 : cmhs   v31.8h, v0.8h, v0.8h          ; 1f 3c 60 6e  
   778 : cmhs   v0.8h, v31.8h, v0.8h          ; e0 3f 60 6e  
   779 : cmhs   v0.8h, v0.8h, v31.8h          ; 00 3c 7f 6e  
   780 : cmhs   v0.4s, v0.4s, v0.4s           ; 00 3c a0 6e  
   781 : cmhs   v31.4s, v0.4s, v0.4s          ; 1f 3c a0 6e  
   782 : cmhs   v0.4s, v31.4s, v0.4s          ; e0 3f a0 6e  
   783 : cmhs   v0.4s, v0.4s, v31.4s          ; 00 3c bf 6e  
   784 : cmhs   v0.2d, v0.2d, v0.2d           ; 00 3c e0 6e  
   785 : cmhs   v31.2d, v0.2d, v0.2d          ; 1f 3c e0 6e  
   786 : cmhs   v0.2d, v31.2d, v0.2d          ; e0 3f e0 6e  
   787 : cmhs   v0.2d, v0.2d, v31.2d          ; 00 3c ff 6e  
   788 : cmeq   v0.16b, v0.16b, v0.16b        ; 00 8c 20 6e  
   789 : cmeq   v31.16b, v0.16b, v0.16b       ; 1f 8c 20 6e  
   790 : cmeq   v0.16b, v31.16b, v0.16b       ; e0 8f 20 6e  
   791 : cmeq   v0.16b, v0.16b, v31.16b       ; 00 8c 3f 6e  
   792 : cmeq   v0.8h, v0.8h, v0.8h           ; 00 8c 60 6e  
   793 : cmeq   v31.8h, v0.8h, v0.8h          ; 1f 8c 60 6e  
   794 : cmeq   v0.8h, v31.8h, v0.8h          ; e0 8f 60 6e  
   795 : cmeq   v0.8h, v0.8h, v31.8h          ; 00 8c 7f 6e  
   796 : cmeq   v0.4s, v0.4s, v0.4s           ; 00 8c a0 6e  
   797 : cmeq   v31.4s, v0.4s, v0.4s          ; 1f 8c a0 6e  
   798 : cmeq   v0.4s, v31.4s, v0.4s          ; e0 8f a0 6e  
   799 : cmeq   v0.4s, v0.4s, v31.4s          ; 00 8c bf 6e  
   800 : cmeq   v0.2d, v0.2d, v0.2d           ; 00 8c e0 6e  
   801 : cmeq   v31.2d, v0.2d, v0.2d          ; 1f 8c e0 6e  
   802 : cmeq   v0.2d, v31.2d, v0.2d          ; e0 8f e0 6e  
   803 : cmeq   v0.2d, v0.2d, v31.2d          ; 00 8c ff 6e  
   804 : cmge   v0.16b, v0.16b, v0.16b        ; 00 3c 20 4e  
   805 : cmge   v31.16b, v0.16b, v0.16b       ; 1f 3c 20 4e  
   806 : cmge   v0.16b, v31.16b, v0.16b       ; e0 3f 20 4e  
   807 : cmge   v0.16b, v0.16b, v31.16b       ; 00 3c 3f 4e  
   808 : cmge   v0.8h, v0.8h, v0.8h           ; 00 3c 60 4e  
   809 : cmge   v31.8h, v0.8h, v0.8h          ; 1f 3c 60 4e  
   810 : cmge   v0.8h, v31.8h, v0.8h          ; e0 3f 60 4e  
   811 : cmge   v0.8h, v0.8h, v31.8h          ; 00 3c 7f 4e  
   812 : cmge   v0.4s, v0.4s, v0.4s           ; 00 3c a0 4e  
   813 : cmge   v31.4s, v0.4s, v0.4s          ; 1f 3c a0 4e  
   814 : cmge   v0.4s, v31.4s, v0.4s          ; e0 3f a0 4e  
   815 : cmge   v0.4s, v0.4s, v31.4s          ; 00 3c bf 4e  
   816 : cmge   v0.2d, v0.2d, v0.2d           ; 00 3c e0 4e  
   817 : cmge   v31.2d, v0.2d, v0.2d          ; 1f 3c e0 4e  
   818 : cmge   v0.2d, v31.2d, v0.2d          ; e0 3f e0 4e  
   819 : cmge   v0.2d, v0.2d, v31.2d          ; 00 3c ff 4e  
   820 : cmgt   v0.16b, v0.16b, v0.16b        ; 00 34 20 4e  
   821 : cmgt   v31.16b, v0.16b, v0.16b       ; 1f 34 20 4e  
   822 : cmgt   v0.16b, v31.16b, v0.16b       ; e0 37 20 4e  
   823 : cmgt   v0.16b, v0.16b, v31.16b       ; 00 34 3f 4e  
   824 : cmgt   v0.8h, v0.8h, v0.8h           ; 00 34 60 4e  
   825 : cmgt   v31.8h, v0.8h, v0.8h          ; 1f 34 60 4e  
   826 : cmgt   v0.8h, v31.8h, v0.8h          ; e0 37 60 4e  
   827 : cmgt   v0.8h, v0.8h, v31.8h          ; 00 34 7f 4e  
   828 : cmgt   v0.4s, v0.4s, v0.4s           ; 00 34 a0 4e  
   829 : cmgt   v31.4s, v0.4s, v0.4s          ; 1f 34 a0 4e  
   830 : cmgt   v0.4s, v31.4s, v0.4s          ; e0 37 a0 4e  
   831 : cmgt   v0.4s, v0.4s, v31.4s          ; 00 34 bf 4e  
   832 : cmgt   v0.2d, v0.2d, v0.2d           ; 00 34 e0 4e  
   833 : cmgt   v31.2d, v0.2d, v0.2d          ; 1f 34 e0 4e  
   834 : cmgt   v0.2d, v31.2d, v0.2d          ; e0 37 e0 4e  
   835 : cmgt   v0.2d, v0.2d, v31.2d          ; 00 34 ff 4e  
   836 : umov   x0, v0.16b, #0                ; 00 3c 01 0e  
   837 : umov   x15, v0.16b, #0               ; 0f 3c 01 0e  
   838 : umov   x0, v31.16b, #0               ; e0 3f 01 0e  
   839 : umov   x0, v0.16b, #0x0f             ; 00 3c 1f 0e  
   840 : umov   x0, v0.8h, #0                 ; 00 3c 02 0e  
   841 : umov   x15, v0.8h, #0                ; 0f 3c 02 0e  
   842 : umov   x0, v31.8h, #0                ; e0 3f 02 0e  
   843 : umov   x0, v0.8h, #0x07              ; 00 3c 1e 0e  
   844 : umov   x0, v0.4s, #0                 ; 00 3c 04 0e  
   845 : umov   x15, v0.4s, #0                ; 0f 3c 04 0e  
   846 : umov   x0, v31.4s, #0                ; e0 3f 04 0e  
   847 : umov   x0, v0.4s, #0x03              ; 00 3c 1c 0e  
   848 : umov   x0, v0.2d, #0                 ; 00 3c 08 4e  
   849 : umov   x15, v0.2d, #0                ; 0f 3c 08 4e  
   850 : umov   x0, v31.2d, #0                ; e0 3f 08 4e  
   851 : umov   x0, v0.2d, #0x01              ; 00 3c 18 4e  
   852 : ins    v0.16b, #0, x0                ; 00 1c 01 4e  
   853 : ins    v31.16b, #0, x0               ; 1f 1c 01 4e  
   854 : ins    v0.16b, #0x0f, x0             ; 00 1c 1f 4e  
   855 : ins    v0.16b, #0, x15               ; e0 1d 01 4e  
   856 : ins    v0.8h, #0, x0                 ; 00 1c 02 4e  
   857 : ins    v31.8h, #0, x0                ; 1f 1c 02 4e  
   858 : ins    v0.8h, #0x07, x0              ; 00 1c 1e 4e  
   859 : ins    v0.8h, #0, x15                ; e0 1d 02 4e  
   860 : ins    v0.4s, #0, x0                 ; 00 1c 04 4e  
   861 : ins    v31.4s, #0, x0                ; 1f 1c 04 4e  
   862 : ins    v0.4s, #0x03, x0              ; 00 1c 1c 4e  
   863 : ins    v0.4s, #0, x15                ; e0 1d 04 4e  
   864 : ins    v0.2d, #0, x0                 ; 00 1c 08 4e  
   865 : ins    v31.2d, #0, x0                ; 1f 1c 08 4e  
   866 : ins    v0.2d, #0x01, x0              ; 00 1c 18 4e  
   867 : ins    v0.2d, #0, x15                ; e0 1d 08 4e  
   868 : ins    v0.16b, #0, v0.16b, #0        ; 00 04 01 6e  
   869 : ins    v31.16b, #0, v0.16b, #0       ; 1f 04 01 6e  
   870 : ins    v0.16b, #0x0f, v0.16b, #0     ; 00 04 1f 6e  
   871 : ins    v0.16b, #0, v31.16b, #0       ; e0 07 01 6e  
   872 : ins    v0.16b, #0, v0.16b, #0x0f     ; 00 7c 01 6e  
   873 : ins    v0.8h, #0, v0.8h, #0          ; 00 04 02 6e  
   874 : ins    v31.8h, #0, v0.8h, #0         ; 1f 04 02 6e  
   875 : ins    v0.8h, #0x07, v0.8h, #0       ; 00 04 1e 6e  
   876 : ins    v0.8h, #0, v31.8h, #0         ; e0 07 02 6e  
   877 : ins    v0.8h, #0, v0.8h, #0x07       ; 00 74 02 6e  
   878 : ins    v0.4s, #0, v0.4s, #0          ; 00 04 04 6e  
   879 : ins    v31.4s, #0, v0.4s, #0         ; 1f 04 04 6e  
   880 : ins    v0.4s, #0x03, v0.4s, #0       ; 00 04 1c 6e  
   881 : ins    v0.4s, #0, v31.4s, #0         ; e0 07 04 6e  
   882 : ins    v0.4s, #0, v0.4s, #0x03       ; 00 64 04 6e  
   883 : ins    v0.2d, #0, v0.2d, #0          ; 00 04 08 6e  
   884 : ins    v31.2d, #0, v0.2d, #0         ; 1f 04 08 6e  
   885 : ins    v0.2d, #0x01, v0.2d, #0       ; 00 04 18 6e  
   886 : ins    v0.2d, #0, v31.2d, #0         ; e0 07 08 6e  
   887 : ins    v0.2d, #0, v0.2d, #0x01       ; 00 44 08 6e  
   888 : bsl    v0.16b, v0.16b, v0.16b        ; 00 1c 60 6e  
   889 : bsl    v31.16b, v0.16b, v0.16b       ; 1f 1c 60 6e  
   890 : bsl    v0.16b, v31.16b, v0.16b       ; e0 1f 60 6e  
   891 : bsl    v0.16b, v0.16b, v31.16b       ; 00 1c 7f 6e  
