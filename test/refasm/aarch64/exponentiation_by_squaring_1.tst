exponentiation_by_squaring_1(i0)
     0 : mov x0, x0     ; e0 03 00 aa  
     1 : mov x1, #0x01  ; 21 00 80 d2  
     2 : mul x0, x1, x0 ; 20 7c 00 9b  
     3 : mov x0, x0     ; e0 03 00 aa  
     4 : ret x30        ; c0 03 5f d6  
