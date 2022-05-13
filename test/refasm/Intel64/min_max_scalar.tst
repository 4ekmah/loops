min_max_scalar(i0, i1, i2, i3)
     0 : sub    rsp, #0x48         ; 48 83 ec 48                 
     1 : mov    [rsp+#0x18], r8    ; 4c 89 44 24 18              
     2 : mov    [rsp+#0x10], r9    ; 4c 89 4c 24 10              
     3 : mov    [rsp+#0x38], r12   ; 4c 89 64 24 38              
     4 : mov    [rsp+#0x40], r13   ; 4c 89 6c 24 40              
     5 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
     6 : mov    [rsp+#0x30], #0    ; 48 c7 44 24 30 00 00 00 00  
     7 : mov    [rsp+#0x20], #0    ; 48 c7 44 24 20 00 00 00 00  
     8 : movsxd r8, rcx            ; 4c 63 01                    
     9 : mov    r9, r8             ; 4d 89 c1                    
    10 : mov    [rsp+#0x28], #0x04 ; 48 c7 44 24 28 04 00 00 00  
    11 : imul   rdx, [rsp+#0x28]   ; 48 0f af 54 24 28           
    12 : movsxd r12, rcx, rax      ; 4c 63 24 01                 
    13 : cmp    r12, r8            ; 4d 39 c4                    
    14 : jge    [17]               ; 0f 8d 08 00 00 00           
    15 : mov    r8, r12            ; 4d 89 e0                    
    16 : mov    [rsp+#0x30], rax   ; 48 89 44 24 30              
    17 : cmp    r12, r9            ; 4d 39 cc                    
    18 : jle    [21]               ; 0f 8e 08 00 00 00           
    19 : mov    r9, r12            ; 4d 89 e1                    
    20 : mov    [rsp+#0x20], rax   ; 48 89 44 24 20              
    21 : add    rax, [rsp+#0x28]   ; 48 03 44 24 28              
    22 : cmp    rax, rdx           ; 48 39 d0                    
    23 : jl     [12]               ; 0f 8c cc ff ff ff           
    24 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    25 : mov    [rsp], rax         ; 48 89 44 24 00              
    26 : mov    [rsp+#0x08], rdx   ; 48 89 54 24 08              
    27 : mov    rax, r13           ; 4c 89 e8                    
    28 : cqo                       ; 48 99                       
    29 : idiv   [rsp+#0x28]        ; 48 f7 7c 24 28              
    30 : mov    rcx, rax           ; 48 89 c1                    
    31 : mov    rax, [rsp]         ; 48 8b 84 24 00 00 00 00     
    32 : mov    rdx, [rsp+#0x08]   ; 48 8b 94 24 08 00 00 00     
    33 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    34 : mov    [rsp], rax         ; 48 89 44 24 00              
    35 : mov    rax, r13           ; 4c 89 e8                    
    36 : cqo                       ; 48 99                       
    37 : idiv   [rsp+#0x28]        ; 48 f7 7c 24 28              
    38 : mov    rdx, rax           ; 48 89 c2                    
    39 : mov    rax, [rsp]         ; 48 8b 84 24 00 00 00 00     
    40 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    41 : mov    [r13], ecx         ; 41 89 4d 00                 
    42 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    43 : mov    [r13], edx         ; 41 89 55 00                 
    44 : mov    rcx, #0            ; 48 c7 c1 00 00 00 00        
    45 : mov    rax, rcx           ; 48 89 c8                    
    46 : mov    r12, [rsp+#0x38]   ; 4c 8b a4 24 38 00 00 00     
    47 : mov    r13, [rsp+#0x40]   ; 4c 8b ac 24 40 00 00 00     
    48 : add    rsp, #0x48         ; 48 83 c4 48                 
    49 : ret                       ; c3                          
