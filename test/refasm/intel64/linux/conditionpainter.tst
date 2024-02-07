conditionpainter(i0)
     0 : sub    rsp, #0x38                       ; 48 81 ec 38 00 00 00                 
     1 : mov    [rsp+#0x08], rdi                 ; 48 89 bc 24 08 00 00 00              
     2 : mov    [rsp+#0x28], r13                 ; 4c 89 ac 24 28 00 00 00              
     3 : mov    r13, #0xfffffffffffffffb         ; 49 c7 c5 fb ff ff ff                 
     4 : mov    [rsp], r13                       ; 4c 89 ac 24 00 00 00 00              
     5 : cmp    [rsp], #0x05                     ; 48 81 bc 24 00 00 00 00 05 00 00 00  
     6 : jg     [141]                            ; 0f 8f f4 02 00 00                    
     7 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
     8 : mov    rdx, r13                         ; 4c 89 ea                             
     9 : add    rdx, #0x05                       ; 48 81 c2 05 00 00 00                 
    10 : imul   rdx, #0x0b                       ; 48 6b d2 0b                          
    11 : mov    r13, rdx                         ; 49 89 d5                             
    12 : imul   r13, #0x08                       ; 4d 6b ed 08                          
    13 : mov    [rsp+#0x18], r13                 ; 4c 89 ac 24 18 00 00 00              
    14 : mov    r13, #0xfffffffffffffffb         ; 49 c7 c5 fb ff ff ff                 
    15 : mov    [rsp+#0x10], r13                 ; 4c 89 ac 24 10 00 00 00              
    16 : cmp    [rsp+#0x10], #0x05               ; 48 81 bc 24 10 00 00 00 05 00 00 00  
    17 : jg     [139]                            ; 0f 8f 9d 02 00 00                    
    18 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    19 : mov    rax, r13                         ; 4c 89 e8                             
    20 : add    rax, #0x03                       ; 48 05 03 00 00 00                    
    21 : xor    rsi, rsi                         ; 48 31 f6                             
    22 : cmp    [rsp], rax                       ; 48 39 84 24 00 00 00 00              
    23 : setge  rsi                              ; 40 0f 9d c6                          
    24 : xor    rax, rax                         ; 48 31 c0                             
    25 : cmp    [rsp], #0x04                     ; 48 81 bc 24 00 00 00 00 04 00 00 00  
    26 : setle  rax                              ; 0f 9e c0                             
    27 : and    rsi, rax                         ; 48 21 c6                             
    28 : xor    rax, rax                         ; 48 31 c0                             
    29 : cmp    [rsp+#0x10], #0xfffffffffffffffe ; 48 81 bc 24 10 00 00 00 fe ff ff ff  
    30 : setge  rax                              ; 0f 9d c0                             
    31 : and    rsi, rax                         ; 48 21 c6                             
    32 : xor    rax, rax                         ; 48 31 c0                             
    33 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
    34 : setle  rax                              ; 0f 9e c0                             
    35 : and    rsi, rax                         ; 48 21 c6                             
    36 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    37 : mov    rax, r13                         ; 4c 89 e8                             
    38 : sub    rax, #0x01                       ; 48 2d 01 00 00 00                    
    39 : xor    rdi, rdi                         ; 48 31 ff                             
    40 : cmp    [rsp], rax                       ; 48 39 84 24 00 00 00 00              
    41 : setle  rdi                              ; 40 0f 9e c7                          
    42 : xor    rax, rax                         ; 48 31 c0                             
    43 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
    44 : setge  rax                              ; 0f 9d c0                             
    45 : and    rdi, rax                         ; 48 21 c7                             
    46 : xor    rax, rax                         ; 48 31 c0                             
    47 : cmp    [rsp], #0                        ; 48 81 bc 24 00 00 00 00 00 00 00 00  
    48 : setle  rax                              ; 0f 9e c0                             
    49 : and    rdi, rax                         ; 48 21 c7                             
    50 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    51 : mov    rax, r13                         ; 4c 89 e8                             
    52 : imul   rax, [rsp+#0x10]                 ; 48 0f af 84 24 10 00 00 00           
    53 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    54 : mov    rcx, r13                         ; 4c 89 e9                             
    55 : imul   rcx, [rsp]                       ; 48 0f af 8c 24 00 00 00 00           
    56 : add    rax, rcx                         ; 48 01 c8                             
    57 : xor    rcx, rcx                         ; 48 31 c9                             
    58 : cmp    rax, #0x09                       ; 48 83 f8 09                          
    59 : setle  rcx                              ; 0f 9e c1                             
    60 : and    rdi, rcx                         ; 48 21 cf                             
    61 : mov    [rsp+#0x20], rsi                 ; 48 89 b4 24 20 00 00 00              
    62 : or     [rsp+#0x20], rdi                 ; 48 09 bc 24 20 00 00 00              
    63 : mov    rdi, #0x02                       ; 48 c7 c7 02 00 00 00                 
    64 : xor    rcx, rcx                         ; 48 31 c9                             
    65 : xor    rax, rax                         ; 48 31 c0                             
    66 : cmp    [rsp+#0x10], #0x02               ; 48 81 bc 24 10 00 00 00 02 00 00 00  
    67 : setge  rax                              ; 0f 9d c0                             
    68 : xor    rdx, rdx                         ; 48 31 d2                             
    69 : cmp    [rsp+#0x10], #0x04               ; 48 81 bc 24 10 00 00 00 04 00 00 00  
    70 : setle  rdx                              ; 0f 9e c2                             
    71 : and    rax, rdx                         ; 48 21 d0                             
    72 : xor    rdx, rdx                         ; 48 31 d2                             
    73 : cmp    [rsp], #0x01                     ; 48 81 bc 24 00 00 00 00 01 00 00 00  
    74 : setge  rdx                              ; 0f 9d c2                             
    75 : xor    rsi, rsi                         ; 48 31 f6                             
    76 : cmp    [rsp], #0x03                     ; 48 81 bc 24 00 00 00 00 03 00 00 00  
    77 : setle  rsi                              ; 40 0f 9e c6                          
    78 : and    rdx, rsi                         ; 48 21 f2                             
    79 : or     rax, rdx                         ; 48 09 d0                             
    80 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    81 : mov    rsi, r13                         ; 4c 89 ee                             
    82 : imul   rsi, [rsp+#0x10]                 ; 48 0f af b4 24 10 00 00 00           
    83 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    84 : mov    rdx, r13                         ; 4c 89 ea                             
    85 : imul   rdx, [rsp]                       ; 48 0f af 94 24 00 00 00 00           
    86 : add    rsi, rdx                         ; 48 01 d6                             
    87 : xor    rdx, rdx                         ; 48 31 d2                             
    88 : cmp    rsi, #0x10                       ; 48 83 fe 10                          
    89 : setle  rdx                              ; 0f 9e c2                             
    90 : and    rax, rdx                         ; 48 21 d0                             
    91 : cmp    rax, #0                          ; 48 83 f8 00                          
    92 : cmovne rcx, rdi                         ; 48 0f 45 cf                          
    93 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    94 : add    rcx, r13                         ; 4c 01 e9                             
    95 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    96 : mov    rdi, r13                         ; 4c 89 ef                             
    97 : imul   rdi, #0x02                       ; 48 6b ff 02                          
    98 : cmp    rdi, [rsp+#0x10]                 ; 48 3b bc 24 10 00 00 00              
    99 : jl     [110]                            ; 0f 8c 39 00 00 00                    
   100 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   101 : mov    rdi, r13                         ; 4c 89 ef                             
   102 : add    rdi, #0x03                       ; 48 81 c7 03 00 00 00                 
   103 : neg    rdi                              ; 48 f7 df                             
   104 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   105 : mov    rsi, r13                         ; 4c 89 ee                             
   106 : add    rsi, #0x03                       ; 48 81 c6 03 00 00 00                 
   107 : imul   rdi, rsi                         ; 48 0f af fe                          
   108 : cmp    [rsp], rdi                       ; 48 39 bc 24 00 00 00 00              
   109 : jle    [125]                            ; 0f 8e 56 00 00 00                    
   110 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   111 : mov    rdi, r13                         ; 4c 89 ef                             
   112 : imul   rdi, #0x02                       ; 48 6b ff 02                          
   113 : cmp    [rsp], rdi                       ; 48 39 bc 24 00 00 00 00              
   114 : jg     [128]                            ; 0f 8f 52 00 00 00                    
   115 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   116 : mov    rdi, r13                         ; 4c 89 ef                             
   117 : add    rdi, #0x03                       ; 48 81 c7 03 00 00 00                 
   118 : neg    rdi                              ; 48 f7 df                             
   119 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   120 : mov    rsi, r13                         ; 4c 89 ee                             
   121 : add    rsi, #0x03                       ; 48 81 c6 03 00 00 00                 
   122 : imul   rdi, rsi                         ; 48 0f af fe                          
   123 : cmp    [rsp], rdi                       ; 48 39 bc 24 00 00 00 00              
   124 : jl     [128]                            ; 0f 8c 19 00 00 00                    
   125 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
   126 : jg     [128]                            ; 0f 8f 07 00 00 00                    
   127 : add    rcx, #0x03                       ; 48 81 c1 03 00 00 00                 
   128 : mov    r13, [rsp+#0x08]                 ; 4c 8b ac 24 08 00 00 00              
   129 : mov    rdi, r13                         ; 4c 89 ef                             
   130 : add    rdi, [rsp+#0x18]                 ; 48 03 bc 24 18 00 00 00              
   131 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   132 : mov    rsi, r13                         ; 4c 89 ee                             
   133 : add    rsi, #0x05                       ; 48 81 c6 05 00 00 00                 
   134 : shl    rsi, #0x03                       ; 48 c1 e6 03                          
   135 : add    rdi, rsi                         ; 48 01 f7                             
   136 : mov    [rdi], rcx                       ; 48 89 0f                             
   137 : add    [rsp+#0x10], #0x01               ; 48 81 84 24 10 00 00 00 01 00 00 00  
   138 : jmp    [16]                             ; e9 51 fd ff ff                       
   139 : add    [rsp], #0x01                     ; 48 81 84 24 00 00 00 00 01 00 00 00  
   140 : jmp    [5]                              ; e9 fa fc ff ff                       
   141 : mov    r13, [rsp+#0x28]                 ; 4c 8b ac 24 28 00 00 00              
   142 : add    rsp, #0x38                       ; 48 81 c4 38 00 00 00                 
   143 : ret                                     ; c3                                   
