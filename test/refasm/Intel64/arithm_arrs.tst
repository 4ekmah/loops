arithm_arrs(i0, i1, i2, i3, i4, i5)
     0 : sub    rsp, #0x28         ; 48 83 ec 28                 
     1 : mov    [rsp+#0x10], r9    ; 4c 89 4c 24 10              
     2 : mov    [rsp+#0x18], r12   ; 4c 89 64 24 18              
     3 : mov    [rsp+#0x20], r13   ; 4c 89 6c 24 20              
     4 : mov    [rsp], #0          ; 48 c7 44 24 00 00 00 00 00  
     5 : mov    [rsp+#0x08], #0    ; 48 c7 44 24 08 00 00 00 00  
     6 : cmp    [rsp+#0x08], r8    ; 4c 39 44 24 08              
     7 : jge    [29]               ; 0f 8d 6f 00 00 00           
     8 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
     9 : movsxd r12, rcx, r13      ; 4e 63 24 29                 
    10 : mov    r13, [rsp]         ; 4c 8b ac 24 00 00 00 00     
    11 : movsxd rax, rdx, r13      ; 4a 63 04 2a                 
    12 : mov    r9, r12            ; 4d 89 e1                    
    13 : add    r9, rax            ; 49 01 c1                    
    14 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    15 : mov    [r13], r9d         ; 45 89 4d 00                 
    16 : mov    r9, r12            ; 4d 89 e1                    
    17 : sub    r9, rax            ; 49 29 c1                    
    18 : mov    r13, [rsp+#0x50]   ; 4c 8b ac 24 50 00 00 00     
    19 : mov    [r13], r9d         ; 45 89 4d 00                 
    20 : imul   r12, rax           ; 4c 0f af e0                 
    21 : mov    r13, [rsp+#0x58]   ; 4c 8b ac 24 58 00 00 00     
    22 : mov    [r13], r12d        ; 45 89 65 00                 
    23 : add    [rsp+#0x08], #0x01 ; 48 83 44 24 08 01           
    24 : add    [rsp], #0x04       ; 48 83 44 24 00 04           
    25 : add    [rsp+#0x10], #0x04 ; 48 83 44 24 10 04           
    26 : add    [rsp+#0x50], #0x04 ; 48 83 44 24 50 04           
    27 : add    [rsp+#0x58], #0x04 ; 48 83 44 24 58 04           
    28 : jmp    [6]                ; e9 86 ff ff ff              
    29 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    30 : mov    r12, [rsp+#0x18]   ; 4c 8b a4 24 18 00 00 00     
    31 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    32 : add    rsp, #0x28         ; 48 83 c4 28                 
    33 : ret                       ; c3                          
