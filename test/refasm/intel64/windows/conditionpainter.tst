conditionpainter(i0)
     0 : sub    rsp, #0x58                       ; 48 83 ec 58                          
     1 : mov    [rsp+#0x28], rcx                 ; 48 89 4c 24 28                       
     2 : mov    [rsp+#0x40], r12                 ; 4c 89 64 24 40                       
     3 : mov    [rsp+#0x48], r13                 ; 4c 89 6c 24 48                       
     4 : mov    [rsp+#0x20], #0xfffffffffffffffb ; 48 c7 44 24 20 fb ff ff ff           
     5 : cmp    [rsp+#0x20], #0x05               ; 48 81 bc 24 20 00 00 00 05 00 00 00  
     6 : jg     [138]                            ; 0f 8f b3 02 00 00                    
     7 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
     8 : mov    r8, r13                          ; 4d 89 e8                             
     9 : sub    r8, #0xfffffffffffffffb          ; 49 83 e8 fb                          
    10 : imul   r8, #0x0b                        ; 4d 6b c0 0b                          
    11 : mov    r13, r8                          ; 4d 89 c5                             
    12 : imul   r13, #0x08                       ; 4d 6b ed 08                          
    13 : mov    [rsp+#0x38], r13                 ; 4c 89 6c 24 38                       
    14 : mov    [rsp+#0x30], #0xfffffffffffffffb ; 48 c7 44 24 30 fb ff ff ff           
    15 : cmp    [rsp+#0x30], #0x05               ; 48 81 bc 24 30 00 00 00 05 00 00 00  
    16 : jg     [136]                            ; 0f 8f 68 02 00 00                    
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
    38 : xor    rdx, rdx                         ; 48 31 d2                             
    39 : cmp    [rsp+#0x20], rax                 ; 48 39 84 24 20 00 00 00              
    40 : setle  rdx                              ; 0f 9e c2                             
    41 : xor    rax, rax                         ; 48 31 c0                             
    42 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
    43 : setge  rax                              ; 0f 9d c0                             
    44 : and    rdx, rax                         ; 48 21 c2                             
    45 : xor    rax, rax                         ; 48 31 c0                             
    46 : cmp    [rsp+#0x20], #0                  ; 48 81 bc 24 20 00 00 00 00 00 00 00  
    47 : setle  rax                              ; 0f 9e c0                             
    48 : and    rdx, rax                         ; 48 21 c2                             
    49 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    50 : mov    rax, r13                         ; 4c 89 e8                             
    51 : imul   rax, [rsp+#0x30]                 ; 48 0f af 44 24 30                    
    52 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    53 : mov    rcx, r13                         ; 4c 89 e9                             
    54 : imul   rcx, [rsp+#0x20]                 ; 48 0f af 4c 24 20                    
    55 : add    rax, rcx                         ; 48 01 c8                             
    56 : xor    rcx, rcx                         ; 48 31 c9                             
    57 : cmp    rax, #0x09                       ; 48 83 f8 09                          
    58 : setle  rcx                              ; 0f 9e c1                             
    59 : and    rdx, rcx                         ; 48 21 ca                             
    60 : or     r12, rdx                         ; 49 09 d4                             
    61 : mov    rcx, #0x02                       ; 48 c7 c1 02 00 00 00                 
    62 : xor    rdx, rdx                         ; 48 31 d2                             
    63 : xor    rax, rax                         ; 48 31 c0                             
    64 : cmp    [rsp+#0x30], #0x02               ; 48 81 bc 24 30 00 00 00 02 00 00 00  
    65 : setge  rax                              ; 0f 9d c0                             
    66 : xor    r9, r9                           ; 4d 31 c9                             
    67 : cmp    [rsp+#0x30], #0x04               ; 48 81 bc 24 30 00 00 00 04 00 00 00  
    68 : setle  r9                               ; 41 0f 9e c1                          
    69 : and    rax, r9                          ; 4c 21 c8                             
    70 : xor    r9, r9                           ; 4d 31 c9                             
    71 : cmp    [rsp+#0x20], #0x01               ; 48 81 bc 24 20 00 00 00 01 00 00 00  
    72 : setge  r9                               ; 41 0f 9d c1                          
    73 : xor    r8, r8                           ; 4d 31 c0                             
    74 : cmp    [rsp+#0x20], #0x03               ; 48 81 bc 24 20 00 00 00 03 00 00 00  
    75 : setle  r8                               ; 41 0f 9e c0                          
    76 : and    r9, r8                           ; 4d 21 c1                             
    77 : or     rax, r9                          ; 4c 09 c8                             
    78 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    79 : mov    r8, r13                          ; 4d 89 e8                             
    80 : imul   r8, [rsp+#0x30]                  ; 4c 0f af 44 24 30                    
    81 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    82 : mov    r9, r13                          ; 4d 89 e9                             
    83 : imul   r9, [rsp+#0x20]                  ; 4c 0f af 4c 24 20                    
    84 : add    r8, r9                           ; 4d 01 c8                             
    85 : xor    r9, r9                           ; 4d 31 c9                             
    86 : cmp    r8, #0x10                        ; 49 83 f8 10                          
    87 : setle  r9                               ; 41 0f 9e c1                          
    88 : and    rax, r9                          ; 4c 21 c8                             
    89 : cmp    rax, #0                          ; 48 83 f8 00                          
    90 : cmovne rdx, rcx                         ; 48 0f 45 d1                          
    91 : add    r12, rdx                         ; 49 01 d4                             
    92 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    93 : mov    rcx, r13                         ; 4c 89 e9                             
    94 : imul   rcx, #0x02                       ; 48 6b c9 02                          
    95 : cmp    rcx, [rsp+#0x30]                 ; 48 3b 8c 24 30 00 00 00              
    96 : jl     [107]                            ; 0f 8c 33 00 00 00                    
    97 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    98 : mov    rcx, r13                         ; 4c 89 e9                             
    99 : add    rcx, #0x03                       ; 48 83 c1 03                          
   100 : neg    rcx                              ; 48 f7 d9                             
   101 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   102 : mov    rdx, r13                         ; 4c 89 ea                             
   103 : add    rdx, #0x03                       ; 48 83 c2 03                          
   104 : imul   rcx, rdx                         ; 48 0f af ca                          
   105 : cmp    [rsp+#0x20], rcx                 ; 48 39 8c 24 20 00 00 00              
   106 : jle    [122]                            ; 0f 8e 50 00 00 00                    
   107 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   108 : mov    rcx, r13                         ; 4c 89 e9                             
   109 : imul   rcx, #0x02                       ; 48 6b c9 02                          
   110 : cmp    [rsp+#0x20], rcx                 ; 48 39 8c 24 20 00 00 00              
   111 : jg     [125]                            ; 0f 8f 49 00 00 00                    
   112 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   113 : mov    rcx, r13                         ; 4c 89 e9                             
   114 : add    rcx, #0x03                       ; 48 83 c1 03                          
   115 : neg    rcx                              ; 48 f7 d9                             
   116 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   117 : mov    rdx, r13                         ; 4c 89 ea                             
   118 : add    rdx, #0x03                       ; 48 83 c2 03                          
   119 : imul   rcx, rdx                         ; 48 0f af ca                          
   120 : cmp    [rsp+#0x20], rcx                 ; 48 39 8c 24 20 00 00 00              
   121 : jl     [125]                            ; 0f 8c 16 00 00 00                    
   122 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
   123 : jg     [125]                            ; 0f 8f 04 00 00 00                    
   124 : add    r12, #0x03                       ; 49 83 c4 03                          
   125 : mov    r13, [rsp+#0x28]                 ; 4c 8b ac 24 28 00 00 00              
   126 : mov    rcx, r13                         ; 4c 89 e9                             
   127 : add    rcx, [rsp+#0x38]                 ; 48 03 8c 24 38 00 00 00              
   128 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   129 : mov    rdx, r13                         ; 4c 89 ea                             
   130 : sub    rdx, #0xfffffffffffffffb         ; 48 83 ea fb                          
   131 : shl    rdx, #0x03                       ; 48 c1 e2 03                          
   132 : add    rcx, rdx                         ; 48 01 d1                             
   133 : mov    [rcx], r12                       ; 4c 89 21                             
   134 : add    [rsp+#0x30], #0x01               ; 48 81 84 24 30 00 00 00 01 00 00 00  
   135 : jmp    [15]                             ; e9 86 fd ff ff                       
   136 : add    [rsp+#0x20], #0x01               ; 48 81 84 24 20 00 00 00 01 00 00 00  
   137 : jmp    [5]                              ; e9 3b fd ff ff                       
   138 : mov    r12, [rsp+#0x40]                 ; 4c 8b a4 24 40 00 00 00              
   139 : mov    r13, [rsp+#0x48]                 ; 4c 8b ac 24 48 00 00 00              
   140 : add    rsp, #0x58                       ; 48 83 c4 58                          
   141 : ret                                     ; c3                                   
