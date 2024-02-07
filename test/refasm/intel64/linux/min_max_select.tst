min_max_select(i0, i1, i2, i3)
     0 : sub    rsp, #0x38         ; 48 81 ec 38 00 00 00                 
     1 : mov    [rsp+#0x08], rdx   ; 48 89 94 24 08 00 00 00              
     2 : mov    [rsp], rcx         ; 48 89 8c 24 00 00 00 00              
     3 : mov    [rsp+#0x28], r13   ; 4c 89 ac 24 28 00 00 00              
     4 : xor    rax, rax           ; 48 31 c0                             
     5 : mov    [rsp+#0x18], #0    ; 48 c7 84 24 18 00 00 00 00 00 00 00  
     6 : mov    [rsp+#0x10], #0    ; 48 c7 84 24 10 00 00 00 00 00 00 00  
     7 : movsxd rdx, rdi           ; 48 63 17                             
     8 : mov    rcx, rdx           ; 48 89 d1                             
     9 : mov    [rsp+#0x20], rsi   ; 48 89 b4 24 20 00 00 00              
    10 : shl    [rsp+#0x20], #0x02 ; 48 c1 a4 24 20 00 00 00 02           
    11 : cmp    rax, [rsp+#0x20]   ; 48 3b 84 24 20 00 00 00              
    12 : jge    [28]               ; 0f 8d 4b 00 00 00                    
    13 : movsxd rsi, rdi, rax      ; 48 63 34 07                          
    14 : cmp    rsi, rdx           ; 48 39 d6                             
    15 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00              
    16 : cmovl  r13, rax           ; 4c 0f 4c e8                          
    17 : mov    [rsp+#0x18], r13   ; 4c 89 ac 24 18 00 00 00              
    18 : cmp    rdx, rsi           ; 48 39 f2                             
    19 : cmovg  rdx, rsi           ; 48 0f 4f d6                          
    20 : cmp    rsi, rcx           ; 48 39 ce                             
    21 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00              
    22 : cmovg  r13, rax           ; 4c 0f 4f e8                          
    23 : mov    [rsp+#0x10], r13   ; 4c 89 ac 24 10 00 00 00              
    24 : cmp    rcx, rsi           ; 48 39 f1                             
    25 : cmovl  rcx, rsi           ; 48 0f 4c ce                          
    26 : add    rax, #0x04         ; 48 05 04 00 00 00                    
    27 : jmp    [11]               ; e9 a7 ff ff ff                       
    28 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00              
    29 : mov    rdi, r13           ; 4c 89 ef                             
    30 : sar    rdi, #0x02         ; 48 c1 ff 02                          
    31 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00              
    32 : mov    rsi, r13           ; 4c 89 ee                             
    33 : sar    rsi, #0x02         ; 48 c1 fe 02                          
    34 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    35 : mov    [r13], edi         ; 41 89 7d 00                          
    36 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    37 : mov    [r13], esi         ; 41 89 75 00                          
    38 : xor    rax, rax           ; 48 31 c0                             
    39 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00              
    40 : add    rsp, #0x38         ; 48 81 c4 38 00 00 00                 
    41 : ret                       ; c3                                   
