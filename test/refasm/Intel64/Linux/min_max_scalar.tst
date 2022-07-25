min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x48       ; 48 83 ec 48                 
     1 : mov    [rsp+#0x18], rdx ; 48 89 54 24 18              
     2 : mov    [rsp+#0x10], rcx ; 48 89 4c 24 10              
     3 : mov    [rsp+#0x30], r12 ; 4c 89 64 24 30              
     4 : mov    [rsp+#0x38], r13 ; 4c 89 6c 24 38              
     5 : mov    rax, #0          ; 48 c7 c0 00 00 00 00        
     6 : mov    [rsp+#0x28], #0  ; 48 c7 44 24 28 00 00 00 00  
     7 : mov    [rsp+#0x20], #0  ; 48 c7 44 24 20 00 00 00 00  
     8 : movsxd rdx, rdi         ; 48 63 17                    
     9 : mov    rcx, rdx         ; 48 89 d1                    
    10 : imul   rsi, #0x04       ; 48 6b f6 04                 
    11 : cmp    rax, rsi         ; 48 39 f0                    
    12 : jge    [24]             ; 0f 8d 2f 00 00 00           
    13 : movsxd r12, rdi, rax    ; 4c 63 24 07                 
    14 : cmp    r12, rdx         ; 49 39 d4                    
    15 : jge    [18]             ; 0f 8d 08 00 00 00           
    16 : mov    rdx, r12         ; 4c 89 e2                    
    17 : mov    [rsp+#0x28], rax ; 48 89 44 24 28              
    18 : cmp    r12, rcx         ; 49 39 cc                    
    19 : jle    [22]             ; 0f 8e 08 00 00 00           
    20 : mov    rcx, r12         ; 4c 89 e1                    
    21 : mov    [rsp+#0x20], rax ; 48 89 44 24 20              
    22 : add    rax, #0x04       ; 48 83 c0 04                 
    23 : jmp    [11]             ; e9 c8 ff ff ff              
    24 : mov    rdi, #0x04       ; 48 c7 c7 04 00 00 00        
    25 : mov    r13, [rsp+#0x28] ; 4c 8b ac 24 28 00 00 00     
    26 : mov    [rsp], rax       ; 48 89 44 24 00              
    27 : mov    [rsp+#0x08], rdx ; 48 89 54 24 08              
    28 : mov    rax, r13         ; 4c 89 e8                    
    29 : cqo                     ; 48 99                       
    30 : idiv   rdi              ; 48 f7 ff                    
    31 : mov    rsi, rax         ; 48 89 c6                    
    32 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00     
    33 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00     
    34 : mov    r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00     
    35 : mov    [rsp], rax       ; 48 89 44 24 00              
    36 : mov    [rsp+#0x08], rdx ; 48 89 54 24 08              
    37 : mov    rax, r13         ; 4c 89 e8                    
    38 : cqo                     ; 48 99                       
    39 : idiv   rdi              ; 48 f7 ff                    
    40 : mov    rdi, rax         ; 48 89 c7                    
    41 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00     
    42 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00     
    43 : mov    r13, [rsp+#0x18] ; 4c 8b ac 24 18 00 00 00     
    44 : mov    [r13], esi       ; 41 89 75 00                 
    45 : mov    r13, [rsp+#0x10] ; 4c 8b ac 24 10 00 00 00     
    46 : mov    [r13], edi       ; 41 89 7d 00                 
    47 : mov    rax, #0          ; 48 c7 c0 00 00 00 00        
    48 : mov    r12, [rsp+#0x30] ; 4c 8b a4 24 30 00 00 00     
    49 : mov    r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00     
    50 : add    rsp, #0x48       ; 48 83 c4 48                 
    51 : ret                     ; c3                          
