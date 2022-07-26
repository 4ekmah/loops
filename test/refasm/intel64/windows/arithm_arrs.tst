arithm_arrs(i0, i1, i2, i3, i4, i5, i6)
     0 : sub    rsp, #0x38         ; 48 83 ec 38                 
     1 : mov    [rsp+#0x20], r9    ; 4c 89 4c 24 20              
     2 : mov    [rsp+#0x28], r12   ; 4c 89 64 24 28              
     3 : mov    [rsp+#0x30], r13   ; 4c 89 6c 24 30              
     4 : mov    [rsp+#0x10], #0    ; 48 c7 44 24 10 00 00 00 00  
     5 : mov    [rsp+#0x18], #0    ; 48 c7 44 24 18 00 00 00 00  
     6 : cmp    [rsp+#0x18], r8    ; 4c 39 44 24 18              
     7 : jge    [41]               ; 0f 8d ab 00 00 00           
     8 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
     9 : movsxd r12, rcx, r13      ; 4e 63 24 29                 
    10 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    11 : movsxd rax, rdx, r13      ; 4a 63 04 2a                 
    12 : mov    r9, r12            ; 4d 89 e1                    
    13 : add    r9, rax            ; 49 01 c1                    
    14 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    15 : mov    [r13], r9d         ; 45 89 4d 00                 
    16 : mov    r9, r12            ; 4d 89 e1                    
    17 : sub    r9, rax            ; 49 29 c1                    
    18 : mov    r13, [rsp+#0x60]   ; 4c 8b ac 24 60 00 00 00     
    19 : mov    [r13], r9d         ; 45 89 4d 00                 
    20 : mov    r9, r12            ; 4d 89 e1                    
    21 : imul   r9, rax            ; 4c 0f af c8                 
    22 : mov    r13, [rsp+#0x68]   ; 4c 8b ac 24 68 00 00 00     
    23 : mov    [r13], r9d         ; 45 89 4d 00                 
    24 : mov    [rsp], rax         ; 48 89 44 24 00              
    25 : mov    [rsp+#0x08], rdx   ; 48 89 54 24 08              
    26 : mov    rax, r12           ; 4c 89 e0                    
    27 : cqo                       ; 48 99                       
    28 : idiv   [rsp]              ; 48 f7 7c 24 00              
    29 : mov    r9, rax            ; 49 89 c1                    
    30 : mov    rax, [rsp]         ; 48 8b 84 24 00 00 00 00     
    31 : mov    rdx, [rsp+#0x08]   ; 48 8b 94 24 08 00 00 00     
    32 : mov    r13, [rsp+#0x70]   ; 4c 8b ac 24 70 00 00 00     
    33 : mov    [r13], r9d         ; 45 89 4d 00                 
    34 : add    [rsp+#0x18], #0x01 ; 48 83 44 24 18 01           
    35 : add    [rsp+#0x10], #0x04 ; 48 83 44 24 10 04           
    36 : add    [rsp+#0x20], #0x04 ; 48 83 44 24 20 04           
    37 : add    [rsp+#0x60], #0x04 ; 48 83 44 24 60 04           
    38 : add    [rsp+#0x68], #0x04 ; 48 83 44 24 68 04           
    39 : add    [rsp+#0x70], #0x04 ; 48 83 44 24 70 04           
    40 : jmp    [6]                ; e9 4a ff ff ff              
    41 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    42 : mov    r12, [rsp+#0x28]   ; 4c 8b a4 24 28 00 00 00     
    43 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    44 : add    rsp, #0x38         ; 48 83 c4 38                 
    45 : ret                       ; c3                          
