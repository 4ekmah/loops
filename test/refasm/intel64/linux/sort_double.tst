sort_double(i0, i1)
     0 : sub  rsp, #0x98           ; 48 81 ec 98 00 00 00                 
     1 : mov  [rsp+#0x78], r12     ; 4c 89 a4 24 78 00 00 00              
     2 : mov  [rsp+#0x80], r13     ; 4c 89 ac 24 80 00 00 00              
     3 : mov  [rsp+#0x90], rbp     ; 48 89 ac 24 90 00 00 00              
     4 : mov  rbp, rsp             ; 48 89 e5                             
     5 : add  rbp, #0x90           ; 48 81 c5 90 00 00 00                 
     6 : mov  [rsp+#0x70], rsi     ; 48 89 b4 24 70 00 00 00              
     7 : shl  [rsp+#0x70], #0x03   ; 48 c1 a4 24 70 00 00 00 03           
     8 : mov  r13, [rsp+#0x70]     ; 4c 8b ac 24 70 00 00 00              
     9 : mov  [rsp+#0x68], r13     ; 4c 89 ac 24 68 00 00 00              
    10 : sub  [rsp+#0x68], #0x08   ; 48 81 ac 24 68 00 00 00 08 00 00 00  
    11 : mov  [rsp+#0x60], #0      ; 48 c7 84 24 60 00 00 00 00 00 00 00  
    12 : mov  r13, [rsp+#0x68]     ; 4c 8b ac 24 68 00 00 00              
    13 : cmp  [rsp+#0x60], r13     ; 4c 39 ac 24 60 00 00 00              
    14 : jge  [60]                 ; 0f 8d 26 01 00 00                    
    15 : mov  rax, [rsp+#0x60]     ; 48 8b 84 24 60 00 00 00              
    16 : mov  r12, rax             ; 49 89 c4                             
    17 : add  r12, #0x08           ; 49 81 c4 08 00 00 00                 
    18 : cmp  r12, [rsp+#0x70]     ; 4c 3b a4 24 70 00 00 00              
    19 : jge  [50]                 ; 0f 8d c7 00 00 00                    
    20 : mov  rcx, [rdi], [r12]    ; 4a 8b 0c 27                          
    21 : mov  rdx, [rdi], [rax]    ; 48 8b 14 07                          
    22 : mov  rsi, #0x563e883e332b ; 48 be 2b 33 3e 88 3e 56 00 00        
    23 : mov  [rsp], rax           ; 48 89 84 24 00 00 00 00              
    24 : mov  [rsp+#0x08], rcx     ; 48 89 8c 24 08 00 00 00              
    25 : mov  [rsp+#0x10], rdx     ; 48 89 94 24 10 00 00 00              
    26 : mov  [rsp+#0x18], rsi     ; 48 89 b4 24 18 00 00 00              
    27 : mov  [rsp+#0x20], rdi     ; 48 89 bc 24 20 00 00 00              
    28 : mov  [rsp+#0x28], r8      ; 4c 89 84 24 28 00 00 00              
    29 : mov  [rsp+#0x30], r9      ; 4c 89 8c 24 30 00 00 00              
    30 : mov  [rsp+#0x38], r10     ; 4c 89 94 24 38 00 00 00              
    31 : mov  [rsp+#0x40], r11     ; 4c 89 9c 24 40 00 00 00              
    32 : mov  rdi, rcx             ; 48 89 cf                             
    33 : mov  rsi, rdx             ; 48 89 d6                             
    34 : mov  r10, [rsp+#0x18]     ; 4c 8b 94 24 18 00 00 00              
    35 : call [r10]                ; 41 ff d2                             
    36 : mov  rsi, rax             ; 48 89 c6                             
    37 : mov  rax, [rsp]           ; 48 8b 84 24 00 00 00 00              
    38 : mov  rcx, [rsp+#0x08]     ; 48 8b 8c 24 08 00 00 00              
    39 : mov  rdx, [rsp+#0x10]     ; 48 8b 94 24 10 00 00 00              
    40 : mov  rdi, [rsp+#0x20]     ; 48 8b bc 24 20 00 00 00              
    41 : mov  r8, [rsp+#0x28]      ; 4c 8b 84 24 28 00 00 00              
    42 : mov  r9, [rsp+#0x30]      ; 4c 8b 8c 24 30 00 00 00              
    43 : mov  r10, [rsp+#0x38]     ; 4c 8b 94 24 38 00 00 00              
    44 : mov  r11, [rsp+#0x40]     ; 4c 8b 9c 24 40 00 00 00              
    45 : cmp  rsi, #0              ; 48 83 fe 00                          
    46 : je   [48]                 ; 0f 84 03 00 00 00                    
    47 : mov  rax, r12             ; 4c 89 e0                             
    48 : add  r12, #0x08           ; 49 81 c4 08 00 00 00                 
    49 : jmp  [18]                 ; e9 2b ff ff ff                       
    50 : cmp  rax, [rsp+#0x60]     ; 48 3b 84 24 60 00 00 00              
    51 : je   [58]                 ; 0f 84 20 00 00 00                    
    52 : mov  r13, [rsp+#0x60]     ; 4c 8b ac 24 60 00 00 00              
    53 : mov  rsi, [rdi], [r13]    ; 4a 8b 34 2f                          
    54 : mov  rdx, [rdi], [rax]    ; 48 8b 14 07                          
    55 : mov  [rdi], [rax], rsi    ; 48 89 34 07                          
    56 : mov  r13, [rsp+#0x60]     ; 4c 8b ac 24 60 00 00 00              
    57 : mov  [rdi], [r13], rdx    ; 4a 89 14 2f                          
    58 : add  [rsp+#0x60], #0x08   ; 48 81 84 24 60 00 00 00 08 00 00 00  
    59 : jmp  [12]                 ; e9 c4 fe ff ff                       
    60 : mov  rbp, [rsp+#0x90]     ; 48 8b ac 24 90 00 00 00              
    61 : mov  r12, [rsp+#0x78]     ; 4c 8b a4 24 78 00 00 00              
    62 : mov  r13, [rsp+#0x80]     ; 4c 8b ac 24 80 00 00 00              
    63 : add  rsp, #0x98           ; 48 81 c4 98 00 00 00                 
    64 : ret                       ; c3                                   
