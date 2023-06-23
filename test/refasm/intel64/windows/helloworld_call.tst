helloworld_call()
     0 : sub  rsp, #0x48           ; 48 81 ec 48 00 00 00           
     1 : mov  [rsp+#0x40], rbp     ; 48 89 ac 24 40 00 00 00        
     2 : mov  rbp, rsp             ; 48 89 e5                       
     3 : add  rbp, #0x40           ; 48 81 c5 40 00 00 00           
     4 : mov  rcx, #0x7ff7f11cdf10 ; 48 b9 10 df 1c f1 f7 7f 00 00  
     5 : mov  [rsp], rax           ; 48 89 84 24 00 00 00 00        
     6 : mov  [rsp+#0x08], rcx     ; 48 89 8c 24 08 00 00 00        
     7 : mov  [rsp+#0x10], rdx     ; 48 89 94 24 10 00 00 00        
     8 : mov  [rsp+#0x18], r8      ; 4c 89 84 24 18 00 00 00        
     9 : mov  [rsp+#0x20], r9      ; 4c 89 8c 24 20 00 00 00        
    10 : mov  [rsp+#0x28], r10     ; 4c 89 94 24 28 00 00 00        
    11 : mov  [rsp+#0x30], r11     ; 4c 89 9c 24 30 00 00 00        
    12 : sub  rsp, #0x20           ; 48 81 ec 20 00 00 00           
    13 : call [rcx]                ; ff d1                          
    14 : add  rsp, #0x20           ; 48 81 c4 20 00 00 00           
    15 : mov  rax, [rsp]           ; 48 8b 84 24 00 00 00 00        
    16 : mov  rcx, [rsp+#0x08]     ; 48 8b 8c 24 08 00 00 00        
    17 : mov  rdx, [rsp+#0x10]     ; 48 8b 94 24 10 00 00 00        
    18 : mov  r8, [rsp+#0x18]      ; 4c 8b 84 24 18 00 00 00        
    19 : mov  r9, [rsp+#0x20]      ; 4c 8b 8c 24 20 00 00 00        
    20 : mov  r10, [rsp+#0x28]     ; 4c 8b 94 24 28 00 00 00        
    21 : mov  r11, [rsp+#0x30]     ; 4c 8b 9c 24 30 00 00 00        
    22 : mov  rbp, [rsp+#0x40]     ; 48 8b ac 24 40 00 00 00        
    23 : add  rsp, #0x48           ; 48 81 c4 48 00 00 00           
    24 : ret                       ; c3                             
