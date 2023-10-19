conditionpainter(i0)
     0 : sub    rsp, #0x38                       ; 48 81 ec 38 00 00 00                 
     1 : mov    [rsp+#0x08], rdi                 ; 48 89 bc 24 08 00 00 00              
     2 : mov    [rsp+#0x20], r12                 ; 4c 89 a4 24 20 00 00 00              
     3 : mov    [rsp+#0x28], r13                 ; 4c 89 ac 24 28 00 00 00              
     4 : mov    r13, #0xfffffffffffffffb         ; 49 c7 c5 fb ff ff ff                 
     5 : mov    [rsp], r13                       ; 4c 89 ac 24 00 00 00 00              
     6 : cmp    [rsp], #0x05                     ; 48 81 bc 24 00 00 00 00 05 00 00 00  
     7 : jg     [140]                            ; 0f 8f df 02 00 00                    
     8 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
     9 : mov    rdx, r13                         ; 4c 89 ea                             
    10 : add    rdx, #0x05                       ; 48 81 c2 05 00 00 00                 
    11 : imul   rdx, #0x0b                       ; 48 6b d2 0b                          
    12 : mov    r13, rdx                         ; 49 89 d5                             
    13 : imul   r13, #0x08                       ; 4d 6b ed 08                          
    14 : mov    [rsp+#0x18], r13                 ; 4c 89 ac 24 18 00 00 00              
    15 : mov    r13, #0xfffffffffffffffb         ; 49 c7 c5 fb ff ff ff                 
    16 : mov    [rsp+#0x10], r13                 ; 4c 89 ac 24 10 00 00 00              
    17 : cmp    [rsp+#0x10], #0x05               ; 48 81 bc 24 10 00 00 00 05 00 00 00  
    18 : jg     [138]                            ; 0f 8f 88 02 00 00                    
    19 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    20 : mov    rax, r13                         ; 4c 89 e8                             
    21 : add    rax, #0x03                       ; 48 05 03 00 00 00                    
    22 : xor    r12, r12                         ; 4d 31 e4                             
    23 : cmp    [rsp], rax                       ; 48 39 84 24 00 00 00 00              
    24 : setge  r12                              ; 41 0f 9d c4                          
    25 : xor    rax, rax                         ; 48 31 c0                             
    26 : cmp    [rsp], #0x04                     ; 48 81 bc 24 00 00 00 00 04 00 00 00  
    27 : setle  rax                              ; 0f 9e c0                             
    28 : and    r12, rax                         ; 49 21 c4                             
    29 : xor    rax, rax                         ; 48 31 c0                             
    30 : cmp    [rsp+#0x10], #0xfffffffffffffffe ; 48 81 bc 24 10 00 00 00 fe ff ff ff  
    31 : setge  rax                              ; 0f 9d c0                             
    32 : and    r12, rax                         ; 49 21 c4                             
    33 : xor    rax, rax                         ; 48 31 c0                             
    34 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
    35 : setle  rax                              ; 0f 9e c0                             
    36 : and    r12, rax                         ; 49 21 c4                             
    37 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    38 : mov    rax, r13                         ; 4c 89 e8                             
    39 : sub    rax, #0x01                       ; 48 2d 01 00 00 00                    
    40 : xor    rsi, rsi                         ; 48 31 f6                             
    41 : cmp    [rsp], rax                       ; 48 39 84 24 00 00 00 00              
    42 : setle  rsi                              ; 40 0f 9e c6                          
    43 : xor    rax, rax                         ; 48 31 c0                             
    44 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
    45 : setge  rax                              ; 0f 9d c0                             
    46 : and    rsi, rax                         ; 48 21 c6                             
    47 : xor    rax, rax                         ; 48 31 c0                             
    48 : cmp    [rsp], #0                        ; 48 81 bc 24 00 00 00 00 00 00 00 00  
    49 : setle  rax                              ; 0f 9e c0                             
    50 : and    rsi, rax                         ; 48 21 c6                             
    51 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    52 : mov    rax, r13                         ; 4c 89 e8                             
    53 : imul   rax, [rsp+#0x10]                 ; 48 0f af 84 24 10 00 00 00           
    54 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    55 : mov    rdi, r13                         ; 4c 89 ef                             
    56 : imul   rdi, [rsp]                       ; 48 0f af bc 24 00 00 00 00           
    57 : add    rax, rdi                         ; 48 01 f8                             
    58 : xor    rdi, rdi                         ; 48 31 ff                             
    59 : cmp    rax, #0x09                       ; 48 83 f8 09                          
    60 : setle  rdi                              ; 40 0f 9e c7                          
    61 : and    rsi, rdi                         ; 48 21 fe                             
    62 : or     r12, rsi                         ; 49 09 f4                             
    63 : mov    rdi, #0x02                       ; 48 c7 c7 02 00 00 00                 
    64 : xor    rsi, rsi                         ; 48 31 f6                             
    65 : xor    rax, rax                         ; 48 31 c0                             
    66 : cmp    [rsp+#0x10], #0x02               ; 48 81 bc 24 10 00 00 00 02 00 00 00  
    67 : setge  rax                              ; 0f 9d c0                             
    68 : xor    rcx, rcx                         ; 48 31 c9                             
    69 : cmp    [rsp+#0x10], #0x04               ; 48 81 bc 24 10 00 00 00 04 00 00 00  
    70 : setle  rcx                              ; 0f 9e c1                             
    71 : and    rax, rcx                         ; 48 21 c8                             
    72 : xor    rcx, rcx                         ; 48 31 c9                             
    73 : cmp    [rsp], #0x01                     ; 48 81 bc 24 00 00 00 00 01 00 00 00  
    74 : setge  rcx                              ; 0f 9d c1                             
    75 : xor    rdx, rdx                         ; 48 31 d2                             
    76 : cmp    [rsp], #0x03                     ; 48 81 bc 24 00 00 00 00 03 00 00 00  
    77 : setle  rdx                              ; 0f 9e c2                             
    78 : and    rcx, rdx                         ; 48 21 d1                             
    79 : or     rax, rcx                         ; 48 09 c8                             
    80 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    81 : mov    rdx, r13                         ; 4c 89 ea                             
    82 : imul   rdx, [rsp+#0x10]                 ; 48 0f af 94 24 10 00 00 00           
    83 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    84 : mov    rcx, r13                         ; 4c 89 e9                             
    85 : imul   rcx, [rsp]                       ; 48 0f af 8c 24 00 00 00 00           
    86 : add    rdx, rcx                         ; 48 01 ca                             
    87 : xor    rcx, rcx                         ; 48 31 c9                             
    88 : cmp    rdx, #0x10                       ; 48 83 fa 10                          
    89 : setle  rcx                              ; 0f 9e c1                             
    90 : and    rax, rcx                         ; 48 21 c8                             
    91 : cmp    rax, #0                          ; 48 83 f8 00                          
    92 : cmovne rsi, rdi                         ; 48 0f 45 f7                          
    93 : add    r12, rsi                         ; 49 01 f4                             
    94 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    95 : mov    rdi, r13                         ; 4c 89 ef                             
    96 : imul   rdi, #0x02                       ; 48 6b ff 02                          
    97 : cmp    rdi, [rsp+#0x10]                 ; 48 3b bc 24 10 00 00 00              
    98 : jl     [109]                            ; 0f 8c 39 00 00 00                    
    99 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   100 : mov    rdi, r13                         ; 4c 89 ef                             
   101 : add    rdi, #0x03                       ; 48 81 c7 03 00 00 00                 
   102 : neg    rdi                              ; 48 f7 df                             
   103 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   104 : mov    rsi, r13                         ; 4c 89 ee                             
   105 : add    rsi, #0x03                       ; 48 81 c6 03 00 00 00                 
   106 : imul   rdi, rsi                         ; 48 0f af fe                          
   107 : cmp    [rsp], rdi                       ; 48 39 bc 24 00 00 00 00              
   108 : jle    [124]                            ; 0f 8e 56 00 00 00                    
   109 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   110 : mov    rdi, r13                         ; 4c 89 ef                             
   111 : imul   rdi, #0x02                       ; 48 6b ff 02                          
   112 : cmp    [rsp], rdi                       ; 48 39 bc 24 00 00 00 00              
   113 : jg     [127]                            ; 0f 8f 52 00 00 00                    
   114 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   115 : mov    rdi, r13                         ; 4c 89 ef                             
   116 : add    rdi, #0x03                       ; 48 81 c7 03 00 00 00                 
   117 : neg    rdi                              ; 48 f7 df                             
   118 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   119 : mov    rsi, r13                         ; 4c 89 ee                             
   120 : add    rsi, #0x03                       ; 48 81 c6 03 00 00 00                 
   121 : imul   rdi, rsi                         ; 48 0f af fe                          
   122 : cmp    [rsp], rdi                       ; 48 39 bc 24 00 00 00 00              
   123 : jl     [127]                            ; 0f 8c 19 00 00 00                    
   124 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
   125 : jg     [127]                            ; 0f 8f 07 00 00 00                    
   126 : add    r12, #0x03                       ; 49 81 c4 03 00 00 00                 
   127 : mov    r13, [rsp+#0x08]                 ; 4c 8b ac 24 08 00 00 00              
   128 : mov    rdi, r13                         ; 4c 89 ef                             
   129 : add    rdi, [rsp+#0x18]                 ; 48 03 bc 24 18 00 00 00              
   130 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   131 : mov    rsi, r13                         ; 4c 89 ee                             
   132 : add    rsi, #0x05                       ; 48 81 c6 05 00 00 00                 
   133 : shl    rsi, #0x03                       ; 48 c1 e6 03                          
   134 : add    rdi, rsi                         ; 48 01 f7                             
   135 : mov    [rdi], r12                       ; 4c 89 27                             
   136 : add    [rsp+#0x10], #0x01               ; 48 81 84 24 10 00 00 00 01 00 00 00  
   137 : jmp    [17]                             ; e9 66 fd ff ff                       
   138 : add    [rsp], #0x01                     ; 48 81 84 24 00 00 00 00 01 00 00 00  
   139 : jmp    [6]                              ; e9 0f fd ff ff                       
   140 : mov    r12, [rsp+#0x20]                 ; 4c 8b a4 24 20 00 00 00              
   141 : mov    r13, [rsp+#0x28]                 ; 4c 8b ac 24 28 00 00 00              
   142 : add    rsp, #0x38                       ; 48 81 c4 38 00 00 00                 
   143 : ret                                     ; c3                                   
