nullify_msb_lsb(i0, i1, i2)
     0 : sub sp, sp, #0x10  ; ff 43 00 d1  
     1 : str x25, [sp], #0  ; f9 03 00 f9  
     2 : lsr x3, x0, #0x01  ; 03 fc 41 d3  
     3 : orr x3, x0, x3     ; 03 00 03 aa  
     4 : lsr x25, x3, #0x02 ; 79 fc 42 d3  
     5 : orr x3, x3, x25    ; 63 00 19 aa  
     6 : lsr x25, x3, #0x04 ; 79 fc 44 d3  
     7 : orr x3, x3, x25    ; 63 00 19 aa  
     8 : lsr x25, x3, #0x08 ; 79 fc 48 d3  
     9 : orr x3, x3, x25    ; 63 00 19 aa  
    10 : lsr x25, x3, #0x10 ; 79 fc 50 d3  
    11 : orr x3, x3, x25    ; 63 00 19 aa  
    12 : lsr x25, x3, #0x20 ; 79 fc 60 d3  
    13 : orr x3, x3, x25    ; 63 00 19 aa  
    14 : add x3, x3, #0x01  ; 63 04 00 91  
    15 : lsr x3, x3, #0x01  ; 63 fc 41 d3  
    16 : eor x3, x3, x0     ; 63 00 00 ca  
    17 : str x3, [x2], #0   ; 43 00 00 f9  
    18 : sub x2, x0, #0x01  ; 02 04 00 d1  
    19 : mvn x2, x2         ; e2 03 22 aa  
    20 : and x2, x0, x2     ; 02 00 02 8a  
    21 : eor x0, x2, x0     ; 40 00 00 ca  
    22 : str x0, [x1], #0   ; 20 00 00 f9  
    23 : ldr x25, [sp], #0  ; f9 03 40 f9  
    24 : add sp, sp, #0x10  ; ff 43 00 91  
    25 : ret x30            ; c0 03 5f d6  
