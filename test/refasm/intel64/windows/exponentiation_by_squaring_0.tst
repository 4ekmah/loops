exponentiation_by_squaring_0(i0, i1, i2)
     0 : sub    rsp, #0x08   ; 48 83 ec 08              
     1 : mov    [rsp], r12   ; 4c 89 64 24 00           
     2 : mov    r9, #0x01    ; 49 c7 c1 01 00 00 00     
     3 : xor    r9, r9       ; 4d 31 c9                 
     4 : xor    rax, rax     ; 48 31 c0                 
     5 : cmp    rax, rdx     ; 48 39 d0                 
     6 : jge    [14]         ; 0f 8d 1f 00 00 00        
     7 : movsxd r12, rcx, r9 ; 4e 63 24 09              
     8 : mov    r12, #0x01   ; 49 c7 c4 01 00 00 00     
     9 : mov    [r8], r12d   ; 45 89 20                 
    10 : add    rax, #0x01   ; 48 83 c0 01              
    11 : add    r9, #0x04    ; 49 83 c1 04              
    12 : add    r8, #0x04    ; 49 83 c0 04              
    13 : jmp    [5]          ; e9 d8 ff ff ff           
    14 : xor    rax, rax     ; 48 31 c0                 
    15 : mov    r12, [rsp]   ; 4c 8b a4 24 00 00 00 00  
    16 : add    rsp, #0x08   ; 48 83 c4 08              
    17 : ret                 ; c3                       
