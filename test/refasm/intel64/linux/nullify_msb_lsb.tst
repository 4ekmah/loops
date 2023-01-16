nullify_msb_lsb(i0, i1, i2)
     0 : sub rsp, #0x28 ; 48 83 ec 28  
     1 : mov rcx, rdi   ; 48 89 f9     
     2 : shr rcx, #0x01 ; 48 c1 e9 01  
     3 : or  rcx, rdi   ; 48 09 f9     
     4 : mov rax, rcx   ; 48 89 c8     
     5 : shr rax, #0x02 ; 48 c1 e8 02  
     6 : or  rcx, rax   ; 48 09 c1     
     7 : mov rax, rcx   ; 48 89 c8     
     8 : shr rax, #0x04 ; 48 c1 e8 04  
     9 : or  rcx, rax   ; 48 09 c1     
    10 : mov rax, rcx   ; 48 89 c8     
    11 : shr rax, #0x08 ; 48 c1 e8 08  
    12 : or  rcx, rax   ; 48 09 c1     
    13 : mov rax, rcx   ; 48 89 c8     
    14 : shr rax, #0x10 ; 48 c1 e8 10  
    15 : or  rcx, rax   ; 48 09 c1     
    16 : mov rax, rcx   ; 48 89 c8     
    17 : shr rax, #0x20 ; 48 c1 e8 20  
    18 : or  rcx, rax   ; 48 09 c1     
    19 : add rcx, #0x01 ; 48 83 c1 01  
    20 : shr rcx, #0x01 ; 48 c1 e9 01  
    21 : xor rcx, rdi   ; 48 31 f9     
    22 : mov [rdx], rcx ; 48 89 0a     
    23 : mov rdx, rdi   ; 48 89 fa     
    24 : sub rdx, #0x01 ; 48 83 ea 01  
    25 : not rdx        ; 48 f7 d2     
    26 : and rdx, rdi   ; 48 21 fa     
    27 : xor rdx, rdi   ; 48 31 fa     
    28 : mov [rsi], rdx ; 48 89 16     
    29 : add rsp, #0x28 ; 48 83 c4 28  
    30 : ret            ; c3           
