exponentiation_by_squaring_0(i0, i1, i2)
     0 : sub    rsp, #0x08    ; 48 81 ec 08 00 00 00     
     1 : mov    [rsp], r12    ; 4c 89 a4 24 00 00 00 00  
     2 : xor    rcx, rcx      ; 48 31 c9                 
     3 : xor    rax, rax      ; 48 31 c0                 
     4 : cmp    rax, rsi      ; 48 39 f0                 
     5 : jge    [13]          ; 0f 8d 27 00 00 00        
     6 : movsxd r12, rdi, rcx ; 4c 63 24 0f              
     7 : mov    r12, #0x01    ; 49 c7 c4 01 00 00 00     
     8 : mov    [rdx], r12d   ; 44 89 22                 
     9 : add    rax, #0x01    ; 48 05 01 00 00 00        
    10 : add    rcx, #0x04    ; 48 81 c1 04 00 00 00     
    11 : add    rdx, #0x04    ; 48 81 c2 04 00 00 00     
    12 : jmp    [4]           ; e9 d0 ff ff ff           
    13 : xor    rax, rax      ; 48 31 c0                 
    14 : mov    r12, [rsp]    ; 4c 8b a4 24 00 00 00 00  
    15 : add    rsp, #0x08    ; 48 81 c4 08 00 00 00     
    16 : ret                  ; c3                       
