triangle_types(i0, i1, i2)
     0 : sub  rsp, #0x08 ; 48 83 ec 08              
     1 : mov  [rsp], r12 ; 4c 89 64 24 00           
     2 : mov  r9, #0     ; 49 c7 c1 00 00 00 00     
     3 : cmp  rcx, r9    ; 4c 39 c9                 
     4 : jg   [9]        ; 0f 8f 18 00 00 00        
     5 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
     6 : jmp  [104]      ; e9 df 01 00 00           
     7 : mov  r9, #0     ; 49 c7 c1 00 00 00 00     
     8 : jmp  [103]      ; e9 cc 01 00 00           
     9 : cmp  rdx, r9    ; 4c 39 ca                 
    10 : jg   [15]       ; 0f 8f 18 00 00 00        
    11 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
    12 : jmp  [104]      ; e9 be 01 00 00           
    13 : mov  r9, #0     ; 49 c7 c1 00 00 00 00     
    14 : jmp  [103]      ; e9 ab 01 00 00           
    15 : cmp  r8, r9     ; 4d 39 c8                 
    16 : jg   [20]       ; 0f 8f 11 00 00 00        
    17 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
    18 : jmp  [104]      ; e9 9d 01 00 00           
    19 : jmp  [103]      ; e9 91 01 00 00           
    20 : mov  r9, rcx    ; 49 89 c9                 
    21 : add  r9, rdx    ; 49 01 d1                 
    22 : mov  rax, rcx   ; 48 89 c8                 
    23 : add  rax, r8    ; 4c 01 c0                 
    24 : mov  r12, rdx   ; 49 89 d4                 
    25 : add  r12, r8    ; 4d 01 c4                 
    26 : cmp  rcx, r12   ; 4c 39 e1                 
    27 : jle  [31]       ; 0f 8e 11 00 00 00        
    28 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
    29 : jmp  [104]      ; e9 71 01 00 00           
    30 : jmp  [103]      ; e9 65 01 00 00           
    31 : cmp  rdx, rax   ; 48 39 c2                 
    32 : jle  [36]       ; 0f 8e 11 00 00 00        
    33 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
    34 : jmp  [104]      ; e9 57 01 00 00           
    35 : jmp  [103]      ; e9 4b 01 00 00           
    36 : cmp  r8, r9     ; 4d 39 c8                 
    37 : jle  [41]       ; 0f 8e 11 00 00 00        
    38 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
    39 : jmp  [104]      ; e9 3d 01 00 00           
    40 : jmp  [103]      ; e9 31 01 00 00           
    41 : cmp  rcx, rdx   ; 48 39 d1                 
    42 : jne  [51]       ; 0f 85 2b 00 00 00        
    43 : cmp  rdx, r8    ; 4c 39 c2                 
    44 : jne  [48]       ; 0f 85 11 00 00 00        
    45 : mov  rax, #0x02 ; 48 c7 c0 02 00 00 00     
    46 : jmp  [104]      ; e9 1a 01 00 00           
    47 : jmp  [50]       ; e9 0c 00 00 00           
    48 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00     
    49 : jmp  [104]      ; e9 09 01 00 00           
    50 : jmp  [60]       ; e9 2f 00 00 00           
    51 : cmp  rcx, r8    ; 4c 39 c1                 
    52 : jne  [56]       ; 0f 85 11 00 00 00        
    53 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00     
    54 : jmp  [104]      ; e9 ef 00 00 00           
    55 : jmp  [60]       ; e9 15 00 00 00           
    56 : cmp  rdx, r8    ; 4c 39 c2                 
    57 : jne  [60]       ; 0f 85 0c 00 00 00        
    58 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00     
    59 : jmp  [104]      ; e9 d5 00 00 00           
    60 : imul rcx, rcx   ; 48 0f af c9              
    61 : imul rdx, rdx   ; 48 0f af d2              
    62 : imul r8, r8     ; 4d 0f af c0              
    63 : mov  r9, rcx    ; 49 89 c9                 
    64 : add  r9, rdx    ; 49 01 d1                 
    65 : mov  rax, rcx   ; 48 89 c8                 
    66 : add  rax, r8    ; 4c 01 c0                 
    67 : mov  r12, rdx   ; 49 89 d4                 
    68 : add  r12, r8    ; 4d 01 c4                 
    69 : cmp  rcx, r12   ; 4c 39 e1                 
    70 : jne  [74]       ; 0f 85 11 00 00 00        
    71 : mov  rax, #0x01 ; 48 c7 c0 01 00 00 00     
    72 : jmp  [104]      ; e9 a2 00 00 00           
    73 : jmp  [83]       ; e9 2f 00 00 00           
    74 : cmp  rdx, rax   ; 48 39 c2                 
    75 : jne  [79]       ; 0f 85 11 00 00 00        
    76 : mov  rax, #0x01 ; 48 c7 c0 01 00 00 00     
    77 : jmp  [104]      ; e9 88 00 00 00           
    78 : jmp  [83]       ; e9 15 00 00 00           
    79 : cmp  r8, r9     ; 4d 39 c8                 
    80 : jne  [83]       ; 0f 85 0c 00 00 00        
    81 : mov  rax, #0x01 ; 48 c7 c0 01 00 00 00     
    82 : jmp  [104]      ; e9 6e 00 00 00           
    83 : sub  r12, rcx   ; 49 29 cc                 
    84 : sub  rax, rdx   ; 48 29 d0                 
    85 : sub  r9, r8     ; 4d 29 c1                 
    86 : mov  rcx, #0    ; 48 c7 c1 00 00 00 00     
    87 : cmp  r12, rcx   ; 49 39 cc                 
    88 : jge  [93]       ; 0f 8d 18 00 00 00        
    89 : mov  rax, #0x05 ; 48 c7 c0 05 00 00 00     
    90 : jmp  [104]      ; e9 49 00 00 00           
    91 : mov  rcx, #0    ; 48 c7 c1 00 00 00 00     
    92 : jmp  [103]      ; e9 36 00 00 00           
    93 : cmp  rax, rcx   ; 48 39 c8                 
    94 : jge  [99]       ; 0f 8d 18 00 00 00        
    95 : mov  rax, #0x05 ; 48 c7 c0 05 00 00 00     
    96 : jmp  [104]      ; e9 28 00 00 00           
    97 : mov  rcx, #0    ; 48 c7 c1 00 00 00 00     
    98 : jmp  [103]      ; e9 15 00 00 00           
    99 : cmp  r9, rcx    ; 49 39 c9                 
   100 : jge  [103]      ; 0f 8d 0c 00 00 00        
   101 : mov  rax, #0x05 ; 48 c7 c0 05 00 00 00     
   102 : jmp  [104]      ; e9 07 00 00 00           
   103 : mov  rax, #0x04 ; 48 c7 c0 04 00 00 00     
   104 : mov  r12, [rsp] ; 4c 8b a4 24 00 00 00 00  
   105 : add  rsp, #0x08 ; 48 83 c4 08              
   106 : ret             ; c3                       
