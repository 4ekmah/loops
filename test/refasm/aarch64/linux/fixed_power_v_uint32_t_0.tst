fixed_power_v_uint32_t_0(i0, i1, i2)
     0 : sub  sp, sp, #0x10    ; ff 43 00 d1  
     1 : str  x25, [sp], #0    ; f9 03 00 f9  
     2 : eor  x3, x3, x3       ; 63 00 03 ca  
     3 : mov  x25, #0x04       ; 99 00 80 d2  
     4 : mul  x2, x2, x25      ; 42 7c 19 9b  
     5 : cmp  x3, x2           ; 7f 00 02 eb  
     6 : b.ge [12]             ; ca 00 00 54  
     7 : ldr  v0.4s, x0, x3    ; 00 68 e3 3c  
     8 : movi v0.4s, #0x01, #0 ; 20 04 00 4f  
     9 : str  v0.4s, x1, x3    ; 20 68 a3 3c  
    10 : add  x3, x3, #0x10    ; 63 40 00 91  
    11 : b    [5]              ; fa ff ff 17  
    12 : eor  x0, x0, x0       ; 00 00 00 ca  
    13 : ldr  x25, [sp], #0    ; f9 03 40 f9  
    14 : add  sp, sp, #0x10    ; ff 43 00 91  
    15 : ret  x30              ; c0 03 5f d6  
