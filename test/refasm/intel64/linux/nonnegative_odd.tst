nonnegative_odd(i0, i1)
     0 : sub    rsp, #0x18               ; 48 83 ec 18              
     1 : mov    [rsp+#0x10], r12         ; 4c 89 64 24 10           
     2 : mov    rdx, #0                  ; 48 c7 c2 00 00 00 00     
     3 : mov    rcx, #0xfffffffffffffffc ; 48 c7 c1 fc ff ff ff     
     4 : imul   rsi, #0x04               ; 48 6b f6 04              
     5 : cmp    rdx, rsi                 ; 48 39 f2                 
     6 : jge    [24]                     ; 0f 8d 4e 00 00 00        
     7 : movsxd rax, rdi, rdx            ; 48 63 04 17              
     8 : cmp    rax, #0                  ; 48 83 f8 00              
     9 : jge    [12]                     ; 0f 8d 09 00 00 00        
    10 : add    rdx, #0x04               ; 48 83 c2 04              
    11 : jmp    [5]                      ; e9 e0 ff ff ff           
    12 : mov    r12, #0x02               ; 49 c7 c4 02 00 00 00     
    13 : mov    [rsp+#0x08], rdx         ; 48 89 54 24 08           
    14 : cqo                             ; 48 99                    
    15 : idiv   r12                      ; 49 f7 fc                 
    16 : mov    rax, rdx                 ; 48 89 d0                 
    17 : mov    rdx, [rsp+#0x08]         ; 48 8b 94 24 08 00 00 00  
    18 : cmp    rax, #0                  ; 48 83 f8 00              
    19 : je     [22]                     ; 0f 84 08 00 00 00        
    20 : mov    rcx, rdx                 ; 48 89 d1                 
    21 : jmp    [24]                     ; e9 09 00 00 00           
    22 : add    rdx, #0x04               ; 48 83 c2 04              
    23 : jmp    [5]                      ; e9 a9 ff ff ff           
    24 : mov    rdi, #0x04               ; 48 c7 c7 04 00 00 00     
    25 : mov    [rsp], rax               ; 48 89 44 24 00           
    26 : mov    [rsp+#0x08], rdx         ; 48 89 54 24 08           
    27 : mov    rax, rcx                 ; 48 89 c8                 
    28 : cqo                             ; 48 99                    
    29 : idiv   rdi                      ; 48 f7 ff                 
    30 : mov    rdi, rax                 ; 48 89 c7                 
    31 : mov    rax, [rsp]               ; 48 8b 84 24 00 00 00 00  
    32 : mov    rdx, [rsp+#0x08]         ; 48 8b 94 24 08 00 00 00  
    33 : mov    rax, rdi                 ; 48 89 f8                 
    34 : mov    r12, [rsp+#0x10]         ; 4c 8b a4 24 10 00 00 00  
    35 : add    rsp, #0x18               ; 48 83 c4 18              
    36 : ret                             ; c3                       
