all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub    rsp, #0x38         ; 48 81 ec 38 00 00 00                 
     1 : mov    [rsp+#0x18], r8    ; 4c 89 84 24 18 00 00 00              
     2 : mov    [rsp+#0x28], r13   ; 4c 89 ac 24 28 00 00 00              
     3 : mov    [rsp+#0x10], #0    ; 48 c7 84 24 10 00 00 00 00 00 00 00  
     4 : mov    [rsp], #0          ; 48 c7 84 24 00 00 00 00 00 00 00 00  
     5 : mov    [rsp+#0x08], #0    ; 48 c7 84 24 08 00 00 00 00 00 00 00  
     6 : mov    rax, #0x02         ; 48 c7 c0 02 00 00 00                 
     7 : mov    r8, #0x01          ; 49 c7 c0 01 00 00 00                 
     8 : cmp    rsi, #0x01         ; 48 83 fe 01                          
     9 : cmovg  r8, rax            ; 4c 0f 4f c0                          
    10 : mov    rax, #0x04         ; 48 c7 c0 04 00 00 00                 
    11 : cmp    rsi, #0x03         ; 48 83 fe 03                          
    12 : cmovg  r8, rax            ; 4c 0f 4f c0                          
    13 : mov    rax, #0x08         ; 48 c7 c0 08 00 00 00                 
    14 : cmp    rsi, #0x05         ; 48 83 fe 05                          
    15 : mov    r13, r8            ; 4d 89 c5                             
    16 : cmovg  r13, rax           ; 4c 0f 4f e8                          
    17 : mov    [rsp+#0x20], r13   ; 4c 89 ac 24 20 00 00 00              
    18 : mov    rax, #0x02         ; 48 c7 c0 02 00 00 00                 
    19 : mov    r8, #0x01          ; 49 c7 c0 01 00 00 00                 
    20 : cmp    rcx, #0x01         ; 48 83 f9 01                          
    21 : cmovg  r8, rax            ; 4c 0f 4f c0                          
    22 : mov    rax, #0x04         ; 48 c7 c0 04 00 00 00                 
    23 : cmp    rcx, #0x03         ; 48 83 f9 03                          
    24 : cmovg  r8, rax            ; 4c 0f 4f c0                          
    25 : mov    rax, #0x08         ; 48 c7 c0 08 00 00 00                 
    26 : cmp    rcx, #0x05         ; 48 83 f9 05                          
    27 : cmovg  r8, rax            ; 4c 0f 4f c0                          
    28 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00              
    29 : cmp    [rsp+#0x10], r13   ; 4c 39 ac 24 10 00 00 00              
    30 : jge    [111]              ; 0f 8d c4 01 00 00                    
    31 : xor    rax, rax           ; 48 31 c0                             
    32 : cmp    rsi, #0            ; 48 83 fe 00                          
    33 : jne    [37]               ; 0f 85 12 00 00 00                    
    34 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    35 : movzx  rax, dil, r13      ; 4a 0f b6 04 2f                       
    36 : jmp    [69]               ; e9 b1 00 00 00                       
    37 : cmp    rsi, #0x01         ; 48 83 fe 01                          
    38 : jne    [42]               ; 0f 85 12 00 00 00                    
    39 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    40 : movsx  rax, dil, r13      ; 4a 0f be 04 2f                       
    41 : jmp    [69]               ; e9 95 00 00 00                       
    42 : cmp    rsi, #0x02         ; 48 83 fe 02                          
    43 : jne    [47]               ; 0f 85 12 00 00 00                    
    44 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    45 : movzx  rax, di, r13       ; 4a 0f b7 04 2f                       
    46 : jmp    [69]               ; e9 79 00 00 00                       
    47 : cmp    rsi, #0x03         ; 48 83 fe 03                          
    48 : jne    [52]               ; 0f 85 12 00 00 00                    
    49 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    50 : movsx  rax, di, r13       ; 4a 0f bf 04 2f                       
    51 : jmp    [69]               ; e9 5d 00 00 00                       
    52 : cmp    rsi, #0x04         ; 48 83 fe 04                          
    53 : jne    [57]               ; 0f 85 11 00 00 00                    
    54 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    55 : mov    eax, [rdi], [r13]  ; 42 8b 04 2f                          
    56 : jmp    [69]               ; e9 42 00 00 00                       
    57 : cmp    rsi, #0x05         ; 48 83 fe 05                          
    58 : jne    [62]               ; 0f 85 11 00 00 00                    
    59 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    60 : movsxd rax, rdi, r13      ; 4a 63 04 2f                          
    61 : jmp    [69]               ; e9 27 00 00 00                       
    62 : cmp    rsi, #0x06         ; 48 83 fe 06                          
    63 : jne    [67]               ; 0f 85 11 00 00 00                    
    64 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    65 : mov    rax, [rdi], [r13]  ; 4a 8b 04 2f                          
    66 : jmp    [69]               ; e9 0c 00 00 00                       
    67 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    68 : mov    rax, [rdi], [r13]  ; 4a 8b 04 2f                          
    69 : cmp    rcx, #0            ; 48 83 f9 00                          
    70 : jne    [74]               ; 0f 85 11 00 00 00                    
    71 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    72 : mov    [rdx], [r13], al   ; 42 88 04 2a                          
    73 : jmp    [106]              ; e9 b0 00 00 00                       
    74 : cmp    rcx, #0x01         ; 48 83 f9 01                          
    75 : jne    [79]               ; 0f 85 11 00 00 00                    
    76 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    77 : mov    [rdx], [r13], al   ; 42 88 04 2a                          
    78 : jmp    [106]              ; e9 95 00 00 00                       
    79 : cmp    rcx, #0x02         ; 48 83 f9 02                          
    80 : jne    [84]               ; 0f 85 12 00 00 00                    
    81 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    82 : mov    [rdx], [r13], ax   ; 66 42 89 04 2a                       
    83 : jmp    [106]              ; e9 79 00 00 00                       
    84 : cmp    rcx, #0x03         ; 48 83 f9 03                          
    85 : jne    [89]               ; 0f 85 12 00 00 00                    
    86 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    87 : mov    [rdx], [r13], ax   ; 66 42 89 04 2a                       
    88 : jmp    [106]              ; e9 5d 00 00 00                       
    89 : cmp    rcx, #0x04         ; 48 83 f9 04                          
    90 : jne    [94]               ; 0f 85 11 00 00 00                    
    91 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    92 : mov    [rdx], [r13], eax  ; 42 89 04 2a                          
    93 : jmp    [106]              ; e9 42 00 00 00                       
    94 : cmp    rcx, #0x05         ; 48 83 f9 05                          
    95 : jne    [99]               ; 0f 85 11 00 00 00                    
    96 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    97 : mov    [rdx], [r13], eax  ; 42 89 04 2a                          
    98 : jmp    [106]              ; e9 27 00 00 00                       
    99 : cmp    rcx, #0x06         ; 48 83 f9 06                          
   100 : jne    [104]              ; 0f 85 11 00 00 00                    
   101 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
   102 : mov    [rdx], [r13], rax  ; 4a 89 04 2a                          
   103 : jmp    [106]              ; e9 0c 00 00 00                       
   104 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
   105 : mov    [rdx], [r13], rax  ; 4a 89 04 2a                          
   106 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00              
   107 : add    [rsp], r13         ; 4c 01 ac 24 00 00 00 00              
   108 : add    [rsp+#0x08], r8    ; 4c 01 84 24 08 00 00 00              
   109 : add    [rsp+#0x10], #0x01 ; 48 81 84 24 10 00 00 00 01 00 00 00  
   110 : jmp    [28]               ; e9 26 fe ff ff                       
   111 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00              
   112 : add    rsp, #0x38         ; 48 81 c4 38 00 00 00                 
   113 : ret                       ; c3                                   
