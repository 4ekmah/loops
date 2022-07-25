exponentiation_by_squaring_1(i0)
     0 : mov  rcx, rcx   ; 48 89 c9              
     1 : mov  rdx, #0x01 ; 48 c7 c2 01 00 00 00  
     2 : imul rdx, rcx   ; 48 0f af d1           
     3 : mov  rax, rdx   ; 48 89 d0              
     4 : ret             ; c3                    
