conditionpainter(i0)
     0 : sub    rsp, #0x58                       ; 48 83 ec 58                          
     1 : mov    [rsp+#0x28], rdi                 ; 48 89 7c 24 28                       
     2 : mov    [rsp+#0x40], r12                 ; 4c 89 64 24 40                       
     3 : mov    [rsp+#0x48], r13                 ; 4c 89 6c 24 48                       
     4 : mov    [rsp+#0x20], #0xfffffffffffffffb ; 48 c7 44 24 20 fb ff ff ff           
     5 : cmp    [rsp+#0x20], #0x05               ; 48 81 bc 24 20 00 00 00 05 00 00 00  
     6 : jg     [138]                            ; 0f 8f b1 02 00 00                    
     7 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
     8 : mov    rdx, r13                         ; 4c 89 ea                             
     9 : sub    rdx, #0xfffffffffffffffb         ; 48 83 ea fb                          
    10 : imul   rdx, #0x0b                       ; 48 6b d2 0b                          
    11 : mov    r13, rdx                         ; 49 89 d5                             
    12 : imul   r13, #0x08                       ; 4d 6b ed 08                          
    13 : mov    [rsp+#0x38], r13                 ; 4c 89 6c 24 38                       
    14 : mov    [rsp+#0x30], #0xfffffffffffffffb ; 48 c7 44 24 30 fb ff ff ff           
    15 : cmp    [rsp+#0x30], #0x05               ; 48 81 bc 24 30 00 00 00 05 00 00 00  
    16 : jg     [136]                            ; 0f 8f 66 02 00 00                    
    17 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    18 : mov    rax, r13                         ; 4c 89 e8                             
    19 : add    rax, #0x03                       ; 48 83 c0 03                          
    20 : xor    r12, r12                         ; 4d 31 e4                             
    21 : cmp    [rsp+#0x20], rax                 ; 48 39 84 24 20 00 00 00              
    22 : setge  r12                              ; 41 0f 9d c4                          
    23 : xor    rax, rax                         ; 48 31 c0                             
    24 : cmp    [rsp+#0x20], #0x04               ; 48 81 bc 24 20 00 00 00 04 00 00 00  
    25 : setle  rax                              ; 0f 9e c0                             
    26 : and    r12, rax                         ; 49 21 c4                             
    27 : xor    rax, rax                         ; 48 31 c0                             
    28 : cmp    [rsp+#0x30], #0xfffffffffffffffe ; 48 81 bc 24 30 00 00 00 fe ff ff ff  
    29 : setge  rax                              ; 0f 9d c0                             
    30 : and    r12, rax                         ; 49 21 c4                             
    31 : xor    rax, rax                         ; 48 31 c0                             
    32 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
    33 : setle  rax                              ; 0f 9e c0                             
    34 : and    r12, rax                         ; 49 21 c4                             
    35 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    36 : mov    rax, r13                         ; 4c 89 e8                             
    37 : sub    rax, #0x01                       ; 48 83 e8 01                          
    38 : xor    rsi, rsi                         ; 48 31 f6                             
    39 : cmp    [rsp+#0x20], rax                 ; 48 39 84 24 20 00 00 00              
    40 : setle  rsi                              ; 40 0f 9e c6                          
    41 : xor    rax, rax                         ; 48 31 c0                             
    42 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
    43 : setge  rax                              ; 0f 9d c0                             
    44 : and    rsi, rax                         ; 48 21 c6                             
    45 : xor    rax, rax                         ; 48 31 c0                             
    46 : cmp    [rsp+#0x20], #0                  ; 48 81 bc 24 20 00 00 00 00 00 00 00  
    47 : setle  rax                              ; 0f 9e c0                             
    48 : and    rsi, rax                         ; 48 21 c6                             
    49 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    50 : mov    rax, r13                         ; 4c 89 e8                             
    51 : imul   rax, [rsp+#0x30]                 ; 48 0f af 44 24 30                    
    52 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    53 : mov    rdi, r13                         ; 4c 89 ef                             
    54 : imul   rdi, [rsp+#0x20]                 ; 48 0f af 7c 24 20                    
    55 : add    rax, rdi                         ; 48 01 f8                             
    56 : xor    rdi, rdi                         ; 48 31 ff                             
    57 : cmp    rax, #0x09                       ; 48 83 f8 09                          
    58 : setle  rdi                              ; 40 0f 9e c7                          
    59 : and    rsi, rdi                         ; 48 21 fe                             
    60 : or     r12, rsi                         ; 49 09 f4                             
    61 : mov    rdi, #0x02                       ; 48 c7 c7 02 00 00 00                 
    62 : xor    rsi, rsi                         ; 48 31 f6                             
    63 : xor    rax, rax                         ; 48 31 c0                             
    64 : cmp    [rsp+#0x30], #0x02               ; 48 81 bc 24 30 00 00 00 02 00 00 00  
    65 : setge  rax                              ; 0f 9d c0                             
    66 : xor    rcx, rcx                         ; 48 31 c9                             
    67 : cmp    [rsp+#0x30], #0x04               ; 48 81 bc 24 30 00 00 00 04 00 00 00  
    68 : setle  rcx                              ; 0f 9e c1                             
    69 : and    rax, rcx                         ; 48 21 c8                             
    70 : xor    rcx, rcx                         ; 48 31 c9                             
    71 : cmp    [rsp+#0x20], #0x01               ; 48 81 bc 24 20 00 00 00 01 00 00 00  
    72 : setge  rcx                              ; 0f 9d c1                             
    73 : xor    rdx, rdx                         ; 48 31 d2                             
    74 : cmp    [rsp+#0x20], #0x03               ; 48 81 bc 24 20 00 00 00 03 00 00 00  
    75 : setle  rdx                              ; 0f 9e c2                             
    76 : and    rcx, rdx                         ; 48 21 d1                             
    77 : or     rax, rcx                         ; 48 09 c8                             
    78 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    79 : mov    rdx, r13                         ; 4c 89 ea                             
    80 : imul   rdx, [rsp+#0x30]                 ; 48 0f af 54 24 30                    
    81 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    82 : mov    rcx, r13                         ; 4c 89 e9                             
    83 : imul   rcx, [rsp+#0x20]                 ; 48 0f af 4c 24 20                    
    84 : add    rdx, rcx                         ; 48 01 ca                             
    85 : xor    rcx, rcx                         ; 48 31 c9                             
    86 : cmp    rdx, #0x10                       ; 48 83 fa 10                          
    87 : setle  rcx                              ; 0f 9e c1                             
    88 : and    rax, rcx                         ; 48 21 c8                             
    89 : cmp    rax, #0                          ; 48 83 f8 00                          
    90 : cmovne rsi, rdi                         ; 48 0f 45 f7                          
    91 : add    r12, rsi                         ; 49 01 f4                             
    92 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    93 : mov    rdi, r13                         ; 4c 89 ef                             
    94 : imul   rdi, #0x02                       ; 48 6b ff 02                          
    95 : cmp    rdi, [rsp+#0x30]                 ; 48 3b bc 24 30 00 00 00              
    96 : jl     [107]                            ; 0f 8c 33 00 00 00                    
    97 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    98 : mov    rdi, r13                         ; 4c 89 ef                             
    99 : add    rdi, #0x03                       ; 48 83 c7 03                          
   100 : neg    rdi                              ; 48 f7 df                             
   101 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   102 : mov    rsi, r13                         ; 4c 89 ee                             
   103 : add    rsi, #0x03                       ; 48 83 c6 03                          
   104 : imul   rdi, rsi                         ; 48 0f af fe                          
   105 : cmp    [rsp+#0x20], rdi                 ; 48 39 bc 24 20 00 00 00              
   106 : jle    [122]                            ; 0f 8e 50 00 00 00                    
   107 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   108 : mov    rdi, r13                         ; 4c 89 ef                             
   109 : imul   rdi, #0x02                       ; 48 6b ff 02                          
   110 : cmp    [rsp+#0x20], rdi                 ; 48 39 bc 24 20 00 00 00              
   111 : jg     [125]                            ; 0f 8f 49 00 00 00                    
   112 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   113 : mov    rdi, r13                         ; 4c 89 ef                             
   114 : add    rdi, #0x03                       ; 48 83 c7 03                          
   115 : neg    rdi                              ; 48 f7 df                             
   116 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   117 : mov    rsi, r13                         ; 4c 89 ee                             
   118 : add    rsi, #0x03                       ; 48 83 c6 03                          
   119 : imul   rdi, rsi                         ; 48 0f af fe                          
   120 : cmp    [rsp+#0x20], rdi                 ; 48 39 bc 24 20 00 00 00              
   121 : jl     [125]                            ; 0f 8c 16 00 00 00                    
   122 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
   123 : jg     [125]                            ; 0f 8f 04 00 00 00                    
   124 : add    r12, #0x03                       ; 49 83 c4 03                          
   125 : mov    r13, [rsp+#0x28]                 ; 4c 8b ac 24 28 00 00 00              
   126 : mov    rdi, r13                         ; 4c 89 ef                             
   127 : add    rdi, [rsp+#0x38]                 ; 48 03 bc 24 38 00 00 00              
   128 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   129 : mov    rsi, r13                         ; 4c 89 ee                             
   130 : sub    rsi, #0xfffffffffffffffb         ; 48 83 ee fb                          
   131 : shl    rsi, #0x03                       ; 48 c1 e6 03                          
   132 : add    rdi, rsi                         ; 48 01 f7                             
   133 : mov    [rdi], r12                       ; 4c 89 27                             
   134 : add    [rsp+#0x30], #0x01               ; 48 81 84 24 30 00 00 00 01 00 00 00  
   135 : jmp    [15]                             ; e9 88 fd ff ff                       
   136 : add    [rsp+#0x20], #0x01               ; 48 81 84 24 20 00 00 00 01 00 00 00  
   137 : jmp    [5]                              ; e9 3d fd ff ff                       
   138 : mov    r12, [rsp+#0x40]                 ; 4c 8b a4 24 40 00 00 00              
   139 : mov    r13, [rsp+#0x48]                 ; 4c 8b ac 24 48 00 00 00              
   140 : add    rsp, #0x58                       ; 48 83 c4 58                          
   141 : ret                                     ; c3                                   
