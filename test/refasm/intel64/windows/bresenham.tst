bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub   rsp, #0x58         ; 48 81 ec 58 00 00 00        
     1 : mov   [rsp+#0x48], r13   ; 4c 89 ac 24 48 00 00 00     
     2 : mov   [rsp+#0x50], r14   ; 4c 89 b4 24 50 00 00 00     
     3 : mov   r13, [rsp+#0x80]   ; 4c 8b ac 24 80 00 00 00     
     4 : mov   rax, r13           ; 4c 89 e8                    
     5 : sub   rax, r8            ; 4c 29 c0                    
     6 : mov   r13, rax           ; 49 89 c5                    
     7 : neg   r13                ; 49 f7 dd                    
     8 : cmovs r13, rax           ; 4c 0f 48 e8                 
     9 : mov   [rsp+#0x20], r13   ; 4c 89 ac 24 20 00 00 00     
    10 : mov   r13, [rsp+#0x80]   ; 4c 8b ac 24 80 00 00 00     
    11 : mov   rax, r13           ; 4c 89 e8                    
    12 : sub   rax, r8            ; 4c 29 c0                    
    13 : mov   [rsp], rcx         ; 48 89 8c 24 00 00 00 00     
    14 : mov   r13, rax           ; 49 89 c5                    
    15 : mov   rcx, r13           ; 4c 89 e9                    
    16 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    17 : neg   rcx                ; 48 f7 d9                    
    18 : adc   r13, r13           ; 4d 11 ed                    
    19 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    20 : mov   [rsp+#0x28], r13   ; 4c 89 ac 24 28 00 00 00     
    21 : mov   r13, [rsp+#0x88]   ; 4c 8b ac 24 88 00 00 00     
    22 : mov   rax, r13           ; 4c 89 e8                    
    23 : sub   rax, r9            ; 4c 29 c8                    
    24 : mov   [rsp], rax         ; 48 89 84 24 00 00 00 00     
    25 : neg   rax                ; 48 f7 d8                    
    26 : cmovs rax, [rsp]         ; 48 0f 48 84 24 00 00 00 00  
    27 : mov   [rsp+#0x30], rax   ; 48 89 84 24 30 00 00 00     
    28 : neg   [rsp+#0x30]        ; 48 f7 9c 24 30 00 00 00     
    29 : mov   r13, [rsp+#0x88]   ; 4c 8b ac 24 88 00 00 00     
    30 : mov   rax, r13           ; 4c 89 e8                    
    31 : sub   rax, r9            ; 4c 29 c8                    
    32 : mov   [rsp], rcx         ; 48 89 8c 24 00 00 00 00     
    33 : mov   r13, rax           ; 49 89 c5                    
    34 : mov   rcx, r13           ; 4c 89 e9                    
    35 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    36 : neg   rcx                ; 48 f7 d9                    
    37 : adc   r13, r13           ; 4d 11 ed                    
    38 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    39 : mov   [rsp+#0x40], r13   ; 4c 89 ac 24 40 00 00 00     
    40 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    41 : mov   r14, [rsp+#0x30]   ; 4c 8b b4 24 30 00 00 00     
    42 : mov   [rsp+#0x38], r13   ; 4c 89 ac 24 38 00 00 00     
    43 : add   [rsp+#0x38], r14   ; 4c 01 b4 24 38 00 00 00     
    44 : cmp   rcx, #0            ; 48 83 f9 00                 
    45 : je    [76]               ; 0f 84 bd 00 00 00           
    46 : mov   rax, r9            ; 4c 89 c8                    
    47 : imul  rax, rdx           ; 48 0f af c2                 
    48 : add   rax, r8            ; 4c 01 c0                    
    49 : mov   r13, [rsp+#0x90]   ; 4c 8b ac 24 90 00 00 00     
    50 : mov   [rcx], [rax], r13b ; 44 88 2c 01                 
    51 : cmp   r8, [rsp+#0x80]    ; 4c 3b 84 24 80 00 00 00     
    52 : jne   [56]               ; 0f 85 13 00 00 00           
    53 : cmp   r9, [rsp+#0x88]    ; 4c 3b 8c 24 88 00 00 00     
    54 : jne   [56]               ; 0f 85 05 00 00 00           
    55 : jmp   [76]               ; e9 86 00 00 00              
    56 : mov   r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    57 : mov   rax, r13           ; 4c 89 e8                    
    58 : shl   rax, #0x01         ; 48 c1 e0 01                 
    59 : cmp   rax, [rsp+#0x30]   ; 48 3b 84 24 30 00 00 00     
    60 : jl    [67]               ; 0f 8c 2b 00 00 00           
    61 : cmp   r8, [rsp+#0x80]    ; 4c 3b 84 24 80 00 00 00     
    62 : jne   [64]               ; 0f 85 05 00 00 00           
    63 : jmp   [76]               ; e9 56 00 00 00              
    64 : mov   r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    65 : add   [rsp+#0x38], r13   ; 4c 01 ac 24 38 00 00 00     
    66 : add   r8, [rsp+#0x28]    ; 4c 03 84 24 28 00 00 00     
    67 : cmp   rax, [rsp+#0x20]   ; 48 3b 84 24 20 00 00 00     
    68 : jg    [75]               ; 0f 8f 2b 00 00 00           
    69 : cmp   r9, [rsp+#0x88]    ; 4c 3b 8c 24 88 00 00 00     
    70 : jne   [72]               ; 0f 85 05 00 00 00           
    71 : jmp   [76]               ; e9 1d 00 00 00              
    72 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    73 : add   [rsp+#0x38], r13   ; 4c 01 ac 24 38 00 00 00     
    74 : add   r9, [rsp+#0x40]    ; 4c 03 8c 24 40 00 00 00     
    75 : jmp   [44]               ; e9 39 ff ff ff              
    76 : mov   r13, [rsp+#0x48]   ; 4c 8b ac 24 48 00 00 00     
    77 : mov   r14, [rsp+#0x50]   ; 4c 8b b4 24 50 00 00 00     
    78 : add   rsp, #0x58         ; 48 81 c4 58 00 00 00        
    79 : ret                      ; c3                          
