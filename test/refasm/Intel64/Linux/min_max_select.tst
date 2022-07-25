min_max_select(i0, i1, i2, i3)
     0 : sub    rsp, #0x48         ; 48 83 ec 48                 
     1 : mov    [rsp+#0x10], rdx   ; 48 89 54 24 10              
     2 : mov    [rsp+#0x08], rcx   ; 48 89 4c 24 08              
     3 : mov    [rsp+#0x30], r12   ; 4c 89 64 24 30              
     4 : mov    [rsp+#0x38], r13   ; 4c 89 6c 24 38              
     5 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
     6 : mov    [rsp+#0x20], #0    ; 48 c7 44 24 20 00 00 00 00  
     7 : mov    [rsp+#0x18], #0    ; 48 c7 44 24 18 00 00 00 00  
     8 : movsxd rdx, rdi           ; 48 63 17                    
     9 : mov    rcx, rdx           ; 48 89 d1                    
    10 : mov    [rsp+#0x28], rsi   ; 48 89 74 24 28              
    11 : shl    [rsp+#0x28], #0x02 ; 48 c1 a4 24 28 00 00 00 02  
    12 : cmp    rax, [rsp+#0x28]   ; 48 3b 44 24 28              
    13 : jge    [34]               ; 0f 8d 52 00 00 00           
    14 : movsxd r12, rdi, rax      ; 4c 63 24 07                 
    15 : cmp    r12, rdx           ; 49 39 d4                    
    16 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    17 : mov    rsi, r13           ; 4c 89 ee                    
    18 : cmovl  rsi, rax           ; 48 0f 4c f0                 
    19 : mov    [rsp+#0x20], rsi   ; 48 89 74 24 20              
    20 : mov    rsi, rdx           ; 48 89 d6                    
    21 : cmp    rsi, r12           ; 4c 39 e6                    
    22 : cmovg  rsi, r12           ; 49 0f 4f f4                 
    23 : mov    rdx, rsi           ; 48 89 f2                    
    24 : cmp    r12, rcx           ; 49 39 cc                    
    25 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    26 : mov    rsi, r13           ; 4c 89 ee                    
    27 : cmovg  rsi, rax           ; 48 0f 4f f0                 
    28 : mov    [rsp+#0x18], rsi   ; 48 89 74 24 18              
    29 : cmp    r12, rcx           ; 49 39 cc                    
    30 : cmovl  r12, rcx           ; 4c 0f 4c e1                 
    31 : mov    rcx, r12           ; 4c 89 e1                    
    32 : add    rax, #0x04         ; 48 83 c0 04                 
    33 : jmp    [12]               ; e9 a3 ff ff ff              
    34 : mov    rdi, #0x04         ; 48 c7 c7 04 00 00 00        
    35 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    36 : mov    rdi, r13           ; 4c 89 ef                    
    37 : sar    rdi, #0x02         ; 48 c1 ff 02                 
    38 : mov    r13, [rsp+#0x18]   ; 4c 8b ac 24 18 00 00 00     
    39 : mov    rsi, r13           ; 4c 89 ee                    
    40 : sar    rsi, #0x02         ; 48 c1 fe 02                 
    41 : mov    r13, [rsp+#0x10]   ; 4c 8b ac 24 10 00 00 00     
    42 : mov    [r13], edi         ; 41 89 7d 00                 
    43 : mov    r13, [rsp+#0x08]   ; 4c 8b ac 24 08 00 00 00     
    44 : mov    [r13], esi         ; 41 89 75 00                 
    45 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    46 : mov    r12, [rsp+#0x30]   ; 4c 8b a4 24 30 00 00 00     
    47 : mov    r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    48 : add    rsp, #0x48         ; 48 83 c4 48                 
    49 : ret                       ; c3                          
