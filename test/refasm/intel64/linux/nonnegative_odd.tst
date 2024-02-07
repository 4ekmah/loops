nonnegative_odd(i0, i1)
     0 : sub    rsp, #0x38               ; 48 81 ec 38 00 00 00     
     1 : mov    [rsp+#0x28], r13         ; 4c 89 ac 24 28 00 00 00  
     2 : xor    rdx, rdx                 ; 48 31 d2                 
     3 : mov    r13, #0xfffffffffffffffc ; 49 c7 c5 fc ff ff ff     
     4 : mov    [rsp+#0x20], r13         ; 4c 89 ac 24 20 00 00 00  
     5 : imul   rsi, #0x04               ; 48 6b f6 04              
     6 : cmp    rdx, rsi                 ; 48 39 f2                 
     7 : jge    [27]                     ; 0f 8d 6c 00 00 00        
     8 : movsxd rax, rdi, rdx            ; 48 63 04 17              
     9 : cmp    rax, #0                  ; 48 83 f8 00              
    10 : jge    [13]                     ; 0f 8d 0c 00 00 00        
    11 : add    rdx, #0x04               ; 48 81 c2 04 00 00 00     
    12 : jmp    [6]                      ; e9 dd ff ff ff           
    13 : mov    rcx, #0x02               ; 48 c7 c1 02 00 00 00     
    14 : mov    [rsp], rax               ; 48 89 84 24 00 00 00 00  
    15 : mov    [rsp+#0x08], rdx         ; 48 89 94 24 08 00 00 00  
    16 : cqo                             ; 48 99                    
    17 : idiv   rcx                      ; 48 f7 f9                 
    18 : mov    rcx, rdx                 ; 48 89 d1                 
    19 : mov    rax, [rsp]               ; 48 8b 84 24 00 00 00 00  
    20 : mov    rdx, [rsp+#0x08]         ; 48 8b 94 24 08 00 00 00  
    21 : cmp    rcx, #0                  ; 48 83 f9 00              
    22 : je     [25]                     ; 0f 84 0d 00 00 00        
    23 : mov    [rsp+#0x20], rdx         ; 48 89 94 24 20 00 00 00  
    24 : jmp    [27]                     ; e9 0c 00 00 00           
    25 : add    rdx, #0x04               ; 48 81 c2 04 00 00 00     
    26 : jmp    [6]                      ; e9 8b ff ff ff           
    27 : mov    rdi, #0x04               ; 48 c7 c7 04 00 00 00     
    28 : mov    r13, [rsp+#0x20]         ; 4c 8b ac 24 20 00 00 00  
    29 : mov    [rsp], rax               ; 48 89 84 24 00 00 00 00  
    30 : mov    [rsp+#0x08], rdx         ; 48 89 94 24 08 00 00 00  
    31 : mov    rax, r13                 ; 4c 89 e8                 
    32 : cqo                             ; 48 99                    
    33 : idiv   rdi                      ; 48 f7 ff                 
    34 : mov    rdi, rax                 ; 48 89 c7                 
    35 : mov    rax, [rsp]               ; 48 8b 84 24 00 00 00 00  
    36 : mov    rdx, [rsp+#0x08]         ; 48 8b 94 24 08 00 00 00  
    37 : mov    rax, rdi                 ; 48 89 f8                 
    38 : mov    r13, [rsp+#0x28]         ; 4c 8b ac 24 28 00 00 00  
    39 : add    rsp, #0x38               ; 48 81 c4 38 00 00 00     
    40 : ret                             ; c3                       
