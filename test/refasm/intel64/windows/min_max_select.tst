min_max_select(i0, i1, i2, i3)
     0 : sub    rsp, #0x58         ; 48 83 ec 58                 
     1 : mov    [rsp+#0x28], r8    ; 4c 89 44 24 28              
     2 : mov    [rsp+#0x20], r9    ; 4c 89 4c 24 20              
     3 : mov    [rsp+#0x48], r12   ; 4c 89 64 24 48              
     4 : mov    [rsp+#0x50], r13   ; 4c 89 6c 24 50              
     5 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
     6 : mov    [rsp+#0x38], #0    ; 48 c7 44 24 38 00 00 00 00  
     7 : mov    [rsp+#0x30], #0    ; 48 c7 44 24 30 00 00 00 00  
     8 : movsxd r8, rcx            ; 4c 63 01                    
     9 : mov    r9, r8             ; 4d 89 c1                    
    10 : mov    [rsp+#0x40], rdx   ; 48 89 54 24 40              
    11 : shl    [rsp+#0x40], #0x02 ; 48 c1 a4 24 40 00 00 00 02  
    12 : cmp    rax, [rsp+#0x40]   ; 48 3b 84 24 40 00 00 00     
    13 : jge    [34]               ; 0f 8d 52 00 00 00           
    14 : movsxd r12, rcx, rax      ; 4c 63 24 01                 
    15 : cmp    r12, r8            ; 4d 39 c4                    
    16 : mov    r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    17 : mov    rdx, r13           ; 4c 89 ea                    
    18 : cmovl  rdx, rax           ; 48 0f 4c d0                 
    19 : mov    [rsp+#0x38], rdx   ; 48 89 54 24 38              
    20 : mov    rdx, r8            ; 4c 89 c2                    
    21 : cmp    rdx, r12           ; 4c 39 e2                    
    22 : cmovg  rdx, r12           ; 49 0f 4f d4                 
    23 : mov    r8, rdx            ; 49 89 d0                    
    24 : cmp    r12, r9            ; 4d 39 cc                    
    25 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    26 : mov    rdx, r13           ; 4c 89 ea                    
    27 : cmovg  rdx, rax           ; 48 0f 4f d0                 
    28 : mov    [rsp+#0x30], rdx   ; 48 89 54 24 30              
    29 : cmp    r12, r9            ; 4d 39 cc                    
    30 : cmovl  r12, r9            ; 4d 0f 4c e1                 
    31 : mov    r9, r12            ; 4d 89 e1                    
    32 : add    rax, #0x04         ; 48 83 c0 04                 
    33 : jmp    [12]               ; e9 a0 ff ff ff              
    34 : mov    rcx, #0x04         ; 48 c7 c1 04 00 00 00        
    35 : mov    r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    36 : mov    rcx, r13           ; 4c 89 e9                    
    37 : sar    rcx, #0x02         ; 48 c1 f9 02                 
    38 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    39 : mov    rdx, r13           ; 4c 89 ea                    
    40 : sar    rdx, #0x02         ; 48 c1 fa 02                 
    41 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00     
    42 : mov    [r13], ecx         ; 41 89 4d 00                 
    43 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    44 : mov    [r13], edx         ; 41 89 55 00                 
    45 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    46 : mov    r12, [rsp+#0x48]   ; 4c 8b a4 24 48 00 00 00     
    47 : mov    r13, [rsp+#0x50]   ; 4c 8b ac 24 50 00 00 00     
    48 : add    rsp, #0x58         ; 48 83 c4 58                 
    49 : ret                       ; c3                          
