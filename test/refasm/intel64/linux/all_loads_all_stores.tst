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
    29 : mov    rax, rax           ; 48 89 c0                    
    30 : cmp    [rsp+#0x10], r8    ; 4c 39 44 24 10              
    31 : jge    [120]              ; 0f 8d d4 01 00 00           
    32 : mov    r12, #0            ; 49 c7 c4 00 00 00 00        
    33 : cmp    rsi, #0            ; 48 83 fe 00                 
    34 : jne    [39]               ; 0f 85 15 00 00 00           
    35 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    36 : movzx  r12, dil, r13      ; 4e 0f b6 24 2f              
    37 : mov    r12, r12           ; 4d 89 e4                    
    38 : jmp    [78]               ; e9 c6 00 00 00              
    39 : cmp    rsi, #0x01         ; 48 83 fe 01                 
    40 : jne    [45]               ; 0f 85 15 00 00 00           
    41 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    42 : movsx  r12, dil, r13      ; 4e 0f be 24 2f              
    43 : mov    r12, r12           ; 4d 89 e4                    
    44 : jmp    [78]               ; e9 a7 00 00 00              
    45 : cmp    rsi, #0x02         ; 48 83 fe 02                 
    46 : jne    [51]               ; 0f 85 15 00 00 00           
    47 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    48 : movzx  r12, di, r13       ; 4e 0f b7 24 2f              
    49 : mov    r12, r12           ; 4d 89 e4                    
    50 : jmp    [78]               ; e9 88 00 00 00              
    51 : cmp    rsi, #0x03         ; 48 83 fe 03                 
    52 : jne    [57]               ; 0f 85 15 00 00 00           
    53 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    54 : movsx  r12, di, r13       ; 4e 0f bf 24 2f              
    55 : mov    r12, r12           ; 4d 89 e4                    
    56 : jmp    [78]               ; e9 69 00 00 00              
    57 : cmp    rsi, #0x04         ; 48 83 fe 04                 
    58 : jne    [63]               ; 0f 85 14 00 00 00           
    59 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    60 : mov    r12d, [rdi], [r13] ; 46 8b 24 2f                 
    61 : mov    r12, r12           ; 4d 89 e4                    
    62 : jmp    [78]               ; e9 4b 00 00 00              
    63 : cmp    rsi, #0x05         ; 48 83 fe 05                 
    64 : jne    [69]               ; 0f 85 14 00 00 00           
    65 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    66 : movsxd r12, rdi, r13      ; 4e 63 24 2f                 
    67 : mov    r12, r12           ; 4d 89 e4                    
    68 : jmp    [78]               ; e9 2d 00 00 00              
    69 : cmp    rsi, #0x06         ; 48 83 fe 06                 
    70 : jne    [75]               ; 0f 85 14 00 00 00           
    71 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    72 : mov    r12, [rdi], [r13]  ; 4e 8b 24 2f                 
    73 : mov    r12, r12           ; 4d 89 e4                    
    74 : jmp    [78]               ; e9 0f 00 00 00              
    75 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    76 : mov    r12, [rdi], [r13]  ; 4e 8b 24 2f                 
    77 : mov    r12, r12           ; 4d 89 e4                    
    78 : cmp    rcx, #0            ; 48 83 f9 00                 
    79 : jne    [83]               ; 0f 85 11 00 00 00           
    80 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    81 : mov    [rdx], [r13], r12b ; 46 88 24 2a                 
    82 : jmp    [115]              ; e9 b0 00 00 00              
    83 : cmp    rcx, #0x01         ; 48 83 f9 01                 
    84 : jne    [88]               ; 0f 85 11 00 00 00           
    85 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    86 : mov    [rdx], [r13], r12b ; 46 88 24 2a                 
    87 : jmp    [115]              ; e9 95 00 00 00              
    88 : cmp    rcx, #0x02         ; 48 83 f9 02                 
    89 : jne    [93]               ; 0f 85 12 00 00 00           
    90 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    91 : mov    [rdx], [r13], r12w ; 66 46 89 24 2a              
    92 : jmp    [115]              ; e9 79 00 00 00              
    93 : cmp    rcx, #0x03         ; 48 83 f9 03                 
    94 : jne    [98]               ; 0f 85 12 00 00 00           
    95 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    96 : mov    [rdx], [r13], r12w ; 66 46 89 24 2a              
    97 : jmp    [115]              ; e9 5d 00 00 00              
    98 : cmp    rcx, #0x04         ; 48 83 f9 04                 
    99 : jne    [103]              ; 0f 85 11 00 00 00           
   100 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
   101 : mov    [rdx], [r13], r12d ; 46 89 24 2a                 
   102 : jmp    [115]              ; e9 42 00 00 00              
   103 : cmp    rcx, #0x05         ; 48 83 f9 05                 
   104 : jne    [108]              ; 0f 85 11 00 00 00           
   105 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
   106 : mov    [rdx], [r13], r12d ; 46 89 24 2a                 
   107 : jmp    [115]              ; e9 27 00 00 00              
   108 : cmp    rcx, #0x06         ; 48 83 f9 06                 
   109 : jne    [113]              ; 0f 85 11 00 00 00           
   110 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
   111 : mov    [rdx], [r13], r12  ; 4e 89 24 2a                 
   112 : jmp    [115]              ; e9 0c 00 00 00              
   113 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
   114 : mov    [rdx], [r13], r12  ; 4e 89 24 2a                 
   115 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
   116 : add    [rsp+#0x08], r13   ; 4c 01 6c 24 08              
   117 : add    [rsp], rax         ; 48 01 44 24 00              
   118 : add    [rsp+#0x10], #0x01 ; 48 83 44 24 10 01           
   119 : jmp    [30]               ; e9 21 fe ff ff              
   120 : mov    r12, [rsp+#0x20]   ; 4c 8b a4 24 20 00 00 00     
   121 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00     
   122 : add    rsp, #0x38         ; 48 83 c4 38                 
   123 : ret                       ; c3                          
