triangle_types(i0, i1, i2)
     0 : sub  rsp, #0x18       ; 48 81 ec 18 00 00 00                 
     1 : mov  [rsp], rdx       ; 48 89 94 24 00 00 00 00              
     2 : mov  [rsp+#0x08], r13 ; 4c 89 ac 24 08 00 00 00              
     3 : cmp  rcx, #0          ; 48 83 f9 00                          
     4 : jle  [9]              ; 0f 8e 1c 00 00 00                    
     5 : cmp  [rsp], #0        ; 48 81 bc 24 00 00 00 00 00 00 00 00  
     6 : jle  [9]              ; 0f 8e 0a 00 00 00                    
     7 : cmp  r8, #0           ; 49 83 f8 00                          
     8 : jg   [11]             ; 0f 8f 08 00 00 00                    
     9 : xor  rax, rax         ; 48 31 c0                             
    10 : jmp  [104]            ; e9 cd 01 00 00                       
    11 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    12 : mov  r9, r13          ; 4d 89 e9                             
    13 : add  r9, r8           ; 4d 01 c1                             
    14 : cmp  rcx, r9          ; 4c 39 c9                             
    15 : jg   [24]             ; 0f 8f 28 00 00 00                    
    16 : mov  r9, rcx          ; 49 89 c9                             
    17 : add  r9, r8           ; 4d 01 c1                             
    18 : cmp  [rsp], r9        ; 4c 39 8c 24 00 00 00 00              
    19 : jg   [24]             ; 0f 8f 14 00 00 00                    
    20 : mov  r9, rcx          ; 49 89 c9                             
    21 : add  r9, [rsp]        ; 4c 03 8c 24 00 00 00 00              
    22 : cmp  r8, r9           ; 4d 39 c8                             
    23 : jle  [26]             ; 0f 8e 08 00 00 00                    
    24 : xor  rax, rax         ; 48 31 c0                             
    25 : jmp  [104]            ; e9 86 01 00 00                       
    26 : cmp  rcx, [rsp]       ; 48 3b 8c 24 00 00 00 00              
    27 : jne  [32]             ; 0f 85 15 00 00 00                    
    28 : cmp  rcx, r8          ; 4c 39 c1                             
    29 : jne  [32]             ; 0f 85 0c 00 00 00                    
    30 : mov  rax, #0x02       ; 48 c7 c0 02 00 00 00                 
    31 : jmp  [104]            ; e9 63 01 00 00                       
    32 : cmp  rcx, [rsp]       ; 48 3b 8c 24 00 00 00 00              
    33 : je   [38]             ; 0f 84 17 00 00 00                    
    34 : cmp  rcx, r8          ; 4c 39 c1                             
    35 : je   [38]             ; 0f 84 0e 00 00 00                    
    36 : cmp  [rsp], r8        ; 4c 39 84 24 00 00 00 00              
    37 : jne  [40]             ; 0f 85 0c 00 00 00                    
    38 : mov  rax, #0x03       ; 48 c7 c0 03 00 00 00                 
    39 : jmp  [104]            ; e9 32 01 00 00                       
    40 : mov  r9, rcx          ; 49 89 c9                             
    41 : imul r9, rcx          ; 4c 0f af c9                          
    42 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    43 : mov  rax, r13         ; 4c 89 e8                             
    44 : imul rax, [rsp]       ; 48 0f af 84 24 00 00 00 00           
    45 : mov  rdx, r8          ; 4c 89 c2                             
    46 : imul rdx, r8          ; 49 0f af d0                          
    47 : add  rax, rdx         ; 48 01 d0                             
    48 : cmp  r9, rax          ; 49 39 c1                             
    49 : je   [70]             ; 0f 84 5c 00 00 00                    
    50 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    51 : mov  rdx, r13         ; 4c 89 ea                             
    52 : imul rdx, [rsp]       ; 48 0f af 94 24 00 00 00 00           
    53 : mov  r9, rcx          ; 49 89 c9                             
    54 : imul r9, rcx          ; 4c 0f af c9                          
    55 : mov  rax, r8          ; 4c 89 c0                             
    56 : imul rax, r8          ; 49 0f af c0                          
    57 : add  r9, rax          ; 49 01 c1                             
    58 : cmp  rdx, r9          ; 4c 39 ca                             
    59 : je   [70]             ; 0f 84 2e 00 00 00                    
    60 : mov  rdx, r8          ; 4c 89 c2                             
    61 : imul rdx, r8          ; 49 0f af d0                          
    62 : mov  r9, rcx          ; 49 89 c9                             
    63 : imul r9, rcx          ; 4c 0f af c9                          
    64 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    65 : mov  rax, r13         ; 4c 89 e8                             
    66 : imul rax, [rsp]       ; 48 0f af 84 24 00 00 00 00           
    67 : add  r9, rax          ; 49 01 c1                             
    68 : cmp  rdx, r9          ; 4c 39 ca                             
    69 : jne  [72]             ; 0f 85 0c 00 00 00                    
    70 : mov  rax, #0x01       ; 48 c7 c0 01 00 00 00                 
    71 : jmp  [104]            ; e9 9c 00 00 00                       
    72 : mov  rdx, rcx         ; 48 89 ca                             
    73 : imul rdx, rcx         ; 48 0f af d1                          
    74 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    75 : mov  r9, r13          ; 4d 89 e9                             
    76 : imul r9, [rsp]        ; 4c 0f af 8c 24 00 00 00 00           
    77 : mov  rax, r8          ; 4c 89 c0                             
    78 : imul rax, r8          ; 49 0f af c0                          
    79 : add  r9, rax          ; 49 01 c1                             
    80 : cmp  rdx, r9          ; 4c 39 ca                             
    81 : jg   [100]            ; 0f 8f 56 00 00 00                    
    82 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    83 : mov  rdx, r13         ; 4c 89 ea                             
    84 : imul rdx, [rsp]       ; 48 0f af 94 24 00 00 00 00           
    85 : mov  r9, rcx          ; 49 89 c9                             
    86 : imul r9, rcx          ; 4c 0f af c9                          
    87 : mov  rax, r8          ; 4c 89 c0                             
    88 : imul rax, r8          ; 49 0f af c0                          
    89 : add  r9, rax          ; 49 01 c1                             
    90 : cmp  rdx, r9          ; 4c 39 ca                             
    91 : jg   [100]            ; 0f 8f 28 00 00 00                    
    92 : imul r8, r8           ; 4d 0f af c0                          
    93 : imul rcx, rcx         ; 48 0f af c9                          
    94 : mov  r13, [rsp]       ; 4c 8b ac 24 00 00 00 00              
    95 : mov  rdx, r13         ; 4c 89 ea                             
    96 : imul rdx, [rsp]       ; 48 0f af 94 24 00 00 00 00           
    97 : add  rcx, rdx         ; 48 01 d1                             
    98 : cmp  r8, rcx          ; 49 39 c8                             
    99 : jle  [102]            ; 0f 8e 0c 00 00 00                    
   100 : mov  rax, #0x05       ; 48 c7 c0 05 00 00 00                 
   101 : jmp  [104]            ; e9 0c 00 00 00                       
   102 : mov  rax, #0x04       ; 48 c7 c0 04 00 00 00                 
   103 : jmp  [104]            ; e9 00 00 00 00                       
   104 : mov  r13, [rsp+#0x08] ; 4c 8b ac 24 08 00 00 00              
   105 : add  rsp, #0x18       ; 48 81 c4 18 00 00 00                 
   106 : ret                   ; c3                                   
