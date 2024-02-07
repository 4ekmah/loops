min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x58       ; 48 81 ec 58 00 00 00                 
     1 : mov    [rsp+#0x28], r8  ; 4c 89 84 24 28 00 00 00              
     2 : mov    [rsp+#0x20], r9  ; 4c 89 8c 24 20 00 00 00              
     3 : mov    [rsp+#0x48], r13 ; 4c 89 ac 24 48 00 00 00              
     4 : xor    rax, rax         ; 48 31 c0                             
     5 : mov    [rsp+#0x38], #0  ; 48 c7 84 24 38 00 00 00 00 00 00 00  
     6 : mov    [rsp+#0x30], #0  ; 48 c7 84 24 30 00 00 00 00 00 00 00  
     7 : movsxd r8, rcx          ; 4c 63 01                             
     8 : mov    r9, r8           ; 4d 89 c1                             
     9 : mov    r13, rdx         ; 49 89 d5                             
    10 : imul   r13, #0x04       ; 4d 6b ed 04                          
    11 : mov    [rsp+#0x40], r13 ; 4c 89 ac 24 40 00 00 00              
    12 : cmp    rax, [rsp+#0x40] ; 48 3b 84 24 40 00 00 00              
    13 : jge    [25]             ; 0f 8d 37 00 00 00                    
    14 : movsxd rdx, rcx, rax    ; 48 63 14 01                          
    15 : cmp    rdx, r8          ; 4c 39 c2                             
    16 : jge    [19]             ; 0f 8d 0b 00 00 00                    
    17 : mov    r8, rdx          ; 49 89 d0                             
    18 : mov    [rsp+#0x38], rax ; 48 89 84 24 38 00 00 00              
    19 : cmp    rdx, r9          ; 4c 39 ca                             
    20 : jle    [23]             ; 0f 8e 0b 00 00 00                    
    21 : mov    r9, rdx          ; 49 89 d1                             
    22 : mov    [rsp+#0x30], rax ; 48 89 84 24 30 00 00 00              
    23 : add    rax, #0x04       ; 48 05 04 00 00 00                    
    24 : jmp    [12]             ; e9 bb ff ff ff                       
    25 : mov    rcx, #0x04       ; 48 c7 c1 04 00 00 00                 
    26 : mov    r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00              
    27 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    28 : mov    rax, r13         ; 4c 89 e8                             
    29 : cqo                     ; 48 99                                
    30 : idiv   rcx              ; 48 f7 f9                             
    31 : mov    rdx, rax         ; 48 89 c2                             
    32 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    33 : mov    r13, [rsp+#0x30] ; 4c 8b ac 24 30 00 00 00              
    34 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    35 : mov    [rsp+#0x08], rdx ; 48 89 94 24 08 00 00 00              
    36 : mov    rax, r13         ; 4c 89 e8                             
    37 : cqo                     ; 48 99                                
    38 : idiv   rcx              ; 48 f7 f9                             
    39 : mov    rcx, rax         ; 48 89 c1                             
    40 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    41 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00              
    42 : mov    r13, [rsp+#0x28] ; 4c 8b ac 24 28 00 00 00              
    43 : mov    [r13], edx       ; 41 89 55 00                          
    44 : mov    r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00              
    45 : mov    [r13], ecx       ; 41 89 4d 00                          
    46 : xor    rax, rax         ; 48 31 c0                             
    47 : mov    r13, [rsp+#0x48] ; 4c 8b ac 24 48 00 00 00              
    48 : add    rsp, #0x58       ; 48 81 c4 58 00 00 00                 
    49 : ret                     ; c3                                   
