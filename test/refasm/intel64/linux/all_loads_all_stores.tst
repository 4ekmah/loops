all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub    rsp, #0x38         ; 48 83 ec 38                          
     1 : mov    [rsp+#0x20], r12   ; 4c 89 64 24 20                       
     2 : mov    [rsp+#0x28], r13   ; 4c 89 6c 24 28                       
     3 : mov    [rsp+#0x10], #0    ; 48 c7 44 24 10 00 00 00 00           
     4 : mov    [rsp+#0x08], #0    ; 48 c7 44 24 08 00 00 00 00           
     5 : mov    [rsp], #0          ; 48 c7 44 24 00 00 00 00 00           
     6 : mov    r12, #0x02         ; 49 c7 c4 02 00 00 00                 
     7 : mov    rax, #0x01         ; 48 c7 c0 01 00 00 00                 
     8 : cmp    rsi, #0x01         ; 48 83 fe 01                          
     9 : cmovg  rax, r12           ; 49 0f 4f c4                          
    10 : mov    r12, #0x04         ; 49 c7 c4 04 00 00 00                 
    11 : cmp    rsi, #0x03         ; 48 83 fe 03                          
    12 : cmovg  rax, r12           ; 49 0f 4f c4                          
    13 : mov    r12, #0x08         ; 49 c7 c4 08 00 00 00                 
    14 : cmp    rsi, #0x05         ; 48 83 fe 05                          
    15 : mov    r13, rax           ; 49 89 c5                             
    16 : cmovg  r13, r12           ; 4d 0f 4f ec                          
    17 : mov    [rsp+#0x18], r13   ; 4c 89 6c 24 18                       
    18 : mov    r12, #0x02         ; 49 c7 c4 02 00 00 00                 
    19 : mov    rax, #0x01         ; 48 c7 c0 01 00 00 00                 
    20 : cmp    rcx, #0x01         ; 48 83 f9 01                          
    21 : cmovg  rax, r12           ; 49 0f 4f c4                          
    22 : mov    r12, #0x04         ; 49 c7 c4 04 00 00 00                 
    23 : cmp    rcx, #0x03         ; 48 83 f9 03                          
    24 : cmovg  rax, r12           ; 49 0f 4f c4                          
    25 : mov    r12, #0x08         ; 49 c7 c4 08 00 00 00                 
    26 : cmp    rcx, #0x05         ; 48 83 f9 05                          
    27 : cmovg  rax, r12           ; 49 0f 4f c4                          
    28 : cmp    [rsp+#0x10], r8    ; 4c 39 84 24 10 00 00 00              
    29 : jge    [110]              ; 0f 8d c8 01 00 00                    
    30 : mov    r12, #0            ; 49 c7 c4 00 00 00 00                 
    31 : cmp    rsi, #0            ; 48 83 fe 00                          
    32 : jne    [36]               ; 0f 85 12 00 00 00                    
    33 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    34 : movzx  r12, dil, r13      ; 4e 0f b6 24 2f                       
    35 : jmp    [68]               ; e9 b1 00 00 00                       
    36 : cmp    rsi, #0x01         ; 48 83 fe 01                          
    37 : jne    [41]               ; 0f 85 12 00 00 00                    
    38 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    39 : movsx  r12, dil, r13      ; 4e 0f be 24 2f                       
    40 : jmp    [68]               ; e9 95 00 00 00                       
    41 : cmp    rsi, #0x02         ; 48 83 fe 02                          
    42 : jne    [46]               ; 0f 85 12 00 00 00                    
    43 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    44 : movzx  r12, di, r13       ; 4e 0f b7 24 2f                       
    45 : jmp    [68]               ; e9 79 00 00 00                       
    46 : cmp    rsi, #0x03         ; 48 83 fe 03                          
    47 : jne    [51]               ; 0f 85 12 00 00 00                    
    48 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    49 : movsx  r12, di, r13       ; 4e 0f bf 24 2f                       
    50 : jmp    [68]               ; e9 5d 00 00 00                       
    51 : cmp    rsi, #0x04         ; 48 83 fe 04                          
    52 : jne    [56]               ; 0f 85 11 00 00 00                    
    53 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    54 : mov    r12d, [rdi], [r13] ; 46 8b 24 2f                          
    55 : jmp    [68]               ; e9 42 00 00 00                       
    56 : cmp    rsi, #0x05         ; 48 83 fe 05                          
    57 : jne    [61]               ; 0f 85 11 00 00 00                    
    58 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    59 : movsxd r12, rdi, r13      ; 4e 63 24 2f                          
    60 : jmp    [68]               ; e9 27 00 00 00                       
    61 : cmp    rsi, #0x06         ; 48 83 fe 06                          
    62 : jne    [66]               ; 0f 85 11 00 00 00                    
    63 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    64 : mov    r12, [rdi], [r13]  ; 4e 8b 24 2f                          
    65 : jmp    [68]               ; e9 0c 00 00 00                       
    66 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00              
    67 : mov    r12, [rdi], [r13]  ; 4e 8b 24 2f                          
    68 : cmp    rcx, #0            ; 48 83 f9 00                          
    69 : jne    [73]               ; 0f 85 11 00 00 00                    
    70 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    71 : mov    [rdx], [r13], r12b ; 46 88 24 2a                          
    72 : jmp    [105]              ; e9 b0 00 00 00                       
    73 : cmp    rcx, #0x01         ; 48 83 f9 01                          
    74 : jne    [78]               ; 0f 85 11 00 00 00                    
    75 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    76 : mov    [rdx], [r13], r12b ; 46 88 24 2a                          
    77 : jmp    [105]              ; e9 95 00 00 00                       
    78 : cmp    rcx, #0x02         ; 48 83 f9 02                          
    79 : jne    [83]               ; 0f 85 12 00 00 00                    
    80 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    81 : mov    [rdx], [r13], r12w ; 66 46 89 24 2a                       
    82 : jmp    [105]              ; e9 79 00 00 00                       
    83 : cmp    rcx, #0x03         ; 48 83 f9 03                          
    84 : jne    [88]               ; 0f 85 12 00 00 00                    
    85 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    86 : mov    [rdx], [r13], r12w ; 66 46 89 24 2a                       
    87 : jmp    [105]              ; e9 5d 00 00 00                       
    88 : cmp    rcx, #0x04         ; 48 83 f9 04                          
    89 : jne    [93]               ; 0f 85 11 00 00 00                    
    90 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    91 : mov    [rdx], [r13], r12d ; 46 89 24 2a                          
    92 : jmp    [105]              ; e9 42 00 00 00                       
    93 : cmp    rcx, #0x05         ; 48 83 f9 05                          
    94 : jne    [98]               ; 0f 85 11 00 00 00                    
    95 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
    96 : mov    [rdx], [r13], r12d ; 46 89 24 2a                          
    97 : jmp    [105]              ; e9 27 00 00 00                       
    98 : cmp    rcx, #0x06         ; 48 83 f9 06                          
    99 : jne    [103]              ; 0f 85 11 00 00 00                    
   100 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
   101 : mov    [rdx], [r13], r12  ; 4e 89 24 2a                          
   102 : jmp    [105]              ; e9 0c 00 00 00                       
   103 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00              
   104 : mov    [rdx], [r13], r12  ; 4e 89 24 2a                          
   105 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00              
   106 : add    [rsp+#0x08], r13   ; 4c 01 ac 24 08 00 00 00              
   107 : add    [rsp], rax         ; 48 01 84 24 00 00 00 00              
   108 : add    [rsp+#0x10], #0x01 ; 48 81 84 24 10 00 00 00 01 00 00 00  
   109 : jmp    [28]               ; e9 2a fe ff ff                       
   110 : mov    r12, [rsp+#0x20]   ; 4c 8b a4 24 20 00 00 00              
   111 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00              
   112 : add    rsp, #0x38         ; 48 83 c4 38                          
   113 : ret                       ; c3                                   
