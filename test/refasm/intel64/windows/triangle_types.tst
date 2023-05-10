triangle_types(i0, i1, i2)
     0 : sub  rsp, #0x08 ; 48 83 ec 08              
     1 : mov  [rsp], r12 ; 4c 89 64 24 00           
     2 : cmp  rcx, #0    ; 48 83 f9 00              
     3 : jle  [8]        ; 0f 8e 14 00 00 00        
     4 : cmp  rdx, #0    ; 48 83 fa 00              
     5 : jle  [8]        ; 0f 8e 0a 00 00 00        
     6 : cmp  r8, #0     ; 49 83 f8 00              
     7 : jg   [10]       ; 0f 8f 0c 00 00 00        
     8 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
     9 : jmp  [95]       ; e9 5f 01 00 00           
    10 : mov  r9, rdx    ; 49 89 d1                 
    11 : add  r9, r8     ; 4d 01 c1                 
    12 : cmp  rcx, r9    ; 4c 39 c9                 
    13 : jg   [22]       ; 0f 8f 1e 00 00 00        
    14 : mov  r9, rcx    ; 49 89 c9                 
    15 : add  r9, r8     ; 4d 01 c1                 
    16 : cmp  rdx, r9    ; 4c 39 ca                 
    17 : jg   [22]       ; 0f 8f 0f 00 00 00        
    18 : mov  r9, rcx    ; 49 89 c9                 
    19 : add  r9, rdx    ; 49 01 d1                 
    20 : cmp  r8, r9     ; 4d 39 c8                 
    21 : jle  [24]       ; 0f 8e 0c 00 00 00        
    22 : mov  rax, #0    ; 48 c7 c0 00 00 00 00     
    23 : jmp  [95]       ; e9 26 01 00 00           
    24 : cmp  rcx, rdx   ; 48 39 d1                 
    25 : jne  [30]       ; 0f 85 15 00 00 00        
    26 : cmp  rcx, r8    ; 4c 39 c1                 
    27 : jne  [30]       ; 0f 85 0c 00 00 00        
    28 : mov  rax, #0x02 ; 48 c7 c0 02 00 00 00     
    29 : jmp  [95]       ; e9 08 01 00 00           
    30 : cmp  rcx, rdx   ; 48 39 d1                 
    31 : je   [36]       ; 0f 84 12 00 00 00        
    32 : cmp  rcx, r8    ; 4c 39 c1                 
    33 : je   [36]       ; 0f 84 09 00 00 00        
    34 : cmp  rdx, r8    ; 4c 39 c2                 
    35 : jne  [38]       ; 0f 85 0c 00 00 00        
    36 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00     
    37 : jmp  [95]       ; e9 e1 00 00 00           
    38 : mov  r9, rcx    ; 49 89 c9                 
    39 : imul r9, rcx    ; 4c 0f af c9              
    40 : mov  rax, rdx   ; 48 89 d0                 
    41 : imul rax, rdx   ; 48 0f af c2              
    42 : mov  r12, r8    ; 4d 89 c4                 
    43 : imul r12, r8    ; 4d 0f af e0              
    44 : add  rax, r12   ; 4c 01 e0                 
    45 : cmp  r9, rax    ; 49 39 c1                 
    46 : je   [65]       ; 0f 84 42 00 00 00        
    47 : mov  r9, rdx    ; 49 89 d1                 
    48 : imul r9, rdx    ; 4c 0f af ca              
    49 : mov  rax, rcx   ; 48 89 c8                 
    50 : imul rax, rcx   ; 48 0f af c1              
    51 : mov  r12, r8    ; 4d 89 c4                 
    52 : imul r12, r8    ; 4d 0f af e0              
    53 : add  rax, r12   ; 4c 01 e0                 
    54 : cmp  r9, rax    ; 49 39 c1                 
    55 : je   [65]       ; 0f 84 21 00 00 00        
    56 : mov  r9, r8     ; 4d 89 c1                 
    57 : imul r9, r8     ; 4d 0f af c8              
    58 : mov  rax, rcx   ; 48 89 c8                 
    59 : imul rax, rcx   ; 48 0f af c1              
    60 : mov  r12, rdx   ; 49 89 d4                 
    61 : imul r12, rdx   ; 4c 0f af e2              
    62 : add  rax, r12   ; 4c 01 e0                 
    63 : cmp  r9, rax    ; 49 39 c1                 
    64 : jne  [67]       ; 0f 85 0c 00 00 00        
    65 : mov  rax, #0x01 ; 48 c7 c0 01 00 00 00     
    66 : jmp  [95]       ; e9 72 00 00 00           
    67 : mov  r9, rcx    ; 49 89 c9                 
    68 : imul r9, rcx    ; 4c 0f af c9              
    69 : mov  rax, rdx   ; 48 89 d0                 
    70 : imul rax, rdx   ; 48 0f af c2              
    71 : mov  r12, r8    ; 4d 89 c4                 
    72 : imul r12, r8    ; 4d 0f af e0              
    73 : add  rax, r12   ; 4c 01 e0                 
    74 : cmp  r9, rax    ; 49 39 c1                 
    75 : jg   [91]       ; 0f 8f 39 00 00 00        
    76 : mov  r9, rdx    ; 49 89 d1                 
    77 : imul r9, rdx    ; 4c 0f af ca              
    78 : mov  rax, rcx   ; 48 89 c8                 
    79 : imul rax, rcx   ; 48 0f af c1              
    80 : mov  r12, r8    ; 4d 89 c4                 
    81 : imul r12, r8    ; 4d 0f af e0              
    82 : add  rax, r12   ; 4c 01 e0                 
    83 : cmp  r9, rax    ; 49 39 c1                 
    84 : jg   [91]       ; 0f 8f 18 00 00 00        
    85 : imul r8, r8     ; 4d 0f af c0              
    86 : imul rcx, rcx   ; 48 0f af c9              
    87 : imul rdx, rdx   ; 48 0f af d2              
    88 : add  rcx, rdx   ; 48 01 d1                 
    89 : cmp  r8, rcx    ; 49 39 c8                 
    90 : jle  [93]       ; 0f 8e 0c 00 00 00        
    91 : mov  rax, #0x05 ; 48 c7 c0 05 00 00 00     
    92 : jmp  [95]       ; e9 0c 00 00 00           
    93 : mov  rax, #0x04 ; 48 c7 c0 04 00 00 00     
    94 : jmp  [95]       ; e9 00 00 00 00           
    95 : mov  r12, [rsp] ; 4c 8b a4 24 00 00 00 00  
    96 : add  rsp, #0x08 ; 48 83 c4 08              
    97 : ret             ; c3                       
