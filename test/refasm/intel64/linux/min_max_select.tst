min_max_select(i0, i1, i2, i3)
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
    10 : shl    rsi, #0x02       ; 48 c1 e6 02                          
    11 : cmp    rax, rsi         ; 48 39 f0                             
    12 : jge    [28]             ; 0f 8d 4b 00 00 00                    
    13 : movsxd r12, rdi, rax    ; 4c 63 24 07                          
    14 : cmp    r12, rdx         ; 49 39 d4                             
    15 : mov    r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00              
    16 : cmovl  r13, rax         ; 4c 0f 4c e8                          
    17 : mov    [rsp+#0x38], r13 ; 4c 89 ac 24 38 00 00 00              
    18 : cmp    rdx, r12         ; 4c 39 e2                             
    19 : cmovg  rdx, r12         ; 49 0f 4f d4                          
    20 : cmp    r12, rcx         ; 49 39 cc                             
    21 : mov    r13, [rsp+#0x30] ; 4c 8b ac 24 30 00 00 00              
    22 : cmovg  r13, rax         ; 4c 0f 4f e8                          
    23 : mov    [rsp+#0x30], r13 ; 4c 89 ac 24 30 00 00 00              
    24 : cmp    rcx, r12         ; 4c 39 e1                             
    25 : cmovl  rcx, r12         ; 49 0f 4c cc                          
    26 : add    rax, #0x04       ; 48 05 04 00 00 00                    
    27 : jmp    [11]             ; e9 ac ff ff ff                       
    28 : mov    rdi, #0x04       ; 48 c7 c7 04 00 00 00                 
    29 : mov    r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00              
    30 : mov    rdi, r13         ; 4c 89 ef                             
    31 : sar    rdi, #0x02       ; 48 c1 ff 02                          
    32 : mov    r13, [rsp+#0x30] ; 4c 8b ac 24 30 00 00 00              
    33 : mov    rsi, r13         ; 4c 89 ee                             
    34 : sar    rsi, #0x02       ; 48 c1 fe 02                          
    35 : mov    r13, [rsp+#0x28] ; 4c 8b ac 24 28 00 00 00              
    36 : mov    [r13], edi       ; 41 89 7d 00                          
    37 : mov    r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00              
    38 : mov    [r13], esi       ; 41 89 75 00                          
    39 : xor    rax, rax         ; 48 31 c0                             
    40 : mov    r12, [rsp+#0x40] ; 4c 8b a4 24 40 00 00 00              
    41 : mov    r13, [rsp+#0x48] ; 4c 8b ac 24 48 00 00 00              
    42 : add    rsp, #0x58       ; 48 81 c4 58 00 00 00                 
    43 : ret                     ; c3                                   
