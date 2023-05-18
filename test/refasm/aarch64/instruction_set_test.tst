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
   105 : cinc   x0, x0, #0x01                    ; 00 14 80 9a  
   106 : cinc   x0, x0, #0                       ; 00 04 80 9a  
   107 : cinc   x0, x0, #0x0a                    ; 00 a4 80 9a  
   108 : cinc   x0, x0, #0x0d                    ; 00 d4 80 9a  
   109 : cinc   x0, x0, #0x0c                    ; 00 c4 80 9a  
   110 : cinc   x0, x0, #0x0b                    ; 00 b4 80 9a  
   111 : cinc   x0, x0, #0x05                    ; 00 54 80 9a  
   112 : cinc   x0, x0, #0x04                    ; 00 44 80 9a  
   113 : cinc   x15, x0, #0x01                   ; 0f 14 80 9a  
   114 : cinc   x0, x15, #0x01                   ; e0 15 8f 9a  
   115 : cneg   x0, x0, #0x01                    ; 00 14 80 da  
   116 : cneg   x0, x0, #0                       ; 00 04 80 da  
   117 : cneg   x0, x0, #0x0a                    ; 00 a4 80 da  
   118 : cneg   x0, x0, #0x0d                    ; 00 d4 80 da  
   119 : cneg   x0, x0, #0x0c                    ; 00 c4 80 da  
   120 : cneg   x0, x0, #0x0b                    ; 00 b4 80 da  
   121 : cneg   x0, x0, #0x05                    ; 00 54 80 da  
   122 : cneg   x0, x0, #0x04                    ; 00 44 80 da  
   123 : cneg   x15, x0, #0x01                   ; 0f 14 80 da  
   124 : cneg   x0, x15, #0x01                   ; e0 15 8f da  
   125 : asr    x0, x0, #0x3f                    ; 00 fc 7f 93  
   126 : asr    x15, x0, #0x3f                   ; 0f fc 7f 93  
   127 : asr    x0, x15, #0x3f                   ; e0 fd 7f 93  
   128 : asr    x0, x0, x0                       ; 00 28 c0 9a  
   129 : asr    x15, x0, x0                      ; 0f 28 c0 9a  
   130 : asr    x0, x15, x0                      ; e0 29 c0 9a  
   131 : asr    x0, x0, x15                      ; 00 28 cf 9a  
   132 : lsr    x0, x0, #0x3f                    ; 00 fc 7f d3  
   133 : lsr    x15, x0, #0x3f                   ; 0f fc 7f d3  
   134 : lsr    x0, x15, #0x3f                   ; e0 fd 7f d3  
   135 : lsr    x0, x0, x0                       ; 00 24 c0 9a  
   136 : lsr    x15, x0, x0                      ; 0f 24 c0 9a  
   137 : lsr    x0, x15, x0                      ; e0 25 c0 9a  
   138 : lsr    x0, x0, x15                      ; 00 24 cf 9a  
   139 : lsl    x0, x0, #0x01, #0                ; 00 00 41 d3  
   140 : lsl    x0, x0, #0x21, #0x20             ; 00 80 61 d3  
   141 : lsl    x0, x0, #0x31, #0x30             ; 00 c0 71 d3  
   142 : lsl    x0, x0, #0x39, #0x38             ; 00 e0 79 d3  
   143 : lsl    x0, x0, #0x3f, #0x3e             ; 00 f8 7f d3  
   144 : lsl    x0, x0, #0, #0x3f                ; 00 fc 40 d3  
   145 : lsl    x15, x0, #0x01, #0               ; 0f 00 41 d3  
   146 : lsl    x0, x15, #0x01, #0               ; e0 01 41 d3  
   147 : lsl    x0, x0, x0                       ; 00 20 c0 9a  
   148 : lsl    x15, x0, x0                      ; 0f 20 c0 9a  
   149 : lsl    x0, x15, x0                      ; e0 21 c0 9a  
   150 : lsl    x0, x0, x15                      ; 00 20 cf 9a  
   151 : and    x0, x0, x0                       ; 00 00 00 8a  
   152 : and    x15, x0, x0                      ; 0f 00 00 8a  
   153 : and    x0, x15, x0                      ; e0 01 00 8a  
   154 : and    x0, x0, x15                      ; 00 00 0f 8a  
   155 : and    x0, x0, #0x01, #0, #0x07         ; 00 1c 40 92  
   156 : and    x0, x0, #0x01, #0x38, #0x07      ; 00 1c 78 92  
   157 : and    x0, x0, #0x01, #0x08, #0x07      ; 00 1c 48 92  
   158 : and    x15, x0, #0x01, #0, #0x07        ; 0f 1c 40 92  
   159 : and    x0, x15, #0x01, #0, #0x07        ; e0 1d 40 92  
   160 : orr    x0, x0, x0                       ; 00 00 00 aa  
   161 : orr    x15, x0, x0                      ; 0f 00 00 aa  
   162 : orr    x0, x15, x0                      ; e0 01 00 aa  
   163 : orr    x0, x0, x15                      ; 00 00 0f aa  
   164 : orr    x0, x0, #0x01, #0, #0x07         ; 00 1c 40 b2  
   165 : orr    x0, x0, #0x01, #0x38, #0x07      ; 00 1c 78 b2  
   166 : orr    x0, x0, #0x01, #0x08, #0x07      ; 00 1c 48 b2  
   167 : orr    x15, x0, #0x01, #0, #0x07        ; 0f 1c 40 b2  
   168 : orr    x0, x15, #0x01, #0, #0x07        ; e0 1d 40 b2  
   169 : eor    x0, x0, x0                       ; 00 00 00 ca  
   170 : eor    x15, x0, x0                      ; 0f 00 00 ca  
   171 : eor    x0, x15, x0                      ; e0 01 00 ca  
   172 : eor    x0, x0, x15                      ; 00 00 0f ca  
   173 : eor    x0, x0, #0x01, #0, #0x07         ; 00 1c 40 d2  
   174 : eor    x0, x0, #0x01, #0x38, #0x07      ; 00 1c 78 d2  
   175 : eor    x0, x0, #0x01, #0x08, #0x07      ; 00 1c 48 d2  
   176 : eor    x15, x0, #0x01, #0, #0x07        ; 0f 1c 40 d2  
   177 : eor    x0, x15, #0x01, #0, #0x07        ; e0 1d 40 d2  
   178 : mvn    x0, x0                           ; e0 03 20 aa  
   179 : mvn    x15, x0                          ; ef 03 20 aa  
   180 : mvn    x0, x15                          ; e0 03 2f aa  
   181 : neg    x0, x0                           ; e0 03 00 cb  
   182 : neg    x15, x0                          ; ef 03 00 cb  
   183 : neg    x0, x15                          ; e0 03 0f cb  
   184 : ldr    v0.2d, x0, #0                    ; 00 00 c0 3d  
   185 : ldr    v0.2d, x0, #0x801                ; 00 04 e0 3d  
   186 : ldr    v31.2d, x0, #0                   ; 1f 00 c0 3d  
   187 : ldr    v0.2d, x15, #0                   ; e0 01 c0 3d  
   188 : ldr    v0.2d, x0, x0                    ; 00 68 e0 3c  
   189 : ldr    v31.2d, x0, x0                   ; 1f 68 e0 3c  
   190 : ldr    v0.2d, x15, x0                   ; e0 69 e0 3c  
   191 : ldr    v0.2d, x0, x15                   ; 00 68 ef 3c  
   192 : str    v0.2d, x0, #0                    ; 00 00 80 3d  
   193 : str    v0.2d, x0, #0x801                ; 00 04 a0 3d  
   194 : str    v31.2d, x0, #0                   ; 1f 00 80 3d  
   195 : str    v0.2d, x15, #0                   ; e0 01 80 3d  
   196 : str    v0.2d, x0, x0                    ; 00 68 a0 3c  
   197 : str    v31.2d, x0, x0                   ; 1f 68 a0 3c  
   198 : str    v0.2d, x15, x0                   ; e0 69 a0 3c  
   199 : str    v0.2d, x0, x15                   ; 00 68 af 3c  
   200 : add    v0.16b, v0.16b, v0.16b           ; 00 84 20 4e  
   201 : add    v31.16b, v0.16b, v0.16b          ; 1f 84 20 4e  
   202 : add    v0.16b, v31.16b, v0.16b          ; e0 87 20 4e  
   203 : add    v0.16b, v0.16b, v31.16b          ; 00 84 3f 4e  
   204 : add    v0.8h, v0.8h, v0.8h              ; 00 84 60 4e  
   205 : add    v31.8h, v0.8h, v0.8h             ; 1f 84 60 4e  
   206 : add    v0.8h, v31.8h, v0.8h             ; e0 87 60 4e  
   207 : add    v0.8h, v0.8h, v31.8h             ; 00 84 7f 4e  
   208 : add    v0.4s, v0.4s, v0.4s              ; 00 84 a0 4e  
   209 : add    v31.4s, v0.4s, v0.4s             ; 1f 84 a0 4e  
   210 : add    v0.4s, v31.4s, v0.4s             ; e0 87 a0 4e  
   211 : add    v0.4s, v0.4s, v31.4s             ; 00 84 bf 4e  
   212 : add    v0.2d, v0.2d, v0.2d              ; 00 84 e0 4e  
   213 : add    v31.2d, v0.2d, v0.2d             ; 1f 84 e0 4e  
   214 : add    v0.2d, v31.2d, v0.2d             ; e0 87 e0 4e  
   215 : add    v0.2d, v0.2d, v31.2d             ; 00 84 ff 4e  
   216 : fadd   v0.8h, v0.8h, v0.8h              ; 00 14 40 4e  
   217 : fadd   v31.8h, v0.8h, v0.8h             ; 1f 14 40 4e  
   218 : fadd   v0.8h, v31.8h, v0.8h             ; e0 17 40 4e  
   219 : fadd   v0.8h, v0.8h, v31.8h             ; 00 14 5f 4e  
   220 : fadd   v0.4s, v0.4s, v0.4s              ; 00 d4 20 4e  
   221 : fadd   v31.4s, v0.4s, v0.4s             ; 1f d4 20 4e  
   222 : fadd   v0.4s, v31.4s, v0.4s             ; e0 d7 20 4e  
   223 : fadd   v0.4s, v0.4s, v31.4s             ; 00 d4 3f 4e  
   224 : fadd   v0.2d, v0.2d, v0.2d              ; 00 d4 60 4e  
   225 : fadd   v31.2d, v0.2d, v0.2d             ; 1f d4 60 4e  
   226 : fadd   v0.2d, v31.2d, v0.2d             ; e0 d7 60 4e  
   227 : fadd   v0.2d, v0.2d, v31.2d             ; 00 d4 7f 4e  
   228 : sub    v0.16b, v0.16b, v0.16b           ; 00 84 20 6e  
   229 : sub    v31.16b, v0.16b, v0.16b          ; 1f 84 20 6e  
   230 : sub    v0.16b, v31.16b, v0.16b          ; e0 87 20 6e  
   231 : sub    v0.16b, v0.16b, v31.16b          ; 00 84 3f 6e  
   232 : sub    v0.8h, v0.8h, v0.8h              ; 00 84 60 6e  
   233 : sub    v31.8h, v0.8h, v0.8h             ; 1f 84 60 6e  
   234 : sub    v0.8h, v31.8h, v0.8h             ; e0 87 60 6e  
   235 : sub    v0.8h, v0.8h, v31.8h             ; 00 84 7f 6e  
   236 : sub    v0.4s, v0.4s, v0.4s              ; 00 84 a0 6e  
   237 : sub    v31.4s, v0.4s, v0.4s             ; 1f 84 a0 6e  
   238 : sub    v0.4s, v31.4s, v0.4s             ; e0 87 a0 6e  
   239 : sub    v0.4s, v0.4s, v31.4s             ; 00 84 bf 6e  
   240 : sub    v0.2d, v0.2d, v0.2d              ; 00 84 e0 6e  
   241 : sub    v31.2d, v0.2d, v0.2d             ; 1f 84 e0 6e  
   242 : sub    v0.2d, v31.2d, v0.2d             ; e0 87 e0 6e  
   243 : sub    v0.2d, v0.2d, v31.2d             ; 00 84 ff 6e  
   244 : fsub   v0.8h, v0.8h, v0.8h              ; 00 14 c0 4e  
   245 : fsub   v31.8h, v0.8h, v0.8h             ; 1f 14 c0 4e  
   246 : fsub   v0.8h, v31.8h, v0.8h             ; e0 17 c0 4e  
   247 : fsub   v0.8h, v0.8h, v31.8h             ; 00 14 df 4e  
   248 : fsub   v0.4s, v0.4s, v0.4s              ; 00 d4 a0 4e  
   249 : fsub   v31.4s, v0.4s, v0.4s             ; 1f d4 a0 4e  
   250 : fsub   v0.4s, v31.4s, v0.4s             ; e0 d7 a0 4e  
   251 : fsub   v0.4s, v0.4s, v31.4s             ; 00 d4 bf 4e  
   252 : fsub   v0.2d, v0.2d, v0.2d              ; 00 d4 e0 4e  
   253 : fsub   v31.2d, v0.2d, v0.2d             ; 1f d4 e0 4e  
   254 : fsub   v0.2d, v31.2d, v0.2d             ; e0 d7 e0 4e  
   255 : fsub   v0.2d, v0.2d, v31.2d             ; 00 d4 ff 4e  
   256 : mul    v0.16b, v0.16b, v0.16b           ; 00 9c 20 4e  
   257 : mul    v31.16b, v0.16b, v0.16b          ; 1f 9c 20 4e  
   258 : mul    v0.16b, v31.16b, v0.16b          ; e0 9f 20 4e  
   259 : mul    v0.16b, v0.16b, v31.16b          ; 00 9c 3f 4e  
   260 : mul    v0.8h, v0.8h, v0.8h              ; 00 9c 60 4e  
   261 : mul    v31.8h, v0.8h, v0.8h             ; 1f 9c 60 4e  
   262 : mul    v0.8h, v31.8h, v0.8h             ; e0 9f 60 4e  
   263 : mul    v0.8h, v0.8h, v31.8h             ; 00 9c 7f 4e  
   264 : mul    v0.4s, v0.4s, v0.4s              ; 00 9c a0 4e  
   265 : mul    v31.4s, v0.4s, v0.4s             ; 1f 9c a0 4e  
   266 : mul    v0.4s, v31.4s, v0.4s             ; e0 9f a0 4e  
   267 : mul    v0.4s, v0.4s, v31.4s             ; 00 9c bf 4e  
   268 : fmul   v0.8h, v0.8h, v0.8h              ; 00 1c 40 6e  
   269 : fmul   v31.8h, v0.8h, v0.8h             ; 1f 1c 40 6e  
   270 : fmul   v0.8h, v31.8h, v0.8h             ; e0 1f 40 6e  
   271 : fmul   v0.8h, v0.8h, v31.8h             ; 00 1c 5f 6e  
   272 : fmul   v0.4s, v0.4s, v0.4s              ; 00 dc 20 6e  
   273 : fmul   v31.4s, v0.4s, v0.4s             ; 1f dc 20 6e  
   274 : fmul   v0.4s, v31.4s, v0.4s             ; e0 df 20 6e  
   275 : fmul   v0.4s, v0.4s, v31.4s             ; 00 dc 3f 6e  
   276 : fmul   v0.2d, v0.2d, v0.2d              ; 00 dc 60 6e  
   277 : fmul   v31.2d, v0.2d, v0.2d             ; 1f dc 60 6e  
   278 : fmul   v0.2d, v31.2d, v0.2d             ; e0 df 60 6e  
   279 : fmul   v0.2d, v0.2d, v31.2d             ; 00 dc 7f 6e  
   280 : fdiv   v0.4s, v0.4s, v0.4s              ; 00 fc 20 6e  
   281 : fdiv   v31.4s, v0.4s, v0.4s             ; 1f fc 20 6e  
   282 : fdiv   v0.4s, v31.4s, v0.4s             ; e0 ff 20 6e  
   283 : fdiv   v0.4s, v0.4s, v31.4s             ; 00 fc 3f 6e  
   284 : fdiv   v0.2d, v0.2d, v0.2d              ; 00 fc 60 6e  
   285 : fdiv   v31.2d, v0.2d, v0.2d             ; 1f fc 60 6e  
   286 : fdiv   v0.2d, v31.2d, v0.2d             ; e0 ff 60 6e  
   287 : fdiv   v0.2d, v0.2d, v31.2d             ; 00 fc 7f 6e  
   288 : fdiv   v0.8h, v0.8h, v0.8h              ; 00 3c 40 6e  
   289 : fdiv   v31.8h, v0.8h, v0.8h             ; 1f 3c 40 6e  
   290 : fdiv   v0.8h, v31.8h, v0.8h             ; e0 3f 40 6e  
   291 : fdiv   v0.8h, v0.8h, v31.8h             ; 00 3c 5f 6e  
   292 : shl    v0.16b, v0.16b, #0x01, #0x01     ; 00 54 09 4f  
   293 : shl    v31.16b, v0.16b, #0x01, #0x01    ; 1f 54 09 4f  
   294 : shl    v0.16b, v31.16b, #0x01, #0x01    ; e0 57 09 4f  
   295 : shl    v0.16b, v0.16b, #0x01, #0x07     ; 00 54 0f 4f  
   296 : shl    v0.8h, v0.8h, #0x02, #0x01       ; 00 54 11 4f  
   297 : shl    v31.8h, v0.8h, #0x02, #0x01      ; 1f 54 11 4f  
   298 : shl    v0.8h, v31.8h, #0x02, #0x01      ; e0 57 11 4f  
   299 : shl    v0.8h, v0.8h, #0x03, #0x07       ; 00 54 1f 4f  
   300 : shl    v0.4s, v0.4s, #0x04, #0x01       ; 00 54 21 4f  
   301 : shl    v31.4s, v0.4s, #0x04, #0x01      ; 1f 54 21 4f  
   302 : shl    v0.4s, v31.4s, #0x04, #0x01      ; e0 57 21 4f  
   303 : shl    v0.4s, v0.4s, #0x07, #0x07       ; 00 54 3f 4f  
   304 : shl    v0.2d, v0.2d, #0x08, #0x01       ; 00 54 41 4f  
   305 : shl    v31.2d, v0.2d, #0x08, #0x01      ; 1f 54 41 4f  
   306 : shl    v0.2d, v31.2d, #0x08, #0x01      ; e0 57 41 4f  
   307 : shl    v0.2d, v0.2d, #0x0f, #0x07       ; 00 54 7f 4f  
   308 : ushl   v0.16b, v0.16b, v0.16b           ; 00 44 20 6e  
   309 : ushl   v31.16b, v0.16b, v0.16b          ; 1f 44 20 6e  
   310 : ushl   v0.16b, v31.16b, v0.16b          ; e0 47 20 6e  
   311 : ushl   v0.16b, v0.16b, v31.16b          ; 00 44 3f 6e  
   312 : ushl   v0.8h, v0.8h, v0.8h              ; 00 44 60 6e  
   313 : ushl   v31.8h, v0.8h, v0.8h             ; 1f 44 60 6e  
   314 : ushl   v0.8h, v31.8h, v0.8h             ; e0 47 60 6e  
   315 : ushl   v0.8h, v0.8h, v31.8h             ; 00 44 7f 6e  
   316 : ushl   v0.4s, v0.4s, v0.4s              ; 00 44 a0 6e  
   317 : ushl   v31.4s, v0.4s, v0.4s             ; 1f 44 a0 6e  
   318 : ushl   v0.4s, v31.4s, v0.4s             ; e0 47 a0 6e  
   319 : ushl   v0.4s, v0.4s, v31.4s             ; 00 44 bf 6e  
   320 : ushl   v0.2d, v0.2d, v0.2d              ; 00 44 e0 6e  
   321 : ushl   v31.2d, v0.2d, v0.2d             ; 1f 44 e0 6e  
   322 : ushl   v0.2d, v31.2d, v0.2d             ; e0 47 e0 6e  
   323 : ushl   v0.2d, v0.2d, v31.2d             ; 00 44 ff 6e  
   324 : sshl   v0.16b, v0.16b, v0.16b           ; 00 44 20 4e  
   325 : sshl   v31.16b, v0.16b, v0.16b          ; 1f 44 20 4e  
   326 : sshl   v0.16b, v31.16b, v0.16b          ; e0 47 20 4e  
   327 : sshl   v0.16b, v0.16b, v31.16b          ; 00 44 3f 4e  
   328 : sshl   v0.8h, v0.8h, v0.8h              ; 00 44 60 4e  
   329 : sshl   v31.8h, v0.8h, v0.8h             ; 1f 44 60 4e  
   330 : sshl   v0.8h, v31.8h, v0.8h             ; e0 47 60 4e  
   331 : sshl   v0.8h, v0.8h, v31.8h             ; 00 44 7f 4e  
   332 : sshl   v0.4s, v0.4s, v0.4s              ; 00 44 a0 4e  
   333 : sshl   v31.4s, v0.4s, v0.4s             ; 1f 44 a0 4e  
   334 : sshl   v0.4s, v31.4s, v0.4s             ; e0 47 a0 4e  
   335 : sshl   v0.4s, v0.4s, v31.4s             ; 00 44 bf 4e  
   336 : sshl   v0.2d, v0.2d, v0.2d              ; 00 44 e0 4e  
   337 : sshl   v31.2d, v0.2d, v0.2d             ; 1f 44 e0 4e  
   338 : sshl   v0.2d, v31.2d, v0.2d             ; e0 47 e0 4e  
   339 : sshl   v0.2d, v0.2d, v31.2d             ; 00 44 ff 4e  
   340 : ushr   v0.16b, v0.16b, #0x01, #0x07     ; 00 04 0f 6f  
   341 : ushr   v31.16b, v0.16b, #0x01, #0x07    ; 1f 04 0f 6f  
   342 : ushr   v0.16b, v31.16b, #0x01, #0x07    ; e0 07 0f 6f  
   343 : ushr   v0.16b, v0.16b, #0x01, #0x01     ; 00 04 09 6f  
   344 : ushr   v0.8h, v0.8h, #0x03, #0x07       ; 00 04 1f 6f  
   345 : ushr   v31.8h, v0.8h, #0x03, #0x07      ; 1f 04 1f 6f  
   346 : ushr   v0.8h, v31.8h, #0x03, #0x07      ; e0 07 1f 6f  
   347 : ushr   v0.8h, v0.8h, #0x02, #0x01       ; 00 04 11 6f  
   348 : ushr   v0.4s, v0.4s, #0x07, #0x07       ; 00 04 3f 6f  
   349 : ushr   v31.4s, v0.4s, #0x07, #0x07      ; 1f 04 3f 6f  
   350 : ushr   v0.4s, v31.4s, #0x07, #0x07      ; e0 07 3f 6f  
   351 : ushr   v0.4s, v0.4s, #0x04, #0x01       ; 00 04 21 6f  
   352 : ushr   v0.2d, v0.2d, #0x0f, #0x07       ; 00 04 7f 6f  
   353 : ushr   v31.2d, v0.2d, #0x0f, #0x07      ; 1f 04 7f 6f  
   354 : ushr   v0.2d, v31.2d, #0x0f, #0x07      ; e0 07 7f 6f  
   355 : ushr   v0.2d, v0.2d, #0x08, #0x01       ; 00 04 41 6f  
   356 : sshr   v0.16b, v0.16b, #0x01, #0x07     ; 00 04 0f 4f  
   357 : sshr   v31.16b, v0.16b, #0x01, #0x07    ; 1f 04 0f 4f  
   358 : sshr   v0.16b, v31.16b, #0x01, #0x07    ; e0 07 0f 4f  
   359 : sshr   v0.16b, v0.16b, #0x01, #0x01     ; 00 04 09 4f  
   360 : sshr   v0.8h, v0.8h, #0x03, #0x07       ; 00 04 1f 4f  
   361 : sshr   v31.8h, v0.8h, #0x03, #0x07      ; 1f 04 1f 4f  
   362 : sshr   v0.8h, v31.8h, #0x03, #0x07      ; e0 07 1f 4f  
   363 : sshr   v0.8h, v0.8h, #0x02, #0x01       ; 00 04 11 4f  
   364 : sshr   v0.4s, v0.4s, #0x07, #0x07       ; 00 04 3f 4f  
   365 : sshr   v31.4s, v0.4s, #0x07, #0x07      ; 1f 04 3f 4f  
   366 : sshr   v0.4s, v31.4s, #0x07, #0x07      ; e0 07 3f 4f  
   367 : sshr   v0.4s, v0.4s, #0x04, #0x01       ; 00 04 21 4f  
   368 : sshr   v0.2d, v0.2d, #0x0f, #0x07       ; 00 04 7f 4f  
   369 : sshr   v31.2d, v0.2d, #0x0f, #0x07      ; 1f 04 7f 4f  
   370 : sshr   v0.2d, v31.2d, #0x0f, #0x07      ; e0 07 7f 4f  
   371 : sshr   v0.2d, v0.2d, #0x08, #0x01       ; 00 04 41 4f  
   372 : and    v0.16b, v0.16b, v0.16b           ; 00 1c 20 4e  
   373 : and    v31.16b, v0.16b, v0.16b          ; 1f 1c 20 4e  
   374 : and    v0.16b, v31.16b, v0.16b          ; e0 1f 20 4e  
   375 : and    v0.16b, v0.16b, v31.16b          ; 00 1c 3f 4e  
   376 : orr    v0.16b, v0.16b, v0.16b           ; 00 1c a0 4e  
   377 : orr    v31.16b, v0.16b, v0.16b          ; 1f 1c a0 4e  
   378 : orr    v0.16b, v31.16b, v0.16b          ; e0 1f a0 4e  
   379 : orr    v0.16b, v0.16b, v31.16b          ; 00 1c bf 4e  
   380 : eor    v0.16b, v0.16b, v0.16b           ; 00 1c 20 6e  
   381 : eor    v31.16b, v0.16b, v0.16b          ; 1f 1c 20 6e  
   382 : eor    v0.16b, v31.16b, v0.16b          ; e0 1f 20 6e  
   383 : eor    v0.16b, v0.16b, v31.16b          ; 00 1c 3f 6e  
   384 : mvn    v0.16b, v0.16b                   ; 00 58 20 6e  
   385 : mvn    v31.16b, v0.16b                  ; 1f 58 20 6e  
   386 : mvn    v0.16b, v31.16b                  ; e0 5b 20 6e  
   387 : movi   v0.16b, #0, #0                   ; 00 e4 00 4f  
   388 : movi   v31.16b, #0, #0                  ; 1f e4 00 4f  
   389 : movi   v0.16b, #0x1f, #0x07             ; e0 e7 07 4f  
   390 : movi   v0.8h, #0, #0                    ; 00 84 00 4f  
   391 : movi   v31.8h, #0, #0                   ; 1f 84 00 4f  
   392 : movi   v0.8h, #0x1f, #0x07              ; e0 87 07 4f  
   393 : movi   v0.4s, #0, #0                    ; 00 04 00 4f  
   394 : movi   v31.4s, #0, #0                   ; 1f 04 00 4f  
   395 : movi   v0.4s, #0x1f, #0x07              ; e0 07 07 4f  
   396 : mvni   v0.8h, #0, #0                    ; 00 84 00 6f  
   397 : mvni   v31.8h, #0, #0                   ; 1f 84 00 6f  
   398 : mvni   v0.8h, #0x1f, #0x07              ; e0 87 07 6f  
   399 : mvni   v0.4s, #0, #0                    ; 00 04 00 6f  
   400 : mvni   v31.4s, #0, #0                   ; 1f 04 00 6f  
   401 : mvni   v0.4s, #0x1f, #0x07              ; e0 07 07 6f  
   402 : fmin   v0.8h, v0.8h, v0.8h              ; 00 34 c0 4e  
   403 : fmin   v31.8h, v0.8h, v0.8h             ; 1f 34 c0 4e  
   404 : fmin   v0.8h, v31.8h, v0.8h             ; e0 37 c0 4e  
   405 : fmin   v0.8h, v0.8h, v31.8h             ; 00 34 df 4e  
   406 : fmin   v0.4s, v0.4s, v0.4s              ; 00 f4 a0 4e  
   407 : fmin   v31.4s, v0.4s, v0.4s             ; 1f f4 a0 4e  
   408 : fmin   v0.4s, v31.4s, v0.4s             ; e0 f7 a0 4e  
   409 : fmin   v0.4s, v0.4s, v31.4s             ; 00 f4 bf 4e  
   410 : fmin   v0.2d, v0.2d, v0.2d              ; 00 f4 e0 4e  
   411 : fmin   v31.2d, v0.2d, v0.2d             ; 1f f4 e0 4e  
   412 : fmin   v0.2d, v31.2d, v0.2d             ; e0 f7 e0 4e  
   413 : fmin   v0.2d, v0.2d, v31.2d             ; 00 f4 ff 4e  
   414 : fmax   v0.8h, v0.8h, v0.8h              ; 00 34 40 4e  
   415 : fmax   v31.8h, v0.8h, v0.8h             ; 1f 34 40 4e  
   416 : fmax   v0.8h, v31.8h, v0.8h             ; e0 37 40 4e  
   417 : fmax   v0.8h, v0.8h, v31.8h             ; 00 34 5f 4e  
   418 : fmax   v0.4s, v0.4s, v0.4s              ; 00 f4 20 4e  
   419 : fmax   v31.4s, v0.4s, v0.4s             ; 1f f4 20 4e  
   420 : fmax   v0.4s, v31.4s, v0.4s             ; e0 f7 20 4e  
   421 : fmax   v0.4s, v0.4s, v31.4s             ; 00 f4 3f 4e  
   422 : fmax   v0.2d, v0.2d, v0.2d              ; 00 f4 60 4e  
   423 : fmax   v31.2d, v0.2d, v0.2d             ; 1f f4 60 4e  
   424 : fmax   v0.2d, v31.2d, v0.2d             ; e0 f7 60 4e  
   425 : fmax   v0.2d, v0.2d, v31.2d             ; 00 f4 7f 4e  
   426 : fmla   v0.8h, v0.8h, v0.8h              ; 00 0c 40 4e  
   427 : fmla   v31.8h, v0.8h, v0.8h             ; 1f 0c 40 4e  
   428 : fmla   v0.8h, v31.8h, v0.8h             ; e0 0f 40 4e  
   429 : fmla   v0.8h, v0.8h, v31.8h             ; 00 0c 5f 4e  
   430 : fmla   v0.4s, v0.4s, v0.4s              ; 00 cc 20 4e  
   431 : fmla   v31.4s, v0.4s, v0.4s             ; 1f cc 20 4e  
   432 : fmla   v0.4s, v31.4s, v0.4s             ; e0 cf 20 4e  
   433 : fmla   v0.4s, v0.4s, v31.4s             ; 00 cc 3f 4e  
   434 : fmla   v0.2d, v0.2d, v0.2d              ; 00 cc 60 4e  
   435 : fmla   v31.2d, v0.2d, v0.2d             ; 1f cc 60 4e  
   436 : fmla   v0.2d, v31.2d, v0.2d             ; e0 cf 60 4e  
   437 : fmla   v0.2d, v0.2d, v31.2d             ; 00 cc 7f 4e  
   438 : fcvtzs v0.8h, v0.8h                     ; 00 b8 f9 4e  
   439 : fcvtzs v31.8h, v0.8h                    ; 1f b8 f9 4e  
   440 : fcvtzs v0.8h, v31.8h                    ; e0 bb f9 4e  
   441 : fcvtzs v0.4s, v0.4s                     ; 00 b8 a1 4e  
   442 : fcvtzs v31.4s, v0.4s                    ; 1f b8 a1 4e  
   443 : fcvtzs v0.4s, v31.4s                    ; e0 bb a1 4e  
   444 : fcvtzs v0.2d, v0.2d                     ; 00 b8 e1 4e  
   445 : fcvtzs v31.2d, v0.2d                    ; 1f b8 e1 4e  
   446 : fcvtzs v0.2d, v31.2d                    ; e0 bb e1 4e  
   447 : fcvtzu v0.8h, v0.8h                     ; 00 b8 f9 6e  
   448 : fcvtzu v31.8h, v0.8h                    ; 1f b8 f9 6e  
   449 : fcvtzu v0.8h, v31.8h                    ; e0 bb f9 6e  
   450 : fcvtzu v0.4s, v0.4s                     ; 00 b8 a1 6e  
   451 : fcvtzu v31.4s, v0.4s                    ; 1f b8 a1 6e  
   452 : fcvtzu v0.4s, v31.4s                    ; e0 bb a1 6e  
   453 : fcvtzu v0.2d, v0.2d                     ; 00 b8 e1 6e  
   454 : fcvtzu v31.2d, v0.2d                    ; 1f b8 e1 6e  
   455 : fcvtzu v0.2d, v31.2d                    ; e0 bb e1 6e  
   456 : fcvtms v0.8h, v0.8h                     ; 00 b8 79 4e  
   457 : fcvtms v31.8h, v0.8h                    ; 1f b8 79 4e  
   458 : fcvtms v0.8h, v31.8h                    ; e0 bb 79 4e  
   459 : fcvtms v0.4s, v0.4s                     ; 00 b8 21 4e  
   460 : fcvtms v31.4s, v0.4s                    ; 1f b8 21 4e  
   461 : fcvtms v0.4s, v31.4s                    ; e0 bb 21 4e  
   462 : fcvtms v0.2d, v0.2d                     ; 00 b8 61 4e  
   463 : fcvtms v31.2d, v0.2d                    ; 1f b8 61 4e  
   464 : fcvtms v0.2d, v31.2d                    ; e0 bb 61 4e  
   465 : fcvtmu v0.8h, v0.8h                     ; 00 b8 79 6e  
   466 : fcvtmu v31.8h, v0.8h                    ; 1f b8 79 6e  
   467 : fcvtmu v0.8h, v31.8h                    ; e0 bb 79 6e  
   468 : fcvtmu v0.4s, v0.4s                     ; 00 b8 21 6e  
   469 : fcvtmu v31.4s, v0.4s                    ; 1f b8 21 6e  
   470 : fcvtmu v0.4s, v31.4s                    ; e0 bb 21 6e  
   471 : fcvtmu v0.2d, v0.2d                     ; 00 b8 61 6e  
   472 : fcvtmu v31.2d, v0.2d                    ; 1f b8 61 6e  
   473 : fcvtmu v0.2d, v31.2d                    ; e0 bb 61 6e  
   474 : scvtf  v0.8h, v0.8h                     ; 00 d8 79 4e  
   475 : scvtf  v31.8h, v0.8h                    ; 1f d8 79 4e  
   476 : scvtf  v0.8h, v31.8h                    ; e0 db 79 4e  
   477 : scvtf  v0.4s, v0.4s                     ; 00 d8 21 4e  
   478 : scvtf  v31.4s, v0.4s                    ; 1f d8 21 4e  
   479 : scvtf  v0.4s, v31.4s                    ; e0 db 21 4e  
   480 : scvtf  v0.2d, v0.2d                     ; 00 d8 61 4e  
   481 : scvtf  v31.2d, v0.2d                    ; 1f d8 61 4e  
   482 : scvtf  v0.2d, v31.2d                    ; e0 db 61 4e  
   483 : ucvtf  v0.8h, v0.8h                     ; 00 d8 79 6e  
   484 : ucvtf  v31.8h, v0.8h                    ; 1f d8 79 6e  
   485 : ucvtf  v0.8h, v31.8h                    ; e0 db 79 6e  
   486 : ucvtf  v0.4s, v0.4s                     ; 00 d8 21 6e  
   487 : ucvtf  v31.4s, v0.4s                    ; 1f d8 21 6e  
   488 : ucvtf  v0.4s, v31.4s                    ; e0 db 21 6e  
   489 : ucvtf  v0.2d, v0.2d                     ; 00 d8 61 6e  
   490 : ucvtf  v31.2d, v0.2d                    ; 1f d8 61 6e  
   491 : ucvtf  v0.2d, v31.2d                    ; e0 db 61 6e  
   492 : fcmgt  v0.8h, v0.8h, v0.8h              ; 00 24 c0 6e  
   493 : fcmgt  v31.8h, v0.8h, v0.8h             ; 1f 24 c0 6e  
   494 : fcmgt  v0.8h, v31.8h, v0.8h             ; e0 27 c0 6e  
   495 : fcmgt  v0.8h, v0.8h, v31.8h             ; 00 24 df 6e  
   496 : fcmgt  v0.4s, v0.4s, v0.4s              ; 00 e4 a0 6e  
   497 : fcmgt  v31.4s, v0.4s, v0.4s             ; 1f e4 a0 6e  
   498 : fcmgt  v0.4s, v31.4s, v0.4s             ; e0 e7 a0 6e  
   499 : fcmgt  v0.4s, v0.4s, v31.4s             ; 00 e4 bf 6e  
   500 : fcmgt  v0.2d, v0.2d, v0.2d              ; 00 e4 e0 6e  
   501 : fcmgt  v31.2d, v0.2d, v0.2d             ; 1f e4 e0 6e  
   502 : fcmgt  v0.2d, v31.2d, v0.2d             ; e0 e7 e0 6e  
   503 : fcmgt  v0.2d, v0.2d, v31.2d             ; 00 e4 ff 6e  
   504 : fcmge  v0.8h, v0.8h, v0.8h              ; 00 24 40 6e  
   505 : fcmge  v31.8h, v0.8h, v0.8h             ; 1f 24 40 6e  
   506 : fcmge  v0.8h, v31.8h, v0.8h             ; e0 27 40 6e  
   507 : fcmge  v0.8h, v0.8h, v31.8h             ; 00 24 5f 6e  
   508 : fcmge  v0.4s, v0.4s, v0.4s              ; 00 e4 20 6e  
   509 : fcmge  v31.4s, v0.4s, v0.4s             ; 1f e4 20 6e  
   510 : fcmge  v0.4s, v31.4s, v0.4s             ; e0 e7 20 6e  
   511 : fcmge  v0.4s, v0.4s, v31.4s             ; 00 e4 3f 6e  
   512 : fcmge  v0.2d, v0.2d, v0.2d              ; 00 e4 60 6e  
   513 : fcmge  v31.2d, v0.2d, v0.2d             ; 1f e4 60 6e  
   514 : fcmge  v0.2d, v31.2d, v0.2d             ; e0 e7 60 6e  
   515 : fcmge  v0.2d, v0.2d, v31.2d             ; 00 e4 7f 6e  
   516 : fcmeq  v0.8h, v0.8h, v0.8h              ; 00 24 40 4e  
   517 : fcmeq  v31.8h, v0.8h, v0.8h             ; 1f 24 40 4e  
   518 : fcmeq  v0.8h, v31.8h, v0.8h             ; e0 27 40 4e  
   519 : fcmeq  v0.8h, v0.8h, v31.8h             ; 00 24 5f 4e  
   520 : fcmeq  v0.4s, v0.4s, v0.4s              ; 00 e4 20 4e  
   521 : fcmeq  v31.4s, v0.4s, v0.4s             ; 1f e4 20 4e  
   522 : fcmeq  v0.4s, v31.4s, v0.4s             ; e0 e7 20 4e  
   523 : fcmeq  v0.4s, v0.4s, v31.4s             ; 00 e4 3f 4e  
   524 : fcmeq  v0.2d, v0.2d, v0.2d              ; 00 e4 60 4e  
   525 : fcmeq  v31.2d, v0.2d, v0.2d             ; 1f e4 60 4e  
   526 : fcmeq  v0.2d, v31.2d, v0.2d             ; e0 e7 60 4e  
   527 : fcmeq  v0.2d, v0.2d, v31.2d             ; 00 e4 7f 4e  
   528 : neg    v0.16b, v0.16b                   ; 00 b8 20 6e  
   529 : neg    v31.16b, v0.16b                  ; 1f b8 20 6e  
   530 : neg    v0.16b, v31.16b                  ; e0 bb 20 6e  
   531 : neg    v0.8h, v0.8h                     ; 00 b8 60 6e  
   532 : neg    v31.8h, v0.8h                    ; 1f b8 60 6e  
   533 : neg    v0.8h, v31.8h                    ; e0 bb 60 6e  
   534 : neg    v0.4s, v0.4s                     ; 00 b8 a0 6e  
   535 : neg    v31.4s, v0.4s                    ; 1f b8 a0 6e  
   536 : neg    v0.4s, v31.4s                    ; e0 bb a0 6e  
   537 : neg    v0.2d, v0.2d                     ; 00 b8 e0 6e  
   538 : neg    v31.2d, v0.2d                    ; 1f b8 e0 6e  
   539 : neg    v0.2d, v31.2d                    ; e0 bb e0 6e  
   540 : fneg   v0.8h, v0.8h                     ; 00 f8 f8 6e  
   541 : fneg   v31.8h, v0.8h                    ; 1f f8 f8 6e  
   542 : fneg   v0.8h, v31.8h                    ; e0 fb f8 6e  
   543 : fneg   v0.4s, v0.4s                     ; 00 f8 a0 6e  
   544 : fneg   v31.4s, v0.4s                    ; 1f f8 a0 6e  
   545 : fneg   v0.4s, v31.4s                    ; e0 fb a0 6e  
   546 : fneg   v0.2d, v0.2d                     ; 00 f8 e0 6e  
   547 : fneg   v31.2d, v0.2d                    ; 1f f8 e0 6e  
   548 : fneg   v0.2d, v31.2d                    ; e0 fb e0 6e  
   549 : movk   x0, #0, #0x01                    ; 00 00 a0 f2  
   550 : movk   x15, #0, #0x01                   ; 0f 00 a0 f2  
   551 : movk   x0, #0xffff, #0x01               ; e0 ff bf f2  
   552 : movk   x0, #0, #0x02                    ; 00 00 c0 f2  
   553 : movk   x15, #0, #0x02                   ; 0f 00 c0 f2  
   554 : movk   x0, #0xffff, #0x02               ; e0 ff df f2  
   555 : movk   x0, #0, #0x03                    ; 00 00 e0 f2  
   556 : movk   x15, #0, #0x03                   ; 0f 00 e0 f2  
   557 : movk   x0, #0xffff, #0x03               ; e0 ff ff f2  
   558 : dup    v0.16b, x0                       ; 00 0c 01 4e  
   559 : dup    v31.16b, x0                      ; 1f 0c 01 4e  
   560 : dup    v0.16b, x15                      ; e0 0d 01 4e  
   561 : dup    v0.8h, x0                        ; 00 0c 02 4e  
   562 : dup    v31.8h, x0                       ; 1f 0c 02 4e  
   563 : dup    v0.8h, x15                       ; e0 0d 02 4e  
   564 : dup    v0.4s, x0                        ; 00 0c 04 4e  
   565 : dup    v31.4s, x0                       ; 1f 0c 04 4e  
   566 : dup    v0.4s, x15                       ; e0 0d 04 4e  
   567 : dup    v0.2d, x0                        ; 00 0c 08 4e  
   568 : dup    v31.2d, x0                       ; 1f 0c 08 4e  
   569 : dup    v0.2d, x15                       ; e0 0d 08 4e  
   570 : dup    v0.16b, v0.16b, #0               ; 00 04 01 4e  
   571 : dup    v31.16b, v0.16b, #0              ; 1f 04 01 4e  
   572 : dup    v0.16b, v31.16b, #0              ; e0 07 01 4e  
   573 : dup    v0.16b, v0.16b, #0x0f            ; 00 04 1f 4e  
   574 : dup    v0.8h, v0.8h, #0                 ; 00 04 02 4e  
   575 : dup    v31.8h, v0.8h, #0                ; 1f 04 02 4e  
   576 : dup    v0.8h, v31.8h, #0                ; e0 07 02 4e  
   577 : dup    v0.8h, v0.8h, #0x07              ; 00 04 1e 4e  
   578 : dup    v0.4s, v0.4s, #0                 ; 00 04 04 4e  
   579 : dup    v31.4s, v0.4s, #0                ; 1f 04 04 4e  
   580 : dup    v0.4s, v31.4s, #0                ; e0 07 04 4e  
   581 : dup    v0.4s, v0.4s, #0x03              ; 00 04 1c 4e  
   582 : dup    v0.2d, v0.2d, #0                 ; 00 04 08 4e  
   583 : dup    v31.2d, v0.2d, #0                ; 1f 04 08 4e  
   584 : dup    v0.2d, v31.2d, #0                ; e0 07 08 4e  
   585 : dup    v0.2d, v0.2d, #0x01              ; 00 04 18 4e  
   586 : sshll  v0.8h, v0.16b, #0                ; 00 a4 08 0f  
   587 : sshll  v31.8h, v0.16b, #0               ; 1f a4 08 0f  
   588 : sshll  v0.8h, v31.16b, #0               ; e0 a7 08 0f  
   589 : sshll  v0.4s, v0.8h, #0                 ; 00 a4 10 0f  
   590 : sshll  v31.4s, v0.8h, #0                ; 1f a4 10 0f  
   591 : sshll  v0.4s, v31.8h, #0                ; e0 a7 10 0f  
   592 : sshll  v0.2d, v0.4s, #0                 ; 00 a4 20 0f  
   593 : sshll  v31.2d, v0.4s, #0                ; 1f a4 20 0f  
   594 : sshll  v0.2d, v31.4s, #0                ; e0 a7 20 0f  
   595 : ushll  v0.8h, v0.16b, #0                ; 00 a4 08 2f  
   596 : ushll  v31.8h, v0.16b, #0               ; 1f a4 08 2f  
   597 : ushll  v0.8h, v31.16b, #0               ; e0 a7 08 2f  
   598 : ushll  v0.4s, v0.8h, #0                 ; 00 a4 10 2f  
   599 : ushll  v31.4s, v0.8h, #0                ; 1f a4 10 2f  
   600 : ushll  v0.4s, v31.8h, #0                ; e0 a7 10 2f  
   601 : ushll  v0.2d, v0.4s, #0                 ; 00 a4 20 2f  
   602 : ushll  v31.2d, v0.4s, #0                ; 1f a4 20 2f  
   603 : ushll  v0.2d, v31.4s, #0                ; e0 a7 20 2f  
   604 : sshll2 v0.8h, v0.16b, #0                ; 00 a4 08 4f  
   605 : sshll2 v31.8h, v0.16b, #0               ; 1f a4 08 4f  
   606 : sshll2 v0.8h, v31.16b, #0               ; e0 a7 08 4f  
   607 : sshll2 v0.4s, v0.8h, #0                 ; 00 a4 10 4f  
   608 : sshll2 v31.4s, v0.8h, #0                ; 1f a4 10 4f  
   609 : sshll2 v0.4s, v31.8h, #0                ; e0 a7 10 4f  
   610 : sshll2 v0.2d, v0.4s, #0                 ; 00 a4 20 4f  
   611 : sshll2 v31.2d, v0.4s, #0                ; 1f a4 20 4f  
   612 : sshll2 v0.2d, v31.4s, #0                ; e0 a7 20 4f  
   613 : ushll2 v0.8h, v0.16b, #0                ; 00 a4 08 6f  
   614 : ushll2 v31.8h, v0.16b, #0               ; 1f a4 08 6f  
   615 : ushll2 v0.8h, v31.16b, #0               ; e0 a7 08 6f  
   616 : ushll2 v0.4s, v0.8h, #0                 ; 00 a4 10 6f  
   617 : ushll2 v31.4s, v0.8h, #0                ; 1f a4 10 6f  
   618 : ushll2 v0.4s, v31.8h, #0                ; e0 a7 10 6f  
   619 : ushll2 v0.2d, v0.4s, #0                 ; 00 a4 20 6f  
   620 : ushll2 v31.2d, v0.4s, #0                ; 1f a4 20 6f  
   621 : ushll2 v0.2d, v31.4s, #0                ; e0 a7 20 6f  
   622 : xtn    v0.16b, v0.8h                    ; 00 28 21 0e  
   623 : xtn    v31.16b, v0.8h                   ; 1f 28 21 0e  
   624 : xtn    v0.16b, v31.8h                   ; e0 2b 21 0e  
   625 : xtn    v0.8h, v0.4s                     ; 00 28 61 0e  
   626 : xtn    v31.8h, v0.4s                    ; 1f 28 61 0e  
   627 : xtn    v0.8h, v31.4s                    ; e0 2b 61 0e  
   628 : xtn    v0.4s, v0.2d                     ; 00 28 a1 0e  
   629 : xtn    v31.4s, v0.2d                    ; 1f 28 a1 0e  
   630 : xtn    v0.4s, v31.2d                    ; e0 2b a1 0e  
   631 : xtn2   v0.16b, v0.8h                    ; 00 28 21 4e  
   632 : xtn2   v31.16b, v0.8h                   ; 1f 28 21 4e  
   633 : xtn2   v0.16b, v31.8h                   ; e0 2b 21 4e  
   634 : xtn2   v0.8h, v0.4s                     ; 00 28 61 4e  
   635 : xtn2   v31.8h, v0.4s                    ; 1f 28 61 4e  
   636 : xtn2   v0.8h, v31.4s                    ; e0 2b 61 4e  
   637 : xtn2   v0.4s, v0.2d                     ; 00 28 a1 4e  
   638 : xtn2   v31.4s, v0.2d                    ; 1f 28 a1 4e  
   639 : xtn2   v0.4s, v31.2d                    ; e0 2b a1 4e  
   640 : smaxv  v0.16b, v0.16b                   ; 00 a8 30 4e  
   641 : smaxv  v31.16b, v0.16b                  ; 1f a8 30 4e  
   642 : smaxv  v0.16b, v31.16b                  ; e0 ab 30 4e  
   643 : smaxv  v0.8h, v0.8h                     ; 00 a8 70 4e  
   644 : smaxv  v31.8h, v0.8h                    ; 1f a8 70 4e  
   645 : smaxv  v0.8h, v31.8h                    ; e0 ab 70 4e  
   646 : smaxv  v0.4s, v0.4s                     ; 00 a8 b0 4e  
   647 : smaxv  v31.4s, v0.4s                    ; 1f a8 b0 4e  
   648 : smaxv  v0.4s, v31.4s                    ; e0 ab b0 4e  
   649 : umaxv  v0.16b, v0.16b                   ; 00 a8 30 6e  
   650 : umaxv  v31.16b, v0.16b                  ; 1f a8 30 6e  
   651 : umaxv  v0.16b, v31.16b                  ; e0 ab 30 6e  
   652 : umaxv  v0.8h, v0.8h                     ; 00 a8 70 6e  
   653 : umaxv  v31.8h, v0.8h                    ; 1f a8 70 6e  
   654 : umaxv  v0.8h, v31.8h                    ; e0 ab 70 6e  
   655 : umaxv  v0.4s, v0.4s                     ; 00 a8 b0 6e  
   656 : umaxv  v31.4s, v0.4s                    ; 1f a8 b0 6e  
   657 : umaxv  v0.4s, v31.4s                    ; e0 ab b0 6e  
   658 : fmaxv  v0.8h, v0.8h                     ; 00 f8 30 4e  
   659 : fmaxv  v31.8h, v0.8h                    ; 1f f8 30 4e  
   660 : fmaxv  v0.8h, v31.8h                    ; e0 fb 30 4e  
   661 : fmaxv  v0.4s, v0.4s                     ; 00 f8 30 6e  
   662 : fmaxv  v31.4s, v0.4s                    ; 1f f8 30 6e  
   663 : fmaxv  v0.4s, v31.4s                    ; e0 fb 30 6e  
   664 : sminv  v0.16b, v0.16b                   ; 00 a8 31 4e  
   665 : sminv  v31.16b, v0.16b                  ; 1f a8 31 4e  
   666 : sminv  v0.16b, v31.16b                  ; e0 ab 31 4e  
   667 : sminv  v0.8h, v0.8h                     ; 00 a8 71 4e  
   668 : sminv  v31.8h, v0.8h                    ; 1f a8 71 4e  
   669 : sminv  v0.8h, v31.8h                    ; e0 ab 71 4e  
   670 : sminv  v0.4s, v0.4s                     ; 00 a8 b1 4e  
   671 : sminv  v31.4s, v0.4s                    ; 1f a8 b1 4e  
   672 : sminv  v0.4s, v31.4s                    ; e0 ab b1 4e  
   673 : uminv  v0.16b, v0.16b                   ; 00 a8 31 6e  
   674 : uminv  v31.16b, v0.16b                  ; 1f a8 31 6e  
   675 : uminv  v0.16b, v31.16b                  ; e0 ab 31 6e  
   676 : uminv  v0.8h, v0.8h                     ; 00 a8 71 6e  
   677 : uminv  v31.8h, v0.8h                    ; 1f a8 71 6e  
   678 : uminv  v0.8h, v31.8h                    ; e0 ab 71 6e  
   679 : uminv  v0.4s, v0.4s                     ; 00 a8 b1 6e  
   680 : uminv  v31.4s, v0.4s                    ; 1f a8 b1 6e  
   681 : uminv  v0.4s, v31.4s                    ; e0 ab b1 6e  
   682 : fminv  v0.8h, v0.8h                     ; 00 f8 b0 4e  
   683 : fminv  v31.8h, v0.8h                    ; 1f f8 b0 4e  
   684 : fminv  v0.8h, v31.8h                    ; e0 fb b0 4e  
   685 : fminv  v0.4s, v0.4s                     ; 00 f8 b0 6e  
   686 : fminv  v31.4s, v0.4s                    ; 1f f8 b0 6e  
   687 : fminv  v0.4s, v31.4s                    ; e0 fb b0 6e  
   688 : ld1    v0.16b, x0, #0                   ; 00 00 40 0d  
   689 : ld1    v31.16b, x0, #0                  ; 1f 00 40 0d  
   690 : ld1    v0.16b, x0, #0x0f                ; 00 1c 40 4d  
   691 : ld1    v0.16b, x15, #0                  ; e0 01 40 0d  
   692 : ld1    v0.8h, x0, #0                    ; 00 40 40 0d  
   693 : ld1    v31.8h, x0, #0                   ; 1f 40 40 0d  
   694 : ld1    v0.8h, x0, #0x07                 ; 00 58 40 4d  
   695 : ld1    v0.8h, x15, #0                   ; e0 41 40 0d  
   696 : ld1    v0.4s, x0, #0                    ; 00 80 40 0d  
   697 : ld1    v31.4s, x0, #0                   ; 1f 80 40 0d  
   698 : ld1    v0.4s, x0, #0x03                 ; 00 90 40 4d  
   699 : ld1    v0.4s, x15, #0                   ; e0 81 40 0d  
   700 : ld1    v0.2d, x0, #0                    ; 00 84 40 0d  
   701 : ld1    v31.2d, x0, #0                   ; 1f 84 40 0d  
   702 : ld1    v0.2d, x0, #0x01                 ; 00 84 40 4d  
   703 : ld1    v0.2d, x15, #0                   ; e0 85 40 0d  
   704 : ld2    v0.16b, v1.16b, x0               ; 00 80 40 4c  
   705 : ld2    v31.16b, v0.16b, x0              ; 1f 80 40 4c  
   706 : ld2    v0.16b, v1.16b, x15              ; e0 81 40 4c  
   707 : ld2    v0.8h, v1.8h, x0                 ; 00 84 40 4c  
   708 : ld2    v31.8h, v0.8h, x0                ; 1f 84 40 4c  
   709 : ld2    v0.8h, v1.8h, x15                ; e0 85 40 4c  
   710 : ld2    v0.4s, v1.4s, x0                 ; 00 88 40 4c  
   711 : ld2    v31.4s, v0.4s, x0                ; 1f 88 40 4c  
   712 : ld2    v0.4s, v1.4s, x15                ; e0 89 40 4c  
   713 : ld2    v0.2d, v1.2d, x0                 ; 00 8c 40 4c  
   714 : ld2    v31.2d, v0.2d, x0                ; 1f 8c 40 4c  
   715 : ld2    v0.2d, v1.2d, x15                ; e0 8d 40 4c  
   716 : ld1    v0.16b, x0                       ; 00 20 df 4c  
   717 : ld1    v31.16b, x0                      ; 1f 20 df 4c  
   718 : ld1    v0.16b, x15                      ; e0 21 df 4c  
   719 : ld1    v0.8h, x0                        ; 00 24 df 4c  
   720 : ld1    v31.8h, x0                       ; 1f 24 df 4c  
   721 : ld1    v0.8h, x15                       ; e0 25 df 4c  
   722 : ld1    v0.4s, x0                        ; 00 28 df 4c  
   723 : ld1    v31.4s, x0                       ; 1f 28 df 4c  
   724 : ld1    v0.4s, x15                       ; e0 29 df 4c  
   725 : ld1    v0.2d, x0                        ; 00 2c df 4c  
   726 : ld1    v31.2d, x0                       ; 1f 2c df 4c  
   727 : ld1    v0.2d, x15                       ; e0 2d df 4c  
   728 : st1    v0.16b, #0, x0                   ; 00 00 00 0d  
   729 : st1    v31.16b, #0, x0                  ; 1f 00 00 0d  
   730 : st1    v0.16b, #0x0f, x0                ; 00 1c 00 4d  
   731 : st1    v0.16b, #0, x15                  ; e0 01 00 0d  
   732 : st1    v0.8h, #0, x0                    ; 00 40 00 0d  
   733 : st1    v31.8h, #0, x0                   ; 1f 40 00 0d  
   734 : st1    v0.8h, #0x07, x0                 ; 00 58 00 4d  
   735 : st1    v0.8h, #0, x15                   ; e0 41 00 0d  
   736 : st1    v0.4s, #0, x0                    ; 00 80 00 0d  
   737 : st1    v31.4s, #0, x0                   ; 1f 80 00 0d  
   738 : st1    v0.4s, #0x03, x0                 ; 00 90 00 4d  
   739 : st1    v0.4s, #0, x15                   ; e0 81 00 0d  
   740 : st1    v0.2d, #0, x0                    ; 00 84 00 0d  
   741 : st1    v31.2d, #0, x0                   ; 1f 84 00 0d  
   742 : st1    v0.2d, #0x01, x0                 ; 00 84 00 4d  
   743 : st1    v0.2d, #0, x15                   ; e0 85 00 0d  
   744 : st1    v0.16b, x0                       ; 00 20 9f 4c  
   745 : st1    v0.16b, x15                      ; e0 21 9f 4c  
   746 : st1    v31.16b, x0                      ; 1f 20 9f 4c  
   747 : st1    v0.8h, x0                        ; 00 24 9f 4c  
   748 : st1    v0.8h, x15                       ; e0 25 9f 4c  
   749 : st1    v31.8h, x0                       ; 1f 24 9f 4c  
   750 : st1    v0.4s, x0                        ; 00 28 9f 4c  
   751 : st1    v0.4s, x15                       ; e0 29 9f 4c  
   752 : st1    v31.4s, x0                       ; 1f 28 9f 4c  
   753 : st1    v0.2d, x0                        ; 00 2c 9f 4c  
   754 : st1    v0.2d, x15                       ; e0 2d 9f 4c  
   755 : st1    v31.2d, x0                       ; 1f 2c 9f 4c  
   756 : ext    v0.16b, v0.16b, v0.16b, #0       ; 00 00 00 6e  
   757 : ext    v31.16b, v0.16b, v0.16b, #0      ; 1f 00 00 6e  
   758 : ext    v0.16b, v31.16b, v0.16b, #0      ; e0 03 00 6e  
   759 : ext    v0.16b, v0.16b, v31.16b, #0      ; 00 00 1f 6e  
   760 : ext    v0.16b, v0.16b, v0.16b, #0x0f    ; 00 78 00 6e  
   761 : ext    v0.8h, v0.8h, v0.8h, #0          ; 00 00 00 6e  
   762 : ext    v31.8h, v0.8h, v0.8h, #0         ; 1f 00 00 6e  
   763 : ext    v0.8h, v31.8h, v0.8h, #0         ; e0 03 00 6e  
   764 : ext    v0.8h, v0.8h, v31.8h, #0         ; 00 00 1f 6e  
   765 : ext    v0.8h, v0.8h, v0.8h, #0x0e       ; 00 70 00 6e  
   766 : ext    v0.4s, v0.4s, v0.4s, #0          ; 00 00 00 6e  
   767 : ext    v31.4s, v0.4s, v0.4s, #0         ; 1f 00 00 6e  
   768 : ext    v0.4s, v31.4s, v0.4s, #0         ; e0 03 00 6e  
   769 : ext    v0.4s, v0.4s, v31.4s, #0         ; 00 00 1f 6e  
   770 : ext    v0.4s, v0.4s, v0.4s, #0x0c       ; 00 60 00 6e  
   771 : ext    v0.2d, v0.2d, v0.2d, #0          ; 00 00 00 6e  
   772 : ext    v31.2d, v0.2d, v0.2d, #0         ; 1f 00 00 6e  
   773 : ext    v0.2d, v31.2d, v0.2d, #0         ; e0 03 00 6e  
   774 : ext    v0.2d, v0.2d, v31.2d, #0         ; 00 00 1f 6e  
   775 : ext    v0.2d, v0.2d, v0.2d, #0x10       ; 00 00 00 6e  
   776 : fmla   v0.8h, v0.8h, v0.8h, #0          ; 00 10 00 4f  
   777 : fmla   v31.8h, v0.8h, v0.8h, #0         ; 1f 10 00 4f  
   778 : fmla   v0.8h, v31.8h, v0.8h, #0         ; e0 13 00 4f  
   779 : fmla   v0.8h, v0.8h, v15.8h, #0         ; 00 10 0f 4f  
   780 : fmla   v0.8h, v0.8h, v0.8h, #0x07       ; 00 18 30 4f  
   781 : fmla   v0.4s, v0.4s, v0.4s, #0          ; 00 10 80 4f  
   782 : fmla   v31.4s, v0.4s, v0.4s, #0         ; 1f 10 80 4f  
   783 : fmla   v0.4s, v31.4s, v0.4s, #0         ; e0 13 80 4f  
   784 : fmla   v0.4s, v0.4s, v31.4s, #0         ; 00 10 9f 4f  
   785 : fmla   v0.4s, v0.4s, v0.4s, #0x03       ; 00 18 a0 4f  
   786 : fmla   v0.2d, v0.2d, v0.2d, #0          ; 00 10 c0 4f  
   787 : fmla   v31.2d, v0.2d, v0.2d, #0         ; 1f 10 c0 4f  
   788 : fmla   v0.2d, v31.2d, v0.2d, #0         ; e0 13 c0 4f  
   789 : fmla   v0.2d, v0.2d, v31.2d, #0         ; 00 10 df 4f  
   790 : fmla   v0.2d, v0.2d, v0.2d, #0x01       ; 00 18 c0 4f  
   791 : cmhi   v0.16b, v0.16b, v0.16b           ; 00 34 20 6e  
   792 : cmhi   v31.16b, v0.16b, v0.16b          ; 1f 34 20 6e  
   793 : cmhi   v0.16b, v31.16b, v0.16b          ; e0 37 20 6e  
   794 : cmhi   v0.16b, v0.16b, v31.16b          ; 00 34 3f 6e  
   795 : cmhi   v0.8h, v0.8h, v0.8h              ; 00 34 60 6e  
   796 : cmhi   v31.8h, v0.8h, v0.8h             ; 1f 34 60 6e  
   797 : cmhi   v0.8h, v31.8h, v0.8h             ; e0 37 60 6e  
   798 : cmhi   v0.8h, v0.8h, v31.8h             ; 00 34 7f 6e  
   799 : cmhi   v0.4s, v0.4s, v0.4s              ; 00 34 a0 6e  
   800 : cmhi   v31.4s, v0.4s, v0.4s             ; 1f 34 a0 6e  
   801 : cmhi   v0.4s, v31.4s, v0.4s             ; e0 37 a0 6e  
   802 : cmhi   v0.4s, v0.4s, v31.4s             ; 00 34 bf 6e  
   803 : cmhi   v0.2d, v0.2d, v0.2d              ; 00 34 e0 6e  
   804 : cmhi   v31.2d, v0.2d, v0.2d             ; 1f 34 e0 6e  
   805 : cmhi   v0.2d, v31.2d, v0.2d             ; e0 37 e0 6e  
   806 : cmhi   v0.2d, v0.2d, v31.2d             ; 00 34 ff 6e  
   807 : cmhs   v0.16b, v0.16b, v0.16b           ; 00 3c 20 6e  
   808 : cmhs   v31.16b, v0.16b, v0.16b          ; 1f 3c 20 6e  
   809 : cmhs   v0.16b, v31.16b, v0.16b          ; e0 3f 20 6e  
   810 : cmhs   v0.16b, v0.16b, v31.16b          ; 00 3c 3f 6e  
   811 : cmhs   v0.8h, v0.8h, v0.8h              ; 00 3c 60 6e  
   812 : cmhs   v31.8h, v0.8h, v0.8h             ; 1f 3c 60 6e  
   813 : cmhs   v0.8h, v31.8h, v0.8h             ; e0 3f 60 6e  
   814 : cmhs   v0.8h, v0.8h, v31.8h             ; 00 3c 7f 6e  
   815 : cmhs   v0.4s, v0.4s, v0.4s              ; 00 3c a0 6e  
   816 : cmhs   v31.4s, v0.4s, v0.4s             ; 1f 3c a0 6e  
   817 : cmhs   v0.4s, v31.4s, v0.4s             ; e0 3f a0 6e  
   818 : cmhs   v0.4s, v0.4s, v31.4s             ; 00 3c bf 6e  
   819 : cmhs   v0.2d, v0.2d, v0.2d              ; 00 3c e0 6e  
   820 : cmhs   v31.2d, v0.2d, v0.2d             ; 1f 3c e0 6e  
   821 : cmhs   v0.2d, v31.2d, v0.2d             ; e0 3f e0 6e  
   822 : cmhs   v0.2d, v0.2d, v31.2d             ; 00 3c ff 6e  
   823 : cmeq   v0.16b, v0.16b, v0.16b           ; 00 8c 20 6e  
   824 : cmeq   v31.16b, v0.16b, v0.16b          ; 1f 8c 20 6e  
   825 : cmeq   v0.16b, v31.16b, v0.16b          ; e0 8f 20 6e  
   826 : cmeq   v0.16b, v0.16b, v31.16b          ; 00 8c 3f 6e  
   827 : cmeq   v0.8h, v0.8h, v0.8h              ; 00 8c 60 6e  
   828 : cmeq   v31.8h, v0.8h, v0.8h             ; 1f 8c 60 6e  
   829 : cmeq   v0.8h, v31.8h, v0.8h             ; e0 8f 60 6e  
   830 : cmeq   v0.8h, v0.8h, v31.8h             ; 00 8c 7f 6e  
   831 : cmeq   v0.4s, v0.4s, v0.4s              ; 00 8c a0 6e  
   832 : cmeq   v31.4s, v0.4s, v0.4s             ; 1f 8c a0 6e  
   833 : cmeq   v0.4s, v31.4s, v0.4s             ; e0 8f a0 6e  
   834 : cmeq   v0.4s, v0.4s, v31.4s             ; 00 8c bf 6e  
   835 : cmeq   v0.2d, v0.2d, v0.2d              ; 00 8c e0 6e  
   836 : cmeq   v31.2d, v0.2d, v0.2d             ; 1f 8c e0 6e  
   837 : cmeq   v0.2d, v31.2d, v0.2d             ; e0 8f e0 6e  
   838 : cmeq   v0.2d, v0.2d, v31.2d             ; 00 8c ff 6e  
   839 : cmge   v0.16b, v0.16b, v0.16b           ; 00 3c 20 4e  
   840 : cmge   v31.16b, v0.16b, v0.16b          ; 1f 3c 20 4e  
   841 : cmge   v0.16b, v31.16b, v0.16b          ; e0 3f 20 4e  
   842 : cmge   v0.16b, v0.16b, v31.16b          ; 00 3c 3f 4e  
   843 : cmge   v0.8h, v0.8h, v0.8h              ; 00 3c 60 4e  
   844 : cmge   v31.8h, v0.8h, v0.8h             ; 1f 3c 60 4e  
   845 : cmge   v0.8h, v31.8h, v0.8h             ; e0 3f 60 4e  
   846 : cmge   v0.8h, v0.8h, v31.8h             ; 00 3c 7f 4e  
   847 : cmge   v0.4s, v0.4s, v0.4s              ; 00 3c a0 4e  
   848 : cmge   v31.4s, v0.4s, v0.4s             ; 1f 3c a0 4e  
   849 : cmge   v0.4s, v31.4s, v0.4s             ; e0 3f a0 4e  
   850 : cmge   v0.4s, v0.4s, v31.4s             ; 00 3c bf 4e  
   851 : cmge   v0.2d, v0.2d, v0.2d              ; 00 3c e0 4e  
   852 : cmge   v31.2d, v0.2d, v0.2d             ; 1f 3c e0 4e  
   853 : cmge   v0.2d, v31.2d, v0.2d             ; e0 3f e0 4e  
   854 : cmge   v0.2d, v0.2d, v31.2d             ; 00 3c ff 4e  
   855 : cmgt   v0.16b, v0.16b, v0.16b           ; 00 34 20 4e  
   856 : cmgt   v31.16b, v0.16b, v0.16b          ; 1f 34 20 4e  
   857 : cmgt   v0.16b, v31.16b, v0.16b          ; e0 37 20 4e  
   858 : cmgt   v0.16b, v0.16b, v31.16b          ; 00 34 3f 4e  
   859 : cmgt   v0.8h, v0.8h, v0.8h              ; 00 34 60 4e  
   860 : cmgt   v31.8h, v0.8h, v0.8h             ; 1f 34 60 4e  
   861 : cmgt   v0.8h, v31.8h, v0.8h             ; e0 37 60 4e  
   862 : cmgt   v0.8h, v0.8h, v31.8h             ; 00 34 7f 4e  
   863 : cmgt   v0.4s, v0.4s, v0.4s              ; 00 34 a0 4e  
   864 : cmgt   v31.4s, v0.4s, v0.4s             ; 1f 34 a0 4e  
   865 : cmgt   v0.4s, v31.4s, v0.4s             ; e0 37 a0 4e  
   866 : cmgt   v0.4s, v0.4s, v31.4s             ; 00 34 bf 4e  
   867 : cmgt   v0.2d, v0.2d, v0.2d              ; 00 34 e0 4e  
   868 : cmgt   v31.2d, v0.2d, v0.2d             ; 1f 34 e0 4e  
   869 : cmgt   v0.2d, v31.2d, v0.2d             ; e0 37 e0 4e  
   870 : cmgt   v0.2d, v0.2d, v31.2d             ; 00 34 ff 4e  
   871 : umov   x0, v0.16b, #0                   ; 00 3c 01 0e  
   872 : umov   x15, v0.16b, #0                  ; 0f 3c 01 0e  
   873 : umov   x0, v31.16b, #0                  ; e0 3f 01 0e  
   874 : umov   x0, v0.16b, #0x0f                ; 00 3c 1f 0e  
   875 : umov   x0, v0.8h, #0                    ; 00 3c 02 0e  
   876 : umov   x15, v0.8h, #0                   ; 0f 3c 02 0e  
   877 : umov   x0, v31.8h, #0                   ; e0 3f 02 0e  
   878 : umov   x0, v0.8h, #0x07                 ; 00 3c 1e 0e  
   879 : umov   x0, v0.4s, #0                    ; 00 3c 04 0e  
   880 : umov   x15, v0.4s, #0                   ; 0f 3c 04 0e  
   881 : umov   x0, v31.4s, #0                   ; e0 3f 04 0e  
   882 : umov   x0, v0.4s, #0x03                 ; 00 3c 1c 0e  
   883 : umov   x0, v0.2d, #0                    ; 00 3c 08 4e  
   884 : umov   x15, v0.2d, #0                   ; 0f 3c 08 4e  
   885 : umov   x0, v31.2d, #0                   ; e0 3f 08 4e  
   886 : umov   x0, v0.2d, #0x01                 ; 00 3c 18 4e  
   887 : ins    v0.16b, #0, x0                   ; 00 1c 01 4e  
   888 : ins    v31.16b, #0, x0                  ; 1f 1c 01 4e  
   889 : ins    v0.16b, #0x0f, x0                ; 00 1c 1f 4e  
   890 : ins    v0.16b, #0, x15                  ; e0 1d 01 4e  
   891 : ins    v0.8h, #0, x0                    ; 00 1c 02 4e  
   892 : ins    v31.8h, #0, x0                   ; 1f 1c 02 4e  
   893 : ins    v0.8h, #0x07, x0                 ; 00 1c 1e 4e  
   894 : ins    v0.8h, #0, x15                   ; e0 1d 02 4e  
   895 : ins    v0.4s, #0, x0                    ; 00 1c 04 4e  
   896 : ins    v31.4s, #0, x0                   ; 1f 1c 04 4e  
   897 : ins    v0.4s, #0x03, x0                 ; 00 1c 1c 4e  
   898 : ins    v0.4s, #0, x15                   ; e0 1d 04 4e  
   899 : ins    v0.2d, #0, x0                    ; 00 1c 08 4e  
   900 : ins    v31.2d, #0, x0                   ; 1f 1c 08 4e  
   901 : ins    v0.2d, #0x01, x0                 ; 00 1c 18 4e  
   902 : ins    v0.2d, #0, x15                   ; e0 1d 08 4e  
   903 : ins    v0.16b, #0, v0.16b, #0           ; 00 04 01 6e  
   904 : ins    v31.16b, #0, v0.16b, #0          ; 1f 04 01 6e  
   905 : ins    v0.16b, #0x0f, v0.16b, #0        ; 00 04 1f 6e  
   906 : ins    v0.16b, #0, v31.16b, #0          ; e0 07 01 6e  
   907 : ins    v0.16b, #0, v0.16b, #0x0f        ; 00 7c 01 6e  
   908 : ins    v0.8h, #0, v0.8h, #0             ; 00 04 02 6e  
   909 : ins    v31.8h, #0, v0.8h, #0            ; 1f 04 02 6e  
   910 : ins    v0.8h, #0x07, v0.8h, #0          ; 00 04 1e 6e  
   911 : ins    v0.8h, #0, v31.8h, #0            ; e0 07 02 6e  
   912 : ins    v0.8h, #0, v0.8h, #0x07          ; 00 74 02 6e  
   913 : ins    v0.4s, #0, v0.4s, #0             ; 00 04 04 6e  
   914 : ins    v31.4s, #0, v0.4s, #0            ; 1f 04 04 6e  
   915 : ins    v0.4s, #0x03, v0.4s, #0          ; 00 04 1c 6e  
   916 : ins    v0.4s, #0, v31.4s, #0            ; e0 07 04 6e  
   917 : ins    v0.4s, #0, v0.4s, #0x03          ; 00 64 04 6e  
   918 : ins    v0.2d, #0, v0.2d, #0             ; 00 04 08 6e  
   919 : ins    v31.2d, #0, v0.2d, #0            ; 1f 04 08 6e  
   920 : ins    v0.2d, #0x01, v0.2d, #0          ; 00 04 18 6e  
   921 : ins    v0.2d, #0, v31.2d, #0            ; e0 07 08 6e  
   922 : ins    v0.2d, #0, v0.2d, #0x01          ; 00 44 08 6e  
   923 : bsl    v0.16b, v0.16b, v0.16b           ; 00 1c 60 6e  
   924 : bsl    v31.16b, v0.16b, v0.16b          ; 1f 1c 60 6e  
   925 : bsl    v0.16b, v31.16b, v0.16b          ; e0 1f 60 6e  
   926 : bsl    v0.16b, v0.16b, v31.16b          ; 00 1c 7f 6e  
   927 : blr    [x0]                             ; 00 00 3f d6  
   928 : blr    [x15]                            ; e0 01 3f d6  
