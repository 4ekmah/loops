triangle_types(i0, i1, i2)
     0 : sub  rsp, #0x28       ; 48 83 ec 28                 
     1 : mov  [rsp+#0x10], r12 ; 4c 89 64 24 10              
     2 : mov  [rsp+#0x18], r13 ; 4c 89 6c 24 18              
     3 : mov  [rsp], #0        ; 48 c7 44 24 00 00 00 00 00  
     4 : cmp  rcx, [rsp]       ; 48 3b 4c 24 00              
     5 : jg   [10]             ; 0f 8f 14 00 00 00           
     6 : mov  rax, #0          ; 48 c7 c0 00 00 00 00        
     7 : mov  rax, rax         ; 48 89 c0                    
     8 : jmp  [119]            ; e9 04 02 00 00              
     9 : jmp  [117]            ; e9 f5 01 00 00              
    10 : cmp  rdx, [rsp]       ; 48 3b 54 24 00              
    11 : jg   [16]             ; 0f 8f 14 00 00 00           
    12 : mov  rax, #0          ; 48 c7 c0 00 00 00 00        
    13 : mov  rax, rax         ; 48 89 c0                    
    14 : jmp  [119]            ; e9 e5 01 00 00              
    15 : jmp  [117]            ; e9 d6 01 00 00              
    16 : cmp  r8, [rsp]        ; 4c 3b 44 24 00              
    17 : jg   [22]             ; 0f 8f 14 00 00 00           
    18 : mov  rax, #0          ; 48 c7 c0 00 00 00 00        
    19 : mov  rax, rax         ; 48 89 c0                    
    20 : jmp  [119]            ; e9 c6 01 00 00              
    21 : jmp  [117]            ; e9 b7 01 00 00              
    22 : mov  rax, rcx         ; 48 89 c8                    
    23 : add  rax, rdx         ; 48 01 d0                    
    24 : mov  r12, rcx         ; 49 89 cc                    
    25 : add  r12, r8          ; 4d 01 c4                    
    26 : mov  r9, rdx          ; 49 89 d1                    
    27 : add  r9, r8           ; 4d 01 c1                    
    28 : cmp  rcx, r9          ; 4c 39 c9                    
    29 : jle  [34]             ; 0f 8e 14 00 00 00           
    30 : mov  r9, #0           ; 49 c7 c1 00 00 00 00        
    31 : mov  rax, r9          ; 4c 89 c8                    
    32 : jmp  [119]            ; e9 97 01 00 00              
    33 : jmp  [117]            ; e9 88 01 00 00              
    34 : cmp  rdx, r12         ; 4c 39 e2                    
    35 : jle  [40]             ; 0f 8e 14 00 00 00           
    36 : mov  r9, #0           ; 49 c7 c1 00 00 00 00        
    37 : mov  rax, r9          ; 4c 89 c8                    
    38 : jmp  [119]            ; e9 7a 01 00 00              
    39 : jmp  [117]            ; e9 6b 01 00 00              
    40 : cmp  r8, rax          ; 49 39 c0                    
    41 : jle  [46]             ; 0f 8e 14 00 00 00           
    42 : mov  r9, #0           ; 49 c7 c1 00 00 00 00        
    43 : mov  rax, r9          ; 4c 89 c8                    
    44 : jmp  [119]            ; e9 5d 01 00 00              
    45 : jmp  [117]            ; e9 4e 01 00 00              
    46 : cmp  rcx, rdx         ; 48 39 d1                    
    47 : jne  [58]             ; 0f 85 31 00 00 00           
    48 : cmp  rdx, r8          ; 4c 39 c2                    
    49 : jne  [54]             ; 0f 85 14 00 00 00           
    50 : mov  r9, #0x02        ; 49 c7 c1 02 00 00 00        
    51 : mov  rax, r9          ; 4c 89 c8                    
    52 : jmp  [119]            ; e9 37 01 00 00              
    53 : jmp  [57]             ; e9 0f 00 00 00              
    54 : mov  r9, #0x03        ; 49 c7 c1 03 00 00 00        
    55 : mov  rax, r9          ; 4c 89 c8                    
    56 : jmp  [119]            ; e9 23 01 00 00              
    57 : jmp  [69]             ; e9 35 00 00 00              
    58 : cmp  rcx, r8          ; 4c 39 c1                    
    59 : jne  [64]             ; 0f 85 14 00 00 00           
    60 : mov  r9, #0x03        ; 49 c7 c1 03 00 00 00        
    61 : mov  rax, r9          ; 4c 89 c8                    
    62 : jmp  [119]            ; e9 06 01 00 00              
    63 : jmp  [69]             ; e9 18 00 00 00              
    64 : cmp  rdx, r8          ; 4c 39 c2                    
    65 : jne  [69]             ; 0f 85 0f 00 00 00           
    66 : mov  r9, #0x03        ; 49 c7 c1 03 00 00 00        
    67 : mov  rax, r9          ; 4c 89 c8                    
    68 : jmp  [119]            ; e9 e9 00 00 00              
    69 : imul rcx, rcx         ; 48 0f af c9                 
    70 : imul rdx, rdx         ; 48 0f af d2                 
    71 : imul r8, r8           ; 4d 0f af c0                 
    72 : mov  [rsp+#0x08], rcx ; 48 89 4c 24 08              
    73 : add  [rsp+#0x08], rdx ; 48 01 54 24 08              
    74 : mov  rax, rcx         ; 48 89 c8                    
    75 : add  rax, r8          ; 4c 01 c0                    
    76 : mov  r12, rdx         ; 49 89 d4                    
    77 : add  r12, r8          ; 4d 01 c4                    
    78 : cmp  rcx, r12         ; 4c 39 e1                    
    79 : jne  [84]             ; 0f 85 14 00 00 00           
    80 : mov  r9, #0x01        ; 49 c7 c1 01 00 00 00        
    81 : mov  rax, r9          ; 4c 89 c8                    
    82 : jmp  [119]            ; e9 af 00 00 00              
    83 : jmp  [95]             ; e9 37 00 00 00              
    84 : cmp  rdx, rax         ; 48 39 c2                    
    85 : jne  [90]             ; 0f 85 14 00 00 00           
    86 : mov  r9, #0x01        ; 49 c7 c1 01 00 00 00        
    87 : mov  rax, r9          ; 4c 89 c8                    
    88 : jmp  [119]            ; e9 92 00 00 00              
    89 : jmp  [95]             ; e9 1a 00 00 00              
    90 : cmp  r8, [rsp+#0x08]  ; 4c 3b 44 24 08              
    91 : jne  [95]             ; 0f 85 0f 00 00 00           
    92 : mov  r9, #0x01        ; 49 c7 c1 01 00 00 00        
    93 : mov  rax, r9          ; 4c 89 c8                    
    94 : jmp  [119]            ; e9 73 00 00 00              
    95 : sub  r12, rcx         ; 49 29 cc                    
    96 : sub  rax, rdx         ; 48 29 d0                    
    97 : mov  r13, [rsp+#0x08] ; 4c 8b 6c 24 08              
    98 : sub  r8, r13          ; 4d 29 e8                    
    99 : neg  r8               ; 49 f7 d8                    
   100 : cmp  r12, [rsp]       ; 4c 3b 64 24 00              
   101 : jge  [106]            ; 0f 8d 14 00 00 00           
   102 : mov  rdx, #0x05       ; 48 c7 c2 05 00 00 00        
   103 : mov  rax, rdx         ; 48 89 d0                    
   104 : jmp  [119]            ; e9 48 00 00 00              
   105 : jmp  [117]            ; e9 39 00 00 00              
   106 : cmp  rax, [rsp]       ; 48 3b 44 24 00              
   107 : jge  [112]            ; 0f 8d 14 00 00 00           
   108 : mov  rdx, #0x05       ; 48 c7 c2 05 00 00 00        
   109 : mov  rax, rdx         ; 48 89 d0                    
   110 : jmp  [119]            ; e9 29 00 00 00              
   111 : jmp  [117]            ; e9 1a 00 00 00              
   112 : cmp  r8, [rsp]        ; 4c 3b 44 24 00              
   113 : jge  [117]            ; 0f 8d 0f 00 00 00           
   114 : mov  r8, #0x05        ; 49 c7 c0 05 00 00 00        
   115 : mov  rax, r8          ; 4c 89 c0                    
   116 : jmp  [119]            ; e9 0a 00 00 00              
   117 : mov  r8, #0x04        ; 49 c7 c0 04 00 00 00        
   118 : mov  rax, r8          ; 4c 89 c0                    
   119 : mov  r12, [rsp+#0x10] ; 4c 8b 64 24 10              
   120 : mov  r13, [rsp+#0x18] ; 4c 8b 6c 24 18              
   121 : add  rsp, #0x28       ; 48 83 c4 28                 
   122 : ret                   ; c3                          
