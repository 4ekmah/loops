all_loads_all_stores(i0, i1, i2, i3, i4)
     0 : sub    rsp, #0x38         ; 48 83 ec 38                 
     1 : mov    [rsp+#0x20], r12   ; 4c 89 64 24 20              
     2 : mov    [rsp+#0x28], r13   ; 4c 89 6c 24 28              
     3 : mov    [rsp+#0x10], #0    ; 48 c7 44 24 10 00 00 00 00  
     4 : mov    [rsp], #0          ; 48 c7 44 24 00 00 00 00 00  
     5 : mov    [rsp+#0x08], #0    ; 48 c7 44 24 08 00 00 00 00  
     6 : mov    r12, #0x01         ; 49 c7 c4 01 00 00 00        
     7 : mov    rax, #0x02         ; 48 c7 c0 02 00 00 00        
     8 : cmp    rdx, #0x01         ; 48 83 fa 01                 
     9 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    10 : mov    rax, #0x04         ; 48 c7 c0 04 00 00 00        
    11 : cmp    rdx, #0x03         ; 48 83 fa 03                 
    12 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    13 : mov    rax, r12           ; 4c 89 e0                    
    14 : mov    r12, #0x08         ; 49 c7 c4 08 00 00 00        
    15 : cmp    rdx, #0x05         ; 48 83 fa 05                 
    16 : cmovg  rax, r12           ; 49 0f 4f c4                 
    17 : mov    [rsp+#0x18], rax   ; 48 89 44 24 18              
    18 : mov    r12, #0x01         ; 49 c7 c4 01 00 00 00        
    19 : mov    rax, #0x02         ; 48 c7 c0 02 00 00 00        
    20 : cmp    r9, #0x01          ; 49 83 f9 01                 
    21 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    22 : mov    rax, #0x04         ; 48 c7 c0 04 00 00 00        
    23 : cmp    r9, #0x03          ; 49 83 f9 03                 
    24 : cmovg  r12, rax           ; 4c 0f 4f e0                 
    25 : mov    rax, r12           ; 4c 89 e0                    
    26 : mov    r12, #0x08         ; 49 c7 c4 08 00 00 00        
    27 : cmp    r9, #0x05          ; 49 83 f9 05                 
    28 : cmovg  rax, r12           ; 49 0f 4f c4                 
    29 : mov    rax, rax           ; 48 89 c0                    
    30 : mov    r12, #0            ; 49 c7 c4 00 00 00 00        
    31 : cmp    rdx, #0            ; 48 83 fa 00                 
    32 : jne    [37]               ; 0f 85 15 00 00 00           
    33 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    34 : movzx  r12, cl, r13       ; 4e 0f b6 24 29              
    35 : mov    r12, r12           ; 4d 89 e4                    
    36 : jmp    [76]               ; e9 c6 00 00 00              
    37 : cmp    rdx, #0x01         ; 48 83 fa 01                 
    38 : jne    [43]               ; 0f 85 15 00 00 00           
    39 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    40 : movsx  r12, cl, r13       ; 4e 0f be 24 29              
    41 : mov    r12, r12           ; 4d 89 e4                    
    42 : jmp    [76]               ; e9 a7 00 00 00              
    43 : cmp    rdx, #0x02         ; 48 83 fa 02                 
    44 : jne    [49]               ; 0f 85 15 00 00 00           
    45 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    46 : movzx  r12, cx, r13       ; 4e 0f b7 24 29              
    47 : mov    r12, r12           ; 4d 89 e4                    
    48 : jmp    [76]               ; e9 88 00 00 00              
    49 : cmp    rdx, #0x03         ; 48 83 fa 03                 
    50 : jne    [55]               ; 0f 85 15 00 00 00           
    51 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    52 : movsx  r12, cx, r13       ; 4e 0f bf 24 29              
    53 : mov    r12, r12           ; 4d 89 e4                    
    54 : jmp    [76]               ; e9 69 00 00 00              
    55 : cmp    rdx, #0x04         ; 48 83 fa 04                 
    56 : jne    [61]               ; 0f 85 14 00 00 00           
    57 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    58 : mov    r12d, [rcx], [r13] ; 46 8b 24 29                 
    59 : mov    r12, r12           ; 4d 89 e4                    
    60 : jmp    [76]               ; e9 4b 00 00 00              
    61 : cmp    rdx, #0x05         ; 48 83 fa 05                 
    62 : jne    [67]               ; 0f 85 14 00 00 00           
    63 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    64 : movsxd r12, rcx, r13      ; 4e 63 24 29                 
    65 : mov    r12, r12           ; 4d 89 e4                    
    66 : jmp    [76]               ; e9 2d 00 00 00              
    67 : cmp    rdx, #0x06         ; 48 83 fa 06                 
    68 : jne    [73]               ; 0f 85 14 00 00 00           
    69 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    70 : mov    r12, [rcx], [r13]  ; 4e 8b 24 29                 
    71 : mov    r12, r12           ; 4d 89 e4                    
    72 : jmp    [76]               ; e9 0f 00 00 00              
    73 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    74 : mov    r12, [rcx], [r13]  ; 4e 8b 24 29                 
    75 : mov    r12, r12           ; 4d 89 e4                    
    76 : cmp    r9, #0             ; 49 83 f9 00                 
    77 : jne    [81]               ; 0f 85 11 00 00 00           
    78 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    79 : mov    [r8], [r13], r12b  ; 47 88 24 28                 
    80 : jmp    [113]              ; e9 b0 00 00 00              
    81 : cmp    r9, #0x01          ; 49 83 f9 01                 
    82 : jne    [86]               ; 0f 85 11 00 00 00           
    83 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    84 : mov    [r8], [r13], r12b  ; 47 88 24 28                 
    85 : jmp    [113]              ; e9 95 00 00 00              
    86 : cmp    r9, #0x02          ; 49 83 f9 02                 
    87 : jne    [91]               ; 0f 85 12 00 00 00           
    88 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    89 : mov    [r8], [r13], r12w  ; 66 47 89 24 28              
    90 : jmp    [113]              ; e9 79 00 00 00              
    91 : cmp    r9, #0x03          ; 49 83 f9 03                 
    92 : jne    [96]               ; 0f 85 12 00 00 00           
    93 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    94 : mov    [r8], [r13], r12w  ; 66 47 89 24 28              
    95 : jmp    [113]              ; e9 5d 00 00 00              
    96 : cmp    r9, #0x04          ; 49 83 f9 04                 
    97 : jne    [101]              ; 0f 85 11 00 00 00           
    98 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    99 : mov    [r8], [r13], r12d  ; 47 89 24 28                 
   100 : jmp    [113]              ; e9 42 00 00 00              
   101 : cmp    r9, #0x05          ; 49 83 f9 05                 
   102 : jne    [106]              ; 0f 85 11 00 00 00           
   103 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
   104 : mov    [r8], [r13], r12d  ; 47 89 24 28                 
   105 : jmp    [113]              ; e9 27 00 00 00              
   106 : cmp    r9, #0x06          ; 49 83 f9 06                 
   107 : jne    [111]              ; 0f 85 11 00 00 00           
   108 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
   109 : mov    [r8], [r13], r12   ; 4f 89 24 28                 
   110 : jmp    [113]              ; e9 0c 00 00 00              
   111 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
   112 : mov    [r8], [r13], r12   ; 4f 89 24 28                 
   113 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
   114 : add    [rsp], r13         ; 4c 01 6c 24 00              
   115 : add    [rsp+#0x08], rax   ; 48 01 44 24 08              
   116 : add    [rsp+#0x10], #0x01 ; 48 83 44 24 10 01           
   117 : mov    r13, [rsp+#0x60]   ; 4c 8b ac 24 60 00 00 00     
   118 : cmp    [rsp+#0x10], r13   ; 4c 39 6c 24 10              
   119 : jl     [30]               ; 0f 8c 1e fe ff ff           
   120 : mov    r12, [rsp+#0x20]   ; 4c 8b a4 24 20 00 00 00     
   121 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00     
   122 : add    rsp, #0x38         ; 48 83 c4 38                 
   123 : ret                       ; c3                          
