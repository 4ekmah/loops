nullify_msb_lsb(i0, i1, i2)
     0 : mov r9, rcx    ; 49 89 c9              
     1 : shr r9, #0x01  ; 49 c1 e9 01           
     2 : or  r9, rcx    ; 49 09 c9              
     3 : mov rax, r9    ; 4c 89 c8              
     4 : shr rax, #0x02 ; 48 c1 e8 02           
     5 : or  r9, rax    ; 49 09 c1              
     6 : mov rax, r9    ; 4c 89 c8              
     7 : shr rax, #0x04 ; 48 c1 e8 04           
     8 : or  r9, rax    ; 49 09 c1              
     9 : mov rax, r9    ; 4c 89 c8              
    10 : shr rax, #0x08 ; 48 c1 e8 08           
    11 : or  r9, rax    ; 49 09 c1              
    12 : mov rax, r9    ; 4c 89 c8              
    13 : shr rax, #0x10 ; 48 c1 e8 10           
    14 : or  r9, rax    ; 49 09 c1              
    15 : mov rax, r9    ; 4c 89 c8              
    16 : shr rax, #0x20 ; 48 c1 e8 20           
    17 : or  r9, rax    ; 49 09 c1              
    18 : add r9, #0x01  ; 49 81 c1 01 00 00 00  
    19 : shr r9, #0x01  ; 49 c1 e9 01           
    20 : xor r9, rcx    ; 49 31 c9              
    21 : mov [r8], r9   ; 4d 89 08              
    22 : mov r8, rcx    ; 49 89 c8              
    23 : sub r8, #0x01  ; 49 81 e8 01 00 00 00  
    24 : not r8         ; 49 f7 d0              
    25 : and r8, rcx    ; 49 21 c8              
    26 : xor r8, rcx    ; 49 31 c8              
    27 : mov [rdx], r8  ; 4c 89 02              
    28 : ret            ; c3                    
