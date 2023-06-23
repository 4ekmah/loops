helloworld_call()
     0 : sub  rsp, #0x68           ; 48 81 ec 68 00 00 00           
     1 : mov  [rsp+#0x60], rbp     ; 48 89 ac 24 60 00 00 00        
     2 : mov  rbp, rsp             ; 48 89 e5                       
     3 : add  rbp, #0x60           ; 48 81 c5 60 00 00 00           
     4 : mov  rdi, #0x563e883e2d38 ; 48 bf 38 2d 3e 88 3e 56 00 00  
     5 : mov  [rsp], rax           ; 48 89 84 24 00 00 00 00        
     6 : mov  [rsp+#0x08], rcx     ; 48 89 8c 24 08 00 00 00        
     7 : mov  [rsp+#0x10], rdx     ; 48 89 94 24 10 00 00 00        
     8 : mov  [rsp+#0x18], rsi     ; 48 89 b4 24 18 00 00 00        
     9 : mov  [rsp+#0x20], rdi     ; 48 89 bc 24 20 00 00 00        
    10 : mov  [rsp+#0x28], r8      ; 4c 89 84 24 28 00 00 00        
    11 : mov  [rsp+#0x30], r9      ; 4c 89 8c 24 30 00 00 00        
    12 : mov  [rsp+#0x38], r10     ; 4c 89 94 24 38 00 00 00        
    13 : mov  [rsp+#0x40], r11     ; 4c 89 9c 24 40 00 00 00        
    14 : call [rdi]                ; ff d7                          
    15 : mov  rax, [rsp]           ; 48 8b 84 24 00 00 00 00        
    16 : mov  rcx, [rsp+#0x08]     ; 48 8b 8c 24 08 00 00 00        
    17 : mov  rdx, [rsp+#0x10]     ; 48 8b 94 24 10 00 00 00        
    18 : mov  rsi, [rsp+#0x18]     ; 48 8b b4 24 18 00 00 00        
    19 : mov  rdi, [rsp+#0x20]     ; 48 8b bc 24 20 00 00 00        
    20 : mov  r8, [rsp+#0x28]      ; 4c 8b 84 24 28 00 00 00        
    21 : mov  r9, [rsp+#0x30]      ; 4c 8b 8c 24 30 00 00 00        
    22 : mov  r10, [rsp+#0x38]     ; 4c 8b 94 24 38 00 00 00        
    23 : mov  r11, [rsp+#0x40]     ; 4c 8b 9c 24 40 00 00 00        
    24 : mov  rbp, [rsp+#0x60]     ; 48 8b ac 24 60 00 00 00        
    25 : add  rsp, #0x68           ; 48 81 c4 68 00 00 00           
    26 : ret                       ; c3                             
