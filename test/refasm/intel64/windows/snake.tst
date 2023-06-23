snake(i0, i1, i2)
     0 : sub   rsp, #0x98           ; 48 81 ec 98 00 00 00                 
     1 : mov   [rsp+#0x70], rdx     ; 48 89 94 24 70 00 00 00              
     2 : mov   [rsp+#0x68], r8      ; 4c 89 84 24 68 00 00 00              
     3 : mov   [rsp+#0x78], r12     ; 4c 89 a4 24 78 00 00 00              
     4 : mov   [rsp+#0x80], r13     ; 4c 89 ac 24 80 00 00 00              
     5 : mov   [rsp+#0x90], rbp     ; 48 89 ac 24 90 00 00 00              
     6 : mov   rbp, rsp             ; 48 89 e5                             
     7 : add   rbp, #0x90           ; 48 81 c5 90 00 00 00                 
     8 : mov   r13, [rsp+#0x70]     ; 4c 8b ac 24 70 00 00 00              
     9 : mov   r9, r13              ; 4d 89 e9                             
    10 : add   r9, [rsp+#0x68]      ; 4c 03 8c 24 68 00 00 00              
    11 : mov   [rsp+#0x60], r9      ; 4c 89 8c 24 60 00 00 00              
    12 : sub   [rsp+#0x60], #0x01   ; 48 81 ac 24 60 00 00 00 01 00 00 00  
    13 : mov   [rsp+#0x58], #0      ; 48 c7 84 24 58 00 00 00 00 00 00 00  
    14 : mov   [rsp+#0x50], #0x01   ; 48 c7 84 24 50 00 00 00 01 00 00 00  
    15 : mov   r13, [rsp+#0x50]     ; 4c 8b ac 24 50 00 00 00              
    16 : neg   r13                  ; 49 f7 dd                             
    17 : mov   [rsp+#0x40], r13     ; 4c 89 ac 24 40 00 00 00              
    18 : mov   [rsp+#0x48], #0      ; 48 c7 84 24 48 00 00 00 00 00 00 00  
    19 : mov   r13, [rsp+#0x60]     ; 4c 8b ac 24 60 00 00 00              
    20 : cmp   [rsp+#0x48], r13     ; 4c 39 ac 24 48 00 00 00              
    21 : jge   [100]                ; 0f 8d d5 01 00 00                    
    22 : xor   r12, r12             ; 4d 31 e4                             
    23 : xor   rax, rax             ; 48 31 c0                             
    24 : mov   r13, [rsp+#0x48]     ; 4c 8b ac 24 48 00 00 00              
    25 : mov   r9, r13              ; 4d 89 e9                             
    26 : and   r9, #0x01            ; 49 81 e1 01 00 00 00                 
    27 : cmp   r9, #0               ; 49 83 f9 00                          
    28 : je    [44]                 ; 0f 84 49 00 00 00                    
    29 : mov   r13, [rsp+#0x68]     ; 4c 8b ac 24 68 00 00 00              
    30 : mov   r9, r13              ; 4d 89 e9                             
    31 : sub   r9, #0x01            ; 49 81 e9 01 00 00 00                 
    32 : mov   r13, [rsp+#0x48]     ; 4c 8b ac 24 48 00 00 00              
    33 : mov   r12, r13             ; 4d 89 ec                             
    34 : cmp   r12, r9              ; 4d 39 cc                             
    35 : cmovg r12, r9              ; 4d 0f 4f e1                          
    36 : mov   r13, [rsp+#0x48]     ; 4c 8b ac 24 48 00 00 00              
    37 : mov   r8, r13              ; 4d 89 e8                             
    38 : sub   r8, r9               ; 4d 29 c8                             
    39 : xor   rdx, rdx             ; 48 31 d2                             
    40 : cmp   [rsp+#0x48], r9      ; 4c 39 8c 24 48 00 00 00              
    41 : mov   rax, rdx             ; 48 89 d0                             
    42 : cmovg rax, r8              ; 49 0f 4f c0                          
    43 : jmp   [58]                 ; e9 44 00 00 00                       
    44 : mov   r13, [rsp+#0x70]     ; 4c 8b ac 24 70 00 00 00              
    45 : mov   rdx, r13             ; 4c 89 ea                             
    46 : sub   rdx, #0x01           ; 48 81 ea 01 00 00 00                 
    47 : mov   r13, [rsp+#0x48]     ; 4c 8b ac 24 48 00 00 00              
    48 : mov   r8, r13              ; 4d 89 e8                             
    49 : sub   r8, rdx              ; 49 29 d0                             
    50 : xor   r9, r9               ; 4d 31 c9                             
    51 : cmp   [rsp+#0x48], rdx     ; 48 39 94 24 48 00 00 00              
    52 : mov   r12, r9              ; 4d 89 cc                             
    53 : cmovg r12, r8              ; 4d 0f 4f e0                          
    54 : mov   r13, [rsp+#0x48]     ; 4c 8b ac 24 48 00 00 00              
    55 : mov   rax, r13             ; 4c 89 e8                             
    56 : cmp   rax, rdx             ; 48 39 d0                             
    57 : cmovg rax, rdx             ; 48 0f 4f c2                          
    58 : cmp   r12, #0              ; 49 83 fc 00                          
    59 : jl    [96]                 ; 0f 8c fb 00 00 00                    
    60 : cmp   r12, [rsp+#0x68]     ; 4c 3b a4 24 68 00 00 00              
    61 : jge   [96]                 ; 0f 8d ed 00 00 00                    
    62 : cmp   rax, #0              ; 48 83 f8 00                          
    63 : jl    [96]                 ; 0f 8c e3 00 00 00                    
    64 : cmp   rax, [rsp+#0x70]     ; 48 3b 84 24 70 00 00 00              
    65 : jge   [96]                 ; 0f 8d d5 00 00 00                    
    66 : mov   rdx, #0x7ff7f11ce0f0 ; 48 ba f0 e0 1c f1 f7 7f 00 00        
    67 : mov   [rsp], rax           ; 48 89 84 24 00 00 00 00              
    68 : mov   [rsp+#0x08], rcx     ; 48 89 8c 24 08 00 00 00              
    69 : mov   [rsp+#0x10], rdx     ; 48 89 94 24 10 00 00 00              
    70 : mov   [rsp+#0x18], r8      ; 4c 89 84 24 18 00 00 00              
    71 : mov   [rsp+#0x20], r9      ; 4c 89 8c 24 20 00 00 00              
    72 : mov   [rsp+#0x28], r10     ; 4c 89 94 24 28 00 00 00              
    73 : mov   [rsp+#0x30], r11     ; 4c 89 9c 24 30 00 00 00              
    74 : mov   rcx, r12             ; 4c 89 e1                             
    75 : mov   rdx, rax             ; 48 89 c2                             
    76 : mov   r10, [rsp+#0x10]     ; 4c 8b 94 24 10 00 00 00              
    77 : sub   rsp, #0x20           ; 48 81 ec 20 00 00 00                 
    78 : call  [r10]                ; 41 ff d2                             
    79 : add   rsp, #0x20           ; 48 81 c4 20 00 00 00                 
    80 : mov   rax, [rsp]           ; 48 8b 84 24 00 00 00 00              
    81 : mov   rcx, [rsp+#0x08]     ; 48 8b 8c 24 08 00 00 00              
    82 : mov   rdx, [rsp+#0x10]     ; 48 8b 94 24 10 00 00 00              
    83 : mov   r8, [rsp+#0x18]      ; 4c 8b 84 24 18 00 00 00              
    84 : mov   r9, [rsp+#0x20]      ; 4c 8b 8c 24 20 00 00 00              
    85 : mov   r10, [rsp+#0x28]     ; 4c 8b 94 24 28 00 00 00              
    86 : mov   r11, [rsp+#0x30]     ; 4c 8b 9c 24 30 00 00 00              
    87 : mov   rdx, rax             ; 48 89 c2                             
    88 : imul  rdx, [rsp+#0x68]     ; 48 0f af 94 24 68 00 00 00           
    89 : add   rdx, r12             ; 4c 01 e2                             
    90 : mov   r13, [rsp+#0x58]     ; 4c 8b ac 24 58 00 00 00              
    91 : mov   [rcx], [rdx], r13b   ; 44 88 2c 11                          
    92 : add   [rsp+#0x58], #0x01   ; 48 81 84 24 58 00 00 00 01 00 00 00  
    93 : add   r12, [rsp+#0x50]     ; 4c 03 a4 24 50 00 00 00              
    94 : add   rax, [rsp+#0x40]     ; 48 03 84 24 40 00 00 00              
    95 : jmp   [58]                 ; e9 fb fe ff ff                       
    96 : neg   [rsp+#0x50]          ; 48 f7 9c 24 50 00 00 00              
    97 : neg   [rsp+#0x40]          ; 48 f7 9c 24 40 00 00 00              
    98 : add   [rsp+#0x48], #0x01   ; 48 81 84 24 48 00 00 00 01 00 00 00  
    99 : jmp   [19]                 ; e9 15 fe ff ff                       
   100 : mov   rbp, [rsp+#0x90]     ; 48 8b ac 24 90 00 00 00              
   101 : mov   r12, [rsp+#0x78]     ; 4c 8b a4 24 78 00 00 00              
   102 : mov   r13, [rsp+#0x80]     ; 4c 8b ac 24 80 00 00 00              
   103 : add   rsp, #0x98           ; 48 81 c4 98 00 00 00                 
   104 : ret                        ; c3                                   
