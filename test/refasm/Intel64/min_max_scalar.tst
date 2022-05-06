min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x48         ; 48 83 ec 48                 
     1 : mov    [rsp+#0x38], r12   ; 4c 89 64 24 38              
     2 : mov    [rsp+#0x40], r13   ; 4c 89 6c 24 40              
     3 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
     4 : mov    [rsp+#0x18], #0    ; 48 c7 44 24 18 00 00 00 00  
     5 : mov    [rsp+#0x10], #0    ; 48 c7 44 24 10 00 00 00 00  
     6 : movsxd r13, rcx           ; 4c 63 29                    
     7 : mov    [rsp+#0x30], r13   ; 4c 89 6c 24 30              
     8 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
     9 : mov    [rsp+#0x20], r13   ; 4c 89 6c 24 20              
    10 : mov    [rsp+#0x28], #0x04 ; 48 c7 44 24 28 04 00 00 00  
    11 : imul   rdx, [rsp+#0x28]   ; 48 0f af 54 24 28           
    12 : movsxd r12, rcx, rax      ; 4c 63 24 01                 
    13 : cmp    r12, [rsp+#0x30]   ; 4c 3b 64 24 30              
    14 : jge    [17]               ; 0f 8d 0a 00 00 00           
    15 : mov    [rsp+#0x30], r12   ; 4c 89 64 24 30              
    16 : mov    [rsp+#0x18], rax   ; 48 89 44 24 18              
    17 : cmp    r12, [rsp+#0x20]   ; 4c 3b 64 24 20              
    18 : jle    [21]               ; 0f 8e 0a 00 00 00           
    19 : mov    [rsp+#0x20], r12   ; 4c 89 64 24 20              
    20 : mov    [rsp+#0x10], rax   ; 48 89 44 24 10              
    21 : add    rax, [rsp+#0x28]   ; 48 03 44 24 28              
    22 : cmp    rax, rdx           ; 48 39 d0                    
    23 : jl     [12]               ; 0f 8c c4 ff ff ff           
    24 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    25 : mov    [rsp], rax         ; 48 89 44 24 00              
    26 : mov    rax, r13           ; 4c 89 e8                    
    27 : cqo                       ; 48 99                       
    28 : idiv   [rsp+#0x28]        ; 48 f7 7c 24 28              
    29 : mov    rdx, rax           ; 48 89 c2                    
    30 : mov    rax, [rsp]         ; 48 8b 84 24 00 00 00 00     
    31 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    32 : mov    [rsp], rax         ; 48 89 44 24 00              
    33 : mov    [rsp+#0x08], rdx   ; 48 89 54 24 08              
    34 : mov    rax, r13           ; 4c 89 e8                    
    35 : cqo                       ; 48 99                       
    36 : idiv   [rsp+#0x28]        ; 48 f7 7c 24 28              
    37 : mov    rcx, rax           ; 48 89 c1                    
    38 : mov    rax, [rsp]         ; 48 8b 84 24 00 00 00 00     
    39 : mov    rdx, [rsp+#0x08]   ; 48 8b 94 24 08 00 00 00     
    40 : mov    [r8], edx          ; 41 89 10                    
    41 : mov    [r9], ecx          ; 41 89 09                    
    42 : mov    r9, #0             ; 49 c7 c1 00 00 00 00        
    43 : mov    rax, r9            ; 4c 89 c8                    
    44 : mov    r12, [rsp+#0x38]   ; 4c 8b a4 24 38 00 00 00     
    45 : mov    r13, [rsp+#0x40]   ; 4c 8b ac 24 40 00 00 00     
    46 : add    rsp, #0x48         ; 48 83 c4 48                 
    47 : ret                       ; c3                          
