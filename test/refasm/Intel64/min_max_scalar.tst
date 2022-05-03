min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x38       ; 48 83 ec 38           
     1 : mov    [rsp+#0x10], rbx ; 48 89 5c 24 10        
     2 : mov    [rsp+#0x18], rbp ; 48 89 6c 24 18        
     3 : mov    [rsp+#0x20], rsi ; 48 89 74 24 20        
     4 : mov    [rsp+#0x28], rdi ; 48 89 7c 24 28        
     5 : mov    rax, #0          ; 48 c7 c0 00 00 00 00  
     6 : mov    r10, #0          ; 49 c7 c2 00 00 00 00  
     7 : mov    r11, #0          ; 49 c7 c3 00 00 00 00  
     8 : movsxd rbx, rcx         ; 48 63 19              
     9 : mov    rsi, rbx         ; 48 89 de              
    10 : mov    rdi, #0x04       ; 48 c7 c7 04 00 00 00  
    11 : imul   rdx, rdi         ; 48 0f af d7           
    12 : movsxd rbp, rcx, rax    ; 48 63 2c 01           
    13 : cmp    rbp, rbx         ; 48 39 dd              
    14 : jge    [17]             ; 0f 8d 06 00 00 00     
    15 : mov    rbx, rbp         ; 48 89 eb              
    16 : mov    r10, rax         ; 49 89 c2              
    17 : cmp    rbp, rsi         ; 48 39 f5              
    18 : jle    [21]             ; 0f 8e 06 00 00 00     
    19 : mov    rsi, rbp         ; 48 89 ee              
    20 : mov    r11, rax         ; 49 89 c3              
    21 : add    rax, rdi         ; 48 01 f8              
    22 : cmp    rax, rdx         ; 48 39 d0              
    23 : jl     [12]             ; 0f 8c d2 ff ff ff     
    24 : mov    [rsp], rax       ; 48 89 44 24 00        
    25 : mov    [rsp+#0x08], rdx ; 48 89 54 24 08        
    26 : mov    rax, r10         ; 4c 89 d0              
    27 : cqo                     ; 48 99                 
    28 : idiv   rdi              ; 48 f7 ff              
    29 : mov    r10, rax         ; 49 89 c2              
    30 : mov    rax, [rsp]       ; 48 8b 44 24 00        
    31 : mov    rdx, [rsp+#0x08] ; 48 8b 54 24 08        
    32 : mov    [rsp], rax       ; 48 89 44 24 00        
    33 : mov    [rsp+#0x08], rdx ; 48 89 54 24 08        
    34 : mov    rax, r11         ; 4c 89 d8              
    35 : cqo                     ; 48 99                 
    36 : idiv   rdi              ; 48 f7 ff              
    37 : mov    r11, rax         ; 49 89 c3              
    38 : mov    rax, [rsp]       ; 48 8b 44 24 00        
    39 : mov    rdx, [rsp+#0x08] ; 48 8b 54 24 08        
    40 : mov    [r8], r10d       ; 45 89 10              
    41 : mov    [r9], r11d       ; 45 89 19              
    42 : mov    r9, #0           ; 49 c7 c1 00 00 00 00  
    43 : mov    rax, r9          ; 4c 89 c8              
    44 : mov    rbx, [rsp+#0x10] ; 48 8b 5c 24 10        
    45 : mov    rbp, [rsp+#0x18] ; 48 8b 6c 24 18        
    46 : mov    rsi, [rsp+#0x20] ; 48 8b 74 24 20        
    47 : mov    rdi, [rsp+#0x28] ; 48 8b 7c 24 28        
    48 : add    rsp, #0x38       ; 48 83 c4 38           
    49 : ret                     ; c3                    
