arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : sub    rsp, #0x28         ; 48 83 ec 28                 
     1 : mov    [rsp+#0x10], r9    ; 4c 89 4c 24 10              
     2 : mov    [rsp+#0x18], r12   ; 4c 89 64 24 18              
     3 : mov    [rsp+#0x20], r13   ; 4c 89 6c 24 20              
     4 : mov    [rsp], #0          ; 48 c7 44 24 00 00 00 00 00  
     5 : mov    [rsp+#0x08], #0    ; 48 c7 44 24 08 00 00 00 00  
     6 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
     7 : movsxd r12, rcx, r13      ; 4e 63 24 29                 
     8 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
     9 : movsxd rax, rdx, r13      ; 4a 63 04 2a                 
    10 : mov    r9, r12            ; 4d 89 e1                    
    11 : add    r9, rax            ; 49 01 c1                    
    12 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    13 : mov    [r13], r9d         ; 45 89 4d 00                 
    14 : mov    r9, r12            ; 4d 89 e1                    
    15 : sub    r9, rax            ; 49 29 c1                    
    16 : mov    r13, [rsp+#0x50]   ; 4c 8b ac 24 50 00 00 00     
    17 : mov    [r13], r9d         ; 45 89 4d 00                 
    18 : imul   r12, rax           ; 4c 0f af e0                 
    19 : mov    r13, [rsp+#0x58]   ; 4c 8b ac 24 58 00 00 00     
    20 : mov    [r13], r12d        ; 45 89 65 00                 
    21 : add    [rsp+#0x08], #0x01 ; 48 83 44 24 08 01           
    22 : add    [rsp], #0x04       ; 48 83 44 24 00 04           
    23 : add    [rsp+#0x10], #0x04 ; 48 83 44 24 10 04           
    24 : add    [rsp+#0x50], #0x04 ; 48 83 44 24 50 04           
    25 : add    [rsp+#0x58], #0x04 ; 48 83 44 24 58 04           
    26 : cmp    [rsp+#0x08], r8    ; 4c 39 44 24 08              
    27 : jl     [6]                ; 0f 8c 8b ff ff ff           
    28 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    29 : mov    r12, [rsp+#0x18]   ; 4c 8b a4 24 18 00 00 00     
    30 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    31 : add    rsp, #0x28         ; 48 83 c4 28                 
    32 : ret                       ; c3                          
