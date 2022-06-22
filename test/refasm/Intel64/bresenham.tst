bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub   rsp, #0x48         ; 48 83 ec 48                 
     1 : mov   [rsp+#0x30], r12   ; 4c 89 64 24 30              
     2 : mov   [rsp+#0x38], r13   ; 4c 89 6c 24 38              
     3 : mov   [rsp+#0x40], r14   ; 4c 89 74 24 40              
     4 : mov   r13, [rsp+#0x70]   ; 4c 8b ac 24 70 00 00 00     
     5 : mov   r12, r13           ; 4d 89 ec                    
     6 : sub   r12, r8            ; 4d 29 c4                    
     7 : mov   r13, r12           ; 4d 89 e5                    
     8 : neg   r13                ; 49 f7 dd                    
     9 : cmovs r13, r12           ; 4d 0f 48 ec                 
    10 : mov   [rsp+#0x08], r13   ; 4c 89 6c 24 08              
    11 : mov   r13, [rsp+#0x70]   ; 4c 8b ac 24 70 00 00 00     
    12 : mov   r12, r13           ; 4d 89 ec                    
    13 : sub   r12, r8            ; 4d 29 c4                    
    14 : mov   [rsp], rcx         ; 48 89 4c 24 00              
    15 : mov   r13, r12           ; 4d 89 e5                    
    16 : mov   rcx, r13           ; 4c 89 e9                    
    17 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    18 : neg   rcx                ; 48 f7 d9                    
    19 : adc   r13, r13           ; 4d 11 ed                    
    20 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    21 : mov   [rsp+#0x10], r13   ; 4c 89 6c 24 10              
    22 : mov   r13, [rsp+#0x78]   ; 4c 8b ac 24 78 00 00 00     
    23 : mov   r12, r13           ; 4d 89 ec                    
    24 : sub   r12, r9            ; 4d 29 cc                    
    25 : mov   [rsp], r12         ; 4c 89 64 24 00              
    26 : neg   r12                ; 49 f7 dc                    
    27 : cmovs r12, [rsp]         ; 4c 0f 48 a4 24 00 00 00 00  
    28 : mov   [rsp+#0x18], r12   ; 4c 89 64 24 18              
    29 : neg   [rsp+#0x18]        ; 48 f7 5c 24 18              
    30 : mov   r13, [rsp+#0x78]   ; 4c 8b ac 24 78 00 00 00     
    31 : mov   r12, r13           ; 4d 89 ec                    
    32 : sub   r12, r9            ; 4d 29 cc                    
    33 : mov   [rsp], rcx         ; 48 89 4c 24 00              
    34 : mov   r13, r12           ; 4d 89 e5                    
    35 : mov   rcx, r13           ; 4c 89 e9                    
    36 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    37 : neg   rcx                ; 48 f7 d9                    
    38 : adc   r13, r13           ; 4d 11 ed                    
    39 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    40 : mov   [rsp+#0x28], r13   ; 4c 89 6c 24 28              
    41 : mov   r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    42 : mov   r14, [rsp+#0x18]   ; 4c 8b b4 24 18 00 00 00     
    43 : mov   [rsp+#0x20], r13   ; 4c 89 6c 24 20              
    44 : add   [rsp+#0x20], r14   ; 4c 01 74 24 20              
    45 : cmp   rcx, #0            ; 48 83 f9 00                 
    46 : je    [85]               ; 0f 84 bb 00 00 00           
    47 : mov   r12, r9            ; 4d 89 cc                    
    48 : imul  r12, rdx           ; 4c 0f af e2                 
    49 : add   r12, r8            ; 4d 01 c4                    
    50 : mov   r13, [rsp+#0x80]   ; 4c 8b ac 24 80 00 00 00     
    51 : mov   [rcx], [r12], r13b ; 46 88 2c 21                 
    52 : cmp   r8, [rsp+#0x70]    ; 4c 3b 44 24 70              
    53 : jne   [57]               ; 0f 85 10 00 00 00           
    54 : cmp   r9, [rsp+#0x78]    ; 4c 3b 4c 24 78              
    55 : jne   [57]               ; 0f 85 05 00 00 00           
    56 : jmp   [85]               ; e9 8a 00 00 00              
    57 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    58 : mov   r12, r13           ; 4d 89 ec                    
    59 : shl   r12, #0x01         ; 49 c1 e4 01                 
    60 : cmp   r12, [rsp+#0x18]   ; 4c 3b 64 24 18              
    61 : jl    [72]               ; 0f 8c 30 00 00 00           
    62 : cmp   r8, [rsp+#0x70]    ; 4c 3b 44 24 70              
    63 : jne   [65]               ; 0f 85 05 00 00 00           
    64 : jmp   [85]               ; e9 60 00 00 00              
    65 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    66 : mov   rax, r13           ; 4c 89 e8                    
    67 : add   rax, [rsp+#0x18]   ; 48 03 44 24 18              
    68 : mov   [rsp+#0x20], rax   ; 48 89 44 24 20              
    69 : mov   rax, r8            ; 4c 89 c0                    
    70 : add   rax, [rsp+#0x10]   ; 48 03 44 24 10              
    71 : mov   r8, rax            ; 49 89 c0                    
    72 : cmp   r12, [rsp+#0x08]   ; 4c 3b 64 24 08              
    73 : jg    [84]               ; 0f 8f 30 00 00 00           
    74 : cmp   r9, [rsp+#0x78]    ; 4c 3b 4c 24 78              
    75 : jne   [77]               ; 0f 85 05 00 00 00           
    76 : jmp   [85]               ; e9 25 00 00 00              
    77 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    78 : mov   rax, r13           ; 4c 89 e8                    
    79 : add   rax, [rsp+#0x08]   ; 48 03 44 24 08              
    80 : mov   [rsp+#0x20], rax   ; 48 89 44 24 20              
    81 : mov   rax, r9            ; 4c 89 c8                    
    82 : add   rax, [rsp+#0x28]   ; 48 03 44 24 28              
    83 : mov   r9, rax            ; 49 89 c1                    
    84 : jmp   [45]               ; e9 3b ff ff ff              
    85 : mov   r12, [rsp+#0x30]   ; 4c 8b a4 24 30 00 00 00     
    86 : mov   r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    87 : mov   r14, [rsp+#0x40]   ; 4c 8b b4 24 40 00 00 00     
    88 : add   rsp, #0x48         ; 48 83 c4 48                 
    89 : ret                      ; c3                          
