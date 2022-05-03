arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : mov    rax, [rsp+#0x28] ; 48 8b 44 24 28              
     1 : mov    r10, [rsp+#0x30] ; 4c 8b 54 24 30              
     2 : sub    rsp, #0x38       ; 48 83 ec 38                 
     3 : mov    [rsp+#0x08], rbx ; 48 89 5c 24 08              
     4 : mov    [rsp+#0x10], rbp ; 48 89 6c 24 10              
     5 : mov    [rsp+#0x18], rsi ; 48 89 74 24 18              
     6 : mov    [rsp+#0x20], rdi ; 48 89 7c 24 20              
     7 : mov    [rsp+#0x28], r12 ; 4c 89 64 24 28              
     8 : mov    r11, #0          ; 49 c7 c3 00 00 00 00        
     9 : mov    rbx, #0          ; 48 c7 c3 00 00 00 00        
    10 : mov    rsi, #0x04       ; 48 c7 c6 04 00 00 00        
    11 : mov    [rsp], #0x01     ; 48 c7 44 24 00 01 00 00 00  
    12 : movsxd rbp, rcx, r11    ; 4a 63 2c 19                 
    13 : movsxd r12, rdx, r11    ; 4e 63 24 1a                 
    14 : mov    rdi, rbp         ; 48 89 ef                    
    15 : add    rdi, r12         ; 4c 01 e7                    
    16 : mov    [r9], edi        ; 41 89 39                    
    17 : mov    rdi, rbp         ; 48 89 ef                    
    18 : sub    rdi, r12         ; 4c 29 e7                    
    19 : mov    [rax], edi       ; 89 38                       
    20 : imul   rbp, r12         ; 49 0f af ec                 
    21 : mov    [r10], ebp       ; 41 89 2a                    
    22 : add    rbx, [rsp]       ; 48 03 5c 24 00              
    23 : add    r11, rsi         ; 49 01 f3                    
    24 : add    r9, rsi          ; 49 01 f1                    
    25 : add    rax, rsi         ; 48 01 f0                    
    26 : add    r10, rsi         ; 49 01 f2                    
    27 : cmp    rbx, r8          ; 4c 39 c3                    
    28 : jl     [12]             ; 0f 8c c6 ff ff ff           
    29 : mov    r9, #0           ; 49 c7 c1 00 00 00 00        
    30 : mov    rax, r9          ; 4c 89 c8                    
    31 : mov    rbx, [rsp+#0x08] ; 48 8b 5c 24 08              
    32 : mov    rbp, [rsp+#0x10] ; 48 8b 6c 24 10              
    33 : mov    rsi, [rsp+#0x18] ; 48 8b 74 24 18              
    34 : mov    rdi, [rsp+#0x20] ; 48 8b 7c 24 20              
    35 : mov    r12, [rsp+#0x28] ; 4c 8b 64 24 28              
    36 : add    rsp, #0x38       ; 48 83 c4 38                 
    37 : ret                     ; c3                          
