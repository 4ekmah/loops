exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub    rsp, #0x08    ; 48 83 ec 08              
     1 : mov    [rsp], r12    ; 4c 89 64 24 00           
     2 : mov    rcx, #0x01    ; 48 c7 c1 01 00 00 00     
     3 : mov    rcx, #0       ; 48 c7 c1 00 00 00 00     
     4 : mov    rax, #0       ; 48 c7 c0 00 00 00 00     
     5 : cmp    rax, rsi      ; 48 39 f0                 
     6 : jge    [15]          ; 0f 8d 1e 00 00 00        
     7 : movsxd r12, rdi, rcx ; 4c 63 24 0f              
     8 : mov    r12, r12      ; 4d 89 e4                 
     9 : mov    r12, r12      ; 4d 89 e4                 
    10 : mov    [rdx], r12d   ; 44 89 22                 
    11 : add    rax, #0x01    ; 48 83 c0 01              
    12 : add    rcx, #0x04    ; 48 83 c1 04              
    13 : add    rdx, #0x04    ; 48 83 c2 04              
    14 : jmp    [5]           ; e9 d9 ff ff ff           
    15 : mov    rax, #0       ; 48 c7 c0 00 00 00 00     
    16 : mov    r12, [rsp]    ; 4c 8b a4 24 00 00 00 00  
    17 : add    rsp, #0x08    ; 48 83 c4 08              
    18 : ret                  ; c3                       
