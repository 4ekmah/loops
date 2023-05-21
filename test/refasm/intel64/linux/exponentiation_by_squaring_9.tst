exponentiation_by_squaring_9(i0, i1, i2)
     0 : sub    rsp, #0x18       ; 48 83 ec 18                          
     1 : mov    [rsp+#0x08], r12 ; 4c 89 64 24 08                       
     2 : mov    rcx, #0x01       ; 48 c7 c1 01 00 00 00                 
     3 : xor    rcx, rcx         ; 48 31 c9                             
     4 : mov    [rsp], #0        ; 48 c7 44 24 00 00 00 00 00           
     5 : cmp    [rsp], rsi       ; 48 39 b4 24 00 00 00 00              
     6 : jge    [18]             ; 0f 8d 33 00 00 00                    
     7 : movsxd r12, rdi, rcx    ; 4c 63 24 0f                          
     8 : mov    rax, r12         ; 4c 89 e0                             
     9 : imul   rax, r12         ; 49 0f af c4                          
    10 : imul   rax, rax         ; 48 0f af c0                          
    11 : imul   rax, rax         ; 48 0f af c0                          
    12 : imul   r12, rax         ; 4c 0f af e0                          
    13 : mov    [rdx], r12d      ; 44 89 22                             
    14 : add    [rsp], #0x01     ; 48 81 84 24 00 00 00 00 01 00 00 00  
    15 : add    rcx, #0x04       ; 48 83 c1 04                          
    16 : add    rdx, #0x04       ; 48 83 c2 04                          
    17 : jmp    [5]              ; e9 bf ff ff ff                       
    18 : xor    rax, rax         ; 48 31 c0                             
    19 : mov    r12, [rsp+#0x08] ; 4c 8b a4 24 08 00 00 00              
    20 : add    rsp, #0x18       ; 48 83 c4 18                          
    21 : ret                     ; c3                                   
