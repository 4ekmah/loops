nonnegative_odd(i0, i1)
     0 : sub    rsp, #0x18              ; 48 83 ec 18              
     1 : mov    [rsp+#0x10], r12        ; 4c 89 64 24 10           
     2 : mov    r8, #0                  ; 49 c7 c0 00 00 00 00     
     3 : mov    r9, #0xfffffffffffffffc ; 49 c7 c1 fc ff ff ff     
     4 : imul   rdx, #0x04              ; 48 6b d2 04              
     5 : cmp    r8, rdx                 ; 49 39 d0                 
     6 : jge    [31]                    ; 0f 8d 71 00 00 00        
     7 : movsxd rax, rcx, r8            ; 4a 63 04 01              
     8 : mov    r12, #0                 ; 49 c7 c4 00 00 00 00     
     9 : cmp    rax, r12                ; 4c 39 e0                 
    10 : jge    [13]                    ; 0f 8d 09 00 00 00        
    11 : add    r8, #0x04               ; 49 83 c0 04              
    12 : jmp    [5]                     ; e9 da ff ff ff           
    13 : mov    r12, #0x02              ; 49 c7 c4 02 00 00 00     
    14 : mov    [rsp], rax              ; 48 89 44 24 00           
    15 : mov    [rsp+#0x08], rdx        ; 48 89 54 24 08           
    16 : cqo                            ; 48 99                    
    17 : idiv   r12                     ; 49 f7 fc                 
    18 : mov    r12, rax                ; 49 89 c4                 
    19 : mov    rax, [rsp]              ; 48 8b 84 24 00 00 00 00  
    20 : mov    rdx, [rsp+#0x08]        ; 48 8b 94 24 08 00 00 00  
    21 : imul   r12, #0x02              ; 4d 6b e4 02              
    22 : sub    rax, r12                ; 4c 29 e0                 
    23 : mov    rax, rax                ; 48 89 c0                 
    24 : mov    r12, #0                 ; 49 c7 c4 00 00 00 00     
    25 : cmp    rax, r12                ; 4c 39 e0                 
    26 : je     [29]                    ; 0f 84 08 00 00 00        
    27 : mov    r9, r8                  ; 4d 89 c1                 
    28 : jmp    [31]                    ; e9 09 00 00 00           
    29 : add    r8, #0x04               ; 49 83 c0 04              
    30 : jmp    [5]                     ; e9 86 ff ff ff           
    31 : mov    rcx, #0x04              ; 48 c7 c1 04 00 00 00     
    32 : mov    [rsp], rax              ; 48 89 44 24 00           
    33 : mov    [rsp+#0x08], rdx        ; 48 89 54 24 08           
    34 : mov    rax, r9                 ; 4c 89 c8                 
    35 : cqo                            ; 48 99                    
    36 : idiv   rcx                     ; 48 f7 f9                 
    37 : mov    rcx, rax                ; 48 89 c1                 
    38 : mov    rax, [rsp]              ; 48 8b 84 24 00 00 00 00  
    39 : mov    rdx, [rsp+#0x08]        ; 48 8b 94 24 08 00 00 00  
    40 : mov    rax, rcx                ; 48 89 c8                 
    41 : mov    r12, [rsp+#0x10]        ; 4c 8b a4 24 10 00 00 00  
    42 : add    rsp, #0x18              ; 48 83 c4 18              
    43 : ret                            ; c3                       
