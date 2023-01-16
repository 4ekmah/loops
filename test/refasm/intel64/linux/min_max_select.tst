min_max_select(i0, i1, i2, i3)
     0 : sub    rsp, #0x58         ; 48 83 ec 58                 
     1 : mov    [rsp+#0x28], rdx   ; 48 89 54 24 28              
     2 : mov    [rsp+#0x20], rcx   ; 48 89 4c 24 20              
     3 : mov    [rsp+#0x48], r12   ; 4c 89 64 24 48              
     4 : mov    [rsp+#0x50], r13   ; 4c 89 6c 24 50              
     5 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
     6 : mov    [rsp+#0x38], #0    ; 48 c7 44 24 38 00 00 00 00  
     7 : mov    [rsp+#0x30], #0    ; 48 c7 44 24 30 00 00 00 00  
     8 : movsxd rdx, rdi           ; 48 63 17                    
     9 : mov    rcx, rdx           ; 48 89 d1                    
    10 : mov    [rsp+#0x40], rsi   ; 48 89 74 24 40              
    11 : shl    [rsp+#0x40], #0x02 ; 48 c1 a4 24 40 00 00 00 02  
    12 : cmp    rax, [rsp+#0x40]   ; 48 3b 84 24 40 00 00 00     
    13 : jge    [34]               ; 0f 8d 52 00 00 00           
    14 : movsxd r12, rdi, rax      ; 4c 63 24 07                 
    15 : cmp    r12, rdx           ; 49 39 d4                    
    16 : mov    r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    17 : mov    rsi, r13           ; 4c 89 ee                    
    18 : cmovl  rsi, rax           ; 48 0f 4c f0                 
    19 : mov    [rsp+#0x38], rsi   ; 48 89 74 24 38              
    20 : mov    rsi, rdx           ; 48 89 d6                    
    21 : cmp    rsi, r12           ; 4c 39 e6                    
    22 : cmovg  rsi, r12           ; 49 0f 4f f4                 
    23 : mov    rdx, rsi           ; 48 89 f2                    
    24 : cmp    r12, rcx           ; 49 39 cc                    
    25 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    26 : mov    rsi, r13           ; 4c 89 ee                    
    27 : cmovg  rsi, rax           ; 48 0f 4f f0                 
    28 : mov    [rsp+#0x30], rsi   ; 48 89 74 24 30              
    29 : cmp    r12, rcx           ; 49 39 cc                    
    30 : cmovl  r12, rcx           ; 4c 0f 4c e1                 
    31 : mov    rcx, r12           ; 4c 89 e1                    
    32 : add    rax, #0x04         ; 48 83 c0 04                 
    33 : jmp    [12]               ; e9 a0 ff ff ff              
    34 : mov    rdi, #0x04         ; 48 c7 c7 04 00 00 00        
    35 : mov    r13, [rsp+#0x38]   ; 4c 8b ac 24 38 00 00 00     
    36 : mov    rdi, r13           ; 4c 89 ef                    
    37 : sar    rdi, #0x02         ; 48 c1 ff 02                 
    38 : mov    r13, [rsp+#0x30]   ; 4c 8b ac 24 30 00 00 00     
    39 : mov    rsi, r13           ; 4c 89 ee                    
    40 : sar    rsi, #0x02         ; 48 c1 fe 02                 
    41 : mov    r13, [rsp+#0x28]   ; 4c 8b ac 24 28 00 00 00     
    42 : mov    [r13], edi         ; 41 89 7d 00                 
    43 : mov    r13, [rsp+#0x20]   ; 4c 8b ac 24 20 00 00 00     
    44 : mov    [r13], esi         ; 41 89 75 00                 
    45 : mov    rax, #0            ; 48 c7 c0 00 00 00 00        
    46 : mov    r12, [rsp+#0x48]   ; 4c 8b a4 24 48 00 00 00     
    47 : mov    r13, [rsp+#0x50]   ; 4c 8b ac 24 50 00 00 00     
    48 : add    rsp, #0x58         ; 48 83 c4 58                 
    49 : ret                       ; c3                          
