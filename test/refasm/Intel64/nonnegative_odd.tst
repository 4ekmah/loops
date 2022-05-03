nonnegative_odd(i0, i1)
     0 : sub    rsp, #0x28       ; 48 83 ec 28           
     1 : mov    [rsp+#0x10], rbx ; 48 89 5c 24 10        
     2 : mov    [rsp+#0x18], rsi ; 48 89 74 24 18        
     3 : mov    r8, #0           ; 49 c7 c0 00 00 00 00  
     4 : mov    r9, #0           ; 49 c7 c1 00 00 00 00  
     5 : mov    rax, #0x02       ; 48 c7 c0 02 00 00 00  
     6 : mov    r10, #0x04       ; 49 c7 c2 04 00 00 00  
     7 : mov    r11, r8          ; 4d 89 c3              
     8 : sub    r11, r10         ; 4d 29 d3              
     9 : imul   rdx, r10         ; 49 0f af d2           
    10 : cmp    r8, rdx          ; 49 39 d0              
    11 : jge    [34]             ; 0f 8d 59 00 00 00     
    12 : movsxd rbx, rcx, r8     ; 4a 63 1c 01           
    13 : cmp    rbx, r9          ; 4c 39 cb              
    14 : jge    [17]             ; 0f 8d 08 00 00 00     
    15 : add    r8, r10          ; 4d 01 d0              
    16 : jmp    [10]             ; e9 e2 ff ff ff        
    17 : mov    [rsp], rax       ; 48 89 44 24 00        
    18 : mov    [rsp+#0x08], rdx ; 48 89 54 24 08        
    19 : mov    rax, rbx         ; 48 89 d8              
    20 : cqo                     ; 48 99                 
    21 : idiv   [rsp]            ; 48 f7 7c 24 00        
    22 : mov    rsi, rax         ; 48 89 c6              
    23 : mov    rax, [rsp]       ; 48 8b 44 24 00        
    24 : mov    rdx, [rsp+#0x08] ; 48 8b 54 24 08        
    25 : imul   rsi, rax         ; 48 0f af f0           
    26 : sub    rbx, rsi         ; 48 29 f3              
    27 : mov    rbx, rbx         ; 48 89 db              
    28 : cmp    rbx, r9          ; 4c 39 cb              
    29 : je     [32]             ; 0f 84 08 00 00 00     
    30 : mov    r11, r8          ; 4d 89 c3              
    31 : jmp    [34]             ; e9 08 00 00 00        
    32 : add    r8, r10          ; 4d 01 d0              
    33 : jmp    [10]             ; e9 9e ff ff ff        
    34 : mov    [rsp], rax       ; 48 89 44 24 00        
    35 : mov    [rsp+#0x08], rdx ; 48 89 54 24 08        
    36 : mov    rax, r11         ; 4c 89 d8              
    37 : cqo                     ; 48 99                 
    38 : idiv   r10              ; 49 f7 fa              
    39 : mov    r11, rax         ; 49 89 c3              
    40 : mov    rax, [rsp]       ; 48 8b 44 24 00        
    41 : mov    rdx, [rsp+#0x08] ; 48 8b 54 24 08        
    42 : mov    rax, r11         ; 4c 89 d8              
    43 : mov    rbx, [rsp+#0x10] ; 48 8b 5c 24 10        
    44 : mov    rsi, [rsp+#0x18] ; 48 8b 74 24 18        
    45 : add    rsp, #0x28       ; 48 83 c4 28           
    46 : ret                     ; c3                    
