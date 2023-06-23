min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x58       ; 48 81 ec 58 00 00 00                 
     1 : mov    [rsp+#0x28], rdx ; 48 89 94 24 28 00 00 00              
     2 : mov    [rsp+#0x20], rcx ; 48 89 8c 24 20 00 00 00              
     3 : mov    [rsp+#0x40], r12 ; 4c 89 a4 24 40 00 00 00              
     4 : mov    [rsp+#0x48], r13 ; 4c 89 ac 24 48 00 00 00              
     5 : xor    rax, rax         ; 48 31 c0                             
     6 : mov    [rsp+#0x38], #0  ; 48 c7 84 24 38 00 00 00 00 00 00 00  
     7 : mov    [rsp+#0x30], #0  ; 48 c7 84 24 30 00 00 00 00 00 00 00  
     8 : movsxd rdx, rdi         ; 48 63 17                             
     9 : mov    rcx, rdx         ; 48 89 d1                             
    10 : imul   rsi, #0x04       ; 48 6b f6 04                          
    11 : cmp    rax, rsi         ; 48 39 f0                             
    12 : jge    [24]             ; 0f 8d 37 00 00 00                    
    13 : movsxd r12, rdi, rax    ; 4c 63 24 07                          
    14 : cmp    r12, rdx         ; 49 39 d4                             
    15 : jge    [18]             ; 0f 8d 0b 00 00 00                    
    16 : mov    rdx, r12         ; 4c 89 e2                             
    17 : mov    [rsp+#0x38], rax ; 48 89 84 24 38 00 00 00              
    18 : cmp    r12, rcx         ; 49 39 cc                             
    19 : jle    [22]             ; 0f 8e 0b 00 00 00                    
    20 : mov    rcx, r12         ; 4c 89 e1                             
    21 : mov    [rsp+#0x30], rax ; 48 89 84 24 30 00 00 00              
    22 : add    rax, #0x04       ; 48 05 04 00 00 00                    
    23 : jmp    [11]             ; e9 c0 ff ff ff                       
    24 : mov    rdi, #0x04       ; 48 c7 c7 04 00 00 00                 
    25 : mov    r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00              
    26 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    27 : mov    [rsp+#0x08], rdx ; 48 89 94 24 08 00 00 00              
    28 : mov    rax, r13         ; 4c 89 e8                             
    29 : cqo                     ; 48 99                                
    30 : idiv   rdi              ; 48 f7 ff                             
    31 : mov    rsi, rax         ; 48 89 c6                             
    32 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    33 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00              
    34 : mov    r13, [rsp+#0x30] ; 4c 8b ac 24 30 00 00 00              
    35 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    36 : mov    [rsp+#0x08], rdx ; 48 89 94 24 08 00 00 00              
    37 : mov    rax, r13         ; 4c 89 e8                             
    38 : cqo                     ; 48 99                                
    39 : idiv   rdi              ; 48 f7 ff                             
    40 : mov    rdi, rax         ; 48 89 c7                             
    41 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    42 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00              
    43 : mov    r13, [rsp+#0x28] ; 4c 8b ac 24 28 00 00 00              
    44 : mov    [r13], esi       ; 41 89 75 00                          
    45 : mov    r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00              
    46 : mov    [r13], edi       ; 41 89 7d 00                          
    47 : xor    rax, rax         ; 48 31 c0                             
    48 : mov    r12, [rsp+#0x40] ; 4c 8b a4 24 40 00 00 00              
    49 : mov    r13, [rsp+#0x48] ; 4c 8b ac 24 48 00 00 00              
    50 : add    rsp, #0x58       ; 48 81 c4 58 00 00 00                 
    51 : ret                     ; c3                                   
