arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : mov    rax, [rsp+#0x28]   ; 48 8b 44 24 28              
     1 : mov    r12, [rsp+#0x30]   ; 4c 8b 64 24 30              
     2 : sub    rsp, #0x68         ; 48 83 ec 68                 
     3 : mov    [rsp+#0x48], r12   ; 4c 89 64 24 48              
     4 : mov    [rsp+#0x50], r13   ; 4c 89 6c 24 50              
     5 : mov    [rsp+#0x58], r14   ; 4c 89 74 24 58              
     6 : mov    [rsp], #0          ; 48 c7 44 24 00 00 00 00 00  
     7 : mov    [rsp+#0x08], #0    ; 48 c7 44 24 08 00 00 00 00  
     8 : mov    [rsp+#0x10], #0x04 ; 48 c7 44 24 10 04 00 00 00  
     9 : mov    [rsp+#0x18], #0x01 ; 48 c7 44 24 18 01 00 00 00  
    10 : mov    r13, [rsp]         ; 4c 8b 6c 24 00              
    11 : movsxd r14, rcx, r13      ; 4e 63 34 29                 
    12 : mov    [rsp+#0x20], r14   ; 4c 89 74 24 20              
    13 : mov    r13, [rsp]         ; 4c 8b 6c 24 00              
    14 : movsxd r14, rdx, r13      ; 4e 63 34 2a                 
    15 : mov    [rsp+#0x28], r14   ; 4c 89 74 24 28              
    16 : mov    r13, [rsp+#0x20]   ; 4c 8b 6c 24 20              
    17 : mov    r14, [rsp+#0x28]   ; 4c 8b 74 24 28              
    18 : mov    [rsp+#0x30], r13   ; 4c 89 6c 24 30              
    19 : add    [rsp+#0x30], r14   ; 4c 01 74 24 30              
    20 : mov    r13, [rsp+#0x30]   ; 4c 8b 6c 24 30              
    21 : mov    [r9], r13d         ; 45 89 29                    
    22 : mov    r13, [rsp+#0x20]   ; 4c 8b 6c 24 20              
    23 : mov    r14, [rsp+#0x28]   ; 4c 8b 74 24 28              
    24 : mov    [rsp+#0x38], r13   ; 4c 89 6c 24 38              
    25 : sub    [rsp+#0x38], r14   ; 4c 29 74 24 38              
    26 : mov    r13, [rsp+#0x38]   ; 4c 8b 6c 24 38              
    27 : mov    [rax], r13d        ; 44 89 28                    
    28 : mov    r13, [rsp+#0x20]   ; 4c 8b 6c 24 20              
    29 : mov    r14, r13           ; 4d 89 ee                    
    30 : imul   r14, [rsp+#0x28]   ; 4c 0f af 74 24 28           
    31 : mov    [rsp+#0x40], r14   ; 4c 89 74 24 40              
    32 : mov    r13, [rsp+#0x40]   ; 4c 8b 6c 24 40              
    33 : mov    [r12], r13d        ; 45 89 2c 24                 
    34 : mov    r13, [rsp+#0x18]   ; 4c 8b 6c 24 18              
    35 : add    [rsp+#0x08], r13   ; 4c 01 6c 24 08              
    36 : mov    r13, [rsp+#0x10]   ; 4c 8b 6c 24 10              
    37 : add    [rsp], r13         ; 4c 01 6c 24 00              
    38 : add    r9, [rsp+#0x10]    ; 4c 03 4c 24 10              
    39 : add    rax, [rsp+#0x10]   ; 48 03 44 24 10              
    40 : add    r12, [rsp+#0x10]   ; 4c 03 64 24 10              
    41 : cmp    [rsp+#0x08], r8    ; 4c 39 44 24 08              
    42 : jl     [10]               ; 0f 8c 62 ff ff ff           
    43 : mov    r9, #0             ; 49 c7 c1 00 00 00 00        
    44 : mov    rax, r9            ; 4c 89 c8                    
    45 : mov    r12, [rsp+#0x48]   ; 4c 8b 64 24 48              
    46 : mov    r13, [rsp+#0x50]   ; 4c 8b 6c 24 50              
    47 : mov    r14, [rsp+#0x58]   ; 4c 8b 74 24 58              
    48 : add    rsp, #0x68         ; 48 83 c4 68                 
    49 : ret                       ; c3                          
