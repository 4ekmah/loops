all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub    rsp, #0x38         ; 48 83 ec 38                 
     1 : mov    [rsp+#0x20], r12   ; 4c 89 64 24 20              
     2 : mov    [rsp+#0x28], r13   ; 4c 89 6c 24 28              
     3 : mov    [rsp+#0x10], #0    ; 48 c7 44 24 10 00 00 00 00  
     4 : mov    [rsp+#0x08], #0    ; 48 c7 44 24 08 00 00 00 00  
     5 : mov    [rsp], #0          ; 48 c7 44 24 00 00 00 00 00  
     6 : mov    r12, #0x01         ; 49 c7 c4 01 00 00 00        
     7 : mov    rax, #0x02         ; 48 c7 c0 02 00 00 00        
     8 : cmp    rsi, #0x01         ; 48 83 fe 01                 
     9 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    10 : mov    rax, #0x04         ; 48 c7 c0 04 00 00 00        
    11 : cmp    rsi, #0x03         ; 48 83 fe 03                 
    12 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    13 : mov    rax, r12           ; 4c 89 e0                    
    14 : mov    r12, #0x08         ; 49 c7 c4 08 00 00 00        
    15 : cmp    rsi, #0x05         ; 48 83 fe 05                 
    16 : cmovg  rax, r12           ; 49 0f 4f c4                 
    17 : mov    [rsp+#0x18], rax   ; 48 89 44 24 18              
    18 : mov    r12, #0x01         ; 49 c7 c4 01 00 00 00        
    19 : mov    rax, #0x02         ; 48 c7 c0 02 00 00 00        
    20 : cmp    rcx, #0x01         ; 48 83 f9 01                 
    21 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    22 : mov    rax, #0x04         ; 48 c7 c0 04 00 00 00        
    23 : cmp    rcx, #0x03         ; 48 83 f9 03                 
    24 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    25 : mov    rax, r12           ; 4c 89 e0                    
    26 : mov    r12, #0x08         ; 49 c7 c4 08 00 00 00        
    27 : cmp    rcx, #0x05         ; 48 83 f9 05                 
    28 : cmovg  rax, r12           ; 49 0f 4f c4                 
    29 : cmp    [rsp+#0x10], r8    ; 4c 39 44 24 10              
    30 : jge    [111]              ; 0f 8d bc 01 00 00           
    31 : mov    r12, #0            ; 49 c7 c4 00 00 00 00        
    32 : cmp    rsi, #0            ; 48 83 fe 00                 
    33 : jne    [37]               ; 0f 85 12 00 00 00           
    34 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    35 : movzx  r12, dil, r13      ; 4e 0f b6 24 2f              
    36 : jmp    [69]               ; e9 b1 00 00 00              
    37 : cmp    rsi, #0x01         ; 48 83 fe 01                 
    38 : jne    [42]               ; 0f 85 12 00 00 00           
    39 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    40 : movsx  r12, dil, r13      ; 4e 0f be 24 2f              
    41 : jmp    [69]               ; e9 95 00 00 00              
    42 : cmp    rsi, #0x02         ; 48 83 fe 02                 
    43 : jne    [47]               ; 0f 85 12 00 00 00           
    44 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    45 : movzx  r12, di, r13       ; 4e 0f b7 24 2f              
    46 : jmp    [69]               ; e9 79 00 00 00              
    47 : cmp    rsi, #0x03         ; 48 83 fe 03                 
    48 : jne    [52]               ; 0f 85 12 00 00 00           
    49 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    50 : movsx  r12, di, r13       ; 4e 0f bf 24 2f              
    51 : jmp    [69]               ; e9 5d 00 00 00              
    52 : cmp    rsi, #0x04         ; 48 83 fe 04                 
    53 : jne    [57]               ; 0f 85 11 00 00 00           
    54 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    55 : mov    r12d, [rdi], [r13] ; 46 8b 24 2f                 
    56 : jmp    [69]               ; e9 42 00 00 00              
    57 : cmp    rsi, #0x05         ; 48 83 fe 05                 
    58 : jne    [62]               ; 0f 85 11 00 00 00           
    59 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    60 : movsxd r12, rdi, r13      ; 4e 63 24 2f                 
    61 : jmp    [69]               ; e9 27 00 00 00              
    62 : cmp    rsi, #0x06         ; 48 83 fe 06                 
    63 : jne    [67]               ; 0f 85 11 00 00 00           
    64 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    65 : mov    r12, [rdi], [r13]  ; 4e 8b 24 2f                 
    66 : jmp    [69]               ; e9 0c 00 00 00              
    67 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    68 : mov    r12, [rdi], [r13]  ; 4e 8b 24 2f                 
    69 : cmp    rcx, #0            ; 48 83 f9 00                 
    70 : jne    [74]               ; 0f 85 11 00 00 00           
    71 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    72 : mov    [rdx], [r13], r12b ; 46 88 24 2a                 
    73 : jmp    [106]              ; e9 b0 00 00 00              
    74 : cmp    rcx, #0x01         ; 48 83 f9 01                 
    75 : jne    [79]               ; 0f 85 11 00 00 00           
    76 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    77 : mov    [rdx], [r13], r12b ; 46 88 24 2a                 
    78 : jmp    [106]              ; e9 95 00 00 00              
    79 : cmp    rcx, #0x02         ; 48 83 f9 02                 
    80 : jne    [84]               ; 0f 85 12 00 00 00           
    81 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    82 : mov    [rdx], [r13], r12w ; 66 46 89 24 2a              
    83 : jmp    [106]              ; e9 79 00 00 00              
    84 : cmp    rcx, #0x03         ; 48 83 f9 03                 
    85 : jne    [89]               ; 0f 85 12 00 00 00           
    86 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    87 : mov    [rdx], [r13], r12w ; 66 46 89 24 2a              
    88 : jmp    [106]              ; e9 5d 00 00 00              
    89 : cmp    rcx, #0x04         ; 48 83 f9 04                 
    90 : jne    [94]               ; 0f 85 11 00 00 00           
    91 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    92 : mov    [rdx], [r13], r12d ; 46 89 24 2a                 
    93 : jmp    [106]              ; e9 42 00 00 00              
    94 : cmp    rcx, #0x05         ; 48 83 f9 05                 
    95 : jne    [99]               ; 0f 85 11 00 00 00           
    96 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    97 : mov    [rdx], [r13], r12d ; 46 89 24 2a                 
    98 : jmp    [106]              ; e9 27 00 00 00              
    99 : cmp    rcx, #0x06         ; 48 83 f9 06                 
   100 : jne    [104]              ; 0f 85 11 00 00 00           
   101 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
   102 : mov    [rdx], [r13], r12  ; 4e 89 24 2a                 
   103 : jmp    [106]              ; e9 0c 00 00 00              
   104 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
   105 : mov    [rdx], [r13], r12  ; 4e 89 24 2a                 
   106 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
   107 : add    [rsp+#0x08], r13   ; 4c 01 6c 24 08              
   108 : add    [rsp], rax         ; 48 01 44 24 00              
   109 : add    [rsp+#0x10], #0x01 ; 48 83 44 24 10 01           
   110 : jmp    [29]               ; e9 39 fe ff ff              
   111 : mov    r12, [rsp+#0x20]   ; 4c 8b a4 24 20 00 00 00     
   112 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00     
   113 : add    rsp, #0x38         ; 48 83 c4 38                 
   114 : ret                       ; c3                          
