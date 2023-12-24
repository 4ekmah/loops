helloworld_call()
     0 : sub  sp, sp, #0x1a0      ; ff 83 06 d1  
     1 : stp  x29, x30, sp, #0x32 ; fd 7b 19 a9  
     2 : mov  x29, sp             ; fd 03 1f aa  
     3 : mov  x0, #0xa510         ; 00 a2 94 d2  
     4 : movk x0, #0xbbf5, #0x01  ; a0 7e b7 f2  
     5 : movk x0, #0xaaaa, #0x02  ; 40 55 d5 f2  
     6 : stp  x0, x1, sp, #0      ; e0 07 00 a9  
     7 : stp  x2, x3, sp, #0x02   ; e2 0f 01 a9  
     8 : stp  x4, x5, sp, #0x04   ; e4 17 02 a9  
     9 : stp  x6, x7, sp, #0x06   ; e6 1f 03 a9  
    10 : stp  x8, x9, sp, #0x08   ; e8 27 04 a9  
    11 : stp  x10, x11, sp, #0x0a ; ea 2f 05 a9  
    12 : stp  x12, x13, sp, #0x0c ; ec 37 06 a9  
    13 : stp  x14, x15, sp, #0x0e ; ee 3f 07 a9  
    14 : stp  x16, x17, sp, #0x10 ; f0 47 08 a9  
    15 : add  x10, sp, #0x90      ; ea 43 02 91  
    16 : st1  v0.2d, x10          ; 40 2d 9f 4c  
    17 : st1  v4.2d, x10          ; 44 2d 9f 4c  
    18 : st1  v8.2d, x10          ; 48 2d 9f 4c  
    19 : st1  v12.2d, x10         ; 4c 2d 9f 4c  
    20 : blr  [x0]                ; 00 00 3f d6  
    21 : add  x10, sp, #0x90      ; ea 43 02 91  
    22 : ld1  v0.2d, x10          ; 40 2d df 4c  
    23 : ld1  v4.2d, x10          ; 44 2d df 4c  
    24 : ld1  v8.2d, x10          ; 48 2d df 4c  
    25 : ld1  v12.2d, x10         ; 4c 2d df 4c  
    26 : ldp  x0, x1, sp, #0      ; e0 07 40 a9  
    27 : ldp  x2, x3, sp, #0x02   ; e2 0f 41 a9  
    28 : ldp  x4, x5, sp, #0x04   ; e4 17 42 a9  
    29 : ldp  x6, x7, sp, #0x06   ; e6 1f 43 a9  
    30 : ldp  x8, x9, sp, #0x08   ; e8 27 44 a9  
    31 : ldp  x10, x11, sp, #0x0a ; ea 2f 45 a9  
    32 : ldp  x12, x13, sp, #0x0c ; ec 37 46 a9  
    33 : ldp  x14, x15, sp, #0x0e ; ee 3f 47 a9  
    34 : ldp  x16, x17, sp, #0x10 ; f0 47 48 a9  
    35 : ldp  x29, x30, sp, #0x32 ; fd 7b 59 a9  
    36 : add  sp, sp, #0x1a0      ; ff 83 06 91  
    37 : ret  x30                 ; c0 03 5f d6  
