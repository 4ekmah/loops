sort_double(i0, i1)
     0 : sub  rsp, #0x78           ; 48 81 ec 78 00 00 00                 
     1 : mov  [rsp+#0x58], r12     ; 4c 89 a4 24 58 00 00 00              
     2 : mov  [rsp+#0x60], r13     ; 4c 89 ac 24 60 00 00 00              
     3 : mov  [rsp+#0x70], rbp     ; 48 89 ac 24 70 00 00 00              
     4 : mov  rbp, rsp             ; 48 89 e5                             
     5 : add  rbp, #0x70           ; 48 81 c5 70 00 00 00                 
     6 : mov  [rsp+#0x50], rdx     ; 48 89 94 24 50 00 00 00              
     7 : shl  [rsp+#0x50], #0x03   ; 48 c1 a4 24 50 00 00 00 03           
     8 : mov  r13, [rsp+#0x50]     ; 4c 8b ac 24 50 00 00 00              
     9 : mov  [rsp+#0x48], r13     ; 4c 89 ac 24 48 00 00 00              
    10 : sub  [rsp+#0x48], #0x08   ; 48 81 ac 24 48 00 00 00 08 00 00 00  
    11 : mov  [rsp+#0x40], #0      ; 48 c7 84 24 40 00 00 00 00 00 00 00  
    12 : mov  r13, [rsp+#0x48]     ; 4c 8b ac 24 48 00 00 00              
    13 : cmp  [rsp+#0x40], r13     ; 4c 39 ac 24 40 00 00 00              
    14 : jge  [58]                 ; 0f 8d 14 01 00 00                    
    15 : mov  rax, [rsp+#0x40]     ; 48 8b 84 24 40 00 00 00              
    16 : mov  r12, rax             ; 49 89 c4                             
    17 : add  r12, #0x08           ; 49 81 c4 08 00 00 00                 
    18 : cmp  r12, [rsp+#0x50]     ; 4c 3b a4 24 50 00 00 00              
    19 : jge  [48]                 ; 0f 8d b5 00 00 00                    
    20 : mov  r9, [rcx], [r12]     ; 4e 8b 0c 21                          
    21 : mov  r8, [rcx], [rax]     ; 4c 8b 04 01                          
    22 : mov  rdx, #0x7ff7f11ce6f0 ; 48 ba f0 e6 1c f1 f7 7f 00 00        
    23 : mov  [rsp], rax           ; 48 89 84 24 00 00 00 00              
    24 : mov  [rsp+#0x08], rcx     ; 48 89 8c 24 08 00 00 00              
    25 : mov  [rsp+#0x10], rdx     ; 48 89 94 24 10 00 00 00              
    26 : mov  [rsp+#0x18], r8      ; 4c 89 84 24 18 00 00 00              
    27 : mov  [rsp+#0x20], r9      ; 4c 89 8c 24 20 00 00 00              
    28 : mov  [rsp+#0x28], r10     ; 4c 89 94 24 28 00 00 00              
    29 : mov  [rsp+#0x30], r11     ; 4c 89 9c 24 30 00 00 00              
    30 : mov  rcx, r9              ; 4c 89 c9                             
    31 : mov  rdx, r8              ; 4c 89 c2                             
    32 : mov  r10, [rsp+#0x10]     ; 4c 8b 94 24 10 00 00 00              
    33 : sub  rsp, #0x20           ; 48 81 ec 20 00 00 00                 
    34 : call [r10]                ; 41 ff d2                             
    35 : add  rsp, #0x20           ; 48 81 c4 20 00 00 00                 
    36 : mov  rdx, rax             ; 48 89 c2                             
    37 : mov  rax, [rsp]           ; 48 8b 84 24 00 00 00 00              
    38 : mov  rcx, [rsp+#0x08]     ; 48 8b 8c 24 08 00 00 00              
    39 : mov  r8, [rsp+#0x18]      ; 4c 8b 84 24 18 00 00 00              
    40 : mov  r9, [rsp+#0x20]      ; 4c 8b 8c 24 20 00 00 00              
    41 : mov  r10, [rsp+#0x28]     ; 4c 8b 94 24 28 00 00 00              
    42 : mov  r11, [rsp+#0x30]     ; 4c 8b 9c 24 30 00 00 00              
    43 : cmp  rdx, #0              ; 48 83 fa 00                          
    44 : je   [46]                 ; 0f 84 03 00 00 00                    
    45 : mov  rax, r12             ; 4c 89 e0                             
    46 : add  r12, #0x08           ; 49 81 c4 08 00 00 00                 
    47 : jmp  [18]                 ; e9 3d ff ff ff                       
    48 : cmp  rax, [rsp+#0x40]     ; 48 3b 84 24 40 00 00 00              
    49 : je   [56]                 ; 0f 84 20 00 00 00                    
    50 : mov  r13, [rsp+#0x40]     ; 4c 8b ac 24 40 00 00 00              
    51 : mov  rdx, [rcx], [r13]    ; 4a 8b 14 29                          
    52 : mov  r8, [rcx], [rax]     ; 4c 8b 04 01                          
    53 : mov  [rcx], [rax], rdx    ; 48 89 14 01                          
    54 : mov  r13, [rsp+#0x40]     ; 4c 8b ac 24 40 00 00 00              
    55 : mov  [rcx], [r13], r8     ; 4e 89 04 29                          
    56 : add  [rsp+#0x40], #0x08   ; 48 81 84 24 40 00 00 00 08 00 00 00  
    57 : jmp  [12]                 ; e9 d6 fe ff ff                       
    58 : mov  rbp, [rsp+#0x70]     ; 48 8b ac 24 70 00 00 00              
    59 : mov  r12, [rsp+#0x58]     ; 4c 8b a4 24 58 00 00 00              
    60 : mov  r13, [rsp+#0x60]     ; 4c 8b ac 24 60 00 00 00              
    61 : add  rsp, #0x78           ; 48 81 c4 78 00 00 00                 
    62 : ret                       ; c3                                   
