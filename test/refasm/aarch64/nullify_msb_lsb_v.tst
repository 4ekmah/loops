nullify_msb_lsb_v(i0, i1, i2, i3)
     0 : sub  sp, sp, #0x10              ; ff 43 00 d1  
     1 : str  x18, [sp], #0              ; f2 03 00 f9  
     2 : str  x19, [sp], #0x01           ; f3 07 00 f9  
     3 : mov  x18, #0                    ; 12 00 80 d2  
     4 : mov  x19, #0x04                 ; 93 00 80 d2  
     5 : mul  x3, x3, x19                ; 63 7c 13 9b  
     6 : movi v0.4s, #0x01, #0           ; 20 04 00 4f  
     7 : cmp  x18, x3                    ; 5f 02 03 eb  
     8 : b.ge [31]                       ; ea 02 00 54  
     9 : ldr  v1.4s, x0, x18             ; 01 68 f2 3c  
    10 : ushr v2.4s, v1.4s, #0x07, #0x07 ; 22 04 3f 6f  
    11 : orr  v2.4s, v1.4s, v2.4s        ; 22 1c a2 4e  
    12 : ushr v3.4s, v2.4s, #0x07, #0x06 ; 43 04 3e 6f  
    13 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    14 : ushr v3.4s, v2.4s, #0x07, #0x04 ; 43 04 3c 6f  
    15 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    16 : ushr v3.4s, v2.4s, #0x07, #0    ; 43 04 38 6f  
    17 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    18 : ushr v3.4s, v2.4s, #0x06, #0    ; 43 04 30 6f  
    19 : orr  v2.4s, v2.4s, v3.4s        ; 42 1c a3 4e  
    20 : add  v2.4s, v2.4s, v0.4s        ; 42 84 a0 4e  
    21 : ushr v2.4s, v2.4s, #0x07, #0x07 ; 42 04 3f 6f  
    22 : eor  v2.4s, v2.4s, v1.4s        ; 42 1c 21 6e  
    23 : str  v2.4s, x1, x18             ; 22 68 b2 3c  
    24 : sub  v2.4s, v1.4s, v0.4s        ; 22 84 a0 6e  
    25 : mvn  v2.4s, v2.4s               ; 42 58 20 6e  
    26 : and  v2.4s, v1.4s, v2.4s        ; 22 1c 22 4e  
    27 : eor  v1.4s, v2.4s, v1.4s        ; 41 1c 21 6e  
    28 : str  v1.4s, x2, x18             ; 41 68 b2 3c  
    29 : add  x18, x18, #0x10            ; 52 42 00 91  
    30 : b    [7]                        ; e9 ff ff 17  
    31 : ldr  x18, [sp], #0              ; f2 03 40 f9  
    32 : ldr  x19, [sp], #0x01           ; f3 07 40 f9  
    33 : add  sp, sp, #0x10              ; ff 43 00 91  
    34 : ret  x30                        ; c0 03 5f d6  
