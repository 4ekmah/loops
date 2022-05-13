arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : sub    rsp, #0x38         ; 48 83 ec 38                 
     1 : mov    [rsp+#0x20], r9    ; 4c 89 4c 24 20              
     2 : mov    [rsp+#0x28], r12   ; 4c 89 64 24 28              
     3 : mov    [rsp+#0x30], r13   ; 4c 89 6c 24 30              
     4 : mov    [rsp], #0          ; 48 c7 44 24 00 00 00 00 00  
     5 : mov    [rsp+#0x08], #0    ; 48 c7 44 24 08 00 00 00 00  
     6 : mov    [rsp+#0x10], #0x04 ; 48 c7 44 24 10 04 00 00 00  
     7 : mov    [rsp+#0x18], #0x01 ; 48 c7 44 24 18 01 00 00 00  
     8 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
     9 : movsxd r12, rcx, r13      ; 4e 63 24 29                 
    10 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    11 : movsxd rax, rdx, r13      ; 4a 63 04 2a                 
    12 : mov    r9, r12            ; 4d 89 e1                    
    13 : add    r9, rax            ; 49 01 c1                    
    14 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    15 : mov    [r13], r9d         ; 45 89 4d 00                 
    16 : mov    r9, r12            ; 4d 89 e1                    
    17 : sub    r9, rax            ; 49 29 c1                    
    18 : mov    r13, [rsp+#0x60]   ; 4c 8b ac 24 60 00 00 00     
    19 : mov    [r13], r9d         ; 45 89 4d 00                 
    20 : imul   r12, rax           ; 4c 0f af e0                 
    21 : mov    r13, [rsp+#0x68]   ; 4c 8b ac 24 68 00 00 00     
    22 : mov    [r13], r12d        ; 45 89 65 00                 
    23 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    24 : add    [rsp+#0x08], r13   ; 4c 01 6c 24 08              
    25 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    26 : add    [rsp], r13         ; 4c 01 6c 24 00              
    27 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    28 : add    [rsp+#0x20], r13   ; 4c 01 6c 24 20              
    29 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    30 : add    [rsp+#0x60], r13   ; 4c 01 6c 24 60              
    31 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    32 : add    [rsp+#0x68], r13   ; 4c 01 6c 24 68              
    33 : cmp    [rsp+#0x08], r8    ; 4c 39 44 24 08              
    34 : jl     [8]                ; 0f 8c 68 ff ff ff           
    35 : mov    rcx, #0            ; 48 c7 c1 00 00 00 00        
    36 : mov    rax, rcx           ; 48 89 c8                    
    37 : mov    r12, [rsp+#0x28]   ; 4c 8b a4 24 28 00 00 00     
    38 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    39 : add    rsp, #0x38         ; 48 83 c4 38                 
    40 : ret                       ; c3                          
