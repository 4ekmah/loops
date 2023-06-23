exponentiation_by_squaring_1(i0, i1, i2)
     0 : sub    rsp, #0x08   ; 48 81 ec 08 00 00 00     
     1 : mov    [rsp], r12   ; 4c 89 a4 24 00 00 00 00  
     2 : mov    r9, #0x01    ; 49 c7 c1 01 00 00 00     
     3 : xor    r9, r9       ; 4d 31 c9                 
     4 : xor    rax, rax     ; 48 31 c0                 
     5 : cmp    rax, rdx     ; 48 39 d0                 
     6 : jge    [13]         ; 0f 8d 20 00 00 00        
     7 : movsxd r12, rcx, r9 ; 4e 63 24 09              
     8 : mov    [r8], r12d   ; 45 89 20                 
     9 : add    rax, #0x01   ; 48 05 01 00 00 00        
    10 : add    r9, #0x04    ; 49 81 c1 04 00 00 00     
    11 : add    r8, #0x04    ; 49 81 c0 04 00 00 00     
    12 : jmp    [5]          ; e9 d7 ff ff ff           
    13 : xor    rax, rax     ; 48 31 c0                 
    14 : mov    r12, [rsp]   ; 4c 8b a4 24 00 00 00 00  
    15 : add    rsp, #0x08   ; 48 81 c4 08 00 00 00     
    16 : ret                 ; c3                       
