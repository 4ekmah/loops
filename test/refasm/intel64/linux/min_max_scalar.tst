min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x58       ; 48 81 ec 58 00 00 00                 
     1 : mov    [rsp+#0x28], rdx ; 48 89 94 24 28 00 00 00              
     2 : mov    [rsp+#0x20], rcx ; 48 89 8c 24 20 00 00 00              
     3 : mov    [rsp+#0x48], r13 ; 4c 89 ac 24 48 00 00 00              
     4 : xor    rax, rax         ; 48 31 c0                             
     5 : mov    [rsp+#0x38], #0  ; 48 c7 84 24 38 00 00 00 00 00 00 00  
     6 : mov    [rsp+#0x30], #0  ; 48 c7 84 24 30 00 00 00 00 00 00 00  
     7 : movsxd rdx, rdi         ; 48 63 17                             
     8 : mov    rcx, rdx         ; 48 89 d1                             
     9 : mov    r13, rsi         ; 49 89 f5                             
    10 : imul   r13, #0x04       ; 4d 6b ed 04                          
    11 : mov    [rsp+#0x40], r13 ; 4c 89 ac 24 40 00 00 00              
    12 : cmp    rax, [rsp+#0x40] ; 48 3b 84 24 40 00 00 00              
    13 : jge    [25]             ; 0f 8d 37 00 00 00                    
    14 : movsxd rsi, rdi, rax    ; 48 63 34 07                          
    15 : cmp    rsi, rdx         ; 48 39 d6                             
    16 : jge    [19]             ; 0f 8d 0b 00 00 00                    
    17 : mov    rdx, rsi         ; 48 89 f2                             
    18 : mov    [rsp+#0x38], rax ; 48 89 84 24 38 00 00 00              
    19 : cmp    rsi, rcx         ; 48 39 ce                             
    20 : jle    [23]             ; 0f 8e 0b 00 00 00                    
    21 : mov    rcx, rsi         ; 48 89 f1                             
    22 : mov    [rsp+#0x30], rax ; 48 89 84 24 30 00 00 00              
    23 : add    rax, #0x04       ; 48 05 04 00 00 00                    
    24 : jmp    [12]             ; e9 bb ff ff ff                       
    25 : mov    rdi, #0x04       ; 48 c7 c7 04 00 00 00                 
    26 : mov    r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00              
    27 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    28 : mov    [rsp+#0x08], rdx ; 48 89 94 24 08 00 00 00              
    29 : mov    rax, r13         ; 4c 89 e8                             
    30 : cqo                     ; 48 99                                
    31 : idiv   rdi              ; 48 f7 ff                             
    32 : mov    rsi, rax         ; 48 89 c6                             
    33 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    34 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00              
    35 : mov    r13, [rsp+#0x30] ; 4c 8b ac 24 30 00 00 00              
    36 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    37 : mov    [rsp+#0x08], rdx ; 48 89 94 24 08 00 00 00              
    38 : mov    rax, r13         ; 4c 89 e8                             
    39 : cqo                     ; 48 99                                
    40 : idiv   rdi              ; 48 f7 ff                             
    41 : mov    rdi, rax         ; 48 89 c7                             
    42 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    43 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00              
    44 : mov    r13, [rsp+#0x28] ; 4c 8b ac 24 28 00 00 00              
    45 : mov    [r13], esi       ; 41 89 75 00                          
    46 : mov    r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00              
    47 : mov    [r13], edi       ; 41 89 7d 00                          
    48 : xor    rax, rax         ; 48 31 c0                             
    49 : mov    r13, [rsp+#0x48] ; 4c 8b ac 24 48 00 00 00              
    50 : add    rsp, #0x58       ; 48 81 c4 58 00 00 00                 
    51 : ret                     ; c3                                   
