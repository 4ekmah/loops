bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub   rsp, #0x68         ; 48 83 ec 68                 
     1 : mov   [rsp+#0x48], r12   ; 4c 89 64 24 48              
     2 : mov   [rsp+#0x50], r13   ; 4c 89 6c 24 50              
     3 : mov   [rsp+#0x58], r14   ; 4c 89 74 24 58              
     4 : mov   r12, r8            ; 4d 89 c4                    
     5 : sub   r12, rdx           ; 49 29 d4                    
     6 : mov   r13, r12           ; 4d 89 e5                    
     7 : neg   r13                ; 49 f7 dd                    
     8 : cmovs r13, r12           ; 4d 0f 48 ec                 
     9 : mov   [rsp+#0x20], r13   ; 4c 89 6c 24 20              
    10 : mov   r12, r8            ; 4d 89 c4                    
    11 : sub   r12, rdx           ; 49 29 d4                    
    12 : mov   [rsp], rcx         ; 48 89 4c 24 00              
    13 : mov   r13, r12           ; 4d 89 e5                    
    14 : mov   rcx, r13           ; 4c 89 e9                    
    15 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    16 : neg   rcx                ; 48 f7 d9                    
    17 : adc   r13, r13           ; 4d 11 ed                    
    18 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    19 : mov   [rsp+#0x28], r13   ; 4c 89 6c 24 28              
    20 : mov   r12, r9            ; 4d 89 cc                    
    21 : sub   r12, rcx           ; 49 29 cc                    
    22 : mov   [rsp], r12         ; 4c 89 64 24 00              
    23 : neg   r12                ; 49 f7 dc                    
    24 : cmovs r12, [rsp]         ; 4c 0f 48 a4 24 00 00 00 00  
    25 : mov   [rsp+#0x30], r12   ; 4c 89 64 24 30              
    26 : neg   [rsp+#0x30]        ; 48 f7 5c 24 30              
    27 : mov   r12, r9            ; 4d 89 cc                    
    28 : sub   r12, rcx           ; 49 29 cc                    
    29 : mov   [rsp], rcx         ; 48 89 4c 24 00              
    30 : mov   r13, r12           ; 4d 89 e5                    
    31 : mov   rcx, r13           ; 4c 89 e9                    
    32 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    33 : neg   rcx                ; 48 f7 d9                    
    34 : adc   r13, r13           ; 4d 11 ed                    
    35 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    36 : mov   [rsp+#0x40], r13   ; 4c 89 6c 24 40              
    37 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    38 : mov   r14, [rsp+#0x30]   ; 4c 8b b4 24 30 00 00 00     
    39 : mov   [rsp+#0x38], r13   ; 4c 89 6c 24 38              
    40 : add   [rsp+#0x38], r14   ; 4c 01 b4 24 38 00 00 00     
    41 : cmp   rdi, #0            ; 48 83 ff 00                 
    42 : je    [81]               ; 0f 84 c5 00 00 00           
    43 : mov   r12, rcx           ; 49 89 cc                    
    44 : imul  r12, rsi           ; 4c 0f af e6                 
    45 : add   r12, rdx           ; 49 01 d4                    
    46 : mov   r13, [rsp+#0x70]   ; 4c 8b ac 24 70 00 00 00     
    47 : mov   [rdi], [r12], r13b ; 46 88 2c 27                 
    48 : cmp   rdx, r8            ; 4c 39 c2                    
    49 : jne   [53]               ; 0f 85 0e 00 00 00           
    50 : cmp   rcx, r9            ; 4c 39 c9                    
    51 : jne   [53]               ; 0f 85 05 00 00 00           
    52 : jmp   [81]               ; e9 98 00 00 00              
    53 : mov   r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    54 : mov   r12, r13           ; 4d 89 ec                    
    55 : shl   r12, #0x01         ; 49 c1 e4 01                 
    56 : cmp   r12, [rsp+#0x30]   ; 4c 3b a4 24 30 00 00 00     
    57 : jl    [68]               ; 0f 8c 34 00 00 00           
    58 : cmp   rdx, r8            ; 4c 39 c2                    
    59 : jne   [61]               ; 0f 85 05 00 00 00           
    60 : jmp   [81]               ; e9 6d 00 00 00              
    61 : mov   r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    62 : mov   rax, r13           ; 4c 89 e8                    
    63 : add   rax, [rsp+#0x30]   ; 48 03 84 24 30 00 00 00     
    64 : mov   [rsp+#0x38], rax   ; 48 89 44 24 38              
    65 : mov   rax, rdx           ; 48 89 d0                    
    66 : add   rax, [rsp+#0x28]   ; 48 03 84 24 28 00 00 00     
    67 : mov   rdx, rax           ; 48 89 c2                    
    68 : cmp   r12, [rsp+#0x20]   ; 4c 3b a4 24 20 00 00 00     
    69 : jg    [80]               ; 0f 8f 34 00 00 00           
    70 : cmp   rcx, r9            ; 4c 39 c9                    
    71 : jne   [73]               ; 0f 85 05 00 00 00           
    72 : jmp   [81]               ; e9 2b 00 00 00              
    73 : mov   r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    74 : mov   rax, r13           ; 4c 89 e8                    
    75 : add   rax, [rsp+#0x20]   ; 48 03 84 24 20 00 00 00     
    76 : mov   [rsp+#0x38], rax   ; 48 89 44 24 38              
    77 : mov   rax, rcx           ; 48 89 c8                    
    78 : add   rax, [rsp+#0x40]   ; 48 03 84 24 40 00 00 00     
    79 : mov   rcx, rax           ; 48 89 c1                    
    80 : jmp   [41]               ; e9 31 ff ff ff              
    81 : mov   r12, [rsp+#0x48]   ; 4c 8b a4 24 48 00 00 00     
    82 : mov   r13, [rsp+#0x50]   ; 4c 8b ac 24 50 00 00 00     
    83 : mov   r14, [rsp+#0x58]   ; 4c 8b b4 24 58 00 00 00     
    84 : add   rsp, #0x68         ; 48 83 c4 68                 
    85 : ret                      ; c3                          
