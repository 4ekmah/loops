conditionpainter(i0)
     0 : sub    rsp, #0x58                       ; 48 83 ec 58                          
     1 : mov    [rsp+#0x28], rcx                 ; 48 89 4c 24 28                       
     2 : mov    [rsp+#0x40], r12                 ; 4c 89 64 24 40                       
     3 : mov    [rsp+#0x48], r13                 ; 4c 89 6c 24 48                       
     4 : mov    [rsp+#0x20], #0xfffffffffffffffb ; 48 c7 44 24 20 fb ff ff ff           
     5 : cmp    [rsp+#0x20], #0x05               ; 48 81 bc 24 20 00 00 00 05 00 00 00  
     6 : jg     [141]                            ; 0f 8f c2 02 00 00                    
     7 : mov    r8, #0xfffffffffffffffb          ; 49 c7 c0 fb ff ff ff                 
     8 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
     9 : sub    r8, r13                          ; 4d 29 e8                             
    10 : neg    r8                               ; 49 f7 d8                             
    11 : imul   r8, #0x0b                        ; 4d 6b c0 0b                          
    12 : mov    r13, r8                          ; 4d 89 c5                             
    13 : imul   r13, #0x08                       ; 4d 6b ed 08                          
    14 : mov    [rsp+#0x38], r13                 ; 4c 89 6c 24 38                       
    15 : mov    [rsp+#0x30], #0xfffffffffffffffb ; 48 c7 44 24 30 fb ff ff ff           
    16 : cmp    [rsp+#0x30], #0x05               ; 48 81 bc 24 30 00 00 00 05 00 00 00  
    17 : jg     [139]                            ; 0f 8f 71 02 00 00                    
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
    29 : xor    rdx, rdx                         ; 48 31 d2                             
    30 : cmp    [rsp+#0x30], rax                 ; 48 39 84 24 30 00 00 00              
    31 : setge  rdx                              ; 0f 9d c2                             
    32 : and    r12, rdx                         ; 49 21 d4                             
    33 : xor    rdx, rdx                         ; 48 31 d2                             
    34 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
    35 : setle  rdx                              ; 0f 9e c2                             
    36 : and    r12, rdx                         ; 49 21 d4                             
    37 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    38 : mov    rdx, r13                         ; 4c 89 ea                             
    39 : sub    rdx, #0x01                       ; 48 83 ea 01                          
    40 : xor    rax, rax                         ; 48 31 c0                             
    41 : cmp    [rsp+#0x20], rdx                 ; 48 39 94 24 20 00 00 00              
    42 : setle  rax                              ; 0f 9e c0                             
    43 : xor    rdx, rdx                         ; 48 31 d2                             
    44 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
    45 : setge  rdx                              ; 0f 9d c2                             
    46 : and    rax, rdx                         ; 48 21 d0                             
    47 : xor    rdx, rdx                         ; 48 31 d2                             
    48 : cmp    [rsp+#0x20], #0                  ; 48 81 bc 24 20 00 00 00 00 00 00 00  
    49 : setle  rdx                              ; 0f 9e c2                             
    50 : and    rax, rdx                         ; 48 21 d0                             
    51 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    52 : mov    rdx, r13                         ; 4c 89 ea                             
    53 : imul   rdx, [rsp+#0x30]                 ; 48 0f af 54 24 30                    
    54 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    55 : mov    rcx, r13                         ; 4c 89 e9                             
    56 : imul   rcx, [rsp+#0x20]                 ; 48 0f af 4c 24 20                    
    57 : add    rdx, rcx                         ; 48 01 ca                             
    58 : xor    rcx, rcx                         ; 48 31 c9                             
    59 : cmp    rdx, #0x09                       ; 48 83 fa 09                          
    60 : setle  rcx                              ; 0f 9e c1                             
    61 : and    rax, rcx                         ; 48 21 c8                             
    62 : or     r12, rax                         ; 49 09 c4                             
    63 : mov    rcx, #0x02                       ; 48 c7 c1 02 00 00 00                 
    64 : xor    rdx, rdx                         ; 48 31 d2                             
    65 : xor    rax, rax                         ; 48 31 c0                             
    66 : cmp    [rsp+#0x30], #0x02               ; 48 81 bc 24 30 00 00 00 02 00 00 00  
    67 : setge  rax                              ; 0f 9d c0                             
    68 : xor    r9, r9                           ; 4d 31 c9                             
    69 : cmp    [rsp+#0x30], #0x04               ; 48 81 bc 24 30 00 00 00 04 00 00 00  
    70 : setle  r9                               ; 41 0f 9e c1                          
    71 : and    rax, r9                          ; 4c 21 c8                             
    72 : xor    r9, r9                           ; 4d 31 c9                             
    73 : cmp    [rsp+#0x20], #0x01               ; 48 81 bc 24 20 00 00 00 01 00 00 00  
    74 : setge  r9                               ; 41 0f 9d c1                          
    75 : xor    r8, r8                           ; 4d 31 c0                             
    76 : cmp    [rsp+#0x20], #0x03               ; 48 81 bc 24 20 00 00 00 03 00 00 00  
    77 : setle  r8                               ; 41 0f 9e c0                          
    78 : and    r9, r8                           ; 4d 21 c1                             
    79 : or     rax, r9                          ; 4c 09 c8                             
    80 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
    81 : mov    r8, r13                          ; 4d 89 e8                             
    82 : imul   r8, [rsp+#0x30]                  ; 4c 0f af 44 24 30                    
    83 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    84 : mov    r9, r13                          ; 4d 89 e9                             
    85 : imul   r9, [rsp+#0x20]                  ; 4c 0f af 4c 24 20                    
    86 : add    r8, r9                           ; 4d 01 c8                             
    87 : xor    r9, r9                           ; 4d 31 c9                             
    88 : cmp    r8, #0x10                        ; 49 83 f8 10                          
    89 : setle  r9                               ; 41 0f 9e c1                          
    90 : and    rax, r9                          ; 4c 21 c8                             
    91 : cmp    rax, #0                          ; 48 83 f8 00                          
    92 : cmovne rdx, rcx                         ; 48 0f 45 d1                          
    93 : add    r12, rdx                         ; 49 01 d4                             
    94 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    95 : mov    rcx, r13                         ; 4c 89 e9                             
    96 : imul   rcx, #0x02                       ; 48 6b c9 02                          
    97 : cmp    rcx, [rsp+#0x30]                 ; 48 3b 8c 24 30 00 00 00              
    98 : jl     [109]                            ; 0f 8c 33 00 00 00                    
    99 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   100 : mov    rcx, r13                         ; 4c 89 e9                             
   101 : add    rcx, #0x03                       ; 48 83 c1 03                          
   102 : neg    rcx                              ; 48 f7 d9                             
   103 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   104 : mov    rdx, r13                         ; 4c 89 ea                             
   105 : add    rdx, #0x03                       ; 48 83 c2 03                          
   106 : imul   rcx, rdx                         ; 48 0f af ca                          
   107 : cmp    [rsp+#0x20], rcx                 ; 48 39 8c 24 20 00 00 00              
   108 : jle    [124]                            ; 0f 8e 50 00 00 00                    
   109 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   110 : mov    rcx, r13                         ; 4c 89 e9                             
   111 : imul   rcx, #0x02                       ; 48 6b c9 02                          
   112 : cmp    [rsp+#0x20], rcx                 ; 48 39 8c 24 20 00 00 00              
   113 : jg     [127]                            ; 0f 8f 49 00 00 00                    
   114 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   115 : mov    rcx, r13                         ; 4c 89 e9                             
   116 : add    rcx, #0x03                       ; 48 83 c1 03                          
   117 : neg    rcx                              ; 48 f7 d9                             
   118 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   119 : mov    rdx, r13                         ; 4c 89 ea                             
   120 : add    rdx, #0x03                       ; 48 83 c2 03                          
   121 : imul   rcx, rdx                         ; 48 0f af ca                          
   122 : cmp    [rsp+#0x20], rcx                 ; 48 39 8c 24 20 00 00 00              
   123 : jl     [127]                            ; 0f 8c 16 00 00 00                    
   124 : cmp    [rsp+#0x30], #0                  ; 48 81 bc 24 30 00 00 00 00 00 00 00  
   125 : jg     [127]                            ; 0f 8f 04 00 00 00                    
   126 : add    r12, #0x03                       ; 49 83 c4 03                          
   127 : mov    r13, [rsp+#0x28]                 ; 4c 8b ac 24 28 00 00 00              
   128 : mov    rcx, r13                         ; 4c 89 e9                             
   129 : add    rcx, [rsp+#0x38]                 ; 48 03 8c 24 38 00 00 00              
   130 : mov    rdx, #0xfffffffffffffffb         ; 48 c7 c2 fb ff ff ff                 
   131 : mov    r13, [rsp+#0x30]                 ; 4c 8b ac 24 30 00 00 00              
   132 : sub    rdx, r13                         ; 4c 29 ea                             
   133 : neg    rdx                              ; 48 f7 da                             
   134 : shl    rdx, #0x03                       ; 48 c1 e2 03                          
   135 : add    rcx, rdx                         ; 48 01 d1                             
   136 : mov    [rcx], r12                       ; 4c 89 21                             
   137 : add    [rsp+#0x30], #0x01               ; 48 81 84 24 30 00 00 00 01 00 00 00  
   138 : jmp    [16]                             ; e9 7d fd ff ff                       
   139 : add    [rsp+#0x20], #0x01               ; 48 81 84 24 20 00 00 00 01 00 00 00  
   140 : jmp    [5]                              ; e9 2c fd ff ff                       
   141 : mov    r12, [rsp+#0x40]                 ; 4c 8b a4 24 40 00 00 00              
   142 : mov    r13, [rsp+#0x48]                 ; 4c 8b ac 24 48 00 00 00              
   143 : add    rsp, #0x58                       ; 48 83 c4 58                          
   144 : ret                                     ; c3                                   
