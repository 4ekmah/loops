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
    30 : mov    r13, [rsp+#0x60]   ; 4c 8b ac 24 60 00 00 00     
    31 : cmp    [rsp+#0x10], r13   ; 4c 39 6c 24 10              
    32 : jge    [121]              ; 0f 8d d4 01 00 00           
    33 : mov    r12, #0            ; 49 c7 c4 00 00 00 00        
    34 : cmp    rdx, #0            ; 48 83 fa 00                 
    35 : jne    [40]               ; 0f 85 15 00 00 00           
    36 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    37 : movzx  r12, cl, r13       ; 4e 0f b6 24 29              
    38 : mov    r12, r12           ; 4d 89 e4                    
    39 : jmp    [79]               ; e9 c6 00 00 00              
    40 : cmp    rdx, #0x01         ; 48 83 fa 01                 
    41 : jne    [46]               ; 0f 85 15 00 00 00           
    42 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    43 : movsx  r12, cl, r13       ; 4e 0f be 24 29              
    44 : mov    r12, r12           ; 4d 89 e4                    
    45 : jmp    [79]               ; e9 a7 00 00 00              
    46 : cmp    rdx, #0x02         ; 48 83 fa 02                 
    47 : jne    [52]               ; 0f 85 15 00 00 00           
    48 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    49 : movzx  r12, cx, r13       ; 4e 0f b7 24 29              
    50 : mov    r12, r12           ; 4d 89 e4                    
    51 : jmp    [79]               ; e9 88 00 00 00              
    52 : cmp    rdx, #0x03         ; 48 83 fa 03                 
    53 : jne    [58]               ; 0f 85 15 00 00 00           
    54 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    55 : movsx  r12, cx, r13       ; 4e 0f bf 24 29              
    56 : mov    r12, r12           ; 4d 89 e4                    
    57 : jmp    [79]               ; e9 69 00 00 00              
    58 : cmp    rdx, #0x04         ; 48 83 fa 04                 
    59 : jne    [64]               ; 0f 85 14 00 00 00           
    60 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    61 : mov    r12d, [rcx], [r13] ; 46 8b 24 29                 
    62 : mov    r12, r12           ; 4d 89 e4                    
    63 : jmp    [79]               ; e9 4b 00 00 00              
    64 : cmp    rdx, #0x05         ; 48 83 fa 05                 
    65 : jne    [70]               ; 0f 85 14 00 00 00           
    66 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    67 : movsxd r12, rcx, r13      ; 4e 63 24 29                 
    68 : mov    r12, r12           ; 4d 89 e4                    
    69 : jmp    [79]               ; e9 2d 00 00 00              
    70 : cmp    rdx, #0x06         ; 48 83 fa 06                 
    71 : jne    [76]               ; 0f 85 14 00 00 00           
    72 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    73 : mov    r12, [rcx], [r13]  ; 4e 8b 24 29                 
    74 : mov    r12, r12           ; 4d 89 e4                    
    75 : jmp    [79]               ; e9 0f 00 00 00              
    76 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    77 : mov    r12, [rcx], [r13]  ; 4e 8b 24 29                 
    78 : mov    r12, r12           ; 4d 89 e4                    
    79 : cmp    r9, #0             ; 49 83 f9 00                 
    80 : jne    [84]               ; 0f 85 11 00 00 00           
    81 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    82 : mov    [r8], [r13], r12b  ; 47 88 24 28                 
    83 : jmp    [116]              ; e9 b0 00 00 00              
    84 : cmp    r9, #0x01          ; 49 83 f9 01                 
    85 : jne    [89]               ; 0f 85 11 00 00 00           
    86 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    87 : mov    [r8], [r13], r12b  ; 47 88 24 28                 
    88 : jmp    [116]              ; e9 95 00 00 00              
    89 : cmp    r9, #0x02          ; 49 83 f9 02                 
    90 : jne    [94]               ; 0f 85 12 00 00 00           
    91 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    92 : mov    [r8], [r13], r12w  ; 66 47 89 24 28              
    93 : jmp    [116]              ; e9 79 00 00 00              
    94 : cmp    r9, #0x03          ; 49 83 f9 03                 
    95 : jne    [99]               ; 0f 85 12 00 00 00           
    96 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    97 : mov    [r8], [r13], r12w  ; 66 47 89 24 28              
    98 : jmp    [116]              ; e9 5d 00 00 00              
    99 : cmp    r9, #0x04          ; 49 83 f9 04                 
   100 : jne    [104]              ; 0f 85 11 00 00 00           
   101 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
   102 : mov    [r8], [r13], r12d  ; 47 89 24 28                 
   103 : jmp    [116]              ; e9 42 00 00 00              
   104 : cmp    r9, #0x05          ; 49 83 f9 05                 
   105 : jne    [109]              ; 0f 85 11 00 00 00           
   106 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
   107 : mov    [r8], [r13], r12d  ; 47 89 24 28                 
   108 : jmp    [116]              ; e9 27 00 00 00              
   109 : cmp    r9, #0x06          ; 49 83 f9 06                 
   110 : jne    [114]              ; 0f 85 11 00 00 00           
   111 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
   112 : mov    [r8], [r13], r12   ; 4f 89 24 28                 
   113 : jmp    [116]              ; e9 0c 00 00 00              
   114 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
   115 : mov    [r8], [r13], r12   ; 4f 89 24 28                 
   116 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
   117 : add    [rsp], r13         ; 4c 01 6c 24 00              
   118 : add    [rsp+#0x08], rax   ; 48 01 44 24 08              
   119 : add    [rsp+#0x10], #0x01 ; 48 83 44 24 10 01           
   120 : jmp    [30]               ; e9 19 fe ff ff              
   121 : mov    r12, [rsp+#0x20]   ; 4c 8b a4 24 20 00 00 00     
   122 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00     
   123 : add    rsp, #0x38         ; 48 83 c4 38                 
   124 : ret                       ; c3                          
