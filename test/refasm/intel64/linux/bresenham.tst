bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub   rsp, #0x58         ; 48 81 ec 58 00 00 00        
     1 : mov   [rsp+#0x48], r13   ; 4c 89 ac 24 48 00 00 00     
     2 : mov   [rsp+#0x50], r14   ; 4c 89 b4 24 50 00 00 00     
     3 : mov   rax, r8            ; 4c 89 c0                    
     4 : sub   rax, rdx           ; 48 29 d0                    
     5 : mov   r13, rax           ; 49 89 c5                    
     6 : neg   r13                ; 49 f7 dd                    
     7 : cmovs r13, rax           ; 4c 0f 48 e8                 
     8 : mov   [rsp+#0x20], r13   ; 4c 89 ac 24 20 00 00 00     
     9 : mov   rax, r8            ; 4c 89 c0                    
    10 : sub   rax, rdx           ; 48 29 d0                    
    11 : mov   [rsp], rcx         ; 48 89 8c 24 00 00 00 00     
    12 : mov   r13, rax           ; 49 89 c5                    
    13 : mov   rcx, r13           ; 4c 89 e9                    
    14 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    15 : neg   rcx                ; 48 f7 d9                    
    16 : adc   r13, r13           ; 4d 11 ed                    
    17 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    18 : mov   [rsp+#0x28], r13   ; 4c 89 ac 24 28 00 00 00     
    19 : mov   rax, r9            ; 4c 89 c8                    
    20 : sub   rax, rcx           ; 48 29 c8                    
    21 : mov   [rsp], rax         ; 48 89 84 24 00 00 00 00     
    22 : neg   rax                ; 48 f7 d8                    
    23 : cmovs rax, [rsp]         ; 48 0f 48 84 24 00 00 00 00  
    24 : mov   [rsp+#0x30], rax   ; 48 89 84 24 30 00 00 00     
    25 : neg   [rsp+#0x30]        ; 48 f7 9c 24 30 00 00 00     
    26 : mov   rax, r9            ; 4c 89 c8                    
    27 : sub   rax, rcx           ; 48 29 c8                    
    28 : mov   [rsp], rcx         ; 48 89 8c 24 00 00 00 00     
    29 : mov   r13, rax           ; 49 89 c5                    
    30 : mov   rcx, r13           ; 4c 89 e9                    
    31 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    32 : neg   rcx                ; 48 f7 d9                    
    33 : adc   r13, r13           ; 4d 11 ed                    
    34 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    35 : mov   [rsp+#0x40], r13   ; 4c 89 ac 24 40 00 00 00     
    36 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    37 : mov   r14, [rsp+#0x30]   ; 4c 8b b4 24 30 00 00 00     
    38 : mov   [rsp+#0x38], r13   ; 4c 89 ac 24 38 00 00 00     
    39 : add   [rsp+#0x38], r14   ; 4c 01 b4 24 38 00 00 00     
    40 : cmp   rdi, #0            ; 48 83 ff 00                 
    41 : je    [72]               ; 0f 84 a9 00 00 00           
    42 : mov   rax, rcx           ; 48 89 c8                    
    43 : imul  rax, rsi           ; 48 0f af c6                 
    44 : add   rax, rdx           ; 48 01 d0                    
    45 : mov   r13, [rsp+#0x60]   ; 4c 8b ac 24 60 00 00 00     
    46 : mov   [rdi], [rax], r13b ; 44 88 2c 07                 
    47 : cmp   rdx, r8            ; 4c 39 c2                    
    48 : jne   [52]               ; 0f 85 0e 00 00 00           
    49 : cmp   rcx, r9            ; 4c 39 c9                    
    50 : jne   [52]               ; 0f 85 05 00 00 00           
    51 : jmp   [72]               ; e9 7c 00 00 00              
    52 : mov   r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    53 : mov   rax, r13           ; 4c 89 e8                    
    54 : shl   rax, #0x01         ; 48 c1 e0 01                 
    55 : cmp   rax, [rsp+#0x30]   ; 48 3b 84 24 30 00 00 00     
    56 : jl    [63]               ; 0f 8c 26 00 00 00           
    57 : cmp   rdx, r8            ; 4c 39 c2                    
    58 : jne   [60]               ; 0f 85 05 00 00 00           
    59 : jmp   [72]               ; e9 51 00 00 00              
    60 : mov   r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    61 : add   [rsp+#0x38], r13   ; 4c 01 ac 24 38 00 00 00     
    62 : add   rdx, [rsp+#0x28]   ; 48 03 94 24 28 00 00 00     
    63 : cmp   rax, [rsp+#0x20]   ; 48 3b 84 24 20 00 00 00     
    64 : jg    [71]               ; 0f 8f 26 00 00 00           
    65 : cmp   rcx, r9            ; 4c 39 c9                    
    66 : jne   [68]               ; 0f 85 05 00 00 00           
    67 : jmp   [72]               ; e9 1d 00 00 00              
    68 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    69 : add   [rsp+#0x38], r13   ; 4c 01 ac 24 38 00 00 00     
    70 : add   rcx, [rsp+#0x40]   ; 48 03 8c 24 40 00 00 00     
    71 : jmp   [40]               ; e9 4d ff ff ff              
    72 : mov   r13, [rsp+#0x48]   ; 4c 8b ac 24 48 00 00 00     
    73 : mov   r14, [rsp+#0x50]   ; 4c 8b b4 24 50 00 00 00     
    74 : add   rsp, #0x58         ; 48 81 c4 58 00 00 00        
    75 : ret                      ; c3                          
