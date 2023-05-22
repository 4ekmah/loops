conditionpainter(i0)
     0 : sub    rsp, #0x58                       ; 48 83 ec 58                          
     1 : mov    [rsp+#0x28], rdi                 ; 48 89 7c 24 28                       
     2 : mov    [rsp+#0x40], r12                 ; 4c 89 64 24 40                       
     3 : mov    [rsp+#0x48], r13                 ; 4c 89 6c 24 48                       
     4 : mov    [rsp+#0x20], #0xfffffffffffffffb ; 48 c7 44 24 20 fb ff ff ff           
     5 : cmp    [rsp+#0x20], #0x05               ; 48 81 bc 24 20 00 00 00 05 00 00 00  
     6 : jg     [141]                            ; 0f 8f c3 02 00 00                    
     7 : mov    rdx, #0xfffffffffffffffb         ; 48 c7 c2 fb ff ff ff                 
     8 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
     9 : sub    rdx, r13                         ; 4c 29 ea                             
    10 : neg    rdx                              ; 48 f7 da                             
    11 : imul   rdx, #0x0b                       ; 48 6b d2 0b                          
    12 : mov    r13, rdx                         ; 49 89 d5                             
    13 : imul   r13, #0x08                       ; 4d 6b ed 08                          
    14 : mov    [rsp+#0x38], r13                 ; 4c 89 6c 24 38                       
    15 : mov    [rsp+#0x30], #0xfffffffffffffffb ; 48 c7 44 24 30 fb ff ff ff           
    16 : cmp    [rsp+#0x30], #0x05               ; 48 81 bc 24 30 00 00 00 05 00 00 00  
    17 : jg     [139]                            ; 0f 8f 72 02 00 00                    
    18 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    19 : mov    rax, r13                         ; 4c 89 e8                             
    20 : add    rax, #0x03                       ; 48 83 c0 03                          
    21 : xor    r12, r12                         ; 4d 31 e4                             
    22 : cmp    [rsp+#0x20], rax                 ; 48 39 84 24 20 00 00 00              
    23 : setge  r12                              ; 41 0f 9d c4                          
    24 : xor    rax, rax                         ; 48 31 c0                             
    25 : cmp    [rsp+#0x20], #0x04               ; 48 81 bc 24 20 00 00 00 04 00 00 00  
    26 : setle  rax                              ; 0f 9e c0                             
    27 : and    r12, rax                         ; 49 21 c4                             
    28 : mov    rax, #0xfffffffffffffffe         ; 48 c7 c0 fe ff ff ff                 
    29 : xor    rsi, rsi                         ; 48 31 f6                             
    30 : cmp    [rsp+#0x30], rax                 ; 48 39 84 24 30 00 00 00              
    31 : setge  rsi                              ; 40 0f 9d c6                          
    32 : and    r12, rsi                         ; 49 21 f4                             
    33 : xor    rsi, rsi                         ; 48 31 f6                             
    34 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
    35 : setle  rsi                              ; 40 0f 9e c6                          
    36 : and    r12, rsi                         ; 49 21 f4                             
    37 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    38 : mov    rsi, r13                         ; 4c 89 ee                             
    39 : sub    rsi, #0x01                       ; 48 83 ee 01                          
    40 : xor    rax, rax                         ; 48 31 c0                             
    41 : cmp    [rsp+#0x20], rsi                 ; 48 39 b4 24 20 00 00 00              
    42 : setle  rax                              ; 0f 9e c0                             
    43 : xor    rsi, rsi                         ; 48 31 f6                             
    44 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
    45 : setge  rsi                              ; 40 0f 9d c6                          
    46 : and    rax, rsi                         ; 48 21 f0                             
    47 : xor    rsi, rsi                         ; 48 31 f6                             
    48 : cmp    [rsp+#0x20], #0                  ; 48 81 bc 24 20 00 00 00 00 00 00 00  
    49 : setle  rsi                              ; 40 0f 9e c6                          
    50 : and    rax, rsi                         ; 48 21 f0                             
    51 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    52 : mov    rsi, r13                         ; 4c 89 ee                             
    53 : imul   rsi, [rsp+#0x30]                 ; 48 0f af 74 24 30                    
    54 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    55 : mov    rdi, r13                         ; 4c 89 ef                             
    56 : imul   rdi, [rsp+#0x20]                 ; 48 0f af 7c 24 20                    
    57 : add    rsi, rdi                         ; 48 01 fe                             
    58 : xor    rdi, rdi                         ; 48 31 ff                             
    59 : cmp    rsi, #0x09                       ; 48 83 fe 09                          
    60 : setle  rdi                              ; 40 0f 9e c7                          
    61 : and    rax, rdi                         ; 48 21 f8                             
    62 : or     r12, rax                         ; 49 09 c4                             
    63 : mov    rdi, #0x02                       ; 48 c7 c7 02 00 00 00                 
    64 : xor    rsi, rsi                         ; 48 31 f6                             
    65 : xor    rax, rax                         ; 48 31 c0                             
    66 : cmp    [rsp+#0x30], #0x02               ; 48 81 bc 24 30 00 00 00 02 00 00 00  
    67 : setge  rax                              ; 0f 9d c0                             
    68 : xor    rcx, rcx                         ; 48 31 c9                             
    69 : cmp    [rsp+#0x30], #0x04               ; 48 81 bc 24 30 00 00 00 04 00 00 00  
    70 : setle  rcx                              ; 0f 9e c1                             
    71 : and    rax, rcx                         ; 48 21 c8                             
    72 : xor    rcx, rcx                         ; 48 31 c9                             
    73 : cmp    [rsp+#0x20], #0x01               ; 48 81 bc 24 20 00 00 00 01 00 00 00  
    74 : setge  rcx                              ; 0f 9d c1                             
    75 : xor    rdx, rdx                         ; 48 31 d2                             
    76 : cmp    [rsp+#0x20], #0x03               ; 48 81 bc 24 20 00 00 00 03 00 00 00  
    77 : setle  rdx                              ; 0f 9e c2                             
    78 : and    rcx, rdx                         ; 48 21 d1                             
    79 : or     rax, rcx                         ; 48 09 c8                             
    80 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    81 : mov    rdx, r13                         ; 4c 89 ea                             
    82 : imul   rdx, [rsp+#0x30]                 ; 48 0f af 54 24 30                    
    83 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    84 : mov    rcx, r13                         ; 4c 89 e9                             
    85 : imul   rcx, [rsp+#0x20]                 ; 48 0f af 4c 24 20                    
    86 : add    rdx, rcx                         ; 48 01 ca                             
    87 : xor    rcx, rcx                         ; 48 31 c9                             
    88 : cmp    rdx, #0x10                       ; 48 83 fa 10                          
    89 : setle  rcx                              ; 0f 9e c1                             
    90 : and    rax, rcx                         ; 48 21 c8                             
    91 : cmp    rax, #0                          ; 48 83 f8 00                          
    92 : cmovne rsi, rdi                         ; 48 0f 45 f7                          
    93 : add    r12, rsi                         ; 49 01 f4                             
    94 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    95 : mov    rdi, r13                         ; 4c 89 ef                             
    96 : imul   rdi, #0x02                       ; 48 6b ff 02                          
    97 : cmp    rdi, [rsp+#0x30]                 ; 48 3b bc 24 30 00 00 00              
    98 : jl     [109]                            ; 0f 8c 33 00 00 00                    
    99 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   100 : mov    rdi, r13                         ; 4c 89 ef                             
   101 : add    rdi, #0x03                       ; 48 83 c7 03                          
   102 : neg    rdi                              ; 48 f7 df                             
   103 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   104 : mov    rsi, r13                         ; 4c 89 ee                             
   105 : add    rsi, #0x03                       ; 48 83 c6 03                          
   106 : imul   rdi, rsi                         ; 48 0f af fe                          
   107 : cmp    [rsp+#0x20], rdi                 ; 48 39 bc 24 20 00 00 00              
   108 : jle    [124]                            ; 0f 8e 50 00 00 00                    
   109 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   110 : mov    rdi, r13                         ; 4c 89 ef                             
   111 : imul   rdi, #0x02                       ; 48 6b ff 02                          
   112 : cmp    [rsp+#0x20], rdi                 ; 48 39 bc 24 20 00 00 00              
   113 : jg     [127]                            ; 0f 8f 49 00 00 00                    
   114 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   115 : mov    rdi, r13                         ; 4c 89 ef                             
   116 : add    rdi, #0x03                       ; 48 83 c7 03                          
   117 : neg    rdi                              ; 48 f7 df                             
   118 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   119 : mov    rsi, r13                         ; 4c 89 ee                             
   120 : add    rsi, #0x03                       ; 48 83 c6 03                          
   121 : imul   rdi, rsi                         ; 48 0f af fe                          
   122 : cmp    [rsp+#0x20], rdi                 ; 48 39 bc 24 20 00 00 00              
   123 : jl     [127]                            ; 0f 8c 16 00 00 00                    
   124 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
   125 : jg     [127]                            ; 0f 8f 04 00 00 00                    
   126 : add    r12, #0x03                       ; 49 83 c4 03                          
   127 : mov    r13, [rsp+#0x28]                 ; 4c 8b ac 24 28 00 00 00              
   128 : mov    rdi, r13                         ; 4c 89 ef                             
   129 : add    rdi, [rsp+#0x38]                 ; 48 03 bc 24 38 00 00 00              
   130 : mov    rsi, #0xfffffffffffffffb         ; 48 c7 c6 fb ff ff ff                 
   131 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   132 : sub    rsi, r13                         ; 4c 29 ee                             
   133 : neg    rsi                              ; 48 f7 de                             
   134 : shl    rsi, #0x03                       ; 48 c1 e6 03                          
   135 : add    rdi, rsi                         ; 48 01 f7                             
   136 : mov    [rdi], r12                       ; 4c 89 27                             
   137 : add    [rsp+#0x30], #0x01               ; 48 81 84 24 30 00 00 00 01 00 00 00  
   138 : jmp    [16]                             ; e9 7c fd ff ff                       
   139 : add    [rsp+#0x20], #0x01               ; 48 81 84 24 20 00 00 00 01 00 00 00  
   140 : jmp    [5]                              ; e9 2b fd ff ff                       
   141 : mov    r12, [rsp+#0x40]                 ; 4c 8b a4 24 40 00 00 00              
   142 : mov    r13, [rsp+#0x48]                 ; 4c 8b ac 24 48 00 00 00              
   143 : add    rsp, #0x58                       ; 48 83 c4 58                          
   144 : ret                                     ; c3                                   
