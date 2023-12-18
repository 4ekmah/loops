exponentiation_by_squaring_9(i0, i1, i2)
     0 : sub    rsp, #0x18       ; 48 81 ec 18 00 00 00                 
     1 : mov    [rsp+#0x08], r12 ; 4c 89 a4 24 08 00 00 00              
     2 : xor    r9, r9           ; 4d 31 c9                             
     3 : mov    [rsp], #0        ; 48 c7 84 24 00 00 00 00 00 00 00 00  
     4 : cmp    [rsp], rdx       ; 48 39 94 24 00 00 00 00              
     5 : jge    [17]             ; 0f 8d 39 00 00 00                    
     6 : movsxd r12, rcx, r9     ; 4e 63 24 09                          
     7 : mov    rax, r12         ; 4c 89 e0                             
     8 : imul   rax, r12         ; 49 0f af c4                          
     9 : imul   rax, rax         ; 48 0f af c0                          
    10 : imul   rax, rax         ; 48 0f af c0                          
    11 : imul   r12, rax         ; 4c 0f af e0                          
    12 : mov    [r8], r12d       ; 45 89 20                             
    13 : add    [rsp], #0x01     ; 48 81 84 24 00 00 00 00 01 00 00 00  
    14 : add    r9, #0x04        ; 49 81 c1 04 00 00 00                 
    15 : add    r8, #0x04        ; 49 81 c0 04 00 00 00                 
    16 : jmp    [4]              ; e9 b9 ff ff ff                       
    17 : xor    rax, rax         ; 48 31 c0                             
    18 : mov    r12, [rsp+#0x08] ; 4c 8b a4 24 08 00 00 00              
    19 : add    rsp, #0x18       ; 48 81 c4 18 00 00 00                 
    20 : ret                     ; c3                                   
