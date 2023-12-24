nullify_msb_lsb_v(i0, i1, i2, i3)
     0 : sub  sp, sp, #0x20              ; ff 83 00 d1  
     1 : str  x25, [sp], #0x01           ; f9 07 00 f9  
     2 : str  x26, [sp], #0x02           ; fa 0b 00 f9  
     3 : eor  x26, x26, x26              ; 5a 03 1a ca  
     4 : str  x26, [sp], #0              ; fa 03 00 f9  
     5 : mov  x25, #0x04                 ; 99 00 80 d2  
     6 : mul  x3, x3, x25                ; 63 7c 19 9b  
     7 : movi v0.4s, #0x01, #0           ; 20 04 00 4f  
     8 : ldr  x26, [sp], #0              ; fa 03 40 f9  
     9 : cmp  x26, x3                    ; 5f 03 03 eb  
    10 : b.ge [38]                       ; 8a 03 00 54  
    11 : ldr  x26, [sp], #0              ; fa 03 40 f9  
    12 : ldr  v1.4s, x0, x26             ; 01 68 fa 3c  
    13 : ushr v2.4s, v1.4s, #0x07, #0x07 ; 22 04 3f 6f  
    14 : orr  v2.4s, v1.4s, v2.4s        ; 22 1c a2 4e  
    15 : ushr v3.4s, v2.4s, #0x07, #0x06 ; 43 04 3e 6f  
    16 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    17 : ushr v3.4s, v2.4s, #0x07, #0x04 ; 43 04 3c 6f  
    18 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    19 : ushr v3.4s, v2.4s, #0x07, #0    ; 43 04 38 6f  
    20 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    21 : ushr v3.4s, v2.4s, #0x06, #0    ; 43 04 30 6f  
    22 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    23 : add  v2.4s, v2.4s, v0.4s        ; 42 84 a0 4e  
    24 : ushr v2.4s, v2.4s, #0x07, #0x07 ; 42 04 3f 6f  
    25 : eor  v2.4s, v2.4s, v1.4s        ; 42 1c 21 6e  
    26 : ldr  x26, [sp], #0              ; fa 03 40 f9  
    27 : str  v2.4s, x1, x26             ; 22 68 ba 3c  
    28 : sub  v2.4s, v1.4s, v0.4s        ; 22 84 a0 6e  
    29 : mvn  v2.4s, v2.4s               ; 42 58 20 6e  
    30 : and  v2.4s, v1.4s, v2.4s        ; 22 1c 22 4e  
    31 : eor  v1.4s, v2.4s, v1.4s        ; 41 1c 21 6e  
    32 : ldr  x26, [sp], #0              ; fa 03 40 f9  
    33 : str  v1.4s, x2, x26             ; 41 68 ba 3c  
    34 : ldr  x26, [sp], #0              ; fa 03 40 f9  
    35 : add  x26, x26, #0x10            ; 5a 43 00 91  
    36 : str  x26, [sp], #0              ; fa 03 00 f9  
    37 : b    [8]                        ; e3 ff ff 17  
    38 : ldr  x25, [sp], #0x01           ; f9 07 40 f9  
    39 : ldr  x26, [sp], #0x02           ; fa 0b 40 f9  
    40 : add  sp, sp, #0x20              ; ff 83 00 91  
    41 : ret  x30                        ; c0 03 5f d6  
