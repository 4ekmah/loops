instruction_set_test()
     0 : str    #0x01, x0, x0, #0x20          ; 00 80 00 f9  
     1 : str    #0x01, x0, x0, #0x20          ; 00 80 00 f9  
     2 : str    #0x01, x0, x0, x0             ; 00 68 20 f8  
     3 : str    #0x01, x0, x0, x0             ; 00 68 20 f8  
     4 : str    #0, w0, x0, #0x40             ; 00 00 01 b9  
     5 : str    #0, w0, x0, #0x40             ; 00 00 01 b9  
     6 : str    #0, w0, x0, x0                ; 00 68 20 b8  
     7 : str    #0, w0, x0, x0                ; 00 68 20 b8  
     8 : strh   w0, x0, #0x80                 ; 00 00 02 79  
     9 : strh   w0, x0, #0x80                 ; 00 00 02 79  
    10 : strh   w0, x0, x0                    ; 00 68 20 78  
    11 : strh   w0, x0, x0                    ; 00 68 20 78  
    12 : strb   w0, x0, #0x100                ; 00 00 04 39  
    13 : strb   w0, x0, #0x100                ; 00 00 04 39  
    14 : strb   w0, x0, x0                    ; 00 68 20 38  
    15 : strb   w0, x0, x0                    ; 00 68 20 38  
    16 : ldr    x0, [x0], #0                  ; 00 00 40 f9  
    17 : ldr    x0, [x0], #0x20               ; 00 80 40 f9  
    18 : ldr    x15, [x0], #0x20              ; 0f 80 40 f9  
    19 : ldr    x0, [x15], #0x20              ; e0 81 40 f9  
    20 : ldr    x0, [x0], x0                  ; 00 68 60 f8  
    21 : ldr    x15, [x0], x0                 ; 0f 68 60 f8  
    22 : ldr    x0, [x15], x0                 ; e0 69 60 f8  
    23 : ldr    x0, [x0], x15                 ; 00 68 6f f8  
    24 : ldr    x0, [x0], #0                  ; 00 00 40 f9  
    25 : ldr    x0, [x0], #0x20               ; 00 80 40 f9  
    26 : ldr    x15, [x0], #0x20              ; 0f 80 40 f9  
    27 : ldr    x0, [x15], #0x20              ; e0 81 40 f9  
    28 : ldr    x0, [x0], x0                  ; 00 68 60 f8  
    29 : ldr    x15, [x0], x0                 ; 0f 68 60 f8  
    30 : ldr    x0, [x15], x0                 ; e0 69 60 f8  
    31 : ldr    x0, [x0], x15                 ; 00 68 6f f8  
    32 : ldr    x0, [x0], #0                  ; 00 00 40 b9  
    33 : ldr    x0, [x0], #0x40               ; 00 00 41 b9  
    34 : ldr    x15, [x0], #0x40              ; 0f 00 41 b9  
    35 : ldr    x0, [x15], #0x40              ; e0 01 41 b9  
    36 : ldr    x0, [x0], x0                  ; 00 68 60 b8  
    37 : ldr    x15, [x0], x0                 ; 0f 68 60 b8  
    38 : ldr    x0, [x15], x0                 ; e0 69 60 b8  
    39 : ldr    x0, [x0], x15                 ; 00 68 6f b8  
    40 : ldrsw  x0, [x0], #0                  ; 00 00 80 b9  
    41 : ldrsw  x0, [x0], #0x40               ; 00 00 81 b9  
    42 : ldrsw  x15, [x0], #0x40              ; 0f 00 81 b9  
    43 : ldrsw  x0, [x15], #0x40              ; e0 01 81 b9  
    44 : ldrsw  x0, [x0], x0                  ; 00 68 a0 b8  
    45 : ldrsw  x15, [x0], x0                 ; 0f 68 a0 b8  
    46 : ldrsw  x0, [x15], x0                 ; e0 69 a0 b8  
    47 : ldrsw  x0, [x0], x15                 ; 00 68 af b8  
    48 : ldrh   x0, [x0], #0                  ; 00 00 40 79  
    49 : ldrh   x0, [x0], #0x80               ; 00 00 42 79  
    50 : ldrh   x15, [x0], #0x80              ; 0f 00 42 79  
    51 : ldrh   x0, [x15], #0x80              ; e0 01 42 79  
    52 : ldrh   x0, [x0], x0                  ; 00 68 60 78  
    53 : ldrh   x15, [x0], x0                 ; 0f 68 60 78  
    54 : ldrh   x0, [x15], x0                 ; e0 69 60 78  
    55 : ldrh   x0, [x0], x15                 ; 00 68 6f 78  
    56 : ldrsh  x0, [x0], #0                  ; 00 00 80 79  
    57 : ldrsh  x0, [x0], #0x80               ; 00 00 82 79  
    58 : ldrsh  x15, [x0], #0x80              ; 0f 00 82 79  
    59 : ldrsh  x0, [x15], #0x80              ; e0 01 82 79  
    60 : ldrsh  x0, [x0], x0                  ; 00 68 a0 78  
    61 : ldrsh  x15, [x0], x0                 ; 0f 68 a0 78  
    62 : ldrsh  x0, [x15], x0                 ; e0 69 a0 78  
    63 : ldrsh  x0, [x0], x15                 ; 00 68 af 78  
    64 : ldrb   x0, [x0], #0                  ; 00 00 40 39  
    65 : ldrb   x0, [x0], #0x100              ; 00 00 44 39  
    66 : ldrb   x15, [x0], #0x100             ; 0f 00 44 39  
    67 : ldrb   x0, [x15], #0x100             ; e0 01 44 39  
    68 : ldrb   x0, [x0], x0                  ; 00 68 60 38  
    69 : ldrb   x15, [x0], x0                 ; 0f 68 60 38  
    70 : ldrb   x0, [x15], x0                 ; e0 69 60 38  
    71 : ldrb   x0, [x0], x15                 ; 00 68 6f 38  
    72 : ldrsb  x0, [x0], #0                  ; 00 00 80 39  
    73 : ldrsb  x0, [x0], #0x100              ; 00 00 84 39  
    74 : ldrsb  x15, [x0], #0x100             ; 0f 00 84 39  
    75 : ldrsb  x0, [x15], #0x100             ; e0 01 84 39  
    76 : ldrsb  x0, [x0], x0                  ; 00 68 a0 38  
    77 : ldrsb  x15, [x0], x0                 ; 0f 68 a0 38  
    78 : ldrsb  x0, [x15], x0                 ; e0 69 a0 38  
    79 : ldrsb  x0, [x0], x15                 ; 00 68 af 38  
    80 : csel   x0, x0, x0, #0                ; 00 00 80 9a  
    81 : csel   x0, x0, x0, #0x01             ; 00 10 80 9a  
    82 : csel   x0, x0, x0, #0x0b             ; 00 b0 80 9a  
    83 : csel   x0, x0, x0, #0x0c             ; 00 c0 80 9a  
    84 : csel   x0, x0, x0, #0x0d             ; 00 d0 80 9a  
    85 : csel   x0, x0, x0, #0x0a             ; 00 a0 80 9a  
    86 : csel   x0, x0, x0, #0x04             ; 00 40 80 9a  
    87 : csel   x0, x0, x0, #0x05             ; 00 50 80 9a  
    88 : csel   x15, x0, x0, #0               ; 0f 00 80 9a  
    89 : csel   x0, x15, x0, #0               ; e0 01 80 9a  
    90 : csel   x0, x0, x15, #0               ; 00 00 8f 9a  
    91 : cinc   x0, x0, #0x01                 ; 00 14 80 9a  
    92 : cinc   x0, x0, #0                    ; 00 04 80 9a  
    93 : cinc   x0, x0, #0x0a                 ; 00 a4 80 9a  
    94 : cinc   x0, x0, #0x0d                 ; 00 d4 80 9a  
    95 : cinc   x0, x0, #0x0c                 ; 00 c4 80 9a  
    96 : cinc   x0, x0, #0x0b                 ; 00 b4 80 9a  
    97 : cinc   x0, x0, #0x05                 ; 00 54 80 9a  
    98 : cinc   x0, x0, #0x04                 ; 00 44 80 9a  
    99 : cinc   x15, x0, #0x01                ; 0f 14 80 9a  
   100 : cinc   x0, x15, #0x01                ; e0 15 8f 9a  
   101 : cneg   x0, x0, #0x01                 ; 00 14 80 da  
   102 : cneg   x0, x0, #0                    ; 00 04 80 da  
   103 : cneg   x0, x0, #0x0a                 ; 00 a4 80 da  
   104 : cneg   x0, x0, #0x0d                 ; 00 d4 80 da  
   105 : cneg   x0, x0, #0x0c                 ; 00 c4 80 da  
   106 : cneg   x0, x0, #0x0b                 ; 00 b4 80 da  
   107 : cneg   x0, x0, #0x05                 ; 00 54 80 da  
   108 : cneg   x0, x0, #0x04                 ; 00 44 80 da  
   109 : cneg   x15, x0, #0x01                ; 0f 14 80 da  
   110 : cneg   x0, x15, #0x01                ; e0 15 8f da  
   111 : asr    x0, x0, #0x3f                 ; 00 fc 7f 93  
   112 : asr    x15, x0, #0x3f                ; 0f fc 7f 93  
   113 : asr    x0, x15, #0x3f                ; e0 fd 7f 93  
   114 : asr    x0, x0, x0                    ; 00 28 c0 9a  
   115 : asr    x15, x0, x0                   ; 0f 28 c0 9a  
   116 : asr    x0, x15, x0                   ; e0 29 c0 9a  
   117 : asr    x0, x0, x15                   ; 00 28 cf 9a  
   118 : lsr    x0, x0, #0x3f                 ; 00 fc 7f d3  
   119 : lsr    x15, x0, #0x3f                ; 0f fc 7f d3  
   120 : lsr    x0, x15, #0x3f                ; e0 fd 7f d3  
   121 : lsr    x0, x0, x0                    ; 00 24 c0 9a  
   122 : lsr    x15, x0, x0                   ; 0f 24 c0 9a  
   123 : lsr    x0, x15, x0                   ; e0 25 c0 9a  
   124 : lsr    x0, x0, x15                   ; 00 24 cf 9a  
   125 : lsl    x0, x0, #0x01, #0             ; 00 00 41 d3  
   126 : lsl    x0, x0, #0x21, #0x20          ; 00 80 61 d3  
   127 : lsl    x0, x0, #0x31, #0x30          ; 00 c0 71 d3  
   128 : lsl    x0, x0, #0x39, #0x38          ; 00 e0 79 d3  
   129 : lsl    x0, x0, #0x3f, #0x3e          ; 00 f8 7f d3  
   130 : lsl    x0, x0, #0, #0x3f             ; 00 fc 40 d3  
   131 : lsl    x15, x0, #0x01, #0            ; 0f 00 41 d3  
   132 : lsl    x0, x15, #0x01, #0            ; e0 01 41 d3  
   133 : lsl    x0, x0, x0                    ; 00 20 c0 9a  
   134 : lsl    x15, x0, x0                   ; 0f 20 c0 9a  
   135 : lsl    x0, x15, x0                   ; e0 21 c0 9a  
   136 : lsl    x0, x0, x15                   ; 00 20 cf 9a  
   137 : and    x0, x0, x0                    ; 00 00 00 8a  
   138 : and    x15, x0, x0                   ; 0f 00 00 8a  
   139 : and    x0, x15, x0                   ; e0 01 00 8a  
   140 : and    x0, x0, x15                   ; 00 00 0f 8a  
   141 : and    x0, x0, #0x01, #0, #0x07      ; 00 1c 40 92  
   142 : and    x0, x0, #0x01, #0x38, #0x07   ; 00 1c 78 92  
   143 : and    x0, x0, #0x01, #0x08, #0x07   ; 00 1c 48 92  
   144 : and    x15, x0, #0x01, #0, #0x07     ; 0f 1c 40 92  
   145 : and    x0, x15, #0x01, #0, #0x07     ; e0 1d 40 92  
   146 : orr    x0, x0, x0                    ; 00 00 00 aa  
   147 : orr    x15, x0, x0                   ; 0f 00 00 aa  
   148 : orr    x0, x15, x0                   ; e0 01 00 aa  
   149 : orr    x0, x0, x15                   ; 00 00 0f aa  
   150 : orr    x0, x0, #0x01, #0, #0x07      ; 00 1c 40 b2  
   151 : orr    x0, x0, #0x01, #0x38, #0x07   ; 00 1c 78 b2  
   152 : orr    x0, x0, #0x01, #0x08, #0x07   ; 00 1c 48 b2  
   153 : orr    x15, x0, #0x01, #0, #0x07     ; 0f 1c 40 b2  
   154 : orr    x0, x15, #0x01, #0, #0x07     ; e0 1d 40 b2  
   155 : eor    x0, x0, x0                    ; 00 00 00 ca  
   156 : eor    x15, x0, x0                   ; 0f 00 00 ca  
   157 : eor    x0, x15, x0                   ; e0 01 00 ca  
   158 : eor    x0, x0, x15                   ; 00 00 0f ca  
   159 : eor    x0, x0, #0x01, #0, #0x07      ; 00 1c 40 d2  
   160 : eor    x0, x0, #0x01, #0x38, #0x07   ; 00 1c 78 d2  
   161 : eor    x0, x0, #0x01, #0x08, #0x07   ; 00 1c 48 d2  
   162 : eor    x15, x0, #0x01, #0, #0x07     ; 0f 1c 40 d2  
   163 : eor    x0, x15, #0x01, #0, #0x07     ; e0 1d 40 d2  
   164 : mvn    x0, x0                        ; e0 03 20 aa  
   165 : mvn    x15, x0                       ; ef 03 20 aa  
   166 : mvn    x0, x15                       ; e0 03 2f aa  
   167 : neg    x0, x0                        ; e0 03 00 cb  
   168 : neg    x15, x0                       ; ef 03 00 cb  
   169 : neg    x0, x15                       ; e0 03 0f cb  
   170 : ldr    v0.2d, x0, #0                 ; 00 00 c0 3d  
   171 : ldr    v0.2d, x0, #0x801             ; 00 04 e0 3d  
   172 : ldr    v31.2d, x0, #0                ; 1f 00 c0 3d  
   173 : ldr    v0.2d, x15, #0                ; e0 01 c0 3d  
   174 : ldr    v0.2d, x0, x0                 ; 00 68 e0 3c  
   175 : ldr    v31.2d, x0, x0                ; 1f 68 e0 3c  
   176 : ldr    v0.2d, x15, x0                ; e0 69 e0 3c  
   177 : ldr    v0.2d, x0, x15                ; 00 68 ef 3c  
   178 : str    v0.2d, x0, #0                 ; 00 00 80 3d  
   179 : str    v0.2d, x0, #0x801             ; 00 04 a0 3d  
   180 : str    v31.2d, x0, #0                ; 1f 00 80 3d  
   181 : str    v0.2d, x15, #0                ; e0 01 80 3d  
   182 : str    v0.2d, x0, x0                 ; 00 68 a0 3c  
   183 : str    v31.2d, x0, x0                ; 1f 68 a0 3c  
   184 : str    v0.2d, x15, x0                ; e0 69 a0 3c  
   185 : str    v0.2d, x0, x15                ; 00 68 af 3c  
   186 : add    v0.16b, v0.16b, v0.16b        ; 00 84 20 4e  
   187 : add    v31.16b, v0.16b, v0.16b       ; 1f 84 20 4e  
   188 : add    v0.16b, v31.16b, v0.16b       ; e0 87 20 4e  
   189 : add    v0.16b, v0.16b, v31.16b       ; 00 84 3f 4e  
   190 : add    v0.8h, v0.8h, v0.8h           ; 00 84 60 4e  
   191 : add    v31.8h, v0.8h, v0.8h          ; 1f 84 60 4e  
   192 : add    v0.8h, v31.8h, v0.8h          ; e0 87 60 4e  
   193 : add    v0.8h, v0.8h, v31.8h          ; 00 84 7f 4e  
   194 : add    v0.4s, v0.4s, v0.4s           ; 00 84 a0 4e  
   195 : add    v31.4s, v0.4s, v0.4s          ; 1f 84 a0 4e  
   196 : add    v0.4s, v31.4s, v0.4s          ; e0 87 a0 4e  
   197 : add    v0.4s, v0.4s, v31.4s          ; 00 84 bf 4e  
   198 : add    v0.2d, v0.2d, v0.2d           ; 00 84 e0 4e  
   199 : add    v31.2d, v0.2d, v0.2d          ; 1f 84 e0 4e  
   200 : add    v0.2d, v31.2d, v0.2d          ; e0 87 e0 4e  
   201 : add    v0.2d, v0.2d, v31.2d          ; 00 84 ff 4e  
   202 : fadd   v0.8h, v0.8h, v0.8h           ; 00 14 40 4e  
   203 : fadd   v31.8h, v0.8h, v0.8h          ; 1f 14 40 4e  
   204 : fadd   v0.8h, v31.8h, v0.8h          ; e0 17 40 4e  
   205 : fadd   v0.8h, v0.8h, v31.8h          ; 00 14 5f 4e  
   206 : fadd   v0.4s, v0.4s, v0.4s           ; 00 d4 20 4e  
   207 : fadd   v31.4s, v0.4s, v0.4s          ; 1f d4 20 4e  
   208 : fadd   v0.4s, v31.4s, v0.4s          ; e0 d7 20 4e  
   209 : fadd   v0.4s, v0.4s, v31.4s          ; 00 d4 3f 4e  
   210 : fadd   v0.2d, v0.2d, v0.2d           ; 00 d4 60 4e  
   211 : fadd   v31.2d, v0.2d, v0.2d          ; 1f d4 60 4e  
   212 : fadd   v0.2d, v31.2d, v0.2d          ; e0 d7 60 4e  
   213 : fadd   v0.2d, v0.2d, v31.2d          ; 00 d4 7f 4e  
   214 : sub    v0.16b, v0.16b, v0.16b        ; 00 84 20 6e  
   215 : sub    v31.16b, v0.16b, v0.16b       ; 1f 84 20 6e  
   216 : sub    v0.16b, v31.16b, v0.16b       ; e0 87 20 6e  
   217 : sub    v0.16b, v0.16b, v31.16b       ; 00 84 3f 6e  
   218 : sub    v0.8h, v0.8h, v0.8h           ; 00 84 60 6e  
   219 : sub    v31.8h, v0.8h, v0.8h          ; 1f 84 60 6e  
   220 : sub    v0.8h, v31.8h, v0.8h          ; e0 87 60 6e  
   221 : sub    v0.8h, v0.8h, v31.8h          ; 00 84 7f 6e  
   222 : sub    v0.4s, v0.4s, v0.4s           ; 00 84 a0 6e  
   223 : sub    v31.4s, v0.4s, v0.4s          ; 1f 84 a0 6e  
   224 : sub    v0.4s, v31.4s, v0.4s          ; e0 87 a0 6e  
   225 : sub    v0.4s, v0.4s, v31.4s          ; 00 84 bf 6e  
   226 : sub    v0.2d, v0.2d, v0.2d           ; 00 84 e0 6e  
   227 : sub    v31.2d, v0.2d, v0.2d          ; 1f 84 e0 6e  
   228 : sub    v0.2d, v31.2d, v0.2d          ; e0 87 e0 6e  
   229 : sub    v0.2d, v0.2d, v31.2d          ; 00 84 ff 6e  
   230 : fsub   v0.8h, v0.8h, v0.8h           ; 00 14 c0 4e  
   231 : fsub   v31.8h, v0.8h, v0.8h          ; 1f 14 c0 4e  
   232 : fsub   v0.8h, v31.8h, v0.8h          ; e0 17 c0 4e  
   233 : fsub   v0.8h, v0.8h, v31.8h          ; 00 14 df 4e  
   234 : fsub   v0.4s, v0.4s, v0.4s           ; 00 d4 a0 4e  
   235 : fsub   v31.4s, v0.4s, v0.4s          ; 1f d4 a0 4e  
   236 : fsub   v0.4s, v31.4s, v0.4s          ; e0 d7 a0 4e  
   237 : fsub   v0.4s, v0.4s, v31.4s          ; 00 d4 bf 4e  
   238 : fsub   v0.2d, v0.2d, v0.2d           ; 00 d4 e0 4e  
   239 : fsub   v31.2d, v0.2d, v0.2d          ; 1f d4 e0 4e  
   240 : fsub   v0.2d, v31.2d, v0.2d          ; e0 d7 e0 4e  
   241 : fsub   v0.2d, v0.2d, v31.2d          ; 00 d4 ff 4e  
   242 : mul    v0.16b, v0.16b, v0.16b        ; 00 9c 20 4e  
   243 : mul    v31.16b, v0.16b, v0.16b       ; 1f 9c 20 4e  
   244 : mul    v0.16b, v31.16b, v0.16b       ; e0 9f 20 4e  
   245 : mul    v0.16b, v0.16b, v31.16b       ; 00 9c 3f 4e  
   246 : mul    v0.8h, v0.8h, v0.8h           ; 00 9c 60 4e  
   247 : mul    v31.8h, v0.8h, v0.8h          ; 1f 9c 60 4e  
   248 : mul    v0.8h, v31.8h, v0.8h          ; e0 9f 60 4e  
   249 : mul    v0.8h, v0.8h, v31.8h          ; 00 9c 7f 4e  
   250 : mul    v0.4s, v0.4s, v0.4s           ; 00 9c a0 4e  
   251 : mul    v31.4s, v0.4s, v0.4s          ; 1f 9c a0 4e  
   252 : mul    v0.4s, v31.4s, v0.4s          ; e0 9f a0 4e  
   253 : mul    v0.4s, v0.4s, v31.4s          ; 00 9c bf 4e  
   254 : fmul   v0.8h, v0.8h, v0.8h           ; 00 1c 40 6e  
   255 : fmul   v31.8h, v0.8h, v0.8h          ; 1f 1c 40 6e  
   256 : fmul   v0.8h, v31.8h, v0.8h          ; e0 1f 40 6e  
   257 : fmul   v0.8h, v0.8h, v31.8h          ; 00 1c 5f 6e  
   258 : fmul   v0.4s, v0.4s, v0.4s           ; 00 dc 20 6e  
   259 : fmul   v31.4s, v0.4s, v0.4s          ; 1f dc 20 6e  
   260 : fmul   v0.4s, v31.4s, v0.4s          ; e0 df 20 6e  
   261 : fmul   v0.4s, v0.4s, v31.4s          ; 00 dc 3f 6e  
   262 : fmul   v0.2d, v0.2d, v0.2d           ; 00 dc 60 6e  
   263 : fmul   v31.2d, v0.2d, v0.2d          ; 1f dc 60 6e  
   264 : fmul   v0.2d, v31.2d, v0.2d          ; e0 df 60 6e  
   265 : fmul   v0.2d, v0.2d, v31.2d          ; 00 dc 7f 6e  
   266 : fdiv   v0.4s, v0.4s, v0.4s           ; 00 fc 20 6e  
   267 : fdiv   v31.4s, v0.4s, v0.4s          ; 1f fc 20 6e  
   268 : fdiv   v0.4s, v31.4s, v0.4s          ; e0 ff 20 6e  
   269 : fdiv   v0.4s, v0.4s, v31.4s          ; 00 fc 3f 6e  
   270 : fdiv   v0.2d, v0.2d, v0.2d           ; 00 fc 60 6e  
   271 : fdiv   v31.2d, v0.2d, v0.2d          ; 1f fc 60 6e  
   272 : fdiv   v0.2d, v31.2d, v0.2d          ; e0 ff 60 6e  
   273 : fdiv   v0.2d, v0.2d, v31.2d          ; 00 fc 7f 6e  
   274 : fdiv   v0.8h, v0.8h, v0.8h           ; 00 3c 40 6e  
   275 : fdiv   v31.8h, v0.8h, v0.8h          ; 1f 3c 40 6e  
   276 : fdiv   v0.8h, v31.8h, v0.8h          ; e0 3f 40 6e  
   277 : fdiv   v0.8h, v0.8h, v31.8h          ; 00 3c 5f 6e  
   278 : shl    v0.16b, v0.16b, #0x01, #0x01  ; 00 54 09 4f  
   279 : shl    v31.16b, v0.16b, #0x01, #0x01 ; 1f 54 09 4f  
   280 : shl    v0.16b, v31.16b, #0x01, #0x01 ; e0 57 09 4f  
   281 : shl    v0.16b, v0.16b, #0x01, #0x07  ; 00 54 0f 4f  
   282 : shl    v0.8h, v0.8h, #0x02, #0x01    ; 00 54 11 4f  
   283 : shl    v31.8h, v0.8h, #0x02, #0x01   ; 1f 54 11 4f  
   284 : shl    v0.8h, v31.8h, #0x02, #0x01   ; e0 57 11 4f  
   285 : shl    v0.8h, v0.8h, #0x03, #0x07    ; 00 54 1f 4f  
   286 : shl    v0.4s, v0.4s, #0x04, #0x01    ; 00 54 21 4f  
   287 : shl    v31.4s, v0.4s, #0x04, #0x01   ; 1f 54 21 4f  
   288 : shl    v0.4s, v31.4s, #0x04, #0x01   ; e0 57 21 4f  
   289 : shl    v0.4s, v0.4s, #0x07, #0x07    ; 00 54 3f 4f  
   290 : shl    v0.2d, v0.2d, #0x08, #0x01    ; 00 54 41 4f  
   291 : shl    v31.2d, v0.2d, #0x08, #0x01   ; 1f 54 41 4f  
   292 : shl    v0.2d, v31.2d, #0x08, #0x01   ; e0 57 41 4f  
   293 : shl    v0.2d, v0.2d, #0x0f, #0x07    ; 00 54 7f 4f  
   294 : ushl   v0.16b, v0.16b, v0.16b        ; 00 44 20 6e  
   295 : ushl   v31.16b, v0.16b, v0.16b       ; 1f 44 20 6e  
   296 : ushl   v0.16b, v31.16b, v0.16b       ; e0 47 20 6e  
   297 : ushl   v0.16b, v0.16b, v31.16b       ; 00 44 3f 6e  
   298 : ushl   v0.8h, v0.8h, v0.8h           ; 00 44 60 6e  
   299 : ushl   v31.8h, v0.8h, v0.8h          ; 1f 44 60 6e  
   300 : ushl   v0.8h, v31.8h, v0.8h          ; e0 47 60 6e  
   301 : ushl   v0.8h, v0.8h, v31.8h          ; 00 44 7f 6e  
   302 : ushl   v0.4s, v0.4s, v0.4s           ; 00 44 a0 6e  
   303 : ushl   v31.4s, v0.4s, v0.4s          ; 1f 44 a0 6e  
   304 : ushl   v0.4s, v31.4s, v0.4s          ; e0 47 a0 6e  
   305 : ushl   v0.4s, v0.4s, v31.4s          ; 00 44 bf 6e  
   306 : ushl   v0.2d, v0.2d, v0.2d           ; 00 44 e0 6e  
   307 : ushl   v31.2d, v0.2d, v0.2d          ; 1f 44 e0 6e  
   308 : ushl   v0.2d, v31.2d, v0.2d          ; e0 47 e0 6e  
   309 : ushl   v0.2d, v0.2d, v31.2d          ; 00 44 ff 6e  
   310 : sshl   v0.16b, v0.16b, v0.16b        ; 00 44 20 4e  
   311 : sshl   v31.16b, v0.16b, v0.16b       ; 1f 44 20 4e  
   312 : sshl   v0.16b, v31.16b, v0.16b       ; e0 47 20 4e  
   313 : sshl   v0.16b, v0.16b, v31.16b       ; 00 44 3f 4e  
   314 : sshl   v0.8h, v0.8h, v0.8h           ; 00 44 60 4e  
   315 : sshl   v31.8h, v0.8h, v0.8h          ; 1f 44 60 4e  
   316 : sshl   v0.8h, v31.8h, v0.8h          ; e0 47 60 4e  
   317 : sshl   v0.8h, v0.8h, v31.8h          ; 00 44 7f 4e  
   318 : sshl   v0.4s, v0.4s, v0.4s           ; 00 44 a0 4e  
   319 : sshl   v31.4s, v0.4s, v0.4s          ; 1f 44 a0 4e  
   320 : sshl   v0.4s, v31.4s, v0.4s          ; e0 47 a0 4e  
   321 : sshl   v0.4s, v0.4s, v31.4s          ; 00 44 bf 4e  
   322 : sshl   v0.2d, v0.2d, v0.2d           ; 00 44 e0 4e  
   323 : sshl   v31.2d, v0.2d, v0.2d          ; 1f 44 e0 4e  
   324 : sshl   v0.2d, v31.2d, v0.2d          ; e0 47 e0 4e  
   325 : sshl   v0.2d, v0.2d, v31.2d          ; 00 44 ff 4e  
   326 : ushr   v0.16b, v0.16b, #0x01, #0x07  ; 00 04 0f 6f  
   327 : ushr   v31.16b, v0.16b, #0x01, #0x07 ; 1f 04 0f 6f  
   328 : ushr   v0.16b, v31.16b, #0x01, #0x07 ; e0 07 0f 6f  
   329 : ushr   v0.16b, v0.16b, #0x01, #0x01  ; 00 04 09 6f  
   330 : ushr   v0.8h, v0.8h, #0x03, #0x07    ; 00 04 1f 6f  
   331 : ushr   v31.8h, v0.8h, #0x03, #0x07   ; 1f 04 1f 6f  
   332 : ushr   v0.8h, v31.8h, #0x03, #0x07   ; e0 07 1f 6f  
   333 : ushr   v0.8h, v0.8h, #0x02, #0x01    ; 00 04 11 6f  
   334 : ushr   v0.4s, v0.4s, #0x07, #0x07    ; 00 04 3f 6f  
   335 : ushr   v31.4s, v0.4s, #0x07, #0x07   ; 1f 04 3f 6f  
   336 : ushr   v0.4s, v31.4s, #0x07, #0x07   ; e0 07 3f 6f  
   337 : ushr   v0.4s, v0.4s, #0x04, #0x01    ; 00 04 21 6f  
   338 : ushr   v0.2d, v0.2d, #0x0f, #0x07    ; 00 04 7f 6f  
   339 : ushr   v31.2d, v0.2d, #0x0f, #0x07   ; 1f 04 7f 6f  
   340 : ushr   v0.2d, v31.2d, #0x0f, #0x07   ; e0 07 7f 6f  
   341 : ushr   v0.2d, v0.2d, #0x08, #0x01    ; 00 04 41 6f  
   342 : sshr   v0.16b, v0.16b, #0x01, #0x07  ; 00 04 0f 4f  
   343 : sshr   v31.16b, v0.16b, #0x01, #0x07 ; 1f 04 0f 4f  
   344 : sshr   v0.16b, v31.16b, #0x01, #0x07 ; e0 07 0f 4f  
   345 : sshr   v0.16b, v0.16b, #0x01, #0x01  ; 00 04 09 4f  
   346 : sshr   v0.8h, v0.8h, #0x03, #0x07    ; 00 04 1f 4f  
   347 : sshr   v31.8h, v0.8h, #0x03, #0x07   ; 1f 04 1f 4f  
   348 : sshr   v0.8h, v31.8h, #0x03, #0x07   ; e0 07 1f 4f  
   349 : sshr   v0.8h, v0.8h, #0x02, #0x01    ; 00 04 11 4f  
   350 : sshr   v0.4s, v0.4s, #0x07, #0x07    ; 00 04 3f 4f  
   351 : sshr   v31.4s, v0.4s, #0x07, #0x07   ; 1f 04 3f 4f  
   352 : sshr   v0.4s, v31.4s, #0x07, #0x07   ; e0 07 3f 4f  
   353 : sshr   v0.4s, v0.4s, #0x04, #0x01    ; 00 04 21 4f  
   354 : sshr   v0.2d, v0.2d, #0x0f, #0x07    ; 00 04 7f 4f  
   355 : sshr   v31.2d, v0.2d, #0x0f, #0x07   ; 1f 04 7f 4f  
   356 : sshr   v0.2d, v31.2d, #0x0f, #0x07   ; e0 07 7f 4f  
   357 : sshr   v0.2d, v0.2d, #0x08, #0x01    ; 00 04 41 4f  
   358 : and    v0.16b, v0.16b, v0.16b        ; 00 1c 20 4e  
   359 : and    v31.16b, v0.16b, v0.16b       ; 1f 1c 20 4e  
   360 : and    v0.16b, v31.16b, v0.16b       ; e0 1f 20 4e  
   361 : and    v0.16b, v0.16b, v31.16b       ; 00 1c 3f 4e  
   362 : orr    v0.16b, v0.16b, v0.16b        ; 00 1c a0 4e  
   363 : orr    v31.16b, v0.16b, v0.16b       ; 1f 1c a0 4e  
   364 : orr    v0.16b, v31.16b, v0.16b       ; e0 1f a0 4e  
   365 : orr    v0.16b, v0.16b, v31.16b       ; 00 1c bf 4e  
   366 : eor    v0.16b, v0.16b, v0.16b        ; 00 1c 20 6e  
   367 : eor    v31.16b, v0.16b, v0.16b       ; 1f 1c 20 6e  
   368 : eor    v0.16b, v31.16b, v0.16b       ; e0 1f 20 6e  
   369 : eor    v0.16b, v0.16b, v31.16b       ; 00 1c 3f 6e  
   370 : mvn    v0.16b, v0.16b                ; 00 58 20 6e  
   371 : mvn    v31.16b, v0.16b               ; 1f 58 20 6e  
   372 : mvn    v0.16b, v31.16b               ; e0 5b 20 6e  
   373 : movi   v0.16b, #0, #0                ; 00 e4 00 4f  
   374 : movi   v31.16b, #0, #0               ; 1f e4 00 4f  
   375 : movi   v0.16b, #0x1f, #0x07          ; e0 e7 07 4f  
   376 : movi   v0.8h, #0, #0                 ; 00 84 00 4f  
   377 : movi   v31.8h, #0, #0                ; 1f 84 00 4f  
   378 : movi   v0.8h, #0x1f, #0x07           ; e0 87 07 4f  
   379 : movi   v0.4s, #0, #0                 ; 00 04 00 4f  
   380 : movi   v31.4s, #0, #0                ; 1f 04 00 4f  
   381 : movi   v0.4s, #0x1f, #0x07           ; e0 07 07 4f  
   382 : fmin   v0.8h, v0.8h, v0.8h           ; 00 34 c0 4e  
   383 : fmin   v31.8h, v0.8h, v0.8h          ; 1f 34 c0 4e  
   384 : fmin   v0.8h, v31.8h, v0.8h          ; e0 37 c0 4e  
   385 : fmin   v0.8h, v0.8h, v31.8h          ; 00 34 df 4e  
   386 : fmin   v0.4s, v0.4s, v0.4s           ; 00 f4 a0 4e  
   387 : fmin   v31.4s, v0.4s, v0.4s          ; 1f f4 a0 4e  
   388 : fmin   v0.4s, v31.4s, v0.4s          ; e0 f7 a0 4e  
   389 : fmin   v0.4s, v0.4s, v31.4s          ; 00 f4 bf 4e  
   390 : fmin   v0.2d, v0.2d, v0.2d           ; 00 f4 e0 4e  
   391 : fmin   v31.2d, v0.2d, v0.2d          ; 1f f4 e0 4e  
   392 : fmin   v0.2d, v31.2d, v0.2d          ; e0 f7 e0 4e  
   393 : fmin   v0.2d, v0.2d, v31.2d          ; 00 f4 ff 4e  
   394 : fmax   v0.8h, v0.8h, v0.8h           ; 00 34 40 4e  
   395 : fmax   v31.8h, v0.8h, v0.8h          ; 1f 34 40 4e  
   396 : fmax   v0.8h, v31.8h, v0.8h          ; e0 37 40 4e  
   397 : fmax   v0.8h, v0.8h, v31.8h          ; 00 34 5f 4e  
   398 : fmax   v0.4s, v0.4s, v0.4s           ; 00 f4 20 4e  
   399 : fmax   v31.4s, v0.4s, v0.4s          ; 1f f4 20 4e  
   400 : fmax   v0.4s, v31.4s, v0.4s          ; e0 f7 20 4e  
   401 : fmax   v0.4s, v0.4s, v31.4s          ; 00 f4 3f 4e  
   402 : fmax   v0.2d, v0.2d, v0.2d           ; 00 f4 60 4e  
   403 : fmax   v31.2d, v0.2d, v0.2d          ; 1f f4 60 4e  
   404 : fmax   v0.2d, v31.2d, v0.2d          ; e0 f7 60 4e  
   405 : fmax   v0.2d, v0.2d, v31.2d          ; 00 f4 7f 4e  
   406 : fmla   v0.8h, v0.8h, v0.8h           ; 00 0c 40 4e  
   407 : fmla   v31.8h, v0.8h, v0.8h          ; 1f 0c 40 4e  
   408 : fmla   v0.8h, v31.8h, v0.8h          ; e0 0f 40 4e  
   409 : fmla   v0.8h, v0.8h, v31.8h          ; 00 0c 5f 4e  
   410 : fmla   v0.4s, v0.4s, v0.4s           ; 00 cc 20 4e  
   411 : fmla   v31.4s, v0.4s, v0.4s          ; 1f cc 20 4e  
   412 : fmla   v0.4s, v31.4s, v0.4s          ; e0 cf 20 4e  
   413 : fmla   v0.4s, v0.4s, v31.4s          ; 00 cc 3f 4e  
   414 : fmla   v0.2d, v0.2d, v0.2d           ; 00 cc 60 4e  
   415 : fmla   v31.2d, v0.2d, v0.2d          ; 1f cc 60 4e  
   416 : fmla   v0.2d, v31.2d, v0.2d          ; e0 cf 60 4e  
   417 : fmla   v0.2d, v0.2d, v31.2d          ; 00 cc 7f 4e  
   418 : fcvtzs v0.8h, v0.8h                  ; 00 b8 f9 4e  
   419 : fcvtzs v31.8h, v0.8h                 ; 1f b8 f9 4e  
   420 : fcvtzs v0.8h, v31.8h                 ; e0 bb f9 4e  
   421 : fcvtzs v0.4s, v0.4s                  ; 00 b8 a1 4e  
   422 : fcvtzs v31.4s, v0.4s                 ; 1f b8 a1 4e  
   423 : fcvtzs v0.4s, v31.4s                 ; e0 bb a1 4e  
   424 : fcvtzs v0.2d, v0.2d                  ; 00 b8 e1 4e  
   425 : fcvtzs v31.2d, v0.2d                 ; 1f b8 e1 4e  
   426 : fcvtzs v0.2d, v31.2d                 ; e0 bb e1 4e  
   427 : fcvtzu v0.8h, v0.8h                  ; 00 b8 f9 6e  
   428 : fcvtzu v31.8h, v0.8h                 ; 1f b8 f9 6e  
   429 : fcvtzu v0.8h, v31.8h                 ; e0 bb f9 6e  
   430 : fcvtzu v0.4s, v0.4s                  ; 00 b8 a1 6e  
   431 : fcvtzu v31.4s, v0.4s                 ; 1f b8 a1 6e  
   432 : fcvtzu v0.4s, v31.4s                 ; e0 bb a1 6e  
   433 : fcvtzu v0.2d, v0.2d                  ; 00 b8 e1 6e  
   434 : fcvtzu v31.2d, v0.2d                 ; 1f b8 e1 6e  
   435 : fcvtzu v0.2d, v31.2d                 ; e0 bb e1 6e  
   436 : scvtf  v0.8h, v0.8h                  ; 00 d8 79 4e  
   437 : scvtf  v31.8h, v0.8h                 ; 1f d8 79 4e  
   438 : scvtf  v0.8h, v31.8h                 ; e0 db 79 4e  
   439 : scvtf  v0.4s, v0.4s                  ; 00 d8 21 4e  
   440 : scvtf  v31.4s, v0.4s                 ; 1f d8 21 4e  
   441 : scvtf  v0.4s, v31.4s                 ; e0 db 21 4e  
   442 : scvtf  v0.2d, v0.2d                  ; 00 d8 61 4e  
   443 : scvtf  v31.2d, v0.2d                 ; 1f d8 61 4e  
   444 : scvtf  v0.2d, v31.2d                 ; e0 db 61 4e  
   445 : ucvtf  v0.8h, v0.8h                  ; 00 d8 79 6e  
   446 : ucvtf  v31.8h, v0.8h                 ; 1f d8 79 6e  
   447 : ucvtf  v0.8h, v31.8h                 ; e0 db 79 6e  
   448 : ucvtf  v0.4s, v0.4s                  ; 00 d8 21 6e  
   449 : ucvtf  v31.4s, v0.4s                 ; 1f d8 21 6e  
   450 : ucvtf  v0.4s, v31.4s                 ; e0 db 21 6e  
   451 : ucvtf  v0.2d, v0.2d                  ; 00 d8 61 6e  
   452 : ucvtf  v31.2d, v0.2d                 ; 1f d8 61 6e  
   453 : ucvtf  v0.2d, v31.2d                 ; e0 db 61 6e  
   454 : fcmgt  v0.8h, v0.8h, v0.8h           ; 00 24 c0 6e  
   455 : fcmgt  v31.8h, v0.8h, v0.8h          ; 1f 24 c0 6e  
   456 : fcmgt  v0.8h, v31.8h, v0.8h          ; e0 27 c0 6e  
   457 : fcmgt  v0.8h, v0.8h, v31.8h          ; 00 24 df 6e  
   458 : fcmgt  v0.4s, v0.4s, v0.4s           ; 00 e4 a0 6e  
   459 : fcmgt  v31.4s, v0.4s, v0.4s          ; 1f e4 a0 6e  
   460 : fcmgt  v0.4s, v31.4s, v0.4s          ; e0 e7 a0 6e  
   461 : fcmgt  v0.4s, v0.4s, v31.4s          ; 00 e4 bf 6e  
   462 : fcmgt  v0.2d, v0.2d, v0.2d           ; 00 e4 e0 6e  
   463 : fcmgt  v31.2d, v0.2d, v0.2d          ; 1f e4 e0 6e  
   464 : fcmgt  v0.2d, v31.2d, v0.2d          ; e0 e7 e0 6e  
   465 : fcmgt  v0.2d, v0.2d, v31.2d          ; 00 e4 ff 6e  
   466 : fcmge  v0.8h, v0.8h, v0.8h           ; 00 24 40 6e  
   467 : fcmge  v31.8h, v0.8h, v0.8h          ; 1f 24 40 6e  
   468 : fcmge  v0.8h, v31.8h, v0.8h          ; e0 27 40 6e  
   469 : fcmge  v0.8h, v0.8h, v31.8h          ; 00 24 5f 6e  
   470 : fcmge  v0.4s, v0.4s, v0.4s           ; 00 e4 20 6e  
   471 : fcmge  v31.4s, v0.4s, v0.4s          ; 1f e4 20 6e  
   472 : fcmge  v0.4s, v31.4s, v0.4s          ; e0 e7 20 6e  
   473 : fcmge  v0.4s, v0.4s, v31.4s          ; 00 e4 3f 6e  
   474 : fcmge  v0.2d, v0.2d, v0.2d           ; 00 e4 60 6e  
   475 : fcmge  v31.2d, v0.2d, v0.2d          ; 1f e4 60 6e  
   476 : fcmge  v0.2d, v31.2d, v0.2d          ; e0 e7 60 6e  
   477 : fcmge  v0.2d, v0.2d, v31.2d          ; 00 e4 7f 6e  
   478 : fcmeq  v0.8h, v0.8h, v0.8h           ; 00 24 40 4e  
   479 : fcmeq  v31.8h, v0.8h, v0.8h          ; 1f 24 40 4e  
   480 : fcmeq  v0.8h, v31.8h, v0.8h          ; e0 27 40 4e  
   481 : fcmeq  v0.8h, v0.8h, v31.8h          ; 00 24 5f 4e  
   482 : fcmeq  v0.4s, v0.4s, v0.4s           ; 00 e4 20 4e  
   483 : fcmeq  v31.4s, v0.4s, v0.4s          ; 1f e4 20 4e  
   484 : fcmeq  v0.4s, v31.4s, v0.4s          ; e0 e7 20 4e  
   485 : fcmeq  v0.4s, v0.4s, v31.4s          ; 00 e4 3f 4e  
   486 : fcmeq  v0.2d, v0.2d, v0.2d           ; 00 e4 60 4e  
   487 : fcmeq  v31.2d, v0.2d, v0.2d          ; 1f e4 60 4e  
   488 : fcmeq  v0.2d, v31.2d, v0.2d          ; e0 e7 60 4e  
   489 : fcmeq  v0.2d, v0.2d, v31.2d          ; 00 e4 7f 4e  
   490 : neg    v0.16b, v0.16b                ; 00 b8 20 6e  
   491 : neg    v31.16b, v0.16b               ; 1f b8 20 6e  
   492 : neg    v0.16b, v31.16b               ; e0 bb 20 6e  
   493 : neg    v0.8h, v0.8h                  ; 00 b8 60 6e  
   494 : neg    v31.8h, v0.8h                 ; 1f b8 60 6e  
   495 : neg    v0.8h, v31.8h                 ; e0 bb 60 6e  
   496 : neg    v0.4s, v0.4s                  ; 00 b8 a0 6e  
   497 : neg    v31.4s, v0.4s                 ; 1f b8 a0 6e  
   498 : neg    v0.4s, v31.4s                 ; e0 bb a0 6e  
   499 : neg    v0.2d, v0.2d                  ; 00 b8 e0 6e  
   500 : neg    v31.2d, v0.2d                 ; 1f b8 e0 6e  
   501 : neg    v0.2d, v31.2d                 ; e0 bb e0 6e  
   502 : fneg   v0.8h, v0.8h                  ; 00 f8 f8 6e  
   503 : fneg   v31.8h, v0.8h                 ; 1f f8 f8 6e  
   504 : fneg   v0.8h, v31.8h                 ; e0 fb f8 6e  
   505 : fneg   v0.4s, v0.4s                  ; 00 f8 a0 6e  
   506 : fneg   v31.4s, v0.4s                 ; 1f f8 a0 6e  
   507 : fneg   v0.4s, v31.4s                 ; e0 fb a0 6e  
   508 : fneg   v0.2d, v0.2d                  ; 00 f8 e0 6e  
   509 : fneg   v31.2d, v0.2d                 ; 1f f8 e0 6e  
   510 : fneg   v0.2d, v31.2d                 ; e0 fb e0 6e  
