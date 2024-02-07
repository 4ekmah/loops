conditionpainter(i0)
     0 : sub    rsp, #0x38                       ; 48 81 ec 38 00 00 00                 
     1 : mov    [rsp+#0x08], rcx                 ; 48 89 8c 24 08 00 00 00              
     2 : mov    [rsp+#0x28], r13                 ; 4c 89 ac 24 28 00 00 00              
     3 : mov    r13, #0xfffffffffffffffb         ; 49 c7 c5 fb ff ff ff                 
     4 : mov    [rsp], r13                       ; 4c 89 ac 24 00 00 00 00              
     5 : cmp    [rsp], #0x05                     ; 48 81 bc 24 00 00 00 00 05 00 00 00  
     6 : jg     [141]                            ; 0f 8f f5 02 00 00                    
     7 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
     8 : mov    r8, r13                          ; 4d 89 e8                             
     9 : add    r8, #0x05                        ; 49 81 c0 05 00 00 00                 
    10 : imul   r8, #0x0b                        ; 4d 6b c0 0b                          
    11 : mov    r13, r8                          ; 4d 89 c5                             
    12 : imul   r13, #0x08                       ; 4d 6b ed 08                          
    13 : mov    [rsp+#0x18], r13                 ; 4c 89 ac 24 18 00 00 00              
    14 : mov    r13, #0xfffffffffffffffb         ; 49 c7 c5 fb ff ff ff                 
    15 : mov    [rsp+#0x10], r13                 ; 4c 89 ac 24 10 00 00 00              
    16 : cmp    [rsp+#0x10], #0x05               ; 48 81 bc 24 10 00 00 00 05 00 00 00  
    17 : jg     [139]                            ; 0f 8f 9e 02 00 00                    
    18 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    19 : mov    rax, r13                         ; 4c 89 e8                             
    20 : add    rax, #0x03                       ; 48 05 03 00 00 00                    
    21 : xor    rdx, rdx                         ; 48 31 d2                             
    22 : cmp    [rsp], rax                       ; 48 39 84 24 00 00 00 00              
    23 : setge  rdx                              ; 0f 9d c2                             
    24 : xor    rax, rax                         ; 48 31 c0                             
    25 : cmp    [rsp], #0x04                     ; 48 81 bc 24 00 00 00 00 04 00 00 00  
    26 : setle  rax                              ; 0f 9e c0                             
    27 : and    rdx, rax                         ; 48 21 c2                             
    28 : xor    rax, rax                         ; 48 31 c0                             
    29 : cmp    [rsp+#0x10], #0xfffffffffffffffe ; 48 81 bc 24 10 00 00 00 fe ff ff ff  
    30 : setge  rax                              ; 0f 9d c0                             
    31 : and    rdx, rax                         ; 48 21 c2                             
    32 : xor    rax, rax                         ; 48 31 c0                             
    33 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
    34 : setle  rax                              ; 0f 9e c0                             
    35 : and    rdx, rax                         ; 48 21 c2                             
    36 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    37 : mov    rax, r13                         ; 4c 89 e8                             
    38 : sub    rax, #0x01                       ; 48 2d 01 00 00 00                    
    39 : xor    rcx, rcx                         ; 48 31 c9                             
    40 : cmp    [rsp], rax                       ; 48 39 84 24 00 00 00 00              
    41 : setle  rcx                              ; 0f 9e c1                             
    42 : xor    rax, rax                         ; 48 31 c0                             
    43 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
    44 : setge  rax                              ; 0f 9d c0                             
    45 : and    rcx, rax                         ; 48 21 c1                             
    46 : xor    rax, rax                         ; 48 31 c0                             
    47 : cmp    [rsp], #0                        ; 48 81 bc 24 00 00 00 00 00 00 00 00  
    48 : setle  rax                              ; 0f 9e c0                             
    49 : and    rcx, rax                         ; 48 21 c1                             
    50 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    51 : mov    rax, r13                         ; 4c 89 e8                             
    52 : imul   rax, [rsp+#0x10]                 ; 48 0f af 84 24 10 00 00 00           
    53 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    54 : mov    r9, r13                          ; 4d 89 e9                             
    55 : imul   r9, [rsp]                        ; 4c 0f af 8c 24 00 00 00 00           
    56 : add    rax, r9                          ; 4c 01 c8                             
    57 : xor    r9, r9                           ; 4d 31 c9                             
    58 : cmp    rax, #0x09                       ; 48 83 f8 09                          
    59 : setle  r9                               ; 41 0f 9e c1                          
    60 : and    rcx, r9                          ; 4c 21 c9                             
    61 : mov    [rsp+#0x20], rdx                 ; 48 89 94 24 20 00 00 00              
    62 : or     [rsp+#0x20], rcx                 ; 48 09 8c 24 20 00 00 00              
    63 : mov    rcx, #0x02                       ; 48 c7 c1 02 00 00 00                 
    64 : xor    r9, r9                           ; 4d 31 c9                             
    65 : xor    rax, rax                         ; 48 31 c0                             
    66 : cmp    [rsp+#0x10], #0x02               ; 48 81 bc 24 10 00 00 00 02 00 00 00  
    67 : setge  rax                              ; 0f 9d c0                             
    68 : xor    r8, r8                           ; 4d 31 c0                             
    69 : cmp    [rsp+#0x10], #0x04               ; 48 81 bc 24 10 00 00 00 04 00 00 00  
    70 : setle  r8                               ; 41 0f 9e c0                          
    71 : and    rax, r8                          ; 4c 21 c0                             
    72 : xor    r8, r8                           ; 4d 31 c0                             
    73 : cmp    [rsp], #0x01                     ; 48 81 bc 24 00 00 00 00 01 00 00 00  
    74 : setge  r8                               ; 41 0f 9d c0                          
    75 : xor    rdx, rdx                         ; 48 31 d2                             
    76 : cmp    [rsp], #0x03                     ; 48 81 bc 24 00 00 00 00 03 00 00 00  
    77 : setle  rdx                              ; 0f 9e c2                             
    78 : and    r8, rdx                          ; 49 21 d0                             
    79 : or     rax, r8                          ; 4c 09 c0                             
    80 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
    81 : mov    rdx, r13                         ; 4c 89 ea                             
    82 : imul   rdx, [rsp+#0x10]                 ; 48 0f af 94 24 10 00 00 00           
    83 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    84 : mov    r8, r13                          ; 4d 89 e8                             
    85 : imul   r8, [rsp]                        ; 4c 0f af 84 24 00 00 00 00           
    86 : add    rdx, r8                          ; 4c 01 c2                             
    87 : xor    r8, r8                           ; 4d 31 c0                             
    88 : cmp    rdx, #0x10                       ; 48 83 fa 10                          
    89 : setle  r8                               ; 41 0f 9e c0                          
    90 : and    rax, r8                          ; 4c 21 c0                             
    91 : cmp    rax, #0                          ; 48 83 f8 00                          
    92 : cmovne r9, rcx                          ; 4c 0f 45 c9                          
    93 : mov    r13, [rsp+#0x20]                 ; 4c 8b ac 24 20 00 00 00              
    94 : add    r9, r13                          ; 4d 01 e9                             
    95 : mov    r13, [rsp]                       ; 4c 8b ac 24 00 00 00 00              
    96 : mov    rcx, r13                         ; 4c 89 e9                             
    97 : imul   rcx, #0x02                       ; 48 6b c9 02                          
    98 : cmp    rcx, [rsp+#0x10]                 ; 48 3b 8c 24 10 00 00 00              
    99 : jl     [110]                            ; 0f 8c 39 00 00 00                    
   100 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   101 : mov    rcx, r13                         ; 4c 89 e9                             
   102 : add    rcx, #0x03                       ; 48 81 c1 03 00 00 00                 
   103 : neg    rcx                              ; 48 f7 d9                             
   104 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   105 : mov    rdx, r13                         ; 4c 89 ea                             
   106 : add    rdx, #0x03                       ; 48 81 c2 03 00 00 00                 
   107 : imul   rcx, rdx                         ; 48 0f af ca                          
   108 : cmp    [rsp], rcx                       ; 48 39 8c 24 00 00 00 00              
   109 : jle    [125]                            ; 0f 8e 56 00 00 00                    
   110 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   111 : mov    rcx, r13                         ; 4c 89 e9                             
   112 : imul   rcx, #0x02                       ; 48 6b c9 02                          
   113 : cmp    [rsp], rcx                       ; 48 39 8c 24 00 00 00 00              
   114 : jg     [128]                            ; 0f 8f 52 00 00 00                    
   115 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   116 : mov    rcx, r13                         ; 4c 89 e9                             
   117 : add    rcx, #0x03                       ; 48 81 c1 03 00 00 00                 
   118 : neg    rcx                              ; 48 f7 d9                             
   119 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   120 : mov    rdx, r13                         ; 4c 89 ea                             
   121 : add    rdx, #0x03                       ; 48 81 c2 03 00 00 00                 
   122 : imul   rcx, rdx                         ; 48 0f af ca                          
   123 : cmp    [rsp], rcx                       ; 48 39 8c 24 00 00 00 00              
   124 : jl     [128]                            ; 0f 8c 19 00 00 00                    
   125 : cmp    [rsp+#0x10], #0                  ; 48 81 bc 24 10 00 00 00 00 00 00 00  
   126 : jg     [128]                            ; 0f 8f 07 00 00 00                    
   127 : add    r9, #0x03                        ; 49 81 c1 03 00 00 00                 
   128 : mov    r13, [rsp+#0x08]                 ; 4c 8b ac 24 08 00 00 00              
   129 : mov    rcx, r13                         ; 4c 89 e9                             
   130 : add    rcx, [rsp+#0x18]                 ; 48 03 8c 24 18 00 00 00              
   131 : mov    r13, [rsp+#0x10]                 ; 4c 8b ac 24 10 00 00 00              
   132 : mov    rdx, r13                         ; 4c 89 ea                             
   133 : add    rdx, #0x05                       ; 48 81 c2 05 00 00 00                 
   134 : shl    rdx, #0x03                       ; 48 c1 e2 03                          
   135 : add    rcx, rdx                         ; 48 01 d1                             
   136 : mov    [rcx], r9                        ; 4c 89 09                             
   137 : add    [rsp+#0x10], #0x01               ; 48 81 84 24 10 00 00 00 01 00 00 00  
   138 : jmp    [16]                             ; e9 50 fd ff ff                       
   139 : add    [rsp], #0x01                     ; 48 81 84 24 00 00 00 00 01 00 00 00  
   140 : jmp    [5]                              ; e9 f9 fc ff ff                       
   141 : mov    r13, [rsp+#0x28]                 ; 4c 8b ac 24 28 00 00 00              
   142 : add    rsp, #0x38                       ; 48 81 c4 38 00 00 00                 
   143 : ret                                     ; c3                                   
