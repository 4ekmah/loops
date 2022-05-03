triangle_types(i0, i1, i2)
     0 : sub  rsp, #0x08 ; 48 83 ec 08           
     1 : mov  [rsp], rbx ; 48 89 5c 24 00        
     2 : mov  r9, #0     ; 49 c7 c1 00 00 00 00  
     3 : cmp  rcx, r9    ; 4c 39 c9              
     4 : jg   [9]        ; 0f 8f 14 00 00 00     
     5 : mov  rax, #0    ; 48 c7 c0 00 00 00 00  
     6 : mov  rax, rax   ; 48 89 c0              
     7 : jmp  [116]      ; e9 ec 01 00 00        
     8 : jmp  [114]      ; e9 dd 01 00 00        
     9 : cmp  rdx, r9    ; 4c 39 ca              
    10 : jg   [15]       ; 0f 8f 14 00 00 00     
    11 : mov  rax, #0    ; 48 c7 c0 00 00 00 00  
    12 : mov  rax, rax   ; 48 89 c0              
    13 : jmp  [116]      ; e9 cf 01 00 00        
    14 : jmp  [114]      ; e9 c0 01 00 00        
    15 : cmp  r8, r9     ; 4d 39 c8              
    16 : jg   [21]       ; 0f 8f 14 00 00 00     
    17 : mov  rax, #0    ; 48 c7 c0 00 00 00 00  
    18 : mov  rax, rax   ; 48 89 c0              
    19 : jmp  [116]      ; e9 b2 01 00 00        
    20 : jmp  [114]      ; e9 a3 01 00 00        
    21 : mov  rax, rcx   ; 48 89 c8              
    22 : add  rax, rdx   ; 48 01 d0              
    23 : mov  r10, rcx   ; 49 89 ca              
    24 : add  r10, r8    ; 4d 01 c2              
    25 : mov  r11, rdx   ; 49 89 d3              
    26 : add  r11, r8    ; 4d 01 c3              
    27 : cmp  rcx, r11   ; 4c 39 d9              
    28 : jle  [33]       ; 0f 8e 14 00 00 00     
    29 : mov  r11, #0    ; 49 c7 c3 00 00 00 00  
    30 : mov  rax, r11   ; 4c 89 d8              
    31 : jmp  [116]      ; e9 83 01 00 00        
    32 : jmp  [114]      ; e9 74 01 00 00        
    33 : cmp  rdx, r10   ; 4c 39 d2              
    34 : jle  [39]       ; 0f 8e 14 00 00 00     
    35 : mov  r10, #0    ; 49 c7 c2 00 00 00 00  
    36 : mov  rax, r10   ; 4c 89 d0              
    37 : jmp  [116]      ; e9 66 01 00 00        
    38 : jmp  [114]      ; e9 57 01 00 00        
    39 : cmp  r8, rax    ; 49 39 c0              
    40 : jle  [45]       ; 0f 8e 14 00 00 00     
    41 : mov  rax, #0    ; 48 c7 c0 00 00 00 00  
    42 : mov  rax, rax   ; 48 89 c0              
    43 : jmp  [116]      ; e9 49 01 00 00        
    44 : jmp  [114]      ; e9 3a 01 00 00        
    45 : cmp  rcx, rdx   ; 48 39 d1              
    46 : jne  [57]       ; 0f 85 31 00 00 00     
    47 : cmp  rdx, r8    ; 4c 39 c2              
    48 : jne  [53]       ; 0f 85 14 00 00 00     
    49 : mov  rax, #0x02 ; 48 c7 c0 02 00 00 00  
    50 : mov  rax, rax   ; 48 89 c0              
    51 : jmp  [116]      ; e9 23 01 00 00        
    52 : jmp  [56]       ; e9 0f 00 00 00        
    53 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00  
    54 : mov  rax, rax   ; 48 89 c0              
    55 : jmp  [116]      ; e9 0f 01 00 00        
    56 : jmp  [68]       ; e9 35 00 00 00        
    57 : cmp  rcx, r8    ; 4c 39 c1              
    58 : jne  [63]       ; 0f 85 14 00 00 00     
    59 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00  
    60 : mov  rax, rax   ; 48 89 c0              
    61 : jmp  [116]      ; e9 f2 00 00 00        
    62 : jmp  [68]       ; e9 18 00 00 00        
    63 : cmp  rdx, r8    ; 4c 39 c2              
    64 : jne  [68]       ; 0f 85 0f 00 00 00     
    65 : mov  rax, #0x03 ; 48 c7 c0 03 00 00 00  
    66 : mov  rax, rax   ; 48 89 c0              
    67 : jmp  [116]      ; e9 d5 00 00 00        
    68 : imul rcx, rcx   ; 48 0f af c9           
    69 : imul rdx, rdx   ; 48 0f af d2           
    70 : imul r8, r8     ; 4d 0f af c0           
    71 : mov  rax, rcx   ; 48 89 c8              
    72 : add  rax, rdx   ; 48 01 d0              
    73 : mov  r10, rcx   ; 49 89 ca              
    74 : add  r10, r8    ; 4d 01 c2              
    75 : mov  r11, rdx   ; 49 89 d3              
    76 : add  r11, r8    ; 4d 01 c3              
    77 : cmp  rcx, r11   ; 4c 39 d9              
    78 : jne  [83]       ; 0f 85 14 00 00 00     
    79 : mov  rbx, #0x01 ; 48 c7 c3 01 00 00 00  
    80 : mov  rax, rbx   ; 48 89 d8              
    81 : jmp  [116]      ; e9 9f 00 00 00        
    82 : jmp  [94]       ; e9 35 00 00 00        
    83 : cmp  rdx, r10   ; 4c 39 d2              
    84 : jne  [89]       ; 0f 85 14 00 00 00     
    85 : mov  rbx, #0x01 ; 48 c7 c3 01 00 00 00  
    86 : mov  rax, rbx   ; 48 89 d8              
    87 : jmp  [116]      ; e9 82 00 00 00        
    88 : jmp  [94]       ; e9 18 00 00 00        
    89 : cmp  r8, rax    ; 49 39 c0              
    90 : jne  [94]       ; 0f 85 0f 00 00 00     
    91 : mov  rbx, #0x01 ; 48 c7 c3 01 00 00 00  
    92 : mov  rax, rbx   ; 48 89 d8              
    93 : jmp  [116]      ; e9 65 00 00 00        
    94 : sub  r11, rcx   ; 49 29 cb              
    95 : sub  r10, rdx   ; 49 29 d2              
    96 : sub  rax, r8    ; 4c 29 c0              
    97 : cmp  r11, r9    ; 4d 39 cb              
    98 : jge  [103]      ; 0f 8d 14 00 00 00     
    99 : mov  r8, #0x05  ; 49 c7 c0 05 00 00 00  
   100 : mov  rax, r8    ; 4c 89 c0              
   101 : jmp  [116]      ; e9 44 00 00 00        
   102 : jmp  [114]      ; e9 35 00 00 00        
   103 : cmp  r10, r9    ; 4d 39 ca              
   104 : jge  [109]      ; 0f 8d 14 00 00 00     
   105 : mov  r8, #0x05  ; 49 c7 c0 05 00 00 00  
   106 : mov  rax, r8    ; 4c 89 c0              
   107 : jmp  [116]      ; e9 27 00 00 00        
   108 : jmp  [114]      ; e9 18 00 00 00        
   109 : cmp  rax, r9    ; 4c 39 c8              
   110 : jge  [114]      ; 0f 8d 0f 00 00 00     
   111 : mov  r9, #0x05  ; 49 c7 c1 05 00 00 00  
   112 : mov  rax, r9    ; 4c 89 c8              
   113 : jmp  [116]      ; e9 0a 00 00 00        
   114 : mov  r9, #0x04  ; 49 c7 c1 04 00 00 00  
   115 : mov  rax, r9    ; 4c 89 c8              
   116 : mov  rbx, [rsp] ; 48 8b 5c 24 00        
   117 : add  rsp, #0x08 ; 48 83 c4 08           
   118 : ret             ; c3                    
