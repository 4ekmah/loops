nonnegative_odd(i0, i1)
     0 : sub    rsp, #0x38         ; 48 83 ec 38                 
     1 : mov    [rsp+#0x28], r12   ; 4c 89 64 24 28              
     2 : mov    [rsp+#0x30], r13   ; 4c 89 6c 24 30              
     3 : mov    r8, #0             ; 49 c7 c0 00 00 00 00        
     4 : mov    r9, #0             ; 49 c7 c1 00 00 00 00        
     5 : mov    [rsp+#0x20], #0x02 ; 48 c7 44 24 20 02 00 00 00  
     6 : mov    [rsp+#0x18], #0x04 ; 48 c7 44 24 18 04 00 00 00  
     7 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
     8 : mov    [rsp+#0x10], r8    ; 4c 89 44 24 10              
     9 : sub    [rsp+#0x10], r13   ; 4c 29 6c 24 10              
    10 : imul   rdx, [rsp+#0x18]   ; 48 0f af 54 24 18           
    11 : cmp    r8, rdx            ; 49 39 d0                    
    12 : jge    [32]               ; 0f 8d 57 00 00 00           
    13 : movsxd r12, rcx, r8       ; 4e 63 24 01                 
    14 : cmp    r12, r9            ; 4d 39 cc                    
    15 : jge    [18]               ; 0f 8d 0a 00 00 00           
    16 : add    r8, [rsp+#0x18]    ; 4c 03 44 24 18              
    17 : jmp    [11]               ; e9 e0 ff ff ff              
    18 : mov    [rsp+#0x08], rdx   ; 48 89 54 24 08              
    19 : mov    rax, r12           ; 4c 89 e0                    
    20 : cqo                       ; 48 99                       
    21 : idiv   [rsp+#0x20]        ; 48 f7 7c 24 20              
    22 : mov    rdx, [rsp+#0x08]   ; 48 8b 94 24 08 00 00 00     
    23 : imul   rax, [rsp+#0x20]   ; 48 0f af 44 24 20           
    24 : sub    r12, rax           ; 49 29 c4                    
    25 : mov    rax, r12           ; 4c 89 e0                    
    26 : cmp    rax, r9            ; 4c 39 c8                    
    27 : je     [30]               ; 0f 84 0a 00 00 00           
    28 : mov    [rsp+#0x10], r8    ; 4c 89 44 24 10              
    29 : jmp    [32]               ; e9 0a 00 00 00              
    30 : add    r8, [rsp+#0x18]    ; 4c 03 44 24 18              
    31 : jmp    [11]               ; e9 a0 ff ff ff              
    32 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    33 : mov    [rsp], rax         ; 48 89 44 24 00              
    34 : mov    rax, r13           ; 4c 89 e8                    
    35 : cqo                       ; 48 99                       
    36 : idiv   [rsp+#0x18]        ; 48 f7 7c 24 18              
    37 : mov    rdx, rax           ; 48 89 c2                    
    38 : mov    rax, [rsp]         ; 48 8b 84 24 00 00 00 00     
    39 : mov    rax, rdx           ; 48 89 d0                    
    40 : mov    r12, [rsp+#0x28]   ; 4c 8b a4 24 28 00 00 00     
    41 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    42 : add    rsp, #0x38         ; 48 83 c4 38                 
    43 : ret                       ; c3                          
