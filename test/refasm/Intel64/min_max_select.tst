min_max_select(i0, i1, i2, i3)
     0 : sub    rsp, #0x48         ; 48 83 ec 48                 
     1 : mov    [rsp+#0x10], r8    ; 4c 89 44 24 10              
     2 : mov    [rsp+#0x08], r9    ; 4c 89 4c 24 08              
     3 : mov    [rsp+#0x30], r12   ; 4c 89 64 24 30              
     4 : mov    [rsp+#0x38], r13   ; 4c 89 6c 24 38              
     5 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
     6 : mov    [rsp+#0x20], #0    ; 48 c7 44 24 20 00 00 00 00  
     7 : mov    [rsp+#0x18], #0    ; 48 c7 44 24 18 00 00 00 00  
     8 : movsxd r8, rcx            ; 4c 63 01                    
     9 : mov    r9, r8             ; 4d 89 c1                    
    10 : mov    [rsp+#0x28], rdx   ; 48 89 54 24 28              
    11 : shl    [rsp+#0x28], #0x02 ; 48 c1 a4 24 28 00 00 00 02  
    12 : movsxd r12, rcx, rax      ; 4c 63 24 01                 
    13 : cmp    r12, r8            ; 4d 39 c4                    
    14 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    15 : mov    rdx, r13           ; 4c 89 ea                    
    16 : cmovl  rdx, rax           ; 48 0f 4c d0                 
    17 : mov    [rsp+#0x20], rdx   ; 48 89 54 24 20              
    18 : mov    rdx, r8            ; 4c 89 c2                    
    19 : cmp    rdx, r12           ; 4c 39 e2                    
    20 : cmovg  rdx, r12           ; 49 0f 4f d4                 
    21 : mov    r8, rdx            ; 49 89 d0                    
    22 : cmp    r12, r9            ; 4d 39 cc                    
    23 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    24 : mov    rdx, r13           ; 4c 89 ea                    
    25 : cmovg  rdx, rax           ; 48 0f 4f d0                 
    26 : mov    [rsp+#0x18], rdx   ; 48 89 54 24 18              
    27 : cmp    r12, r9            ; 4d 39 cc                    
    28 : cmovl  r12, r9            ; 4d 0f 4c e1                 
    29 : mov    r9, r12            ; 4d 89 e1                    
    30 : add    rax, #0x04         ; 48 83 c0 04                 
    31 : cmp    rax, [rsp+#0x28]   ; 48 3b 44 24 28              
    32 : jl     [12]               ; 0f 8c a8 ff ff ff           
    33 : mov    rcx, #0x04         ; 48 c7 c1 04 00 00 00        
    34 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    35 : mov    rcx, r13           ; 4c 89 e9                    
    36 : sar    rcx, #0x02         ; 48 c1 f9 02                 
    37 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    38 : mov    rdx, r13           ; 4c 89 ea                    
    39 : sar    rdx, #0x02         ; 48 c1 fa 02                 
    40 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    41 : mov    [r13], ecx         ; 41 89 4d 00                 
    42 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    43 : mov    [r13], edx         ; 41 89 55 00                 
    44 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    45 : mov    r12, [rsp+#0x30]   ; 4c 8b a4 24 30 00 00 00     
    46 : mov    r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    47 : add    rsp, #0x48         ; 48 83 c4 48                 
    48 : ret                       ; c3                          
