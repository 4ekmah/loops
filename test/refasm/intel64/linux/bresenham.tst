bresenham(i0, i1, i2, i3, i4, i5, i6)
     0 : sub   rsp, #0x68       ; 48 81 ec 68 00 00 00        
     1 : mov   [rsp+#0x48], r12 ; 4c 89 a4 24 48 00 00 00     
     2 : mov   [rsp+#0x50], r13 ; 4c 89 ac 24 50 00 00 00     
     3 : mov   [rsp+#0x58], r14 ; 4c 89 b4 24 58 00 00 00     
     4 : mov   rax, [rsp+#0x70] ; 48 8b 84 24 70 00 00 00     
     5 : mov   r12, r8          ; 4d 89 c4                    
     6 : sub   r12, rdx         ; 49 29 d4                    
     7 : mov   r13, r12         ; 4d 89 e5                    
     8 : neg   r13              ; 49 f7 dd                    
     9 : cmovs r13, r12         ; 4d 0f 48 ec                 
    10 : mov   [rsp+#0x20], r13 ; 4c 89 ac 24 20 00 00 00     
    11 : mov   r12, r8          ; 4d 89 c4                    
    12 : sub   r12, rdx         ; 49 29 d4                    
    13 : mov   [rsp], rcx       ; 48 89 8c 24 00 00 00 00     
    14 : mov   r13, r12         ; 4d 89 e5                    
    15 : mov   rcx, r13         ; 4c 89 e9                    
    16 : sar   r13, #0x3f       ; 49 c1 fd 3f                 
    17 : neg   rcx              ; 48 f7 d9                    
    18 : adc   r13, r13         ; 4d 11 ed                    
    19 : mov   rcx, [rsp]       ; 48 8b 8c 24 00 00 00 00     
    20 : mov   [rsp+#0x28], r13 ; 4c 89 ac 24 28 00 00 00     
    21 : mov   r12, r9          ; 4d 89 cc                    
    22 : sub   r12, rcx         ; 49 29 cc                    
    23 : mov   [rsp], r12       ; 4c 89 a4 24 00 00 00 00     
    24 : neg   r12              ; 49 f7 dc                    
    25 : cmovs r12, [rsp]       ; 4c 0f 48 a4 24 00 00 00 00  
    26 : mov   [rsp+#0x30], r12 ; 4c 89 a4 24 30 00 00 00     
    27 : neg   [rsp+#0x30]      ; 48 f7 9c 24 30 00 00 00     
    28 : mov   r12, r9          ; 4d 89 cc                    
    29 : sub   r12, rcx         ; 49 29 cc                    
    30 : mov   [rsp], rcx       ; 48 89 8c 24 00 00 00 00     
    31 : mov   r13, r12         ; 4d 89 e5                    
    32 : mov   rcx, r13         ; 4c 89 e9                    
    33 : sar   r13, #0x3f       ; 49 c1 fd 3f                 
    34 : neg   rcx              ; 48 f7 d9                    
    35 : adc   r13, r13         ; 4d 11 ed                    
    36 : mov   rcx, [rsp]       ; 48 8b 8c 24 00 00 00 00     
    37 : mov   [rsp+#0x40], r13 ; 4c 89 ac 24 40 00 00 00     
    38 : mov   r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00     
    39 : mov   r14, [rsp+#0x30] ; 4c 8b b4 24 30 00 00 00     
    40 : mov   [rsp+#0x38], r13 ; 4c 89 ac 24 38 00 00 00     
    41 : add   [rsp+#0x38], r14 ; 4c 01 b4 24 38 00 00 00     
    42 : cmp   rdi, #0          ; 48 83 ff 00                 
    43 : je    [73]             ; 0f 84 a1 00 00 00           
    44 : mov   r12, rcx         ; 49 89 cc                    
    45 : imul  r12, rsi         ; 4c 0f af e6                 
    46 : add   r12, rdx         ; 49 01 d4                    
    47 : mov   [rdi], [r12], al ; 42 88 04 27                 
    48 : cmp   rdx, r8          ; 4c 39 c2                    
    49 : jne   [53]             ; 0f 85 0e 00 00 00           
    50 : cmp   rcx, r9          ; 4c 39 c9                    
    51 : jne   [53]             ; 0f 85 05 00 00 00           
    52 : jmp   [73]             ; e9 7c 00 00 00              
    53 : mov   r13, [rsp+#0x38] ; 4c 8b ac 24 38 00 00 00     
    54 : mov   r12, r13         ; 4d 89 ec                    
    55 : shl   r12, #0x01       ; 49 c1 e4 01                 
    56 : cmp   r12, [rsp+#0x30] ; 4c 3b a4 24 30 00 00 00     
    57 : jl    [64]             ; 0f 8c 26 00 00 00           
    58 : cmp   rdx, r8          ; 4c 39 c2                    
    59 : jne   [61]             ; 0f 85 05 00 00 00           
    60 : jmp   [73]             ; e9 51 00 00 00              
    61 : mov   r13, [rsp+#0x30] ; 4c 8b ac 24 30 00 00 00     
    62 : add   [rsp+#0x38], r13 ; 4c 01 ac 24 38 00 00 00     
    63 : add   rdx, [rsp+#0x28] ; 48 03 94 24 28 00 00 00     
    64 : cmp   r12, [rsp+#0x20] ; 4c 3b a4 24 20 00 00 00     
    65 : jg    [72]             ; 0f 8f 26 00 00 00           
    66 : cmp   rcx, r9          ; 4c 39 c9                    
    67 : jne   [69]             ; 0f 85 05 00 00 00           
    68 : jmp   [73]             ; e9 1d 00 00 00              
    69 : mov   r13, [rsp+#0x20] ; 4c 8b ac 24 20 00 00 00     
    70 : add   [rsp+#0x38], r13 ; 4c 01 ac 24 38 00 00 00     
    71 : add   rcx, [rsp+#0x40] ; 48 03 8c 24 40 00 00 00     
    72 : jmp   [42]             ; e9 55 ff ff ff              
    73 : mov   r12, [rsp+#0x48] ; 4c 8b a4 24 48 00 00 00     
    74 : mov   r13, [rsp+#0x50] ; 4c 8b ac 24 50 00 00 00     
    75 : mov   r14, [rsp+#0x58] ; 4c 8b b4 24 58 00 00 00     
    76 : add   rsp, #0x68       ; 48 81 c4 68 00 00 00        
    77 : ret                    ; c3                          
