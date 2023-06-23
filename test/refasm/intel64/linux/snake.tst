snake(i0, i1, i2)
     0 : sub   rsp, #0xb8           ; 48 81 ec b8 00 00 00                 
     1 : mov   [rsp+#0x90], rsi     ; 48 89 b4 24 90 00 00 00              
     2 : mov   [rsp+#0x88], rdx     ; 48 89 94 24 88 00 00 00              
     3 : mov   [rsp+#0x98], r12     ; 4c 89 a4 24 98 00 00 00              
     4 : mov   [rsp+#0xa0], r13     ; 4c 89 ac 24 a0 00 00 00              
     5 : mov   [rsp+#0xb0], rbp     ; 48 89 ac 24 b0 00 00 00              
     6 : mov   rbp, rsp             ; 48 89 e5                             
     7 : add   rbp, #0xb0           ; 48 81 c5 b0 00 00 00                 
     8 : mov   r13, [rsp+#0x90]     ; 4c 8b ac 24 90 00 00 00              
     9 : mov   rcx, r13             ; 4c 89 e9                             
    10 : add   rcx, [rsp+#0x88]     ; 48 03 8c 24 88 00 00 00              
    11 : mov   [rsp+#0x80], rcx     ; 48 89 8c 24 80 00 00 00              
    12 : sub   [rsp+#0x80], #0x01   ; 48 81 ac 24 80 00 00 00 01 00 00 00  
    13 : mov   [rsp+#0x78], #0      ; 48 c7 84 24 78 00 00 00 00 00 00 00  
    14 : mov   [rsp+#0x70], #0x01   ; 48 c7 84 24 70 00 00 00 01 00 00 00  
    15 : mov   r13, [rsp+#0x70]     ; 4c 8b ac 24 70 00 00 00              
    16 : neg   r13                  ; 49 f7 dd                             
    17 : mov   [rsp+#0x60], r13     ; 4c 89 ac 24 60 00 00 00              
    18 : mov   [rsp+#0x68], #0      ; 48 c7 84 24 68 00 00 00 00 00 00 00  
    19 : mov   r13, [rsp+#0x80]     ; 4c 8b ac 24 80 00 00 00              
    20 : cmp   [rsp+#0x68], r13     ; 4c 39 ac 24 68 00 00 00              
    21 : jge   [102]                ; 0f 8d e7 01 00 00                    
    22 : xor   r12, r12             ; 4d 31 e4                             
    23 : xor   rax, rax             ; 48 31 c0                             
    24 : mov   r13, [rsp+#0x68]     ; 4c 8b ac 24 68 00 00 00              
    25 : mov   rcx, r13             ; 4c 89 e9                             
    26 : and   rcx, #0x01           ; 48 81 e1 01 00 00 00                 
    27 : cmp   rcx, #0              ; 48 83 f9 00                          
    28 : je    [44]                 ; 0f 84 49 00 00 00                    
    29 : mov   r13, [rsp+#0x88]     ; 4c 8b ac 24 88 00 00 00              
    30 : mov   rcx, r13             ; 4c 89 e9                             
    31 : sub   rcx, #0x01           ; 48 81 e9 01 00 00 00                 
    32 : mov   r13, [rsp+#0x68]     ; 4c 8b ac 24 68 00 00 00              
    33 : mov   r12, r13             ; 4d 89 ec                             
    34 : cmp   r12, rcx             ; 49 39 cc                             
    35 : cmovg r12, rcx             ; 4c 0f 4f e1                          
    36 : mov   r13, [rsp+#0x68]     ; 4c 8b ac 24 68 00 00 00              
    37 : mov   rdx, r13             ; 4c 89 ea                             
    38 : sub   rdx, rcx             ; 48 29 ca                             
    39 : xor   rsi, rsi             ; 48 31 f6                             
    40 : cmp   [rsp+#0x68], rcx     ; 48 39 8c 24 68 00 00 00              
    41 : mov   rax, rsi             ; 48 89 f0                             
    42 : cmovg rax, rdx             ; 48 0f 4f c2                          
    43 : jmp   [58]                 ; e9 44 00 00 00                       
    44 : mov   r13, [rsp+#0x90]     ; 4c 8b ac 24 90 00 00 00              
    45 : mov   rsi, r13             ; 4c 89 ee                             
    46 : sub   rsi, #0x01           ; 48 81 ee 01 00 00 00                 
    47 : mov   r13, [rsp+#0x68]     ; 4c 8b ac 24 68 00 00 00              
    48 : mov   rdx, r13             ; 4c 89 ea                             
    49 : sub   rdx, rsi             ; 48 29 f2                             
    50 : xor   rcx, rcx             ; 48 31 c9                             
    51 : cmp   [rsp+#0x68], rsi     ; 48 39 b4 24 68 00 00 00              
    52 : mov   r12, rcx             ; 49 89 cc                             
    53 : cmovg r12, rdx             ; 4c 0f 4f e2                          
    54 : mov   r13, [rsp+#0x68]     ; 4c 8b ac 24 68 00 00 00              
    55 : mov   rax, r13             ; 4c 89 e8                             
    56 : cmp   rax, rsi             ; 48 39 f0                             
    57 : cmovg rax, rsi             ; 48 0f 4f c6                          
    58 : cmp   r12, #0              ; 49 83 fc 00                          
    59 : jl    [98]                 ; 0f 8c 0d 01 00 00                    
    60 : cmp   r12, [rsp+#0x88]     ; 4c 3b a4 24 88 00 00 00              
    61 : jge   [98]                 ; 0f 8d ff 00 00 00                    
    62 : cmp   rax, #0              ; 48 83 f8 00                          
    63 : jl    [98]                 ; 0f 8c f5 00 00 00                    
    64 : cmp   rax, [rsp+#0x90]     ; 48 3b 84 24 90 00 00 00              
    65 : jge   [98]                 ; 0f 8d e7 00 00 00                    
    66 : mov   rsi, #0x563e883e2ec7 ; 48 be c7 2e 3e 88 3e 56 00 00        
    67 : mov   [rsp], rax           ; 48 89 84 24 00 00 00 00              
    68 : mov   [rsp+#0x08], rcx     ; 48 89 8c 24 08 00 00 00              
    69 : mov   [rsp+#0x10], rdx     ; 48 89 94 24 10 00 00 00              
    70 : mov   [rsp+#0x18], rsi     ; 48 89 b4 24 18 00 00 00              
    71 : mov   [rsp+#0x20], rdi     ; 48 89 bc 24 20 00 00 00              
    72 : mov   [rsp+#0x28], r8      ; 4c 89 84 24 28 00 00 00              
    73 : mov   [rsp+#0x30], r9      ; 4c 89 8c 24 30 00 00 00              
    74 : mov   [rsp+#0x38], r10     ; 4c 89 94 24 38 00 00 00              
    75 : mov   [rsp+#0x40], r11     ; 4c 89 9c 24 40 00 00 00              
    76 : mov   rdi, r12             ; 4c 89 e7                             
    77 : mov   rsi, rax             ; 48 89 c6                             
    78 : mov   r10, [rsp+#0x18]     ; 4c 8b 94 24 18 00 00 00              
    79 : call  [r10]                ; 41 ff d2                             
    80 : mov   rax, [rsp]           ; 48 8b 84 24 00 00 00 00              
    81 : mov   rcx, [rsp+#0x08]     ; 48 8b 8c 24 08 00 00 00              
    82 : mov   rdx, [rsp+#0x10]     ; 48 8b 94 24 10 00 00 00              
    83 : mov   rsi, [rsp+#0x18]     ; 48 8b b4 24 18 00 00 00              
    84 : mov   rdi, [rsp+#0x20]     ; 48 8b bc 24 20 00 00 00              
    85 : mov   r8, [rsp+#0x28]      ; 4c 8b 84 24 28 00 00 00              
    86 : mov   r9, [rsp+#0x30]      ; 4c 8b 8c 24 30 00 00 00              
    87 : mov   r10, [rsp+#0x38]     ; 4c 8b 94 24 38 00 00 00              
    88 : mov   r11, [rsp+#0x40]     ; 4c 8b 9c 24 40 00 00 00              
    89 : mov   rsi, rax             ; 48 89 c6                             
    90 : imul  rsi, [rsp+#0x88]     ; 48 0f af b4 24 88 00 00 00           
    91 : add   rsi, r12             ; 4c 01 e6                             
    92 : mov   r13, [rsp+#0x78]     ; 4c 8b ac 24 78 00 00 00              
    93 : mov   [rdi], [rsi], r13b   ; 44 88 2c 37                          
    94 : add   [rsp+#0x78], #0x01   ; 48 81 84 24 78 00 00 00 01 00 00 00  
    95 : add   r12, [rsp+#0x70]     ; 4c 03 a4 24 70 00 00 00              
    96 : add   rax, [rsp+#0x60]     ; 48 03 84 24 60 00 00 00              
    97 : jmp   [58]                 ; e9 e9 fe ff ff                       
    98 : neg   [rsp+#0x70]          ; 48 f7 9c 24 70 00 00 00              
    99 : neg   [rsp+#0x60]          ; 48 f7 9c 24 60 00 00 00              
   100 : add   [rsp+#0x68], #0x01   ; 48 81 84 24 68 00 00 00 01 00 00 00  
   101 : jmp   [19]                 ; e9 03 fe ff ff                       
   102 : mov   rbp, [rsp+#0xb0]     ; 48 8b ac 24 b0 00 00 00              
   103 : mov   r12, [rsp+#0x98]     ; 4c 8b a4 24 98 00 00 00              
   104 : mov   r13, [rsp+#0xa0]     ; 4c 8b ac 24 a0 00 00 00              
   105 : add   rsp, #0xb8           ; 48 81 c4 b8 00 00 00                 
   106 : ret                        ; c3                                   
