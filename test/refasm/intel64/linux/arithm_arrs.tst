arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub    rsp, #0x38         ; 48 83 ec 38                 
     1 : mov    [rsp+#0x20], r9    ; 4c 89 4c 24 20              
     2 : mov    [rsp+#0x28], r12   ; 4c 89 64 24 28              
     3 : mov    [rsp+#0x30], r13   ; 4c 89 6c 24 30              
     4 : mov    [rsp+#0x18], #0    ; 48 c7 44 24 18 00 00 00 00  
     5 : mov    [rsp+#0x10], #0    ; 48 c7 44 24 10 00 00 00 00  
     6 : cmp    [rsp+#0x10], rdx   ; 48 39 54 24 10              
     7 : jge    [39]               ; 0f 8d 95 00 00 00           
     8 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
     9 : movsxd r12, rdi, r13      ; 4e 63 24 2f                 
    10 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    11 : movsxd rax, rsi, r13      ; 4a 63 04 2e                 
    12 : mov    r9, r12            ; 4d 89 e1                    
    13 : add    r9, rax            ; 49 01 c1                    
    14 : mov    [rcx], r9d         ; 44 89 09                    
    15 : mov    r9, r12            ; 4d 89 e1                    
    16 : sub    r9, rax            ; 49 29 c1                    
    17 : mov    [r8], r9d          ; 45 89 08                    
    18 : mov    r9, r12            ; 4d 89 e1                    
    19 : imul   r9, rax            ; 4c 0f af c8                 
    20 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    21 : mov    [r13], r9d         ; 45 89 4d 00                 
    22 : mov    [rsp], rax         ; 48 89 44 24 00              
    23 : mov    [rsp+#0x08], rdx   ; 48 89 54 24 08              
    24 : mov    rax, r12           ; 4c 89 e0                    
    25 : cqo                       ; 48 99                       
    26 : idiv   [rsp]              ; 48 f7 7c 24 00              
    27 : mov    r9, rax            ; 49 89 c1                    
    28 : mov    rax, [rsp]         ; 48 8b 84 24 00 00 00 00     
    29 : mov    rdx, [rsp+#0x08]   ; 48 8b 94 24 08 00 00 00     
    30 : mov    r13, [rsp+#0x40]   ; 4c 8b ac 24 40 00 00 00     
    31 : mov    [r13], r9d         ; 45 89 4d 00                 
    32 : add    [rsp+#0x10], #0x01 ; 48 83 44 24 10 01           
    33 : add    [rsp+#0x18], #0x04 ; 48 83 44 24 18 04           
    34 : add    rcx, #0x04         ; 48 83 c1 04                 
    35 : add    r8, #0x04          ; 49 83 c0 04                 
    36 : add    [rsp+#0x20], #0x04 ; 48 83 44 24 20 04           
    37 : add    [rsp+#0x40], #0x04 ; 48 83 44 24 40 04           
    38 : jmp    [6]                ; e9 60 ff ff ff              
    39 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    40 : mov    r12, [rsp+#0x28]   ; 4c 8b a4 24 28 00 00 00     
    41 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    42 : add    rsp, #0x38         ; 48 83 c4 38                 
    43 : ret                       ; c3                          
