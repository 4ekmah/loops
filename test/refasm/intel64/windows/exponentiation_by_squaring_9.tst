exponentiation_by_squaring_9(i0, i1, i2)
     0 : sub    rsp, #0x18       ; 48 83 ec 18                          
     1 : mov    [rsp+#0x08], r12 ; 4c 89 64 24 08                       
     2 : mov    r9, #0x01        ; 49 c7 c1 01 00 00 00                 
     3 : mov    r9, #0           ; 49 c7 c1 00 00 00 00                 
     4 : mov    [rsp], #0        ; 48 c7 44 24 00 00 00 00 00           
     5 : cmp    [rsp], rdx       ; 48 39 94 24 00 00 00 00              
     6 : jge    [18]             ; 0f 8d 33 00 00 00                    
     7 : movsxd r12, rcx, r9     ; 4e 63 24 09                          
     8 : mov    rax, r12         ; 4c 89 e0                             
     9 : imul   r12, r12         ; 4d 0f af e4                          
    10 : imul   r12, r12         ; 4d 0f af e4                          
    11 : imul   r12, r12         ; 4d 0f af e4                          
    12 : imul   rax, r12         ; 49 0f af c4                          
    13 : mov    [r8], eax        ; 41 89 00                             
    14 : add    [rsp], #0x01     ; 48 81 84 24 00 00 00 00 01 00 00 00  
    15 : add    r9, #0x04        ; 49 83 c1 04                          
    16 : add    r8, #0x04        ; 49 83 c0 04                          
    17 : jmp    [5]              ; e9 bf ff ff ff                       
    18 : mov    rax, #0          ; 48 c7 c0 00 00 00 00                 
    19 : mov    r12, [rsp+#0x08] ; 4c 8b a4 24 08 00 00 00              
    20 : add    rsp, #0x18       ; 48 83 c4 18                          
    21 : ret                     ; c3                                   
