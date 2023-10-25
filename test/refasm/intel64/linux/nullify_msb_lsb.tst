nullify_msb_lsb(i0, i1, i2)
     0 : mov rcx, rdi   ; 48 89 f9              
     1 : shr rcx, #0x01 ; 48 c1 e9 01           
     2 : or  rcx, rdi   ; 48 09 f9              
     3 : mov rax, rcx   ; 48 89 c8              
     4 : shr rax, #0x02 ; 48 c1 e8 02           
     5 : or  rcx, rax   ; 48 09 c1              
     6 : mov rax, rcx   ; 48 89 c8              
     7 : shr rax, #0x04 ; 48 c1 e8 04           
     8 : or  rcx, rax   ; 48 09 c1              
     9 : mov rax, rcx   ; 48 89 c8              
    10 : shr rax, #0x08 ; 48 c1 e8 08           
    11 : or  rcx, rax   ; 48 09 c1              
    12 : mov rax, rcx   ; 48 89 c8              
    13 : shr rax, #0x10 ; 48 c1 e8 10           
    14 : or  rcx, rax   ; 48 09 c1              
    15 : mov rax, rcx   ; 48 89 c8              
    16 : shr rax, #0x20 ; 48 c1 e8 20           
    17 : or  rcx, rax   ; 48 09 c1              
    18 : add rcx, #0x01 ; 48 81 c1 01 00 00 00  
    19 : shr rcx, #0x01 ; 48 c1 e9 01           
    20 : xor rcx, rdi   ; 48 31 f9              
    21 : mov [rdx], rcx ; 48 89 0a              
    22 : mov rdx, rdi   ; 48 89 fa              
    23 : sub rdx, #0x01 ; 48 81 ea 01 00 00 00  
    24 : not rdx        ; 48 f7 d2              
    25 : and rdx, rdi   ; 48 21 fa              
    26 : xor rdx, rdi   ; 48 31 fa              
    27 : mov [rsi], rdx ; 48 89 16              
    28 : ret            ; c3                    
