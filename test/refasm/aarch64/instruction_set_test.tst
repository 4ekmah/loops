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
   559 : ld1    v0.16b, x0, #0                ; 00 00 40 0d  
   560 : ld1    v31.16b, x0, #0               ; 1f 00 40 0d  
   561 : ld1    v0.16b, x0, #0x0f             ; 00 1c 40 4d  
   562 : ld1    v0.16b, x15, #0               ; e0 01 40 0d  
   563 : ld1    v0.8h, x0, #0                 ; 00 40 40 0d  
   564 : ld1    v31.8h, x0, #0                ; 1f 40 40 0d  
   565 : ld1    v0.8h, x0, #0x07              ; 00 58 40 4d  
   566 : ld1    v0.8h, x15, #0                ; e0 41 40 0d  
   567 : ld1    v0.4s, x0, #0                 ; 00 80 40 0d  
   568 : ld1    v31.4s, x0, #0                ; 1f 80 40 0d  
   569 : ld1    v0.4s, x0, #0x03              ; 00 90 40 4d  
   570 : ld1    v0.4s, x15, #0                ; e0 81 40 0d  
   571 : ld1    v0.2d, x0, #0                 ; 00 84 40 0d  
   572 : ld1    v31.2d, x0, #0                ; 1f 84 40 0d  
   573 : ld1    v0.2d, x0, #0x01              ; 00 84 40 4d  
   574 : ld1    v0.2d, x15, #0                ; e0 85 40 0d  
   575 : ld2    v0.16b, v1.16b, x0            ; 00 80 40 4c  
   576 : ld2    v31.16b, v0.16b, x0           ; 1f 80 40 4c  
   577 : ld2    v0.16b, v1.16b, x15           ; e0 81 40 4c  
   578 : ld2    v0.8h, v1.8h, x0              ; 00 84 40 4c  
   579 : ld2    v31.8h, v0.8h, x0             ; 1f 84 40 4c  
   580 : ld2    v0.8h, v1.8h, x15             ; e0 85 40 4c  
   581 : ld2    v0.4s, v1.4s, x0              ; 00 88 40 4c  
   582 : ld2    v31.4s, v0.4s, x0             ; 1f 88 40 4c  
   583 : ld2    v0.4s, v1.4s, x15             ; e0 89 40 4c  
   584 : ld2    v0.2d, v1.2d, x0              ; 00 8c 40 4c  
   585 : ld2    v31.2d, v0.2d, x0             ; 1f 8c 40 4c  
   586 : ld2    v0.2d, v1.2d, x15             ; e0 8d 40 4c  
   587 : st1    v0.16b, #0, x0                ; 00 00 00 0d  
   588 : st1    v31.16b, #0, x0               ; 1f 00 00 0d  
   589 : st1    v0.16b, #0x0f, x0             ; 00 1c 00 4d  
   590 : st1    v0.16b, #0, x15               ; e0 01 00 0d  
   591 : st1    v0.8h, #0, x0                 ; 00 40 00 0d  
   592 : st1    v31.8h, #0, x0                ; 1f 40 00 0d  
   593 : st1    v0.8h, #0x07, x0              ; 00 58 00 4d  
   594 : st1    v0.8h, #0, x15                ; e0 41 00 0d  
   595 : st1    v0.4s, #0, x0                 ; 00 80 00 0d  
   596 : st1    v31.4s, #0, x0                ; 1f 80 00 0d  
   597 : st1    v0.4s, #0x03, x0              ; 00 90 00 4d  
   598 : st1    v0.4s, #0, x15                ; e0 81 00 0d  
   599 : st1    v0.2d, #0, x0                 ; 00 84 00 0d  
   600 : st1    v31.2d, #0, x0                ; 1f 84 00 0d  
   601 : st1    v0.2d, #0x01, x0              ; 00 84 00 4d  
   602 : st1    v0.2d, #0, x15                ; e0 85 00 0d  
   603 : ext    v0.16b, v0.16b, v0.16b, #0    ; 00 00 00 6e  
   604 : ext    v31.16b, v0.16b, v0.16b, #0   ; 1f 00 00 6e  
   605 : ext    v0.16b, v31.16b, v0.16b, #0   ; e0 03 00 6e  
   606 : ext    v0.16b, v0.16b, v31.16b, #0   ; 00 00 1f 6e  
   607 : ext    v0.16b, v0.16b, v0.16b, #0x0f ; 00 78 00 6e  
   608 : ext    v0.8h, v0.8h, v0.8h, #0       ; 00 00 00 6e  
   609 : ext    v31.8h, v0.8h, v0.8h, #0      ; 1f 00 00 6e  
   610 : ext    v0.8h, v31.8h, v0.8h, #0      ; e0 03 00 6e  
   611 : ext    v0.8h, v0.8h, v31.8h, #0      ; 00 00 1f 6e  
   612 : ext    v0.8h, v0.8h, v0.8h, #0x0e    ; 00 70 00 6e  
   613 : ext    v0.4s, v0.4s, v0.4s, #0       ; 00 00 00 6e  
   614 : ext    v31.4s, v0.4s, v0.4s, #0      ; 1f 00 00 6e  
   615 : ext    v0.4s, v31.4s, v0.4s, #0      ; e0 03 00 6e  
   616 : ext    v0.4s, v0.4s, v31.4s, #0      ; 00 00 1f 6e  
   617 : ext    v0.4s, v0.4s, v0.4s, #0x0c    ; 00 60 00 6e  
   618 : ext    v0.2d, v0.2d, v0.2d, #0       ; 00 00 00 6e  
   619 : ext    v31.2d, v0.2d, v0.2d, #0      ; 1f 00 00 6e  
   620 : ext    v0.2d, v31.2d, v0.2d, #0      ; e0 03 00 6e  
   621 : ext    v0.2d, v0.2d, v31.2d, #0      ; 00 00 1f 6e  
   622 : ext    v0.2d, v0.2d, v0.2d, #0x10    ; 00 00 00 6e  
   623 : fmla   v0.8h, v0.8h, v0.8h, #0       ; 00 10 00 4f  
   624 : fmla   v31.8h, v0.8h, v0.8h, #0      ; 1f 10 00 4f  
   625 : fmla   v0.8h, v31.8h, v0.8h, #0      ; e0 13 00 4f  
   626 : fmla   v0.8h, v0.8h, v15.8h, #0      ; 00 10 0f 4f  
   627 : fmla   v0.8h, v0.8h, v0.8h, #0x07    ; 00 18 30 4f  
   628 : fmla   v0.4s, v0.4s, v0.4s, #0       ; 00 10 80 4f  
   629 : fmla   v31.4s, v0.4s, v0.4s, #0      ; 1f 10 80 4f  
   630 : fmla   v0.4s, v31.4s, v0.4s, #0      ; e0 13 80 4f  
   631 : fmla   v0.4s, v0.4s, v31.4s, #0      ; 00 10 9f 4f  
   632 : fmla   v0.4s, v0.4s, v0.4s, #0x03    ; 00 18 a0 4f  
   633 : fmla   v0.2d, v0.2d, v0.2d, #0       ; 00 10 c0 4f  
   634 : fmla   v31.2d, v0.2d, v0.2d, #0      ; 1f 10 c0 4f  
   635 : fmla   v0.2d, v31.2d, v0.2d, #0      ; e0 13 c0 4f  
   636 : fmla   v0.2d, v0.2d, v31.2d, #0      ; 00 10 df 4f  
   637 : fmla   v0.2d, v0.2d, v0.2d, #0x01    ; 00 18 c0 4f  
   638 : cmhi   v0.16b, v0.16b, v0.16b        ; 00 34 20 6e  
   639 : cmhi   v31.16b, v0.16b, v0.16b       ; 1f 34 20 6e  
   640 : cmhi   v0.16b, v31.16b, v0.16b       ; e0 37 20 6e  
   641 : cmhi   v0.16b, v0.16b, v31.16b       ; 00 34 3f 6e  
   642 : cmhi   v0.8h, v0.8h, v0.8h           ; 00 34 60 6e  
   643 : cmhi   v31.8h, v0.8h, v0.8h          ; 1f 34 60 6e  
   644 : cmhi   v0.8h, v31.8h, v0.8h          ; e0 37 60 6e  
   645 : cmhi   v0.8h, v0.8h, v31.8h          ; 00 34 7f 6e  
   646 : cmhi   v0.4s, v0.4s, v0.4s           ; 00 34 a0 6e  
   647 : cmhi   v31.4s, v0.4s, v0.4s          ; 1f 34 a0 6e  
   648 : cmhi   v0.4s, v31.4s, v0.4s          ; e0 37 a0 6e  
   649 : cmhi   v0.4s, v0.4s, v31.4s          ; 00 34 bf 6e  
   650 : cmhi   v0.2d, v0.2d, v0.2d           ; 00 34 e0 6e  
   651 : cmhi   v31.2d, v0.2d, v0.2d          ; 1f 34 e0 6e  
   652 : cmhi   v0.2d, v31.2d, v0.2d          ; e0 37 e0 6e  
   653 : cmhi   v0.2d, v0.2d, v31.2d          ; 00 34 ff 6e  
   654 : cmhs   v0.16b, v0.16b, v0.16b        ; 00 3c 20 6e  
   655 : cmhs   v31.16b, v0.16b, v0.16b       ; 1f 3c 20 6e  
   656 : cmhs   v0.16b, v31.16b, v0.16b       ; e0 3f 20 6e  
   657 : cmhs   v0.16b, v0.16b, v31.16b       ; 00 3c 3f 6e  
   658 : cmhs   v0.8h, v0.8h, v0.8h           ; 00 3c 60 6e  
   659 : cmhs   v31.8h, v0.8h, v0.8h          ; 1f 3c 60 6e  
   660 : cmhs   v0.8h, v31.8h, v0.8h          ; e0 3f 60 6e  
   661 : cmhs   v0.8h, v0.8h, v31.8h          ; 00 3c 7f 6e  
   662 : cmhs   v0.4s, v0.4s, v0.4s           ; 00 3c a0 6e  
   663 : cmhs   v31.4s, v0.4s, v0.4s          ; 1f 3c a0 6e  
   664 : cmhs   v0.4s, v31.4s, v0.4s          ; e0 3f a0 6e  
   665 : cmhs   v0.4s, v0.4s, v31.4s          ; 00 3c bf 6e  
   666 : cmhs   v0.2d, v0.2d, v0.2d           ; 00 3c e0 6e  
   667 : cmhs   v31.2d, v0.2d, v0.2d          ; 1f 3c e0 6e  
   668 : cmhs   v0.2d, v31.2d, v0.2d          ; e0 3f e0 6e  
   669 : cmhs   v0.2d, v0.2d, v31.2d          ; 00 3c ff 6e  
   670 : cmeq   v0.16b, v0.16b, v0.16b        ; 00 8c 20 6e  
   671 : cmeq   v31.16b, v0.16b, v0.16b       ; 1f 8c 20 6e  
   672 : cmeq   v0.16b, v31.16b, v0.16b       ; e0 8f 20 6e  
   673 : cmeq   v0.16b, v0.16b, v31.16b       ; 00 8c 3f 6e  
   674 : cmeq   v0.8h, v0.8h, v0.8h           ; 00 8c 60 6e  
   675 : cmeq   v31.8h, v0.8h, v0.8h          ; 1f 8c 60 6e  
   676 : cmeq   v0.8h, v31.8h, v0.8h          ; e0 8f 60 6e  
   677 : cmeq   v0.8h, v0.8h, v31.8h          ; 00 8c 7f 6e  
   678 : cmeq   v0.4s, v0.4s, v0.4s           ; 00 8c a0 6e  
   679 : cmeq   v31.4s, v0.4s, v0.4s          ; 1f 8c a0 6e  
   680 : cmeq   v0.4s, v31.4s, v0.4s          ; e0 8f a0 6e  
   681 : cmeq   v0.4s, v0.4s, v31.4s          ; 00 8c bf 6e  
   682 : cmeq   v0.2d, v0.2d, v0.2d           ; 00 8c e0 6e  
   683 : cmeq   v31.2d, v0.2d, v0.2d          ; 1f 8c e0 6e  
   684 : cmeq   v0.2d, v31.2d, v0.2d          ; e0 8f e0 6e  
   685 : cmeq   v0.2d, v0.2d, v31.2d          ; 00 8c ff 6e  
   686 : cmge   v0.16b, v0.16b, v0.16b        ; 00 3c 20 4e  
   687 : cmge   v31.16b, v0.16b, v0.16b       ; 1f 3c 20 4e  
   688 : cmge   v0.16b, v31.16b, v0.16b       ; e0 3f 20 4e  
   689 : cmge   v0.16b, v0.16b, v31.16b       ; 00 3c 3f 4e  
   690 : cmge   v0.8h, v0.8h, v0.8h           ; 00 3c 60 4e  
   691 : cmge   v31.8h, v0.8h, v0.8h          ; 1f 3c 60 4e  
   692 : cmge   v0.8h, v31.8h, v0.8h          ; e0 3f 60 4e  
   693 : cmge   v0.8h, v0.8h, v31.8h          ; 00 3c 7f 4e  
   694 : cmge   v0.4s, v0.4s, v0.4s           ; 00 3c a0 4e  
   695 : cmge   v31.4s, v0.4s, v0.4s          ; 1f 3c a0 4e  
   696 : cmge   v0.4s, v31.4s, v0.4s          ; e0 3f a0 4e  
   697 : cmge   v0.4s, v0.4s, v31.4s          ; 00 3c bf 4e  
   698 : cmge   v0.2d, v0.2d, v0.2d           ; 00 3c e0 4e  
   699 : cmge   v31.2d, v0.2d, v0.2d          ; 1f 3c e0 4e  
   700 : cmge   v0.2d, v31.2d, v0.2d          ; e0 3f e0 4e  
   701 : cmge   v0.2d, v0.2d, v31.2d          ; 00 3c ff 4e  
   702 : cmgt   v0.16b, v0.16b, v0.16b        ; 00 34 20 4e  
   703 : cmgt   v31.16b, v0.16b, v0.16b       ; 1f 34 20 4e  
   704 : cmgt   v0.16b, v31.16b, v0.16b       ; e0 37 20 4e  
   705 : cmgt   v0.16b, v0.16b, v31.16b       ; 00 34 3f 4e  
   706 : cmgt   v0.8h, v0.8h, v0.8h           ; 00 34 60 4e  
   707 : cmgt   v31.8h, v0.8h, v0.8h          ; 1f 34 60 4e  
   708 : cmgt   v0.8h, v31.8h, v0.8h          ; e0 37 60 4e  
   709 : cmgt   v0.8h, v0.8h, v31.8h          ; 00 34 7f 4e  
   710 : cmgt   v0.4s, v0.4s, v0.4s           ; 00 34 a0 4e  
   711 : cmgt   v31.4s, v0.4s, v0.4s          ; 1f 34 a0 4e  
   712 : cmgt   v0.4s, v31.4s, v0.4s          ; e0 37 a0 4e  
   713 : cmgt   v0.4s, v0.4s, v31.4s          ; 00 34 bf 4e  
   714 : cmgt   v0.2d, v0.2d, v0.2d           ; 00 34 e0 4e  
   715 : cmgt   v31.2d, v0.2d, v0.2d          ; 1f 34 e0 4e  
   716 : cmgt   v0.2d, v31.2d, v0.2d          ; e0 37 e0 4e  
   717 : cmgt   v0.2d, v0.2d, v31.2d          ; 00 34 ff 4e  
   718 : umov   x0, v0.16b, #0                ; 00 3c 01 0e  
   719 : umov   x15, v0.16b, #0               ; 0f 3c 01 0e  
   720 : umov   x0, v31.16b, #0               ; e0 3f 01 0e  
   721 : umov   x0, v0.16b, #0x0f             ; 00 3c 1f 0e  
   722 : umov   x0, v0.8h, #0                 ; 00 3c 02 0e  
   723 : umov   x15, v0.8h, #0                ; 0f 3c 02 0e  
   724 : umov   x0, v31.8h, #0                ; e0 3f 02 0e  
   725 : umov   x0, v0.8h, #0x07              ; 00 3c 1e 0e  
   726 : umov   x0, v0.4s, #0                 ; 00 3c 04 0e  
   727 : umov   x15, v0.4s, #0                ; 0f 3c 04 0e  
   728 : umov   x0, v31.4s, #0                ; e0 3f 04 0e  
   729 : umov   x0, v0.4s, #0x03              ; 00 3c 1c 0e  
   730 : umov   x0, v0.2d, #0                 ; 00 3c 08 4e  
   731 : umov   x15, v0.2d, #0                ; 0f 3c 08 4e  
   732 : umov   x0, v31.2d, #0                ; e0 3f 08 4e  
   733 : umov   x0, v0.2d, #0x01              ; 00 3c 18 4e  
   734 : ins    v0.16b, #0, x0                ; 00 1c 01 4e  
   735 : ins    v31.16b, #0, x0               ; 1f 1c 01 4e  
   736 : ins    v0.16b, #0x0f, x0             ; 00 1c 1f 4e  
   737 : ins    v0.16b, #0, x15               ; e0 1d 01 4e  
   738 : ins    v0.8h, #0, x0                 ; 00 1c 02 4e  
   739 : ins    v31.8h, #0, x0                ; 1f 1c 02 4e  
   740 : ins    v0.8h, #0x07, x0              ; 00 1c 1e 4e  
   741 : ins    v0.8h, #0, x15                ; e0 1d 02 4e  
   742 : ins    v0.4s, #0, x0                 ; 00 1c 04 4e  
   743 : ins    v31.4s, #0, x0                ; 1f 1c 04 4e  
   744 : ins    v0.4s, #0x03, x0              ; 00 1c 1c 4e  
   745 : ins    v0.4s, #0, x15                ; e0 1d 04 4e  
   746 : ins    v0.2d, #0, x0                 ; 00 1c 08 4e  
   747 : ins    v31.2d, #0, x0                ; 1f 1c 08 4e  
   748 : ins    v0.2d, #0x01, x0              ; 00 1c 18 4e  
   749 : ins    v0.2d, #0, x15                ; e0 1d 08 4e  
   750 : bsl    v0.16b, v0.16b, v0.16b        ; 00 1c 60 6e  
   751 : bsl    v31.16b, v0.16b, v0.16b       ; 1f 1c 60 6e  
   752 : bsl    v0.16b, v31.16b, v0.16b       ; e0 1f 60 6e  
   753 : bsl    v0.16b, v0.16b, v31.16b       ; 00 1c 7f 6e  
