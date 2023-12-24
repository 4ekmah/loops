fixed_power_v_double_0(i0, i1, i2)
     0 : sub  sp, sp, #0x10       ; ff 43 00 d1  
     1 : str  x25, [sp], #0       ; f9 03 00 f9  
     2 : eor  x3, x3, x3          ; 63 00 03 ca  
     3 : mov  x25, #0x08          ; 19 01 80 d2  
     4 : mul  x2, x2, x25         ; 42 7c 19 9b  
     5 : cmp  x3, x2              ; 7f 00 02 eb  
     6 : b.ge [14]                ; 0a 01 00 54  
     7 : ldr  v0.2d, x0, x3       ; 00 68 e3 3c  
     8 : eor  x25, x25, x25       ; 39 03 19 ca  
     9 : movk x25, #0x3ff0, #0x03 ; 19 fe e7 f2  
    10 : dup  v0.2d, x25          ; 20 0f 08 4e  
    11 : str  v0.2d, x1, x3       ; 20 68 a3 3c  
    12 : add  x3, x3, #0x10       ; 63 40 00 91  
    13 : b    [5]                 ; f8 ff ff 17  
    14 : eor  x0, x0, x0          ; 00 00 00 ca  
    15 : ldr  x25, [sp], #0       ; f9 03 40 f9  
    16 : add  sp, sp, #0x10       ; ff 43 00 91  
    17 : ret  x30                 ; c0 03 5f d6  
