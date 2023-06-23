triangle_types(i0, i1, i2)
     0 : sub  rsp, #0x08 ; 48 81 ec 08 00 00 00     
     1 : mov  [rsp], r12 ; 4c 89 a4 24 00 00 00 00  
     2 : cmp  rdi, #0    ; 48 83 ff 00              
     3 : jle  [8]        ; 0f 8e 14 00 00 00        
     4 : cmp  rsi, #0    ; 48 83 fe 00              
     5 : jle  [8]        ; 0f 8e 0a 00 00 00        
     6 : cmp  rdx, #0    ; 48 83 fa 00              
     7 : jg   [10]       ; 0f 8f 08 00 00 00        
     8 : xor  rax, rax   ; 48 31 c0                 
     9 : jmp  [95]       ; e9 5b 01 00 00           
    10 : mov  rcx, rsi   ; 48 89 f1                 
    11 : add  rcx, rdx   ; 48 01 d1                 
    12 : cmp  rdi, rcx   ; 48 39 cf                 
    13 : jg   [22]       ; 0f 8f 1e 00 00 00        
    14 : mov  rcx, rdi   ; 48 89 f9                 
    15 : add  rcx, rdx   ; 48 01 d1                 
    16 : cmp  rsi, rcx   ; 48 39 ce                 
    17 : jg   [22]       ; 0f 8f 0f 00 00 00        
    18 : mov  rcx, rdi   ; 48 89 f9                 
    19 : add  rcx, rsi   ; 48 01 f1                 
    20 : cmp  rdx, rcx   ; 48 39 ca                 
    21 : jle  [24]       ; 0f 8e 08 00 00 00        
    22 : xor  rax, rax   ; 48 31 c0                 
    23 : jmp  [95]       ; e9 26 01 00 00           
    24 : cmp  rdi, rsi   ; 48 39 f7                 
    25 : jne  [30]       ; 0f 85 15 00 00 00        
    26 : cmp  rdi, rdx   ; 48 39 d7                 
    27 : jne  [30]       ; 0f 85 0c 00 00 00        
    28 : mov  rax, #0x02 ; 48 c7 c0 02 00 00 00     
    29 : jmp  [95]       ; e9 08 01 00 00           
    30 : cmp  rdi, rsi   ; 48 39 f7                 
    31 : je   [36]       ; 0f 84 12 00 00 00        
    32 : cmp  rdi, rdx   ; 48 39 d7                 
    33 : je   [36]       ; 0f 84 09 00 00 00        
    34 : cmp  rsi, rdx   ; 48 39 d6                 
    35 : jne  [38]       ; 0f 85 0c 00 00 00        
    36 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00     
    37 : jmp  [95]       ; e9 e1 00 00 00           
    38 : mov  rcx, rdi   ; 48 89 f9                 
    39 : imul rcx, rdi   ; 48 0f af cf              
    40 : mov  rax, rsi   ; 48 89 f0                 
    41 : imul rax, rsi   ; 48 0f af c6              
    42 : mov  r12, rdx   ; 49 89 d4                 
    43 : imul r12, rdx   ; 4c 0f af e2              
    44 : add  rax, r12   ; 4c 01 e0                 
    45 : cmp  rcx, rax   ; 48 39 c1                 
    46 : je   [65]       ; 0f 84 42 00 00 00        
    47 : mov  rcx, rsi   ; 48 89 f1                 
    48 : imul rcx, rsi   ; 48 0f af ce              
    49 : mov  rax, rdi   ; 48 89 f8                 
    50 : imul rax, rdi   ; 48 0f af c7              
    51 : mov  r12, rdx   ; 49 89 d4                 
    52 : imul r12, rdx   ; 4c 0f af e2              
    53 : add  rax, r12   ; 4c 01 e0                 
    54 : cmp  rcx, rax   ; 48 39 c1                 
    55 : je   [65]       ; 0f 84 21 00 00 00        
    56 : mov  rcx, rdx   ; 48 89 d1                 
    57 : imul rcx, rdx   ; 48 0f af ca              
    58 : mov  rax, rdi   ; 48 89 f8                 
    59 : imul rax, rdi   ; 48 0f af c7              
    60 : mov  r12, rsi   ; 49 89 f4                 
    61 : imul r12, rsi   ; 4c 0f af e6              
    62 : add  rax, r12   ; 4c 01 e0                 
    63 : cmp  rcx, rax   ; 48 39 c1                 
    64 : jne  [67]       ; 0f 85 0c 00 00 00        
    65 : mov  rax, #0x01 ; 48 c7 c0 01 00 00 00     
    66 : jmp  [95]       ; e9 72 00 00 00           
    67 : mov  rcx, rdi   ; 48 89 f9                 
    68 : imul rcx, rdi   ; 48 0f af cf              
    69 : mov  rax, rsi   ; 48 89 f0                 
    70 : imul rax, rsi   ; 48 0f af c6              
    71 : mov  r12, rdx   ; 49 89 d4                 
    72 : imul r12, rdx   ; 4c 0f af e2              
    73 : add  rax, r12   ; 4c 01 e0                 
    74 : cmp  rcx, rax   ; 48 39 c1                 
    75 : jg   [91]       ; 0f 8f 39 00 00 00        
    76 : mov  rcx, rsi   ; 48 89 f1                 
    77 : imul rcx, rsi   ; 48 0f af ce              
    78 : mov  rax, rdi   ; 48 89 f8                 
    79 : imul rax, rdi   ; 48 0f af c7              
    80 : mov  r12, rdx   ; 49 89 d4                 
    81 : imul r12, rdx   ; 4c 0f af e2              
    82 : add  rax, r12   ; 4c 01 e0                 
    83 : cmp  rcx, rax   ; 48 39 c1                 
    84 : jg   [91]       ; 0f 8f 18 00 00 00        
    85 : imul rdx, rdx   ; 48 0f af d2              
    86 : imul rdi, rdi   ; 48 0f af ff              
    87 : imul rsi, rsi   ; 48 0f af f6              
    88 : add  rdi, rsi   ; 48 01 f7                 
    89 : cmp  rdx, rdi   ; 48 39 fa                 
    90 : jle  [93]       ; 0f 8e 0c 00 00 00        
    91 : mov  rax, #0x05 ; 48 c7 c0 05 00 00 00     
    92 : jmp  [95]       ; e9 0c 00 00 00           
    93 : mov  rax, #0x04 ; 48 c7 c0 04 00 00 00     
    94 : jmp  [95]       ; e9 00 00 00 00           
    95 : mov  r12, [rsp] ; 4c 8b a4 24 00 00 00 00  
    96 : add  rsp, #0x08 ; 48 81 c4 08 00 00 00     
    97 : ret             ; c3                       
