nonnegative_odd(i0, i1)
     0 : sub    rsp, #0x38               ; 48 81 ec 38 00 00 00     
     1 : mov    [rsp+#0x28], r13         ; 4c 89 ac 24 28 00 00 00  
     2 : xor    r8, r8                   ; 4d 31 c0                 
     3 : mov    r13, #0xfffffffffffffffc ; 49 c7 c5 fc ff ff ff     
     4 : mov    [rsp+#0x20], r13         ; 4c 89 ac 24 20 00 00 00  
     5 : imul   rdx, #0x04               ; 48 6b d2 04              
     6 : cmp    r8, rdx                  ; 49 39 d0                 
     7 : jge    [27]                     ; 0f 8d 6c 00 00 00        
     8 : movsxd rax, rcx, r8             ; 4a 63 04 01              
     9 : cmp    rax, #0                  ; 48 83 f8 00              
    10 : jge    [13]                     ; 0f 8d 0c 00 00 00        
    11 : add    r8, #0x04                ; 49 81 c0 04 00 00 00     
    12 : jmp    [6]                      ; e9 dd ff ff ff           
    13 : mov    r9, #0x02                ; 49 c7 c1 02 00 00 00     
    14 : mov    [rsp], rax               ; 48 89 84 24 00 00 00 00  
    15 : mov    [rsp+#0x08], rdx         ; 48 89 94 24 08 00 00 00  
    16 : cqo                             ; 48 99                    
    17 : idiv   r9                       ; 49 f7 f9                 
    18 : mov    r9, rdx                  ; 49 89 d1                 
    19 : mov    rax, [rsp]               ; 48 8b 84 24 00 00 00 00  
    20 : mov    rdx, [rsp+#0x08]         ; 48 8b 94 24 08 00 00 00  
    21 : cmp    r9, #0                   ; 49 83 f9 00              
    22 : je     [25]                     ; 0f 84 0d 00 00 00        
    23 : mov    [rsp+#0x20], r8          ; 4c 89 84 24 20 00 00 00  
    24 : jmp    [27]                     ; e9 0c 00 00 00           
    25 : add    r8, #0x04                ; 49 81 c0 04 00 00 00     
    26 : jmp    [6]                      ; e9 8b ff ff ff           
    27 : mov    rcx, #0x04               ; 48 c7 c1 04 00 00 00     
    28 : mov    r13, [rsp+#0x20]         ; 4c 8b ac 24 20 00 00 00  
    29 : mov    [rsp], rax               ; 48 89 84 24 00 00 00 00  
    30 : mov    [rsp+#0x08], rdx         ; 48 89 94 24 08 00 00 00  
    31 : mov    rax, r13                 ; 4c 89 e8                 
    32 : cqo                             ; 48 99                    
    33 : idiv   rcx                      ; 48 f7 f9                 
    34 : mov    rcx, rax                 ; 48 89 c1                 
    35 : mov    rax, [rsp]               ; 48 8b 84 24 00 00 00 00  
    36 : mov    rdx, [rsp+#0x08]         ; 48 8b 94 24 08 00 00 00  
    37 : mov    rax, rcx                 ; 48 89 c8                 
    38 : mov    r13, [rsp+#0x28]         ; 4c 8b ac 24 28 00 00 00  
    39 : add    rsp, #0x38               ; 48 81 c4 38 00 00 00     
    40 : ret                             ; c3                       
