exponentiation_by_squaring_9(i0)
     0 : mov x0, x0     ; e0 03 00 aa  
     1 : mov x1, #0x01  ; 21 00 80 d2  
     2 : mul x1, x1, x0 ; 21 7c 00 9b  
     3 : mul x0, x0, x0 ; 00 7c 00 9b  
     4 : mul x0, x0, x0 ; 00 7c 00 9b  
     5 : mul x0, x0, x0 ; 00 7c 00 9b  
     6 : mul x0, x1, x0 ; 20 7c 00 9b  
     7 : mov x0, x0     ; e0 03 00 aa  
     8 : ret x30        ; c0 03 5f d6  
