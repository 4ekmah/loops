min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x58       ; 48 81 ec 58 00 00 00                 
     1 : mov    [rsp+#0x28], r8  ; 4c 89 84 24 28 00 00 00              
     2 : mov    [rsp+#0x20], r9  ; 4c 89 8c 24 20 00 00 00              
     3 : mov    [rsp+#0x40], r12 ; 4c 89 a4 24 40 00 00 00              
     4 : mov    [rsp+#0x48], r13 ; 4c 89 ac 24 48 00 00 00              
     5 : xor    rax, rax         ; 48 31 c0                             
     6 : mov    [rsp+#0x38], #0  ; 48 c7 84 24 38 00 00 00 00 00 00 00  
     7 : mov    [rsp+#0x30], #0  ; 48 c7 84 24 30 00 00 00 00 00 00 00  
     8 : movsxd r8, rcx          ; 4c 63 01                             
     9 : mov    r9, r8           ; 4d 89 c1                             
    10 : imul   rdx, #0x04       ; 48 6b d2 04                          
    11 : cmp    rax, rdx         ; 48 39 d0                             
    12 : jge    [24]             ; 0f 8d 37 00 00 00                    
    13 : movsxd r12, rcx, rax    ; 4c 63 24 01                          
    14 : cmp    r12, r8          ; 4d 39 c4                             
    15 : jge    [18]             ; 0f 8d 0b 00 00 00                    
    16 : mov    r8, r12          ; 4d 89 e0                             
    17 : mov    [rsp+#0x38], rax ; 48 89 84 24 38 00 00 00              
    18 : cmp    r12, r9          ; 4d 39 cc                             
    19 : jle    [22]             ; 0f 8e 0b 00 00 00                    
    20 : mov    r9, r12          ; 4d 89 e1                             
    21 : mov    [rsp+#0x30], rax ; 48 89 84 24 30 00 00 00              
    22 : add    rax, #0x04       ; 48 05 04 00 00 00                    
    23 : jmp    [11]             ; e9 c0 ff ff ff                       
    24 : mov    rcx, #0x04       ; 48 c7 c1 04 00 00 00                 
    25 : mov    r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00              
    26 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    27 : mov    rax, r13         ; 4c 89 e8                             
    28 : cqo                     ; 48 99                                
    29 : idiv   rcx              ; 48 f7 f9                             
    30 : mov    rdx, rax         ; 48 89 c2                             
    31 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    32 : mov    r13, [rsp+#0x30] ; 4c 8b ac 24 30 00 00 00              
    33 : mov    [rsp], rax       ; 48 89 84 24 00 00 00 00              
    34 : mov    [rsp+#0x08], rdx ; 48 89 94 24 08 00 00 00              
    35 : mov    rax, r13         ; 4c 89 e8                             
    36 : cqo                     ; 48 99                                
    37 : idiv   rcx              ; 48 f7 f9                             
    38 : mov    rcx, rax         ; 48 89 c1                             
    39 : mov    rax, [rsp]       ; 48 8b 84 24 00 00 00 00              
    40 : mov    rdx, [rsp+#0x08] ; 48 8b 94 24 08 00 00 00              
    41 : mov    r13, [rsp+#0x28] ; 4c 8b ac 24 28 00 00 00              
    42 : mov    [r13], edx       ; 41 89 55 00                          
    43 : mov    r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00              
    44 : mov    [r13], ecx       ; 41 89 4d 00                          
    45 : xor    rax, rax         ; 48 31 c0                             
    46 : mov    r12, [rsp+#0x40] ; 4c 8b a4 24 40 00 00 00              
    47 : mov    r13, [rsp+#0x48] ; 4c 8b ac 24 48 00 00 00              
    48 : add    rsp, #0x58       ; 48 81 c4 58 00 00 00                 
    49 : ret                     ; c3                                   
