triangle_types(i0, i1, i2)
     0 : sub  rsp, #0x18       ; 48 81 ec 18 00 00 00                 
     1 : mov  [rsp], rsi       ; 48 89 b4 24 00 00 00 00              
     2 : mov  [rsp+#0x08], r13 ; 4c 89 ac 24 08 00 00 00              
     3 : cmp  rdi, #0          ; 48 83 ff 00                          
     4 : jle  [9]              ; 0f 8e 1c 00 00 00                    
     5 : cmp  [rsp], #0        ; 48 81 bc 24 00 00 00 00 00 00 00 00  
     6 : jle  [9]              ; 0f 8e 0a 00 00 00                    
     7 : cmp  rdx, #0          ; 48 83 fa 00                          
     8 : jg   [11]             ; 0f 8f 08 00 00 00                    
     9 : xor  rax, rax         ; 48 31 c0                             
    10 : jmp  [104]            ; e9 cd 01 00 00                       
    11 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    12 : mov  rcx, r13         ; 4c 89 e9                             
    13 : add  rcx, rdx         ; 48 01 d1                             
    14 : cmp  rdi, rcx         ; 48 39 cf                             
    15 : jg   [24]             ; 0f 8f 28 00 00 00                    
    16 : mov  rcx, rdi         ; 48 89 f9                             
    17 : add  rcx, rdx         ; 48 01 d1                             
    18 : cmp  [rsp], rcx       ; 48 39 8c 24 00 00 00 00              
    19 : jg   [24]             ; 0f 8f 14 00 00 00                    
    20 : mov  rcx, rdi         ; 48 89 f9                             
    21 : add  rcx, [rsp]       ; 48 03 8c 24 00 00 00 00              
    22 : cmp  rdx, rcx         ; 48 39 ca                             
    23 : jle  [26]             ; 0f 8e 08 00 00 00                    
    24 : xor  rax, rax         ; 48 31 c0                             
    25 : jmp  [104]            ; e9 86 01 00 00                       
    26 : cmp  rdi, [rsp]       ; 48 3b bc 24 00 00 00 00              
    27 : jne  [32]             ; 0f 85 15 00 00 00                    
    28 : cmp  rdi, rdx         ; 48 39 d7                             
    29 : jne  [32]             ; 0f 85 0c 00 00 00                    
    30 : mov  rax, #0x02       ; 48 c7 c0 02 00 00 00                 
    31 : jmp  [104]            ; e9 63 01 00 00                       
    32 : cmp  rdi, [rsp]       ; 48 3b bc 24 00 00 00 00              
    33 : je   [38]             ; 0f 84 17 00 00 00                    
    34 : cmp  rdi, rdx         ; 48 39 d7                             
    35 : je   [38]             ; 0f 84 0e 00 00 00                    
    36 : cmp  [rsp], rdx       ; 48 39 94 24 00 00 00 00              
    37 : jne  [40]             ; 0f 85 0c 00 00 00                    
    38 : mov  rax, #0x03       ; 48 c7 c0 03 00 00 00                 
    39 : jmp  [104]            ; e9 32 01 00 00                       
    40 : mov  rcx, rdi         ; 48 89 f9                             
    41 : imul rcx, rdi         ; 48 0f af cf                          
    42 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    43 : mov  rax, r13         ; 4c 89 e8                             
    44 : imul rax, [rsp]       ; 48 0f af 84 24 00 00 00 00           
    45 : mov  rsi, rdx         ; 48 89 d6                             
    46 : imul rsi, rdx         ; 48 0f af f2                          
    47 : add  rax, rsi         ; 48 01 f0                             
    48 : cmp  rcx, rax         ; 48 39 c1                             
    49 : je   [70]             ; 0f 84 5c 00 00 00                    
    50 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    51 : mov  rsi, r13         ; 4c 89 ee                             
    52 : imul rsi, [rsp]       ; 48 0f af b4 24 00 00 00 00           
    53 : mov  rcx, rdi         ; 48 89 f9                             
    54 : imul rcx, rdi         ; 48 0f af cf                          
    55 : mov  rax, rdx         ; 48 89 d0                             
    56 : imul rax, rdx         ; 48 0f af c2                          
    57 : add  rcx, rax         ; 48 01 c1                             
    58 : cmp  rsi, rcx         ; 48 39 ce                             
    59 : je   [70]             ; 0f 84 2e 00 00 00                    
    60 : mov  rsi, rdx         ; 48 89 d6                             
    61 : imul rsi, rdx         ; 48 0f af f2                          
    62 : mov  rcx, rdi         ; 48 89 f9                             
    63 : imul rcx, rdi         ; 48 0f af cf                          
    64 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    65 : mov  rax, r13         ; 4c 89 e8                             
    66 : imul rax, [rsp]       ; 48 0f af 84 24 00 00 00 00           
    67 : add  rcx, rax         ; 48 01 c1                             
    68 : cmp  rsi, rcx         ; 48 39 ce                             
    69 : jne  [72]             ; 0f 85 0c 00 00 00                    
    70 : mov  rax, #0x01       ; 48 c7 c0 01 00 00 00                 
    71 : jmp  [104]            ; e9 9c 00 00 00                       
    72 : mov  rsi, rdi         ; 48 89 fe                             
    73 : imul rsi, rdi         ; 48 0f af f7                          
    74 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    75 : mov  rcx, r13         ; 4c 89 e9                             
    76 : imul rcx, [rsp]       ; 48 0f af 8c 24 00 00 00 00           
    77 : mov  rax, rdx         ; 48 89 d0                             
    78 : imul rax, rdx         ; 48 0f af c2                          
    79 : add  rcx, rax         ; 48 01 c1                             
    80 : cmp  rsi, rcx         ; 48 39 ce                             
    81 : jg   [100]            ; 0f 8f 56 00 00 00                    
    82 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    83 : mov  rsi, r13         ; 4c 89 ee                             
    84 : imul rsi, [rsp]       ; 48 0f af b4 24 00 00 00 00           
    85 : mov  rcx, rdi         ; 48 89 f9                             
    86 : imul rcx, rdi         ; 48 0f af cf                          
    87 : mov  rax, rdx         ; 48 89 d0                             
    88 : imul rax, rdx         ; 48 0f af c2                          
    89 : add  rcx, rax         ; 48 01 c1                             
    90 : cmp  rsi, rcx         ; 48 39 ce                             
    91 : jg   [100]            ; 0f 8f 28 00 00 00                    
    92 : imul rdx, rdx         ; 48 0f af d2                          
    93 : imul rdi, rdi         ; 48 0f af ff                          
    94 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    95 : mov  rsi, r13         ; 4c 89 ee                             
    96 : imul rsi, [rsp]       ; 48 0f af b4 24 00 00 00 00           
    97 : add  rdi, rsi         ; 48 01 f7                             
    98 : cmp  rdx, rdi         ; 48 39 fa                             
    99 : jle  [102]            ; 0f 8e 0c 00 00 00                    
   100 : mov  rax, #0x05       ; 48 c7 c0 05 00 00 00                 
   101 : jmp  [104]            ; e9 0c 00 00 00                       
   102 : mov  rax, #0x04       ; 48 c7 c0 04 00 00 00                 
   103 : jmp  [104]            ; e9 00 00 00 00                       
   104 : mov  r13, [rsp+#0x08] ; 4c 8b ac 24 08 00 00 00              
   105 : add  rsp, #0x18       ; 48 81 c4 18 00 00 00                 
   106 : ret                   ; c3                                   
