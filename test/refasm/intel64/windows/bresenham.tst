bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub   rsp, #0x68         ; 48 81 ec 68 00 00 00        
     1 : mov   [rsp+#0x48], r12   ; 4c 89 a4 24 48 00 00 00     
     2 : mov   [rsp+#0x50], r13   ; 4c 89 ac 24 50 00 00 00     
     3 : mov   [rsp+#0x58], r14   ; 4c 89 b4 24 58 00 00 00     
     4 : mov   rax, [rsp+#0x90]   ; 48 8b 84 24 90 00 00 00     
     5 : mov   r12, rax           ; 49 89 c4                    
     6 : sub   r12, r8            ; 4d 29 c4                    
     7 : mov   r13, r12           ; 4d 89 e5                    
     8 : neg   r13                ; 49 f7 dd                    
     9 : cmovs r13, r12           ; 4d 0f 48 ec                 
    10 : mov   [rsp+#0x20], r13   ; 4c 89 ac 24 20 00 00 00     
    11 : mov   r12, rax           ; 49 89 c4                    
    12 : sub   r12, r8            ; 4d 29 c4                    
    13 : mov   [rsp], rcx         ; 48 89 8c 24 00 00 00 00     
    14 : mov   r13, r12           ; 4d 89 e5                    
    15 : mov   rcx, r13           ; 4c 89 e9                    
    16 : sar   r13, #0x3f         ; 49 c1 fd 3f                 
    17 : neg   rcx                ; 48 f7 d9                    
    18 : adc   r13, r13           ; 4d 11 ed                    
    19 : mov   rcx, [rsp]         ; 48 8b 8c 24 00 00 00 00     
    20 : mov   [rsp+#0x28], r13   ; 4c 89 ac 24 28 00 00 00     
    21 : mov   r13, [rsp+#0x98]   ; 4c 8b ac 24 98 00 00 00     
    22 : mov   r12, r13           ; 4d 89 ec                    
    23 : sub   r12, r9            ; 4d 29 cc                    
    24 : mov   [rsp], r12         ; 4c 89 a4 24 00 00 00 00     
    25 : neg   r12                ; 49 f7 dc                    
    26 : cmovs r12, [rsp]         ; 4c 0f 48 a4 24 00 00 00 00  
    27 : mov   [rsp+#0x30], r12   ; 4c 89 a4 24 30 00 00 00     
    28 : neg   [rsp+#0x30]        ; 48 f7 9c 24 30 00 00 00     
    29 : mov   r13, [rsp+#0x98]   ; 4c 8b ac 24 98 00 00 00     
    30 : mov   r12, r13           ; 4d 89 ec                    
    31 : sub   r12, r9            ; 4d 29 cc                    
    32 : mov   [rsp], rcx         ; 48 89 8c 24 00 00 00 00     
    33 : mov   r13, r12           ; 4d 89 e5                    
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
    45 : je    [76]               ; 0f 84 b3 00 00 00           
    46 : mov   r12, r9            ; 4d 89 cc                    
    47 : imul  r12, rdx           ; 4c 0f af e2                 
    48 : add   r12, r8            ; 4d 01 c4                    
    49 : mov   r13, [rsp+#0xa0]   ; 4c 8b ac 24 a0 00 00 00     
    50 : mov   [rcx], [r12], r13b ; 46 88 2c 21                 
    51 : cmp   r8, rax            ; 49 39 c0                    
    52 : jne   [56]               ; 0f 85 13 00 00 00           
    53 : cmp   r9, [rsp+#0x98]    ; 4c 3b 8c 24 98 00 00 00     
    54 : jne   [56]               ; 0f 85 05 00 00 00           
    55 : jmp   [76]               ; e9 81 00 00 00              
    56 : mov   r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    57 : mov   r12, r13           ; 4d 89 ec                    
    58 : shl   r12, #0x01         ; 49 c1 e4 01                 
    59 : cmp   r12, [rsp+#0x30]   ; 4c 3b a4 24 30 00 00 00     
    60 : jl    [67]               ; 0f 8c 26 00 00 00           
    61 : cmp   r8, rax            ; 49 39 c0                    
    62 : jne   [64]               ; 0f 85 05 00 00 00           
    63 : jmp   [76]               ; e9 56 00 00 00              
    64 : mov   r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    65 : add   [rsp+#0x38], r13   ; 4c 01 ac 24 38 00 00 00     
    66 : add   r8, [rsp+#0x28]    ; 4c 03 84 24 28 00 00 00     
    67 : cmp   r12, [rsp+#0x20]   ; 4c 3b a4 24 20 00 00 00     
    68 : jg    [75]               ; 0f 8f 2b 00 00 00           
    69 : cmp   r9, [rsp+#0x98]    ; 4c 3b 8c 24 98 00 00 00     
    70 : jne   [72]               ; 0f 85 05 00 00 00           
    71 : jmp   [76]               ; e9 1d 00 00 00              
    72 : mov   r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    73 : add   [rsp+#0x38], r13   ; 4c 01 ac 24 38 00 00 00     
    74 : add   r9, [rsp+#0x40]    ; 4c 03 8c 24 40 00 00 00     
    75 : jmp   [44]               ; e9 43 ff ff ff              
    76 : mov   r12, [rsp+#0x48]   ; 4c 8b a4 24 48 00 00 00     
    77 : mov   r13, [rsp+#0x50]   ; 4c 8b ac 24 50 00 00 00     
    78 : mov   r14, [rsp+#0x58]   ; 4c 8b b4 24 58 00 00 00     
    79 : add   rsp, #0x68         ; 48 81 c4 68 00 00 00        
    80 : ret                      ; c3                          
