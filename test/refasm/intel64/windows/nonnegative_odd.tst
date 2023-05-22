nonnegative_odd(i0, i1)
     0 : sub    rsp, #0x28              ; 48 83 ec 28              
     1 : mov    [rsp+#0x20], r12        ; 4c 89 64 24 20           
     2 : xor    r8, r8                  ; 4d 31 c0                 
     3 : mov    r9, #0xfffffffffffffffc ; 49 c7 c1 fc ff ff ff     
     4 : imul   rdx, #0x04              ; 48 6b d2 04              
     5 : cmp    r8, rdx                 ; 49 39 d0                 
     6 : jge    [24]                    ; 0f 8d 4e 00 00 00        
     7 : movsxd rax, rcx, r8            ; 4a 63 04 01              
     8 : cmp    rax, #0                 ; 48 83 f8 00              
     9 : jge    [12]                    ; 0f 8d 09 00 00 00        
    10 : add    r8, #0x04               ; 49 83 c0 04              
    11 : jmp    [5]                     ; e9 e0 ff ff ff           
    12 : mov    r12, #0x02              ; 49 c7 c4 02 00 00 00     
    13 : mov    [rsp+#0x08], rdx        ; 48 89 54 24 08           
    14 : cqo                            ; 48 99                    
    15 : idiv   r12                     ; 49 f7 fc                 
    16 : mov    rax, rdx                ; 48 89 d0                 
    17 : mov    rdx, [rsp+#0x08]        ; 48 8b 94 24 08 00 00 00  
    18 : cmp    rax, #0                 ; 48 83 f8 00              
    19 : je     [22]                    ; 0f 84 08 00 00 00        
    20 : mov    r9, r8                  ; 4d 89 c1                 
    21 : jmp    [24]                    ; e9 09 00 00 00           
    22 : add    r8, #0x04               ; 49 83 c0 04              
    23 : jmp    [5]                     ; e9 a9 ff ff ff           
    24 : mov    rcx, #0x04              ; 48 c7 c1 04 00 00 00     
    25 : mov    [rsp], rax              ; 48 89 44 24 00           
    26 : mov    [rsp+#0x08], rdx        ; 48 89 54 24 08           
    27 : mov    rax, r9                 ; 4c 89 c8                 
    28 : cqo                            ; 48 99                    
    29 : idiv   rcx                     ; 48 f7 f9                 
    30 : mov    rcx, rax                ; 48 89 c1                 
    31 : mov    rax, [rsp]              ; 48 8b 84 24 00 00 00 00  
    32 : mov    rdx, [rsp+#0x08]        ; 48 8b 94 24 08 00 00 00  
    33 : mov    rax, rcx                ; 48 89 c8                 
    34 : mov    r12, [rsp+#0x20]        ; 4c 8b a4 24 20 00 00 00  
    35 : add    rsp, #0x28              ; 48 83 c4 28              
    36 : ret                            ; c3                       
