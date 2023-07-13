instruction_set_test()
     0 : movn   x0, #0                           ; 00 00 80 92  
     1 : movn   x15, #0                          ; 0f 00 80 92  
     2 : movn   x0, #0xffff                      ; e0 ff 9f 92  
     3 : str    #0x01, x0, x0, #0x20             ; 00 80 00 f9  
     4 : str    #0x01, x0, x0, #0x20             ; 00 80 00 f9  
     5 : str    #0x01, x0, x0, x0                ; 00 68 20 f8  
     6 : str    #0x01, x0, x0, x0                ; 00 68 20 f8  
     7 : str    #0, w0, x0, #0x40                ; 00 00 01 b9  
     8 : str    #0, w0, x0, #0x40                ; 00 00 01 b9  
     9 : str    #0, w0, x0, x0                   ; 00 68 20 b8  
    10 : str    #0, w0, x0, x0                   ; 00 68 20 b8  
    11 : strh   w0, x0, #0x80                    ; 00 00 02 79  
    12 : strh   w0, x0, #0x80                    ; 00 00 02 79  
    13 : strh   w0, x0, x0                       ; 00 68 20 78  
    14 : strh   w0, x0, x0                       ; 00 68 20 78  
    15 : strb   w0, x0, #0x100                   ; 00 00 04 39  
    16 : strb   w0, x0, #0x100                   ; 00 00 04 39  
    17 : strb   w0, x0, x0                       ; 00 68 20 38  
    18 : strb   w0, x0, x0                       ; 00 68 20 38  
    19 : stp    x0, x0, x0, #0                   ; 00 00 00 a9  
    20 : stp    x0, x0, x15, #0                  ; e0 01 00 a9  
    21 : stp    x0, x0, x0, #0x3f                ; 00 80 1f a9  
    22 : stp    x0, x0, x0, #0xffffffffffffffc0  ; 00 00 20 a9  
    23 : stp    x15, x0, x0, #0                  ; 0f 00 00 a9  
    24 : stp    x0, x15, x0, #0                  ; 00 3c 00 a9  
    25 : ldr    x0, [x0], #0                     ; 00 00 40 f9  
    26 : ldr    x0, [x0], #0x20                  ; 00 80 40 f9  
    27 : ldr    x15, [x0], #0x20                 ; 0f 80 40 f9  
    28 : ldr    x0, [x15], #0x20                 ; e0 81 40 f9  
    29 : ldr    x0, [x0], x0                     ; 00 68 60 f8  
    30 : ldr    x15, [x0], x0                    ; 0f 68 60 f8  
    31 : ldr    x0, [x15], x0                    ; e0 69 60 f8  
    32 : ldr    x0, [x0], x15                    ; 00 68 6f f8  
    33 : ldr    x0, [x0], #0                     ; 00 00 40 f9  
    34 : ldr    x0, [x0], #0x20                  ; 00 80 40 f9  
    35 : ldr    x15, [x0], #0x20                 ; 0f 80 40 f9  
    36 : ldr    x0, [x15], #0x20                 ; e0 81 40 f9  
    37 : ldr    x0, [x0], x0                     ; 00 68 60 f8  
    38 : ldr    x15, [x0], x0                    ; 0f 68 60 f8  
    39 : ldr    x0, [x15], x0                    ; e0 69 60 f8  
    40 : ldr    x0, [x0], x15                    ; 00 68 6f f8  
    41 : ldr    x0, [x0], #0                     ; 00 00 40 b9  
    42 : ldr    x0, [x0], #0x40                  ; 00 00 41 b9  
    43 : ldr    x15, [x0], #0x40                 ; 0f 00 41 b9  
    44 : ldr    x0, [x15], #0x40                 ; e0 01 41 b9  
    45 : ldr    x0, [x0], x0                     ; 00 68 60 b8  
    46 : ldr    x15, [x0], x0                    ; 0f 68 60 b8  
    47 : ldr    x0, [x15], x0                    ; e0 69 60 b8  
    48 : ldr    x0, [x0], x15                    ; 00 68 6f b8  
    49 : ldrsw  x0, [x0], #0                     ; 00 00 80 b9  
    50 : ldrsw  x0, [x0], #0x40                  ; 00 00 81 b9  
    51 : ldrsw  x15, [x0], #0x40                 ; 0f 00 81 b9  
    52 : ldrsw  x0, [x15], #0x40                 ; e0 01 81 b9  
    53 : ldrsw  x0, [x0], x0                     ; 00 68 a0 b8  
    54 : ldrsw  x15, [x0], x0                    ; 0f 68 a0 b8  
    55 : ldrsw  x0, [x15], x0                    ; e0 69 a0 b8  
    56 : ldrsw  x0, [x0], x15                    ; 00 68 af b8  
    57 : ldrh   x0, [x0], #0                     ; 00 00 40 79  
    58 : ldrh   x0, [x0], #0x80                  ; 00 00 42 79  
    59 : ldrh   x15, [x0], #0x80                 ; 0f 00 42 79  
    60 : ldrh   x0, [x15], #0x80                 ; e0 01 42 79  
    61 : ldrh   x0, [x0], x0                     ; 00 68 60 78  
    62 : ldrh   x15, [x0], x0                    ; 0f 68 60 78  
    63 : ldrh   x0, [x15], x0                    ; e0 69 60 78  
    64 : ldrh   x0, [x0], x15                    ; 00 68 6f 78  
    65 : ldrsh  x0, [x0], #0                     ; 00 00 80 79  
    66 : ldrsh  x0, [x0], #0x80                  ; 00 00 82 79  
    67 : ldrsh  x15, [x0], #0x80                 ; 0f 00 82 79  
    68 : ldrsh  x0, [x15], #0x80                 ; e0 01 82 79  
    69 : ldrsh  x0, [x0], x0                     ; 00 68 a0 78  
    70 : ldrsh  x15, [x0], x0                    ; 0f 68 a0 78  
    71 : ldrsh  x0, [x15], x0                    ; e0 69 a0 78  
    72 : ldrsh  x0, [x0], x15                    ; 00 68 af 78  
    73 : ldrb   x0, [x0], #0                     ; 00 00 40 39  
    74 : ldrb   x0, [x0], #0x100                 ; 00 00 44 39  
    75 : ldrb   x15, [x0], #0x100                ; 0f 00 44 39  
    76 : ldrb   x0, [x15], #0x100                ; e0 01 44 39  
    77 : ldrb   x0, [x0], x0                     ; 00 68 60 38  
    78 : ldrb   x15, [x0], x0                    ; 0f 68 60 38  
    79 : ldrb   x0, [x15], x0                    ; e0 69 60 38  
    80 : ldrb   x0, [x0], x15                    ; 00 68 6f 38  
    81 : ldrsb  x0, [x0], #0                     ; 00 00 80 39  
    82 : ldrsb  x0, [x0], #0x100                 ; 00 00 84 39  
    83 : ldrsb  x15, [x0], #0x100                ; 0f 00 84 39  
    84 : ldrsb  x0, [x15], #0x100                ; e0 01 84 39  
    85 : ldrsb  x0, [x0], x0                     ; 00 68 a0 38  
    86 : ldrsb  x15, [x0], x0                    ; 0f 68 a0 38  
    87 : ldrsb  x0, [x15], x0                    ; e0 69 a0 38  
    88 : ldrsb  x0, [x0], x15                    ; 00 68 af 38  
    89 : ldp    x0, x7, x15, #0                  ; e0 1d 40 a9  
    90 : ldp    x7, x0, x15, #0                  ; e7 01 40 a9  
    91 : ldp    x7, x15, x0, #0                  ; 07 3c 40 a9  
    92 : ldp    x0, x7, x15, #0x3f               ; e0 9d 5f a9  
    93 : ldp    x0, x7, x15, #0xffffffffffffffc0 ; e0 1d 60 a9  
    94 : csel   x0, x0, x0, #0                   ; 00 00 80 9a  
    95 : csel   x0, x0, x0, #0x01                ; 00 10 80 9a  
    96 : csel   x0, x0, x0, #0x0b                ; 00 b0 80 9a  
    97 : csel   x0, x0, x0, #0x0c                ; 00 c0 80 9a  
    98 : csel   x0, x0, x0, #0x0d                ; 00 d0 80 9a  
    99 : csel   x0, x0, x0, #0x0a                ; 00 a0 80 9a  
   100 : csel   x0, x0, x0, #0x04                ; 00 40 80 9a  
   101 : csel   x0, x0, x0, #0x05                ; 00 50 80 9a  
   102 : csel   x15, x0, x0, #0                  ; 0f 00 80 9a  
   103 : csel   x0, x15, x0, #0                  ; e0 01 80 9a  
   104 : csel   x0, x0, x15, #0                  ; 00 00 8f 9a  
   105 : cset   x0, #0x01                        ; e0 17 9f 9a  
   106 : cset   x0, #0                           ; e0 07 9f 9a  
   107 : cset   x0, #0x0a                        ; e0 a7 9f 9a  
   108 : cset   x0, #0x0d                        ; e0 d7 9f 9a  
   109 : cset   x0, #0x0c                        ; e0 c7 9f 9a  
   110 : cset   x0, #0x0b                        ; e0 b7 9f 9a  
   111 : cset   x0, #0x05                        ; e0 57 9f 9a  
   112 : cset   x0, #0x04                        ; e0 47 9f 9a  
   113 : cset   x15, #0x01                       ; ef 17 9f 9a  
   114 : cinc   x0, x0, #0x01                    ; 00 14 80 9a  
   115 : cinc   x0, x0, #0                       ; 00 04 80 9a  
   116 : cinc   x0, x0, #0x0a                    ; 00 a4 80 9a  
   117 : cinc   x0, x0, #0x0d                    ; 00 d4 80 9a  
   118 : cinc   x0, x0, #0x0c                    ; 00 c4 80 9a  
   119 : cinc   x0, x0, #0x0b                    ; 00 b4 80 9a  
   120 : cinc   x0, x0, #0x05                    ; 00 54 80 9a  
   121 : cinc   x0, x0, #0x04                    ; 00 44 80 9a  
   122 : cinc   x15, x0, #0x01                   ; 0f 14 80 9a  
   123 : cinc   x0, x15, #0x01                   ; e0 15 8f 9a  
   124 : cneg   x0, x0, #0x01                    ; 00 14 80 da  
   125 : cneg   x0, x0, #0                       ; 00 04 80 da  
   126 : cneg   x0, x0, #0x0a                    ; 00 a4 80 da  
   127 : cneg   x0, x0, #0x0d                    ; 00 d4 80 da  
   128 : cneg   x0, x0, #0x0c                    ; 00 c4 80 da  
   129 : cneg   x0, x0, #0x0b                    ; 00 b4 80 da  
   130 : cneg   x0, x0, #0x05                    ; 00 54 80 da  
   131 : cneg   x0, x0, #0x04                    ; 00 44 80 da  
   132 : cneg   x15, x0, #0x01                   ; 0f 14 80 da  
   133 : cneg   x0, x15, #0x01                   ; e0 15 8f da  
   134 : asr    x0, x0, #0x3f                    ; 00 fc 7f 93  
   135 : asr    x15, x0, #0x3f                   ; 0f fc 7f 93  
   136 : asr    x0, x15, #0x3f                   ; e0 fd 7f 93  
   137 : asr    x0, x0, x0                       ; 00 28 c0 9a  
   138 : asr    x15, x0, x0                      ; 0f 28 c0 9a  
   139 : asr    x0, x15, x0                      ; e0 29 c0 9a  
   140 : asr    x0, x0, x15                      ; 00 28 cf 9a  
   141 : lsr    x0, x0, #0x3f                    ; 00 fc 7f d3  
   142 : lsr    x15, x0, #0x3f                   ; 0f fc 7f d3  
   143 : lsr    x0, x15, #0x3f                   ; e0 fd 7f d3  
   144 : lsr    x0, x0, x0                       ; 00 24 c0 9a  
   145 : lsr    x15, x0, x0                      ; 0f 24 c0 9a  
   146 : lsr    x0, x15, x0                      ; e0 25 c0 9a  
   147 : lsr    x0, x0, x15                      ; 00 24 cf 9a  
   148 : lsl    x0, x0, #0x01, #0                ; 00 00 41 d3  
   149 : lsl    x0, x0, #0x21, #0x20             ; 00 80 61 d3  
   150 : lsl    x0, x0, #0x31, #0x30             ; 00 c0 71 d3  
   151 : lsl    x0, x0, #0x39, #0x38             ; 00 e0 79 d3  
   152 : lsl    x0, x0, #0x3f, #0x3e             ; 00 f8 7f d3  
   153 : lsl    x0, x0, #0, #0x3f                ; 00 fc 40 d3  
   154 : lsl    x15, x0, #0x01, #0               ; 0f 00 41 d3  
   155 : lsl    x0, x15, #0x01, #0               ; e0 01 41 d3  
   156 : lsl    x0, x0, x0                       ; 00 20 c0 9a  
   157 : lsl    x15, x0, x0                      ; 0f 20 c0 9a  
   158 : lsl    x0, x15, x0                      ; e0 21 c0 9a  
   159 : lsl    x0, x0, x15                      ; 00 20 cf 9a  
   160 : and    x0, x0, x0                       ; 00 00 00 8a  
   161 : and    x15, x0, x0                      ; 0f 00 00 8a  
   162 : and    x0, x15, x0                      ; e0 01 00 8a  
   163 : and    x0, x0, x15                      ; 00 00 0f 8a  
   164 : and    x0, x0, #0x01, #0, #0x07         ; 00 1c 40 92  
   165 : and    x0, x0, #0x01, #0x38, #0x07      ; 00 1c 78 92  
   166 : and    x0, x0, #0x01, #0x08, #0x07      ; 00 1c 48 92  
   167 : and    x15, x0, #0x01, #0, #0x07        ; 0f 1c 40 92  
   168 : and    x0, x15, #0x01, #0, #0x07        ; e0 1d 40 92  
   169 : orr    x0, x0, x0                       ; 00 00 00 aa  
   170 : orr    x15, x0, x0                      ; 0f 00 00 aa  
   171 : orr    x0, x15, x0                      ; e0 01 00 aa  
   172 : orr    x0, x0, x15                      ; 00 00 0f aa  
   173 : orr    x0, x0, #0x01, #0, #0x07         ; 00 1c 40 b2  
   174 : orr    x0, x0, #0x01, #0x38, #0x07      ; 00 1c 78 b2  
   175 : orr    x0, x0, #0x01, #0x08, #0x07      ; 00 1c 48 b2  
   176 : orr    x15, x0, #0x01, #0, #0x07        ; 0f 1c 40 b2  
   177 : orr    x0, x15, #0x01, #0, #0x07        ; e0 1d 40 b2  
   178 : eor    x0, x0, x0                       ; 00 00 00 ca  
   179 : eor    x15, x0, x0                      ; 0f 00 00 ca  
   180 : eor    x0, x15, x0                      ; e0 01 00 ca  
   181 : eor    x0, x0, x15                      ; 00 00 0f ca  
   182 : eor    x0, x0, #0x01, #0, #0x07         ; 00 1c 40 d2  
   183 : eor    x0, x0, #0x01, #0x38, #0x07      ; 00 1c 78 d2  
   184 : eor    x0, x0, #0x01, #0x08, #0x07      ; 00 1c 48 d2  
   185 : eor    x15, x0, #0x01, #0, #0x07        ; 0f 1c 40 d2  
   186 : eor    x0, x15, #0x01, #0, #0x07        ; e0 1d 40 d2  
   187 : mvn    x0, x0                           ; e0 03 20 aa  
   188 : mvn    x15, x0                          ; ef 03 20 aa  
   189 : mvn    x0, x15                          ; e0 03 2f aa  
   190 : neg    x0, x0                           ; e0 03 00 cb  
   191 : neg    x15, x0                          ; ef 03 00 cb  
   192 : neg    x0, x15                          ; e0 03 0f cb  
   193 : ldr    v0.2d, x0, #0                    ; 00 00 c0 3d  
   194 : ldr    v0.2d, x0, #0x801                ; 00 04 e0 3d  
   195 : ldr    v31.2d, x0, #0                   ; 1f 00 c0 3d  
   196 : ldr    v0.2d, x15, #0                   ; e0 01 c0 3d  
   197 : ldr    v0.2d, x0, x0                    ; 00 68 e0 3c  
   198 : ldr    v31.2d, x0, x0                   ; 1f 68 e0 3c  
   199 : ldr    v0.2d, x15, x0                   ; e0 69 e0 3c  
   200 : ldr    v0.2d, x0, x15                   ; 00 68 ef 3c  
   201 : str    v0.2d, x0, #0                    ; 00 00 80 3d  
   202 : str    v0.2d, x0, #0x801                ; 00 04 a0 3d  
   203 : str    v31.2d, x0, #0                   ; 1f 00 80 3d  
   204 : str    v0.2d, x15, #0                   ; e0 01 80 3d  
   205 : str    v0.2d, x0, x0                    ; 00 68 a0 3c  
   206 : str    v31.2d, x0, x0                   ; 1f 68 a0 3c  
   207 : str    v0.2d, x15, x0                   ; e0 69 a0 3c  
   208 : str    v0.2d, x0, x15                   ; 00 68 af 3c  
   209 : add    v0.16b, v0.16b, v0.16b           ; 00 84 20 4e  
   210 : add    v31.16b, v0.16b, v0.16b          ; 1f 84 20 4e  
   211 : add    v0.16b, v31.16b, v0.16b          ; e0 87 20 4e  
   212 : add    v0.16b, v0.16b, v31.16b          ; 00 84 3f 4e  
   213 : add    v0.8h, v0.8h, v0.8h              ; 00 84 60 4e  
   214 : add    v31.8h, v0.8h, v0.8h             ; 1f 84 60 4e  
   215 : add    v0.8h, v31.8h, v0.8h             ; e0 87 60 4e  
   216 : add    v0.8h, v0.8h, v31.8h             ; 00 84 7f 4e  
   217 : add    v0.4s, v0.4s, v0.4s              ; 00 84 a0 4e  
   218 : add    v31.4s, v0.4s, v0.4s             ; 1f 84 a0 4e  
   219 : add    v0.4s, v31.4s, v0.4s             ; e0 87 a0 4e  
   220 : add    v0.4s, v0.4s, v31.4s             ; 00 84 bf 4e  
   221 : add    v0.2d, v0.2d, v0.2d              ; 00 84 e0 4e  
   222 : add    v31.2d, v0.2d, v0.2d             ; 1f 84 e0 4e  
   223 : add    v0.2d, v31.2d, v0.2d             ; e0 87 e0 4e  
   224 : add    v0.2d, v0.2d, v31.2d             ; 00 84 ff 4e  
   225 : fadd   v0.8h, v0.8h, v0.8h              ; 00 14 40 4e  
   226 : fadd   v31.8h, v0.8h, v0.8h             ; 1f 14 40 4e  
   227 : fadd   v0.8h, v31.8h, v0.8h             ; e0 17 40 4e  
   228 : fadd   v0.8h, v0.8h, v31.8h             ; 00 14 5f 4e  
   229 : fadd   v0.4s, v0.4s, v0.4s              ; 00 d4 20 4e  
   230 : fadd   v31.4s, v0.4s, v0.4s             ; 1f d4 20 4e  
   231 : fadd   v0.4s, v31.4s, v0.4s             ; e0 d7 20 4e  
   232 : fadd   v0.4s, v0.4s, v31.4s             ; 00 d4 3f 4e  
   233 : fadd   v0.2d, v0.2d, v0.2d              ; 00 d4 60 4e  
   234 : fadd   v31.2d, v0.2d, v0.2d             ; 1f d4 60 4e  
   235 : fadd   v0.2d, v31.2d, v0.2d             ; e0 d7 60 4e  
   236 : fadd   v0.2d, v0.2d, v31.2d             ; 00 d4 7f 4e  
   237 : sub    v0.16b, v0.16b, v0.16b           ; 00 84 20 6e  
   238 : sub    v31.16b, v0.16b, v0.16b          ; 1f 84 20 6e  
   239 : sub    v0.16b, v31.16b, v0.16b          ; e0 87 20 6e  
   240 : sub    v0.16b, v0.16b, v31.16b          ; 00 84 3f 6e  
   241 : sub    v0.8h, v0.8h, v0.8h              ; 00 84 60 6e  
   242 : sub    v31.8h, v0.8h, v0.8h             ; 1f 84 60 6e  
   243 : sub    v0.8h, v31.8h, v0.8h             ; e0 87 60 6e  
   244 : sub    v0.8h, v0.8h, v31.8h             ; 00 84 7f 6e  
   245 : sub    v0.4s, v0.4s, v0.4s              ; 00 84 a0 6e  
   246 : sub    v31.4s, v0.4s, v0.4s             ; 1f 84 a0 6e  
   247 : sub    v0.4s, v31.4s, v0.4s             ; e0 87 a0 6e  
   248 : sub    v0.4s, v0.4s, v31.4s             ; 00 84 bf 6e  
   249 : sub    v0.2d, v0.2d, v0.2d              ; 00 84 e0 6e  
   250 : sub    v31.2d, v0.2d, v0.2d             ; 1f 84 e0 6e  
   251 : sub    v0.2d, v31.2d, v0.2d             ; e0 87 e0 6e  
   252 : sub    v0.2d, v0.2d, v31.2d             ; 00 84 ff 6e  
   253 : fsub   v0.8h, v0.8h, v0.8h              ; 00 14 c0 4e  
   254 : fsub   v31.8h, v0.8h, v0.8h             ; 1f 14 c0 4e  
   255 : fsub   v0.8h, v31.8h, v0.8h             ; e0 17 c0 4e  
   256 : fsub   v0.8h, v0.8h, v31.8h             ; 00 14 df 4e  
   257 : fsub   v0.4s, v0.4s, v0.4s              ; 00 d4 a0 4e  
   258 : fsub   v31.4s, v0.4s, v0.4s             ; 1f d4 a0 4e  
   259 : fsub   v0.4s, v31.4s, v0.4s             ; e0 d7 a0 4e  
   260 : fsub   v0.4s, v0.4s, v31.4s             ; 00 d4 bf 4e  
   261 : fsub   v0.2d, v0.2d, v0.2d              ; 00 d4 e0 4e  
   262 : fsub   v31.2d, v0.2d, v0.2d             ; 1f d4 e0 4e  
   263 : fsub   v0.2d, v31.2d, v0.2d             ; e0 d7 e0 4e  
   264 : fsub   v0.2d, v0.2d, v31.2d             ; 00 d4 ff 4e  
   265 : mul    v0.16b, v0.16b, v0.16b           ; 00 9c 20 4e  
   266 : mul    v31.16b, v0.16b, v0.16b          ; 1f 9c 20 4e  
   267 : mul    v0.16b, v31.16b, v0.16b          ; e0 9f 20 4e  
   268 : mul    v0.16b, v0.16b, v31.16b          ; 00 9c 3f 4e  
   269 : mul    v0.8h, v0.8h, v0.8h              ; 00 9c 60 4e  
   270 : mul    v31.8h, v0.8h, v0.8h             ; 1f 9c 60 4e  
   271 : mul    v0.8h, v31.8h, v0.8h             ; e0 9f 60 4e  
   272 : mul    v0.8h, v0.8h, v31.8h             ; 00 9c 7f 4e  
   273 : mul    v0.4s, v0.4s, v0.4s              ; 00 9c a0 4e  
   274 : mul    v31.4s, v0.4s, v0.4s             ; 1f 9c a0 4e  
   275 : mul    v0.4s, v31.4s, v0.4s             ; e0 9f a0 4e  
   276 : mul    v0.4s, v0.4s, v31.4s             ; 00 9c bf 4e  
   277 : fmul   v0.8h, v0.8h, v0.8h              ; 00 1c 40 6e  
   278 : fmul   v31.8h, v0.8h, v0.8h             ; 1f 1c 40 6e  
   279 : fmul   v0.8h, v31.8h, v0.8h             ; e0 1f 40 6e  
   280 : fmul   v0.8h, v0.8h, v31.8h             ; 00 1c 5f 6e  
   281 : fmul   v0.4s, v0.4s, v0.4s              ; 00 dc 20 6e  
   282 : fmul   v31.4s, v0.4s, v0.4s             ; 1f dc 20 6e  
   283 : fmul   v0.4s, v31.4s, v0.4s             ; e0 df 20 6e  
   284 : fmul   v0.4s, v0.4s, v31.4s             ; 00 dc 3f 6e  
   285 : fmul   v0.2d, v0.2d, v0.2d              ; 00 dc 60 6e  
   286 : fmul   v31.2d, v0.2d, v0.2d             ; 1f dc 60 6e  
   287 : fmul   v0.2d, v31.2d, v0.2d             ; e0 df 60 6e  
   288 : fmul   v0.2d, v0.2d, v31.2d             ; 00 dc 7f 6e  
   289 : fdiv   v0.4s, v0.4s, v0.4s              ; 00 fc 20 6e  
   290 : fdiv   v31.4s, v0.4s, v0.4s             ; 1f fc 20 6e  
   291 : fdiv   v0.4s, v31.4s, v0.4s             ; e0 ff 20 6e  
   292 : fdiv   v0.4s, v0.4s, v31.4s             ; 00 fc 3f 6e  
   293 : fdiv   v0.2d, v0.2d, v0.2d              ; 00 fc 60 6e  
   294 : fdiv   v31.2d, v0.2d, v0.2d             ; 1f fc 60 6e  
   295 : fdiv   v0.2d, v31.2d, v0.2d             ; e0 ff 60 6e  
   296 : fdiv   v0.2d, v0.2d, v31.2d             ; 00 fc 7f 6e  
   297 : fdiv   v0.8h, v0.8h, v0.8h              ; 00 3c 40 6e  
   298 : fdiv   v31.8h, v0.8h, v0.8h             ; 1f 3c 40 6e  
   299 : fdiv   v0.8h, v31.8h, v0.8h             ; e0 3f 40 6e  
   300 : fdiv   v0.8h, v0.8h, v31.8h             ; 00 3c 5f 6e  
   301 : shl    v0.16b, v0.16b, #0x01, #0x01     ; 00 54 09 4f  
   302 : shl    v31.16b, v0.16b, #0x01, #0x01    ; 1f 54 09 4f  
   303 : shl    v0.16b, v31.16b, #0x01, #0x01    ; e0 57 09 4f  
   304 : shl    v0.16b, v0.16b, #0x01, #0x07     ; 00 54 0f 4f  
   305 : shl    v0.8h, v0.8h, #0x02, #0x01       ; 00 54 11 4f  
   306 : shl    v31.8h, v0.8h, #0x02, #0x01      ; 1f 54 11 4f  
   307 : shl    v0.8h, v31.8h, #0x02, #0x01      ; e0 57 11 4f  
   308 : shl    v0.8h, v0.8h, #0x03, #0x07       ; 00 54 1f 4f  
   309 : shl    v0.4s, v0.4s, #0x04, #0x01       ; 00 54 21 4f  
   310 : shl    v31.4s, v0.4s, #0x04, #0x01      ; 1f 54 21 4f  
   311 : shl    v0.4s, v31.4s, #0x04, #0x01      ; e0 57 21 4f  
   312 : shl    v0.4s, v0.4s, #0x07, #0x07       ; 00 54 3f 4f  
   313 : shl    v0.2d, v0.2d, #0x08, #0x01       ; 00 54 41 4f  
   314 : shl    v31.2d, v0.2d, #0x08, #0x01      ; 1f 54 41 4f  
   315 : shl    v0.2d, v31.2d, #0x08, #0x01      ; e0 57 41 4f  
   316 : shl    v0.2d, v0.2d, #0x0f, #0x07       ; 00 54 7f 4f  
   317 : ushl   v0.16b, v0.16b, v0.16b           ; 00 44 20 6e  
   318 : ushl   v31.16b, v0.16b, v0.16b          ; 1f 44 20 6e  
   319 : ushl   v0.16b, v31.16b, v0.16b          ; e0 47 20 6e  
   320 : ushl   v0.16b, v0.16b, v31.16b          ; 00 44 3f 6e  
   321 : ushl   v0.8h, v0.8h, v0.8h              ; 00 44 60 6e  
   322 : ushl   v31.8h, v0.8h, v0.8h             ; 1f 44 60 6e  
   323 : ushl   v0.8h, v31.8h, v0.8h             ; e0 47 60 6e  
   324 : ushl   v0.8h, v0.8h, v31.8h             ; 00 44 7f 6e  
   325 : ushl   v0.4s, v0.4s, v0.4s              ; 00 44 a0 6e  
   326 : ushl   v31.4s, v0.4s, v0.4s             ; 1f 44 a0 6e  
   327 : ushl   v0.4s, v31.4s, v0.4s             ; e0 47 a0 6e  
   328 : ushl   v0.4s, v0.4s, v31.4s             ; 00 44 bf 6e  
   329 : ushl   v0.2d, v0.2d, v0.2d              ; 00 44 e0 6e  
   330 : ushl   v31.2d, v0.2d, v0.2d             ; 1f 44 e0 6e  
   331 : ushl   v0.2d, v31.2d, v0.2d             ; e0 47 e0 6e  
   332 : ushl   v0.2d, v0.2d, v31.2d             ; 00 44 ff 6e  
   333 : sshl   v0.16b, v0.16b, v0.16b           ; 00 44 20 4e  
   334 : sshl   v31.16b, v0.16b, v0.16b          ; 1f 44 20 4e  
   335 : sshl   v0.16b, v31.16b, v0.16b          ; e0 47 20 4e  
   336 : sshl   v0.16b, v0.16b, v31.16b          ; 00 44 3f 4e  
   337 : sshl   v0.8h, v0.8h, v0.8h              ; 00 44 60 4e  
   338 : sshl   v31.8h, v0.8h, v0.8h             ; 1f 44 60 4e  
   339 : sshl   v0.8h, v31.8h, v0.8h             ; e0 47 60 4e  
   340 : sshl   v0.8h, v0.8h, v31.8h             ; 00 44 7f 4e  
   341 : sshl   v0.4s, v0.4s, v0.4s              ; 00 44 a0 4e  
   342 : sshl   v31.4s, v0.4s, v0.4s             ; 1f 44 a0 4e  
   343 : sshl   v0.4s, v31.4s, v0.4s             ; e0 47 a0 4e  
   344 : sshl   v0.4s, v0.4s, v31.4s             ; 00 44 bf 4e  
   345 : sshl   v0.2d, v0.2d, v0.2d              ; 00 44 e0 4e  
   346 : sshl   v31.2d, v0.2d, v0.2d             ; 1f 44 e0 4e  
   347 : sshl   v0.2d, v31.2d, v0.2d             ; e0 47 e0 4e  
   348 : sshl   v0.2d, v0.2d, v31.2d             ; 00 44 ff 4e  
   349 : ushr   v0.16b, v0.16b, #0x01, #0x07     ; 00 04 0f 6f  
   350 : ushr   v31.16b, v0.16b, #0x01, #0x07    ; 1f 04 0f 6f  
   351 : ushr   v0.16b, v31.16b, #0x01, #0x07    ; e0 07 0f 6f  
   352 : ushr   v0.16b, v0.16b, #0x01, #0x01     ; 00 04 09 6f  
   353 : ushr   v0.8h, v0.8h, #0x03, #0x07       ; 00 04 1f 6f  
   354 : ushr   v31.8h, v0.8h, #0x03, #0x07      ; 1f 04 1f 6f  
   355 : ushr   v0.8h, v31.8h, #0x03, #0x07      ; e0 07 1f 6f  
   356 : ushr   v0.8h, v0.8h, #0x02, #0x01       ; 00 04 11 6f  
   357 : ushr   v0.4s, v0.4s, #0x07, #0x07       ; 00 04 3f 6f  
   358 : ushr   v31.4s, v0.4s, #0x07, #0x07      ; 1f 04 3f 6f  
   359 : ushr   v0.4s, v31.4s, #0x07, #0x07      ; e0 07 3f 6f  
   360 : ushr   v0.4s, v0.4s, #0x04, #0x01       ; 00 04 21 6f  
   361 : ushr   v0.2d, v0.2d, #0x0f, #0x07       ; 00 04 7f 6f  
   362 : ushr   v31.2d, v0.2d, #0x0f, #0x07      ; 1f 04 7f 6f  
   363 : ushr   v0.2d, v31.2d, #0x0f, #0x07      ; e0 07 7f 6f  
   364 : ushr   v0.2d, v0.2d, #0x08, #0x01       ; 00 04 41 6f  
   365 : sshr   v0.16b, v0.16b, #0x01, #0x07     ; 00 04 0f 4f  
   366 : sshr   v31.16b, v0.16b, #0x01, #0x07    ; 1f 04 0f 4f  
   367 : sshr   v0.16b, v31.16b, #0x01, #0x07    ; e0 07 0f 4f  
   368 : sshr   v0.16b, v0.16b, #0x01, #0x01     ; 00 04 09 4f  
   369 : sshr   v0.8h, v0.8h, #0x03, #0x07       ; 00 04 1f 4f  
   370 : sshr   v31.8h, v0.8h, #0x03, #0x07      ; 1f 04 1f 4f  
   371 : sshr   v0.8h, v31.8h, #0x03, #0x07      ; e0 07 1f 4f  
   372 : sshr   v0.8h, v0.8h, #0x02, #0x01       ; 00 04 11 4f  
   373 : sshr   v0.4s, v0.4s, #0x07, #0x07       ; 00 04 3f 4f  
   374 : sshr   v31.4s, v0.4s, #0x07, #0x07      ; 1f 04 3f 4f  
   375 : sshr   v0.4s, v31.4s, #0x07, #0x07      ; e0 07 3f 4f  
   376 : sshr   v0.4s, v0.4s, #0x04, #0x01       ; 00 04 21 4f  
   377 : sshr   v0.2d, v0.2d, #0x0f, #0x07       ; 00 04 7f 4f  
   378 : sshr   v31.2d, v0.2d, #0x0f, #0x07      ; 1f 04 7f 4f  
   379 : sshr   v0.2d, v31.2d, #0x0f, #0x07      ; e0 07 7f 4f  
   380 : sshr   v0.2d, v0.2d, #0x08, #0x01       ; 00 04 41 4f  
   381 : and    v0.16b, v0.16b, v0.16b           ; 00 1c 20 4e  
   382 : and    v31.16b, v0.16b, v0.16b          ; 1f 1c 20 4e  
   383 : and    v0.16b, v31.16b, v0.16b          ; e0 1f 20 4e  
   384 : and    v0.16b, v0.16b, v31.16b          ; 00 1c 3f 4e  
   385 : orr    v0.16b, v0.16b, v0.16b           ; 00 1c a0 4e  
   386 : orr    v31.16b, v0.16b, v0.16b          ; 1f 1c a0 4e  
   387 : orr    v0.16b, v31.16b, v0.16b          ; e0 1f a0 4e  
   388 : orr    v0.16b, v0.16b, v31.16b          ; 00 1c bf 4e  
   389 : eor    v0.16b, v0.16b, v0.16b           ; 00 1c 20 6e  
   390 : eor    v31.16b, v0.16b, v0.16b          ; 1f 1c 20 6e  
   391 : eor    v0.16b, v31.16b, v0.16b          ; e0 1f 20 6e  
   392 : eor    v0.16b, v0.16b, v31.16b          ; 00 1c 3f 6e  
   393 : mvn    v0.16b, v0.16b                   ; 00 58 20 6e  
   394 : mvn    v31.16b, v0.16b                  ; 1f 58 20 6e  
   395 : mvn    v0.16b, v31.16b                  ; e0 5b 20 6e  
   396 : movi   v0.16b, #0, #0                   ; 00 e4 00 4f  
   397 : movi   v31.16b, #0, #0                  ; 1f e4 00 4f  
   398 : movi   v0.16b, #0x1f, #0x07             ; e0 e7 07 4f  
   399 : movi   v0.8h, #0, #0                    ; 00 84 00 4f  
   400 : movi   v31.8h, #0, #0                   ; 1f 84 00 4f  
   401 : movi   v0.8h, #0x1f, #0x07              ; e0 87 07 4f  
   402 : movi   v0.4s, #0, #0                    ; 00 04 00 4f  
   403 : movi   v31.4s, #0, #0                   ; 1f 04 00 4f  
   404 : movi   v0.4s, #0x1f, #0x07              ; e0 07 07 4f  
   405 : mvni   v0.8h, #0, #0                    ; 00 84 00 6f  
   406 : mvni   v31.8h, #0, #0                   ; 1f 84 00 6f  
   407 : mvni   v0.8h, #0x1f, #0x07              ; e0 87 07 6f  
   408 : mvni   v0.4s, #0, #0                    ; 00 04 00 6f  
   409 : mvni   v31.4s, #0, #0                   ; 1f 04 00 6f  
   410 : mvni   v0.4s, #0x1f, #0x07              ; e0 07 07 6f  
   411 : movi   v0.2d, #0, #0                    ; 00 e4 00 6f  
   412 : movi   v31.2d, #0, #0                   ; 1f e4 00 6f  
   413 : movi   v0.2d, #0x01, #0                 ; 20 e4 00 6f  
   414 : movi   v31.2d, #0x01, #0                ; 3f e4 00 6f  
   415 : movi   v0.2d, #0x0a, #0x05              ; 40 e5 05 6f  
   416 : movi   v31.2d, #0x0a, #0x05             ; 5f e5 05 6f  
   417 : fmin   v0.8h, v0.8h, v0.8h              ; 00 34 c0 4e  
   418 : fmin   v31.8h, v0.8h, v0.8h             ; 1f 34 c0 4e  
   419 : fmin   v0.8h, v31.8h, v0.8h             ; e0 37 c0 4e  
   420 : fmin   v0.8h, v0.8h, v31.8h             ; 00 34 df 4e  
   421 : fmin   v0.4s, v0.4s, v0.4s              ; 00 f4 a0 4e  
   422 : fmin   v31.4s, v0.4s, v0.4s             ; 1f f4 a0 4e  
   423 : fmin   v0.4s, v31.4s, v0.4s             ; e0 f7 a0 4e  
   424 : fmin   v0.4s, v0.4s, v31.4s             ; 00 f4 bf 4e  
   425 : fmin   v0.2d, v0.2d, v0.2d              ; 00 f4 e0 4e  
   426 : fmin   v31.2d, v0.2d, v0.2d             ; 1f f4 e0 4e  
   427 : fmin   v0.2d, v31.2d, v0.2d             ; e0 f7 e0 4e  
   428 : fmin   v0.2d, v0.2d, v31.2d             ; 00 f4 ff 4e  
   429 : fmax   v0.8h, v0.8h, v0.8h              ; 00 34 40 4e  
   430 : fmax   v31.8h, v0.8h, v0.8h             ; 1f 34 40 4e  
   431 : fmax   v0.8h, v31.8h, v0.8h             ; e0 37 40 4e  
   432 : fmax   v0.8h, v0.8h, v31.8h             ; 00 34 5f 4e  
   433 : fmax   v0.4s, v0.4s, v0.4s              ; 00 f4 20 4e  
   434 : fmax   v31.4s, v0.4s, v0.4s             ; 1f f4 20 4e  
   435 : fmax   v0.4s, v31.4s, v0.4s             ; e0 f7 20 4e  
   436 : fmax   v0.4s, v0.4s, v31.4s             ; 00 f4 3f 4e  
   437 : fmax   v0.2d, v0.2d, v0.2d              ; 00 f4 60 4e  
   438 : fmax   v31.2d, v0.2d, v0.2d             ; 1f f4 60 4e  
   439 : fmax   v0.2d, v31.2d, v0.2d             ; e0 f7 60 4e  
   440 : fmax   v0.2d, v0.2d, v31.2d             ; 00 f4 7f 4e  
   441 : fmla   v0.8h, v0.8h, v0.8h              ; 00 0c 40 4e  
   442 : fmla   v31.8h, v0.8h, v0.8h             ; 1f 0c 40 4e  
   443 : fmla   v0.8h, v31.8h, v0.8h             ; e0 0f 40 4e  
   444 : fmla   v0.8h, v0.8h, v31.8h             ; 00 0c 5f 4e  
   445 : fmla   v0.4s, v0.4s, v0.4s              ; 00 cc 20 4e  
   446 : fmla   v31.4s, v0.4s, v0.4s             ; 1f cc 20 4e  
   447 : fmla   v0.4s, v31.4s, v0.4s             ; e0 cf 20 4e  
   448 : fmla   v0.4s, v0.4s, v31.4s             ; 00 cc 3f 4e  
   449 : fmla   v0.2d, v0.2d, v0.2d              ; 00 cc 60 4e  
   450 : fmla   v31.2d, v0.2d, v0.2d             ; 1f cc 60 4e  
   451 : fmla   v0.2d, v31.2d, v0.2d             ; e0 cf 60 4e  
   452 : fmla   v0.2d, v0.2d, v31.2d             ; 00 cc 7f 4e  
   453 : fcvtzs v0.8h, v0.8h                     ; 00 b8 f9 4e  
   454 : fcvtzs v31.8h, v0.8h                    ; 1f b8 f9 4e  
   455 : fcvtzs v0.8h, v31.8h                    ; e0 bb f9 4e  
   456 : fcvtzs v0.4s, v0.4s                     ; 00 b8 a1 4e  
   457 : fcvtzs v31.4s, v0.4s                    ; 1f b8 a1 4e  
   458 : fcvtzs v0.4s, v31.4s                    ; e0 bb a1 4e  
   459 : fcvtzs v0.2d, v0.2d                     ; 00 b8 e1 4e  
   460 : fcvtzs v31.2d, v0.2d                    ; 1f b8 e1 4e  
   461 : fcvtzs v0.2d, v31.2d                    ; e0 bb e1 4e  
   462 : fcvtzu v0.8h, v0.8h                     ; 00 b8 f9 6e  
   463 : fcvtzu v31.8h, v0.8h                    ; 1f b8 f9 6e  
   464 : fcvtzu v0.8h, v31.8h                    ; e0 bb f9 6e  
   465 : fcvtzu v0.4s, v0.4s                     ; 00 b8 a1 6e  
   466 : fcvtzu v31.4s, v0.4s                    ; 1f b8 a1 6e  
   467 : fcvtzu v0.4s, v31.4s                    ; e0 bb a1 6e  
   468 : fcvtzu v0.2d, v0.2d                     ; 00 b8 e1 6e  
   469 : fcvtzu v31.2d, v0.2d                    ; 1f b8 e1 6e  
   470 : fcvtzu v0.2d, v31.2d                    ; e0 bb e1 6e  
   471 : fcvtms v0.8h, v0.8h                     ; 00 b8 79 4e  
   472 : fcvtms v31.8h, v0.8h                    ; 1f b8 79 4e  
   473 : fcvtms v0.8h, v31.8h                    ; e0 bb 79 4e  
   474 : fcvtms v0.4s, v0.4s                     ; 00 b8 21 4e  
   475 : fcvtms v31.4s, v0.4s                    ; 1f b8 21 4e  
   476 : fcvtms v0.4s, v31.4s                    ; e0 bb 21 4e  
   477 : fcvtms v0.2d, v0.2d                     ; 00 b8 61 4e  
   478 : fcvtms v31.2d, v0.2d                    ; 1f b8 61 4e  
   479 : fcvtms v0.2d, v31.2d                    ; e0 bb 61 4e  
   480 : fcvtmu v0.8h, v0.8h                     ; 00 b8 79 6e  
   481 : fcvtmu v31.8h, v0.8h                    ; 1f b8 79 6e  
   482 : fcvtmu v0.8h, v31.8h                    ; e0 bb 79 6e  
   483 : fcvtmu v0.4s, v0.4s                     ; 00 b8 21 6e  
   484 : fcvtmu v31.4s, v0.4s                    ; 1f b8 21 6e  
   485 : fcvtmu v0.4s, v31.4s                    ; e0 bb 21 6e  
   486 : fcvtmu v0.2d, v0.2d                     ; 00 b8 61 6e  
   487 : fcvtmu v31.2d, v0.2d                    ; 1f b8 61 6e  
   488 : fcvtmu v0.2d, v31.2d                    ; e0 bb 61 6e  
   489 : scvtf  v0.8h, v0.8h                     ; 00 d8 79 4e  
   490 : scvtf  v31.8h, v0.8h                    ; 1f d8 79 4e  
   491 : scvtf  v0.8h, v31.8h                    ; e0 db 79 4e  
   492 : scvtf  v0.4s, v0.4s                     ; 00 d8 21 4e  
   493 : scvtf  v31.4s, v0.4s                    ; 1f d8 21 4e  
   494 : scvtf  v0.4s, v31.4s                    ; e0 db 21 4e  
   495 : scvtf  v0.2d, v0.2d                     ; 00 d8 61 4e  
   496 : scvtf  v31.2d, v0.2d                    ; 1f d8 61 4e  
   497 : scvtf  v0.2d, v31.2d                    ; e0 db 61 4e  
   498 : ucvtf  v0.8h, v0.8h                     ; 00 d8 79 6e  
   499 : ucvtf  v31.8h, v0.8h                    ; 1f d8 79 6e  
   500 : ucvtf  v0.8h, v31.8h                    ; e0 db 79 6e  
   501 : ucvtf  v0.4s, v0.4s                     ; 00 d8 21 6e  
   502 : ucvtf  v31.4s, v0.4s                    ; 1f d8 21 6e  
   503 : ucvtf  v0.4s, v31.4s                    ; e0 db 21 6e  
   504 : ucvtf  v0.2d, v0.2d                     ; 00 d8 61 6e  
   505 : ucvtf  v31.2d, v0.2d                    ; 1f d8 61 6e  
   506 : ucvtf  v0.2d, v31.2d                    ; e0 db 61 6e  
   507 : fcmgt  v0.8h, v0.8h, v0.8h              ; 00 24 c0 6e  
   508 : fcmgt  v31.8h, v0.8h, v0.8h             ; 1f 24 c0 6e  
   509 : fcmgt  v0.8h, v31.8h, v0.8h             ; e0 27 c0 6e  
   510 : fcmgt  v0.8h, v0.8h, v31.8h             ; 00 24 df 6e  
   511 : fcmgt  v0.4s, v0.4s, v0.4s              ; 00 e4 a0 6e  
   512 : fcmgt  v31.4s, v0.4s, v0.4s             ; 1f e4 a0 6e  
   513 : fcmgt  v0.4s, v31.4s, v0.4s             ; e0 e7 a0 6e  
   514 : fcmgt  v0.4s, v0.4s, v31.4s             ; 00 e4 bf 6e  
   515 : fcmgt  v0.2d, v0.2d, v0.2d              ; 00 e4 e0 6e  
   516 : fcmgt  v31.2d, v0.2d, v0.2d             ; 1f e4 e0 6e  
   517 : fcmgt  v0.2d, v31.2d, v0.2d             ; e0 e7 e0 6e  
   518 : fcmgt  v0.2d, v0.2d, v31.2d             ; 00 e4 ff 6e  
   519 : cmhi   v0.16b, v0.16b, v0.16b           ; 00 34 20 6e  
   520 : cmhi   v31.16b, v0.16b, v0.16b          ; 1f 34 20 6e  
   521 : cmhi   v0.16b, v31.16b, v0.16b          ; e0 37 20 6e  
   522 : cmhi   v0.16b, v0.16b, v31.16b          ; 00 34 3f 6e  
   523 : cmhi   v0.8h, v0.8h, v0.8h              ; 00 34 60 6e  
   524 : cmhi   v31.8h, v0.8h, v0.8h             ; 1f 34 60 6e  
   525 : cmhi   v0.8h, v31.8h, v0.8h             ; e0 37 60 6e  
   526 : cmhi   v0.8h, v0.8h, v31.8h             ; 00 34 7f 6e  
   527 : cmhi   v0.4s, v0.4s, v0.4s              ; 00 34 a0 6e  
   528 : cmhi   v31.4s, v0.4s, v0.4s             ; 1f 34 a0 6e  
   529 : cmhi   v0.4s, v31.4s, v0.4s             ; e0 37 a0 6e  
   530 : cmhi   v0.4s, v0.4s, v31.4s             ; 00 34 bf 6e  
   531 : cmhi   v0.2d, v0.2d, v0.2d              ; 00 34 e0 6e  
   532 : cmhi   v31.2d, v0.2d, v0.2d             ; 1f 34 e0 6e  
   533 : cmhi   v0.2d, v31.2d, v0.2d             ; e0 37 e0 6e  
   534 : cmhi   v0.2d, v0.2d, v31.2d             ; 00 34 ff 6e  
   535 : cmgt   v0.16b, v0.16b, v0.16b           ; 00 34 20 4e  
   536 : cmgt   v31.16b, v0.16b, v0.16b          ; 1f 34 20 4e  
   537 : cmgt   v0.16b, v31.16b, v0.16b          ; e0 37 20 4e  
   538 : cmgt   v0.16b, v0.16b, v31.16b          ; 00 34 3f 4e  
   539 : cmgt   v0.8h, v0.8h, v0.8h              ; 00 34 60 4e  
   540 : cmgt   v31.8h, v0.8h, v0.8h             ; 1f 34 60 4e  
   541 : cmgt   v0.8h, v31.8h, v0.8h             ; e0 37 60 4e  
   542 : cmgt   v0.8h, v0.8h, v31.8h             ; 00 34 7f 4e  
   543 : cmgt   v0.4s, v0.4s, v0.4s              ; 00 34 a0 4e  
   544 : cmgt   v31.4s, v0.4s, v0.4s             ; 1f 34 a0 4e  
   545 : cmgt   v0.4s, v31.4s, v0.4s             ; e0 37 a0 4e  
   546 : cmgt   v0.4s, v0.4s, v31.4s             ; 00 34 bf 4e  
   547 : cmgt   v0.2d, v0.2d, v0.2d              ; 00 34 e0 4e  
   548 : cmgt   v31.2d, v0.2d, v0.2d             ; 1f 34 e0 4e  
   549 : cmgt   v0.2d, v31.2d, v0.2d             ; e0 37 e0 4e  
   550 : cmgt   v0.2d, v0.2d, v31.2d             ; 00 34 ff 4e  
   551 : fcmge  v0.8h, v0.8h, v0.8h              ; 00 24 40 6e  
   552 : fcmge  v31.8h, v0.8h, v0.8h             ; 1f 24 40 6e  
   553 : fcmge  v0.8h, v31.8h, v0.8h             ; e0 27 40 6e  
   554 : fcmge  v0.8h, v0.8h, v31.8h             ; 00 24 5f 6e  
   555 : fcmge  v0.4s, v0.4s, v0.4s              ; 00 e4 20 6e  
   556 : fcmge  v31.4s, v0.4s, v0.4s             ; 1f e4 20 6e  
   557 : fcmge  v0.4s, v31.4s, v0.4s             ; e0 e7 20 6e  
   558 : fcmge  v0.4s, v0.4s, v31.4s             ; 00 e4 3f 6e  
   559 : fcmge  v0.2d, v0.2d, v0.2d              ; 00 e4 60 6e  
   560 : fcmge  v31.2d, v0.2d, v0.2d             ; 1f e4 60 6e  
   561 : fcmge  v0.2d, v31.2d, v0.2d             ; e0 e7 60 6e  
   562 : fcmge  v0.2d, v0.2d, v31.2d             ; 00 e4 7f 6e  
   563 : cmhs   v0.16b, v0.16b, v0.16b           ; 00 3c 20 6e  
   564 : cmhs   v31.16b, v0.16b, v0.16b          ; 1f 3c 20 6e  
   565 : cmhs   v0.16b, v31.16b, v0.16b          ; e0 3f 20 6e  
   566 : cmhs   v0.16b, v0.16b, v31.16b          ; 00 3c 3f 6e  
   567 : cmhs   v0.8h, v0.8h, v0.8h              ; 00 3c 60 6e  
   568 : cmhs   v31.8h, v0.8h, v0.8h             ; 1f 3c 60 6e  
   569 : cmhs   v0.8h, v31.8h, v0.8h             ; e0 3f 60 6e  
   570 : cmhs   v0.8h, v0.8h, v31.8h             ; 00 3c 7f 6e  
   571 : cmhs   v0.4s, v0.4s, v0.4s              ; 00 3c a0 6e  
   572 : cmhs   v31.4s, v0.4s, v0.4s             ; 1f 3c a0 6e  
   573 : cmhs   v0.4s, v31.4s, v0.4s             ; e0 3f a0 6e  
   574 : cmhs   v0.4s, v0.4s, v31.4s             ; 00 3c bf 6e  
   575 : cmhs   v0.2d, v0.2d, v0.2d              ; 00 3c e0 6e  
   576 : cmhs   v31.2d, v0.2d, v0.2d             ; 1f 3c e0 6e  
   577 : cmhs   v0.2d, v31.2d, v0.2d             ; e0 3f e0 6e  
   578 : cmhs   v0.2d, v0.2d, v31.2d             ; 00 3c ff 6e  
   579 : cmge   v0.16b, v0.16b, v0.16b           ; 00 3c 20 4e  
   580 : cmge   v31.16b, v0.16b, v0.16b          ; 1f 3c 20 4e  
   581 : cmge   v0.16b, v31.16b, v0.16b          ; e0 3f 20 4e  
   582 : cmge   v0.16b, v0.16b, v31.16b          ; 00 3c 3f 4e  
   583 : cmge   v0.8h, v0.8h, v0.8h              ; 00 3c 60 4e  
   584 : cmge   v31.8h, v0.8h, v0.8h             ; 1f 3c 60 4e  
   585 : cmge   v0.8h, v31.8h, v0.8h             ; e0 3f 60 4e  
   586 : cmge   v0.8h, v0.8h, v31.8h             ; 00 3c 7f 4e  
   587 : cmge   v0.4s, v0.4s, v0.4s              ; 00 3c a0 4e  
   588 : cmge   v31.4s, v0.4s, v0.4s             ; 1f 3c a0 4e  
   589 : cmge   v0.4s, v31.4s, v0.4s             ; e0 3f a0 4e  
   590 : cmge   v0.4s, v0.4s, v31.4s             ; 00 3c bf 4e  
   591 : cmge   v0.2d, v0.2d, v0.2d              ; 00 3c e0 4e  
   592 : cmge   v31.2d, v0.2d, v0.2d             ; 1f 3c e0 4e  
   593 : cmge   v0.2d, v31.2d, v0.2d             ; e0 3f e0 4e  
   594 : cmge   v0.2d, v0.2d, v31.2d             ; 00 3c ff 4e  
   595 : fcmeq  v0.8h, v0.8h, v0.8h              ; 00 24 40 4e  
   596 : fcmeq  v31.8h, v0.8h, v0.8h             ; 1f 24 40 4e  
   597 : fcmeq  v0.8h, v31.8h, v0.8h             ; e0 27 40 4e  
   598 : fcmeq  v0.8h, v0.8h, v31.8h             ; 00 24 5f 4e  
   599 : fcmeq  v0.4s, v0.4s, v0.4s              ; 00 e4 20 4e  
   600 : fcmeq  v31.4s, v0.4s, v0.4s             ; 1f e4 20 4e  
   601 : fcmeq  v0.4s, v31.4s, v0.4s             ; e0 e7 20 4e  
   602 : fcmeq  v0.4s, v0.4s, v31.4s             ; 00 e4 3f 4e  
   603 : fcmeq  v0.2d, v0.2d, v0.2d              ; 00 e4 60 4e  
   604 : fcmeq  v31.2d, v0.2d, v0.2d             ; 1f e4 60 4e  
   605 : fcmeq  v0.2d, v31.2d, v0.2d             ; e0 e7 60 4e  
   606 : fcmeq  v0.2d, v0.2d, v31.2d             ; 00 e4 7f 4e  
   607 : cmeq   v0.16b, v0.16b, v0.16b           ; 00 8c 20 6e  
   608 : cmeq   v31.16b, v0.16b, v0.16b          ; 1f 8c 20 6e  
   609 : cmeq   v0.16b, v31.16b, v0.16b          ; e0 8f 20 6e  
   610 : cmeq   v0.16b, v0.16b, v31.16b          ; 00 8c 3f 6e  
   611 : cmeq   v0.8h, v0.8h, v0.8h              ; 00 8c 60 6e  
   612 : cmeq   v31.8h, v0.8h, v0.8h             ; 1f 8c 60 6e  
   613 : cmeq   v0.8h, v31.8h, v0.8h             ; e0 8f 60 6e  
   614 : cmeq   v0.8h, v0.8h, v31.8h             ; 00 8c 7f 6e  
   615 : cmeq   v0.4s, v0.4s, v0.4s              ; 00 8c a0 6e  
   616 : cmeq   v31.4s, v0.4s, v0.4s             ; 1f 8c a0 6e  
   617 : cmeq   v0.4s, v31.4s, v0.4s             ; e0 8f a0 6e  
   618 : cmeq   v0.4s, v0.4s, v31.4s             ; 00 8c bf 6e  
   619 : cmeq   v0.2d, v0.2d, v0.2d              ; 00 8c e0 6e  
   620 : cmeq   v31.2d, v0.2d, v0.2d             ; 1f 8c e0 6e  
   621 : cmeq   v0.2d, v31.2d, v0.2d             ; e0 8f e0 6e  
   622 : cmeq   v0.2d, v0.2d, v31.2d             ; 00 8c ff 6e  
   623 : neg    v0.16b, v0.16b                   ; 00 b8 20 6e  
   624 : neg    v31.16b, v0.16b                  ; 1f b8 20 6e  
   625 : neg    v0.16b, v31.16b                  ; e0 bb 20 6e  
   626 : neg    v0.8h, v0.8h                     ; 00 b8 60 6e  
   627 : neg    v31.8h, v0.8h                    ; 1f b8 60 6e  
   628 : neg    v0.8h, v31.8h                    ; e0 bb 60 6e  
   629 : neg    v0.4s, v0.4s                     ; 00 b8 a0 6e  
   630 : neg    v31.4s, v0.4s                    ; 1f b8 a0 6e  
   631 : neg    v0.4s, v31.4s                    ; e0 bb a0 6e  
   632 : neg    v0.2d, v0.2d                     ; 00 b8 e0 6e  
   633 : neg    v31.2d, v0.2d                    ; 1f b8 e0 6e  
   634 : neg    v0.2d, v31.2d                    ; e0 bb e0 6e  
   635 : fneg   v0.8h, v0.8h                     ; 00 f8 f8 6e  
   636 : fneg   v31.8h, v0.8h                    ; 1f f8 f8 6e  
   637 : fneg   v0.8h, v31.8h                    ; e0 fb f8 6e  
   638 : fneg   v0.4s, v0.4s                     ; 00 f8 a0 6e  
   639 : fneg   v31.4s, v0.4s                    ; 1f f8 a0 6e  
   640 : fneg   v0.4s, v31.4s                    ; e0 fb a0 6e  
   641 : fneg   v0.2d, v0.2d                     ; 00 f8 e0 6e  
   642 : fneg   v31.2d, v0.2d                    ; 1f f8 e0 6e  
   643 : fneg   v0.2d, v31.2d                    ; e0 fb e0 6e  
   644 : movk   x0, #0, #0x01                    ; 00 00 a0 f2  
   645 : movk   x15, #0, #0x01                   ; 0f 00 a0 f2  
   646 : movk   x0, #0xffff, #0x01               ; e0 ff bf f2  
   647 : movk   x0, #0, #0x02                    ; 00 00 c0 f2  
   648 : movk   x15, #0, #0x02                   ; 0f 00 c0 f2  
   649 : movk   x0, #0xffff, #0x02               ; e0 ff df f2  
   650 : movk   x0, #0, #0x03                    ; 00 00 e0 f2  
   651 : movk   x15, #0, #0x03                   ; 0f 00 e0 f2  
   652 : movk   x0, #0xffff, #0x03               ; e0 ff ff f2  
   653 : dup    v0.16b, x0                       ; 00 0c 01 4e  
   654 : dup    v31.16b, x0                      ; 1f 0c 01 4e  
   655 : dup    v0.16b, x15                      ; e0 0d 01 4e  
   656 : dup    v0.8h, x0                        ; 00 0c 02 4e  
   657 : dup    v31.8h, x0                       ; 1f 0c 02 4e  
   658 : dup    v0.8h, x15                       ; e0 0d 02 4e  
   659 : dup    v0.4s, x0                        ; 00 0c 04 4e  
   660 : dup    v31.4s, x0                       ; 1f 0c 04 4e  
   661 : dup    v0.4s, x15                       ; e0 0d 04 4e  
   662 : dup    v0.2d, x0                        ; 00 0c 08 4e  
   663 : dup    v31.2d, x0                       ; 1f 0c 08 4e  
   664 : dup    v0.2d, x15                       ; e0 0d 08 4e  
   665 : dup    v0.16b, v0.16b, #0               ; 00 04 01 4e  
   666 : dup    v31.16b, v0.16b, #0              ; 1f 04 01 4e  
   667 : dup    v0.16b, v31.16b, #0              ; e0 07 01 4e  
   668 : dup    v0.16b, v0.16b, #0x0f            ; 00 04 1f 4e  
   669 : dup    v0.8h, v0.8h, #0                 ; 00 04 02 4e  
   670 : dup    v31.8h, v0.8h, #0                ; 1f 04 02 4e  
   671 : dup    v0.8h, v31.8h, #0                ; e0 07 02 4e  
   672 : dup    v0.8h, v0.8h, #0x07              ; 00 04 1e 4e  
   673 : dup    v0.4s, v0.4s, #0                 ; 00 04 04 4e  
   674 : dup    v31.4s, v0.4s, #0                ; 1f 04 04 4e  
   675 : dup    v0.4s, v31.4s, #0                ; e0 07 04 4e  
   676 : dup    v0.4s, v0.4s, #0x03              ; 00 04 1c 4e  
   677 : dup    v0.2d, v0.2d, #0                 ; 00 04 08 4e  
   678 : dup    v31.2d, v0.2d, #0                ; 1f 04 08 4e  
   679 : dup    v0.2d, v31.2d, #0                ; e0 07 08 4e  
   680 : dup    v0.2d, v0.2d, #0x01              ; 00 04 18 4e  
   681 : sshll  v0.8h, v0.16b, #0                ; 00 a4 08 0f  
   682 : sshll  v31.8h, v0.16b, #0               ; 1f a4 08 0f  
   683 : sshll  v0.8h, v31.16b, #0               ; e0 a7 08 0f  
   684 : sshll  v0.4s, v0.8h, #0                 ; 00 a4 10 0f  
   685 : sshll  v31.4s, v0.8h, #0                ; 1f a4 10 0f  
   686 : sshll  v0.4s, v31.8h, #0                ; e0 a7 10 0f  
   687 : sshll  v0.2d, v0.4s, #0                 ; 00 a4 20 0f  
   688 : sshll  v31.2d, v0.4s, #0                ; 1f a4 20 0f  
   689 : sshll  v0.2d, v31.4s, #0                ; e0 a7 20 0f  
   690 : ushll  v0.8h, v0.16b, #0                ; 00 a4 08 2f  
   691 : ushll  v31.8h, v0.16b, #0               ; 1f a4 08 2f  
   692 : ushll  v0.8h, v31.16b, #0               ; e0 a7 08 2f  
   693 : ushll  v0.4s, v0.8h, #0                 ; 00 a4 10 2f  
   694 : ushll  v31.4s, v0.8h, #0                ; 1f a4 10 2f  
   695 : ushll  v0.4s, v31.8h, #0                ; e0 a7 10 2f  
   696 : ushll  v0.2d, v0.4s, #0                 ; 00 a4 20 2f  
   697 : ushll  v31.2d, v0.4s, #0                ; 1f a4 20 2f  
   698 : ushll  v0.2d, v31.4s, #0                ; e0 a7 20 2f  
   699 : sshll2 v0.8h, v0.16b, #0                ; 00 a4 08 4f  
   700 : sshll2 v31.8h, v0.16b, #0               ; 1f a4 08 4f  
   701 : sshll2 v0.8h, v31.16b, #0               ; e0 a7 08 4f  
   702 : sshll2 v0.4s, v0.8h, #0                 ; 00 a4 10 4f  
   703 : sshll2 v31.4s, v0.8h, #0                ; 1f a4 10 4f  
   704 : sshll2 v0.4s, v31.8h, #0                ; e0 a7 10 4f  
   705 : sshll2 v0.2d, v0.4s, #0                 ; 00 a4 20 4f  
   706 : sshll2 v31.2d, v0.4s, #0                ; 1f a4 20 4f  
   707 : sshll2 v0.2d, v31.4s, #0                ; e0 a7 20 4f  
   708 : ushll2 v0.8h, v0.16b, #0                ; 00 a4 08 6f  
   709 : ushll2 v31.8h, v0.16b, #0               ; 1f a4 08 6f  
   710 : ushll2 v0.8h, v31.16b, #0               ; e0 a7 08 6f  
   711 : ushll2 v0.4s, v0.8h, #0                 ; 00 a4 10 6f  
   712 : ushll2 v31.4s, v0.8h, #0                ; 1f a4 10 6f  
   713 : ushll2 v0.4s, v31.8h, #0                ; e0 a7 10 6f  
   714 : ushll2 v0.2d, v0.4s, #0                 ; 00 a4 20 6f  
   715 : ushll2 v31.2d, v0.4s, #0                ; 1f a4 20 6f  
   716 : ushll2 v0.2d, v31.4s, #0                ; e0 a7 20 6f  
   717 : xtn    v0.16b, v0.8h                    ; 00 28 21 0e  
   718 : xtn    v31.16b, v0.8h                   ; 1f 28 21 0e  
   719 : xtn    v0.16b, v31.8h                   ; e0 2b 21 0e  
   720 : xtn    v0.8h, v0.4s                     ; 00 28 61 0e  
   721 : xtn    v31.8h, v0.4s                    ; 1f 28 61 0e  
   722 : xtn    v0.8h, v31.4s                    ; e0 2b 61 0e  
   723 : xtn    v0.4s, v0.2d                     ; 00 28 a1 0e  
   724 : xtn    v31.4s, v0.2d                    ; 1f 28 a1 0e  
   725 : xtn    v0.4s, v31.2d                    ; e0 2b a1 0e  
   726 : xtn2   v0.16b, v0.8h                    ; 00 28 21 4e  
   727 : xtn2   v31.16b, v0.8h                   ; 1f 28 21 4e  
   728 : xtn2   v0.16b, v31.8h                   ; e0 2b 21 4e  
   729 : xtn2   v0.8h, v0.4s                     ; 00 28 61 4e  
   730 : xtn2   v31.8h, v0.4s                    ; 1f 28 61 4e  
   731 : xtn2   v0.8h, v31.4s                    ; e0 2b 61 4e  
   732 : xtn2   v0.4s, v0.2d                     ; 00 28 a1 4e  
   733 : xtn2   v31.4s, v0.2d                    ; 1f 28 a1 4e  
   734 : xtn2   v0.4s, v31.2d                    ; e0 2b a1 4e  
   735 : cnt    v0.16b, v0.16b                   ; 00 58 20 4e  
   736 : cnt    v31.16b, v0.16b                  ; 1f 58 20 4e  
   737 : cnt    v0.16b, v31.16b                  ; e0 5b 20 4e  
   738 : smaxv  v0.16b, v0.16b                   ; 00 a8 30 4e  
   739 : smaxv  v31.16b, v0.16b                  ; 1f a8 30 4e  
   740 : smaxv  v0.16b, v31.16b                  ; e0 ab 30 4e  
   741 : smaxv  v0.8h, v0.8h                     ; 00 a8 70 4e  
   742 : smaxv  v31.8h, v0.8h                    ; 1f a8 70 4e  
   743 : smaxv  v0.8h, v31.8h                    ; e0 ab 70 4e  
   744 : smaxv  v0.4s, v0.4s                     ; 00 a8 b0 4e  
   745 : smaxv  v31.4s, v0.4s                    ; 1f a8 b0 4e  
   746 : smaxv  v0.4s, v31.4s                    ; e0 ab b0 4e  
   747 : umaxv  v0.16b, v0.16b                   ; 00 a8 30 6e  
   748 : umaxv  v31.16b, v0.16b                  ; 1f a8 30 6e  
   749 : umaxv  v0.16b, v31.16b                  ; e0 ab 30 6e  
   750 : umaxv  v0.8h, v0.8h                     ; 00 a8 70 6e  
   751 : umaxv  v31.8h, v0.8h                    ; 1f a8 70 6e  
   752 : umaxv  v0.8h, v31.8h                    ; e0 ab 70 6e  
   753 : umaxv  v0.4s, v0.4s                     ; 00 a8 b0 6e  
   754 : umaxv  v31.4s, v0.4s                    ; 1f a8 b0 6e  
   755 : umaxv  v0.4s, v31.4s                    ; e0 ab b0 6e  
   756 : fmaxv  v0.8h, v0.8h                     ; 00 f8 30 4e  
   757 : fmaxv  v31.8h, v0.8h                    ; 1f f8 30 4e  
   758 : fmaxv  v0.8h, v31.8h                    ; e0 fb 30 4e  
   759 : fmaxv  v0.4s, v0.4s                     ; 00 f8 30 6e  
   760 : fmaxv  v31.4s, v0.4s                    ; 1f f8 30 6e  
   761 : fmaxv  v0.4s, v31.4s                    ; e0 fb 30 6e  
   762 : sminv  v0.16b, v0.16b                   ; 00 a8 31 4e  
   763 : sminv  v31.16b, v0.16b                  ; 1f a8 31 4e  
   764 : sminv  v0.16b, v31.16b                  ; e0 ab 31 4e  
   765 : sminv  v0.8h, v0.8h                     ; 00 a8 71 4e  
   766 : sminv  v31.8h, v0.8h                    ; 1f a8 71 4e  
   767 : sminv  v0.8h, v31.8h                    ; e0 ab 71 4e  
   768 : sminv  v0.4s, v0.4s                     ; 00 a8 b1 4e  
   769 : sminv  v31.4s, v0.4s                    ; 1f a8 b1 4e  
   770 : sminv  v0.4s, v31.4s                    ; e0 ab b1 4e  
   771 : uminv  v0.16b, v0.16b                   ; 00 a8 31 6e  
   772 : uminv  v31.16b, v0.16b                  ; 1f a8 31 6e  
   773 : uminv  v0.16b, v31.16b                  ; e0 ab 31 6e  
   774 : uminv  v0.8h, v0.8h                     ; 00 a8 71 6e  
   775 : uminv  v31.8h, v0.8h                    ; 1f a8 71 6e  
   776 : uminv  v0.8h, v31.8h                    ; e0 ab 71 6e  
   777 : uminv  v0.4s, v0.4s                     ; 00 a8 b1 6e  
   778 : uminv  v31.4s, v0.4s                    ; 1f a8 b1 6e  
   779 : uminv  v0.4s, v31.4s                    ; e0 ab b1 6e  
   780 : fminv  v0.8h, v0.8h                     ; 00 f8 b0 4e  
   781 : fminv  v31.8h, v0.8h                    ; 1f f8 b0 4e  
   782 : fminv  v0.8h, v31.8h                    ; e0 fb b0 4e  
   783 : fminv  v0.4s, v0.4s                     ; 00 f8 b0 6e  
   784 : fminv  v31.4s, v0.4s                    ; 1f f8 b0 6e  
   785 : fminv  v0.4s, v31.4s                    ; e0 fb b0 6e  
   786 : addv   v0.16b, v0.16b                   ; 00 b8 31 4e  
   787 : addv   v31.16b, v0.16b                  ; 1f b8 31 4e  
   788 : addv   v0.16b, v31.16b                  ; e0 bb 31 4e  
   789 : addv   v0.8h, v0.8h                     ; 00 b8 71 4e  
   790 : addv   v31.8h, v0.8h                    ; 1f b8 71 4e  
   791 : addv   v0.8h, v31.8h                    ; e0 bb 71 4e  
   792 : addv   v0.4s, v0.4s                     ; 00 b8 b1 4e  
   793 : addv   v31.4s, v0.4s                    ; 1f b8 b1 4e  
   794 : addv   v0.4s, v31.4s                    ; e0 bb b1 4e  
   795 : uaddlv v0.8h, v0.16b                    ; 00 38 30 6e  
   796 : uaddlv v31.8h, v0.16b                   ; 1f 38 30 6e  
   797 : uaddlv v0.8h, v31.16b                   ; e0 3b 30 6e  
   798 : uaddlv v0.4s, v0.8h                     ; 00 38 70 6e  
   799 : uaddlv v31.4s, v0.8h                    ; 1f 38 70 6e  
   800 : uaddlv v0.4s, v31.8h                    ; e0 3b 70 6e  
   801 : uaddlv v0.2d, v0.4s                     ; 00 38 b0 6e  
   802 : uaddlv v31.2d, v0.4s                    ; 1f 38 b0 6e  
   803 : uaddlv v0.2d, v31.4s                    ; e0 3b b0 6e  
   804 : saddlv v0.8h, v0.16b                    ; 00 38 30 4e  
   805 : saddlv v31.8h, v0.16b                   ; 1f 38 30 4e  
   806 : saddlv v0.8h, v31.16b                   ; e0 3b 30 4e  
   807 : saddlv v0.4s, v0.8h                     ; 00 38 70 4e  
   808 : saddlv v31.4s, v0.8h                    ; 1f 38 70 4e  
   809 : saddlv v0.4s, v31.8h                    ; e0 3b 70 4e  
   810 : saddlv v0.2d, v0.4s                     ; 00 38 b0 4e  
   811 : saddlv v31.2d, v0.4s                    ; 1f 38 b0 4e  
   812 : saddlv v0.2d, v31.4s                    ; e0 3b b0 4e  
   813 : ld1    v0.16b, x0, #0                   ; 00 00 40 0d  
   814 : ld1    v31.16b, x0, #0                  ; 1f 00 40 0d  
   815 : ld1    v0.16b, x0, #0x0f                ; 00 1c 40 4d  
   816 : ld1    v0.16b, x15, #0                  ; e0 01 40 0d  
   817 : ld1    v0.8h, x0, #0                    ; 00 40 40 0d  
   818 : ld1    v31.8h, x0, #0                   ; 1f 40 40 0d  
   819 : ld1    v0.8h, x0, #0x07                 ; 00 58 40 4d  
   820 : ld1    v0.8h, x15, #0                   ; e0 41 40 0d  
   821 : ld1    v0.4s, x0, #0                    ; 00 80 40 0d  
   822 : ld1    v31.4s, x0, #0                   ; 1f 80 40 0d  
   823 : ld1    v0.4s, x0, #0x03                 ; 00 90 40 4d  
   824 : ld1    v0.4s, x15, #0                   ; e0 81 40 0d  
   825 : ld1    v0.2d, x0, #0                    ; 00 84 40 0d  
   826 : ld1    v31.2d, x0, #0                   ; 1f 84 40 0d  
   827 : ld1    v0.2d, x0, #0x01                 ; 00 84 40 4d  
   828 : ld1    v0.2d, x15, #0                   ; e0 85 40 0d  
   829 : ld2    v0.16b, v1.16b, x0               ; 00 80 40 4c  
   830 : ld2    v31.16b, v0.16b, x0              ; 1f 80 40 4c  
   831 : ld2    v0.16b, v1.16b, x15              ; e0 81 40 4c  
   832 : ld2    v0.8h, v1.8h, x0                 ; 00 84 40 4c  
   833 : ld2    v31.8h, v0.8h, x0                ; 1f 84 40 4c  
   834 : ld2    v0.8h, v1.8h, x15                ; e0 85 40 4c  
   835 : ld2    v0.4s, v1.4s, x0                 ; 00 88 40 4c  
   836 : ld2    v31.4s, v0.4s, x0                ; 1f 88 40 4c  
   837 : ld2    v0.4s, v1.4s, x15                ; e0 89 40 4c  
   838 : ld2    v0.2d, v1.2d, x0                 ; 00 8c 40 4c  
   839 : ld2    v31.2d, v0.2d, x0                ; 1f 8c 40 4c  
   840 : ld2    v0.2d, v1.2d, x15                ; e0 8d 40 4c  
   841 : ld1    v0.16b, x0                       ; 00 20 df 4c  
   842 : ld1    v31.16b, x0                      ; 1f 20 df 4c  
   843 : ld1    v0.16b, x15                      ; e0 21 df 4c  
   844 : ld1    v0.8h, x0                        ; 00 24 df 4c  
   845 : ld1    v31.8h, x0                       ; 1f 24 df 4c  
   846 : ld1    v0.8h, x15                       ; e0 25 df 4c  
   847 : ld1    v0.4s, x0                        ; 00 28 df 4c  
   848 : ld1    v31.4s, x0                       ; 1f 28 df 4c  
   849 : ld1    v0.4s, x15                       ; e0 29 df 4c  
   850 : ld1    v0.2d, x0                        ; 00 2c df 4c  
   851 : ld1    v31.2d, x0                       ; 1f 2c df 4c  
   852 : ld1    v0.2d, x15                       ; e0 2d df 4c  
   853 : st1    v0.16b, #0, x0                   ; 00 00 00 0d  
   854 : st1    v31.16b, #0, x0                  ; 1f 00 00 0d  
   855 : st1    v0.16b, #0x0f, x0                ; 00 1c 00 4d  
   856 : st1    v0.16b, #0, x15                  ; e0 01 00 0d  
   857 : st1    v0.8h, #0, x0                    ; 00 40 00 0d  
   858 : st1    v31.8h, #0, x0                   ; 1f 40 00 0d  
   859 : st1    v0.8h, #0x07, x0                 ; 00 58 00 4d  
   860 : st1    v0.8h, #0, x15                   ; e0 41 00 0d  
   861 : st1    v0.4s, #0, x0                    ; 00 80 00 0d  
   862 : st1    v31.4s, #0, x0                   ; 1f 80 00 0d  
   863 : st1    v0.4s, #0x03, x0                 ; 00 90 00 4d  
   864 : st1    v0.4s, #0, x15                   ; e0 81 00 0d  
   865 : st1    v0.2d, #0, x0                    ; 00 84 00 0d  
   866 : st1    v31.2d, #0, x0                   ; 1f 84 00 0d  
   867 : st1    v0.2d, #0x01, x0                 ; 00 84 00 4d  
   868 : st1    v0.2d, #0, x15                   ; e0 85 00 0d  
   869 : st1    v0.16b, x0                       ; 00 20 9f 4c  
   870 : st1    v0.16b, x15                      ; e0 21 9f 4c  
   871 : st1    v31.16b, x0                      ; 1f 20 9f 4c  
   872 : st1    v0.8h, x0                        ; 00 24 9f 4c  
   873 : st1    v0.8h, x15                       ; e0 25 9f 4c  
   874 : st1    v31.8h, x0                       ; 1f 24 9f 4c  
   875 : st1    v0.4s, x0                        ; 00 28 9f 4c  
   876 : st1    v0.4s, x15                       ; e0 29 9f 4c  
   877 : st1    v31.4s, x0                       ; 1f 28 9f 4c  
   878 : st1    v0.2d, x0                        ; 00 2c 9f 4c  
   879 : st1    v0.2d, x15                       ; e0 2d 9f 4c  
   880 : st1    v31.2d, x0                       ; 1f 2c 9f 4c  
   881 : ext    v0.16b, v0.16b, v0.16b, #0       ; 00 00 00 6e  
   882 : ext    v31.16b, v0.16b, v0.16b, #0      ; 1f 00 00 6e  
   883 : ext    v0.16b, v31.16b, v0.16b, #0      ; e0 03 00 6e  
   884 : ext    v0.16b, v0.16b, v31.16b, #0      ; 00 00 1f 6e  
   885 : ext    v0.16b, v0.16b, v0.16b, #0x0f    ; 00 78 00 6e  
   886 : ext    v0.8h, v0.8h, v0.8h, #0          ; 00 00 00 6e  
   887 : ext    v31.8h, v0.8h, v0.8h, #0         ; 1f 00 00 6e  
   888 : ext    v0.8h, v31.8h, v0.8h, #0         ; e0 03 00 6e  
   889 : ext    v0.8h, v0.8h, v31.8h, #0         ; 00 00 1f 6e  
   890 : ext    v0.8h, v0.8h, v0.8h, #0x0e       ; 00 70 00 6e  
   891 : ext    v0.4s, v0.4s, v0.4s, #0          ; 00 00 00 6e  
   892 : ext    v31.4s, v0.4s, v0.4s, #0         ; 1f 00 00 6e  
   893 : ext    v0.4s, v31.4s, v0.4s, #0         ; e0 03 00 6e  
   894 : ext    v0.4s, v0.4s, v31.4s, #0         ; 00 00 1f 6e  
   895 : ext    v0.4s, v0.4s, v0.4s, #0x0c       ; 00 60 00 6e  
   896 : ext    v0.2d, v0.2d, v0.2d, #0          ; 00 00 00 6e  
   897 : ext    v31.2d, v0.2d, v0.2d, #0         ; 1f 00 00 6e  
   898 : ext    v0.2d, v31.2d, v0.2d, #0         ; e0 03 00 6e  
   899 : ext    v0.2d, v0.2d, v31.2d, #0         ; 00 00 1f 6e  
   900 : ext    v0.2d, v0.2d, v0.2d, #0x10       ; 00 00 00 6e  
   901 : fmla   v0.8h, v0.8h, v0.8h, #0          ; 00 10 00 4f  
   902 : fmla   v31.8h, v0.8h, v0.8h, #0         ; 1f 10 00 4f  
   903 : fmla   v0.8h, v31.8h, v0.8h, #0         ; e0 13 00 4f  
   904 : fmla   v0.8h, v0.8h, v15.8h, #0         ; 00 10 0f 4f  
   905 : fmla   v0.8h, v0.8h, v0.8h, #0x07       ; 00 18 30 4f  
   906 : fmla   v0.4s, v0.4s, v0.4s, #0          ; 00 10 80 4f  
   907 : fmla   v31.4s, v0.4s, v0.4s, #0         ; 1f 10 80 4f  
   908 : fmla   v0.4s, v31.4s, v0.4s, #0         ; e0 13 80 4f  
   909 : fmla   v0.4s, v0.4s, v31.4s, #0         ; 00 10 9f 4f  
   910 : fmla   v0.4s, v0.4s, v0.4s, #0x03       ; 00 18 a0 4f  
   911 : fmla   v0.2d, v0.2d, v0.2d, #0          ; 00 10 c0 4f  
   912 : fmla   v31.2d, v0.2d, v0.2d, #0         ; 1f 10 c0 4f  
   913 : fmla   v0.2d, v31.2d, v0.2d, #0         ; e0 13 c0 4f  
   914 : fmla   v0.2d, v0.2d, v31.2d, #0         ; 00 10 df 4f  
   915 : fmla   v0.2d, v0.2d, v0.2d, #0x01       ; 00 18 c0 4f  
   916 : umov   x0, v0.16b, #0                   ; 00 3c 01 0e  
   917 : umov   x15, v0.16b, #0                  ; 0f 3c 01 0e  
   918 : umov   x0, v31.16b, #0                  ; e0 3f 01 0e  
   919 : umov   x0, v0.16b, #0x0f                ; 00 3c 1f 0e  
   920 : umov   x0, v0.8h, #0                    ; 00 3c 02 0e  
   921 : umov   x15, v0.8h, #0                   ; 0f 3c 02 0e  
   922 : umov   x0, v31.8h, #0                   ; e0 3f 02 0e  
   923 : umov   x0, v0.8h, #0x07                 ; 00 3c 1e 0e  
   924 : umov   x0, v0.4s, #0                    ; 00 3c 04 0e  
   925 : umov   x15, v0.4s, #0                   ; 0f 3c 04 0e  
   926 : umov   x0, v31.4s, #0                   ; e0 3f 04 0e  
   927 : umov   x0, v0.4s, #0x03                 ; 00 3c 1c 0e  
   928 : umov   x0, v0.2d, #0                    ; 00 3c 08 4e  
   929 : umov   x15, v0.2d, #0                   ; 0f 3c 08 4e  
   930 : umov   x0, v31.2d, #0                   ; e0 3f 08 4e  
   931 : umov   x0, v0.2d, #0x01                 ; 00 3c 18 4e  
   932 : ins    v0.16b, #0, x0                   ; 00 1c 01 4e  
   933 : ins    v31.16b, #0, x0                  ; 1f 1c 01 4e  
   934 : ins    v0.16b, #0x0f, x0                ; 00 1c 1f 4e  
   935 : ins    v0.16b, #0, x15                  ; e0 1d 01 4e  
   936 : ins    v0.8h, #0, x0                    ; 00 1c 02 4e  
   937 : ins    v31.8h, #0, x0                   ; 1f 1c 02 4e  
   938 : ins    v0.8h, #0x07, x0                 ; 00 1c 1e 4e  
   939 : ins    v0.8h, #0, x15                   ; e0 1d 02 4e  
   940 : ins    v0.4s, #0, x0                    ; 00 1c 04 4e  
   941 : ins    v31.4s, #0, x0                   ; 1f 1c 04 4e  
   942 : ins    v0.4s, #0x03, x0                 ; 00 1c 1c 4e  
   943 : ins    v0.4s, #0, x15                   ; e0 1d 04 4e  
   944 : ins    v0.2d, #0, x0                    ; 00 1c 08 4e  
   945 : ins    v31.2d, #0, x0                   ; 1f 1c 08 4e  
   946 : ins    v0.2d, #0x01, x0                 ; 00 1c 18 4e  
   947 : ins    v0.2d, #0, x15                   ; e0 1d 08 4e  
   948 : ins    v0.16b, #0, v0.16b, #0           ; 00 04 01 6e  
   949 : ins    v31.16b, #0, v0.16b, #0          ; 1f 04 01 6e  
   950 : ins    v0.16b, #0x0f, v0.16b, #0        ; 00 04 1f 6e  
   951 : ins    v0.16b, #0, v31.16b, #0          ; e0 07 01 6e  
   952 : ins    v0.16b, #0, v0.16b, #0x0f        ; 00 7c 01 6e  
   953 : ins    v0.8h, #0, v0.8h, #0             ; 00 04 02 6e  
   954 : ins    v31.8h, #0, v0.8h, #0            ; 1f 04 02 6e  
   955 : ins    v0.8h, #0x07, v0.8h, #0          ; 00 04 1e 6e  
   956 : ins    v0.8h, #0, v31.8h, #0            ; e0 07 02 6e  
   957 : ins    v0.8h, #0, v0.8h, #0x07          ; 00 74 02 6e  
   958 : ins    v0.4s, #0, v0.4s, #0             ; 00 04 04 6e  
   959 : ins    v31.4s, #0, v0.4s, #0            ; 1f 04 04 6e  
   960 : ins    v0.4s, #0x03, v0.4s, #0          ; 00 04 1c 6e  
   961 : ins    v0.4s, #0, v31.4s, #0            ; e0 07 04 6e  
   962 : ins    v0.4s, #0, v0.4s, #0x03          ; 00 64 04 6e  
   963 : ins    v0.2d, #0, v0.2d, #0             ; 00 04 08 6e  
   964 : ins    v31.2d, #0, v0.2d, #0            ; 1f 04 08 6e  
   965 : ins    v0.2d, #0x01, v0.2d, #0          ; 00 04 18 6e  
   966 : ins    v0.2d, #0, v31.2d, #0            ; e0 07 08 6e  
   967 : ins    v0.2d, #0, v0.2d, #0x01          ; 00 44 08 6e  
   968 : bsl    v0.16b, v0.16b, v0.16b           ; 00 1c 60 6e  
   969 : bsl    v31.16b, v0.16b, v0.16b          ; 1f 1c 60 6e  
   970 : bsl    v0.16b, v31.16b, v0.16b          ; e0 1f 60 6e  
   971 : bsl    v0.16b, v0.16b, v31.16b          ; 00 1c 7f 6e  
   972 : blr    [x0]                             ; 00 00 3f d6  
   973 : blr    [x15]                            ; e0 01 3f d6  
