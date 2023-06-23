nullify_msb_lsb(i0, i1, i2)
     0 : sub rsp, #0x28 ; 48 81 ec 28 00 00 00  
     1 : mov r9, rcx    ; 49 89 c9              
     2 : shr r9, #0x01  ; 49 c1 e9 01           
     3 : or  r9, rcx    ; 49 09 c9              
     4 : mov rax, r9    ; 4c 89 c8              
     5 : shr rax, #0x02 ; 48 c1 e8 02           
     6 : or  r9, rax    ; 49 09 c1              
     7 : mov rax, r9    ; 4c 89 c8              
     8 : shr rax, #0x04 ; 48 c1 e8 04           
     9 : or  r9, rax    ; 49 09 c1              
    10 : mov rax, r9    ; 4c 89 c8              
    11 : shr rax, #0x08 ; 48 c1 e8 08           
    12 : or  r9, rax    ; 49 09 c1              
    13 : mov rax, r9    ; 4c 89 c8              
    14 : shr rax, #0x10 ; 48 c1 e8 10           
    15 : or  r9, rax    ; 49 09 c1              
    16 : mov rax, r9    ; 4c 89 c8              
    17 : shr rax, #0x20 ; 48 c1 e8 20           
    18 : or  r9, rax    ; 49 09 c1              
    19 : add r9, #0x01  ; 49 81 c1 01 00 00 00  
    20 : shr r9, #0x01  ; 49 c1 e9 01           
    21 : xor r9, rcx    ; 49 31 c9              
    22 : mov [r8], r9   ; 4d 89 08              
    23 : mov r8, rcx    ; 49 89 c8              
    24 : sub r8, #0x01  ; 49 81 e8 01 00 00 00  
    25 : not r8         ; 49 f7 d0              
    26 : and r8, rcx    ; 49 21 c8              
    27 : xor r8, rcx    ; 49 31 c8              
    28 : mov [rdx], r8  ; 4c 89 02              
    29 : add rsp, #0x28 ; 48 81 c4 28 00 00 00  
    30 : ret            ; c3                    
