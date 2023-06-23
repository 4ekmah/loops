instruction_set_test()
     0 : mov    rax, #0x10fffffff       ; 48 b8 ff ff ff 0f 01 00 00 00        
     1 : mov    rdi, #0x10fffffff       ; 48 bf ff ff ff 0f 01 00 00 00        
     2 : mov    r8, #0x10fffffff        ; 49 b8 ff ff ff 0f 01 00 00 00        
     3 : mov    r15, #0x10fffffff       ; 49 bf ff ff ff 0f 01 00 00 00        
     4 : mov    [rsp+#0x100], rax       ; 48 89 84 24 00 01 00 00              
     5 : mov    [rsp+#0x100], rdi       ; 48 89 bc 24 00 01 00 00              
     6 : mov    [rsp+#0x100], r8        ; 4c 89 84 24 00 01 00 00              
     7 : mov    [rsp+#0x100], r15       ; 4c 89 bc 24 00 01 00 00              
     8 : mov    [rsp+#0x100], #0x101    ; 48 c7 84 24 00 01 00 00 01 01 00 00  
     9 : add    rax, #0x100             ; 48 05 00 01 00 00                    
    10 : add    rcx, #0x100             ; 48 81 c1 00 01 00 00                 
    11 : add    rdi, #0x100             ; 48 81 c7 00 01 00 00                 
    12 : add    r8, #0x100              ; 49 81 c0 00 01 00 00                 
    13 : add    r15, #0x100             ; 49 81 c7 00 01 00 00                 
    14 : add    rax, [rsp+#0x100]       ; 48 03 84 24 00 01 00 00              
    15 : add    rdi, [rsp+#0x100]       ; 48 03 bc 24 00 01 00 00              
    16 : add    r8, [rsp+#0x100]        ; 4c 03 84 24 00 01 00 00              
    17 : add    r15, [rsp+#0x100]       ; 4c 03 bc 24 00 01 00 00              
    18 : add    [rsp+#0x100], rax       ; 48 01 84 24 00 01 00 00              
    19 : add    [rsp+#0x100], rdi       ; 48 01 bc 24 00 01 00 00              
    20 : add    [rsp+#0x100], r8        ; 4c 01 84 24 00 01 00 00              
    21 : add    [rsp+#0x100], r15       ; 4c 01 bc 24 00 01 00 00              
    22 : add    [rsp+#0x100], #0x101    ; 48 81 84 24 00 01 00 00 01 01 00 00  
    23 : sub    rax, #0x100             ; 48 2d 00 01 00 00                    
    24 : sub    rcx, #0x100             ; 48 81 e9 00 01 00 00                 
    25 : sub    rdi, #0x100             ; 48 81 ef 00 01 00 00                 
    26 : sub    r8, #0x100              ; 49 81 e8 00 01 00 00                 
    27 : sub    r15, #0x100             ; 49 81 ef 00 01 00 00                 
    28 : sub    rax, [rsp+#0x100]       ; 48 2b 84 24 00 01 00 00              
    29 : sub    rdi, [rsp+#0x100]       ; 48 2b bc 24 00 01 00 00              
    30 : sub    r8, [rsp+#0x100]        ; 4c 2b 84 24 00 01 00 00              
    31 : sub    r15, [rsp+#0x100]       ; 4c 2b bc 24 00 01 00 00              
    32 : sub    [rsp+#0x100], rax       ; 48 29 84 24 00 01 00 00              
    33 : sub    [rsp+#0x100], rdi       ; 48 29 bc 24 00 01 00 00              
    34 : sub    [rsp+#0x100], r8        ; 4c 29 84 24 00 01 00 00              
    35 : sub    [rsp+#0x100], r15       ; 4c 29 bc 24 00 01 00 00              
    36 : sub    [rsp+#0x100], #0x101    ; 48 81 ac 24 00 01 00 00 01 01 00 00  
    37 : imul   rax, [rsp+#0x100]       ; 48 0f af 84 24 00 01 00 00           
    38 : imul   rdi, [rsp+#0x100]       ; 48 0f af bc 24 00 01 00 00           
    39 : imul   r8, [rsp+#0x100]        ; 4c 0f af 84 24 00 01 00 00           
    40 : imul   r15, [rsp+#0x100]       ; 4c 0f af bc 24 00 01 00 00           
    41 : idiv   [rsp+#0x100]            ; 48 f7 bc 24 00 01 00 00              
    42 : neg    [rsp+#0x100]            ; 48 f7 9c 24 00 01 00 00              
    43 : mov    [rax], #0x100           ; 48 c7 00 00 01 00 00                 
    44 : mov    [rax], #0x100           ; 48 c7 00 00 01 00 00                 
    45 : mov    [rdi], #0x100           ; 48 c7 07 00 01 00 00                 
    46 : mov    [r8], #0x100            ; 49 c7 00 00 01 00 00                 
    47 : mov    [r12], #0x100           ; 49 c7 04 24 00 01 00 00              
    48 : mov    [r13], #0x100           ; 49 c7 45 00 00 01 00 00              
    49 : mov    [rax], #0x100           ; c7 00 00 01 00 00                    
    50 : mov    [rax], #0x100           ; c7 00 00 01 00 00                    
    51 : mov    [rdi], #0x100           ; c7 07 00 01 00 00                    
    52 : mov    [r8], #0x100            ; 41 c7 00 00 01 00 00                 
    53 : mov    [r12], #0x100           ; 41 c7 04 24 00 01 00 00              
    54 : mov    [r13], #0x100           ; 41 c7 45 00 00 01 00 00              
    55 : mov    [rax], #0x100           ; 66 c7 00 00 01                       
    56 : mov    [rax], #0x100           ; 66 c7 00 00 01                       
    57 : mov    [rdi], #0x100           ; 66 c7 07 00 01                       
    58 : mov    [r8], #0x100            ; 66 41 c7 00 00 01                    
    59 : mov    [r12], #0x100           ; 66 41 c7 04 24 00 01                 
    60 : mov    [r13], #0x100           ; 66 41 c7 45 00 00 01                 
    61 : mov    [rax], #0xff            ; c6 00 ff                             
    62 : mov    [rax], #0xff            ; c6 00 ff                             
    63 : mov    [rdi], #0xff            ; c6 07 ff                             
    64 : mov    [r8], #0xff             ; 41 c6 00 ff                          
    65 : mov    [r12], #0xff            ; 41 c6 04 24 ff                       
    66 : mov    [r13], #0xff            ; 41 c6 45 00 ff                       
    67 : mov    [rax], rax              ; 48 89 00                             
    68 : mov    [rax], rax              ; 48 89 00                             
    69 : mov    [rax], rdi              ; 48 89 38                             
    70 : mov    [rax], r8               ; 4c 89 00                             
    71 : mov    [rdi], rax              ; 48 89 07                             
    72 : mov    [r8], rax               ; 49 89 00                             
    73 : mov    [r8], r8                ; 4d 89 00                             
    74 : mov    [r12], rax              ; 49 89 04 24                          
    75 : mov    [r12], rdi              ; 49 89 3c 24                          
    76 : mov    [r12], r8               ; 4d 89 04 24                          
    77 : mov    [r13], rax              ; 49 89 45 00                          
    78 : mov    [r13], rdi              ; 49 89 7d 00                          
    79 : mov    [r13], r8               ; 4d 89 45 00                          
    80 : mov    [rax], ax               ; 66 89 00                             
    81 : mov    [rax], ax               ; 66 89 00                             
    82 : mov    [rax], di               ; 66 89 38                             
    83 : mov    [rax], r8w              ; 66 44 89 00                          
    84 : mov    [rdi], ax               ; 66 89 07                             
    85 : mov    [r8], ax                ; 66 41 89 00                          
    86 : mov    [r8], r8w               ; 66 45 89 00                          
    87 : mov    [r12], ax               ; 66 41 89 04 24                       
    88 : mov    [r12], di               ; 66 41 89 3c 24                       
    89 : mov    [r12], r8w              ; 66 45 89 04 24                       
    90 : mov    [r13], ax               ; 66 41 89 45 00                       
    91 : mov    [r13], di               ; 66 41 89 7d 00                       
    92 : mov    [r13], r8w              ; 66 45 89 45 00                       
    93 : mov    [rax], al               ; 88 00                                
    94 : mov    [rax], al               ; 88 00                                
    95 : mov    [rax], dil              ; 40 88 38                             
    96 : mov    [rax], r8b              ; 44 88 00                             
    97 : mov    [rdi], al               ; 88 07                                
    98 : mov    [r8], al                ; 41 88 00                             
    99 : mov    [r8], r8b               ; 45 88 00                             
   100 : mov    [r12], al               ; 41 88 04 24                          
   101 : mov    [r12], dil              ; 41 88 3c 24                          
   102 : mov    [r12], r8b              ; 45 88 04 24                          
   103 : mov    [r13], al               ; 41 88 45 00                          
   104 : mov    [r13], dil              ; 41 88 7d 00                          
   105 : mov    [r13], r8b              ; 45 88 45 00                          
   106 : mov    [rax], [rax], rax       ; 48 89 04 00                          
   107 : mov    [rax], [rax], rax       ; 48 89 04 00                          
   108 : mov    [rax], [rax], rdi       ; 48 89 3c 00                          
   109 : mov    [rax], [rax], r8        ; 4c 89 04 00                          
   110 : mov    [rax], [rdi], rax       ; 48 89 04 38                          
   111 : mov    [rax], [r8], rax        ; 4a 89 04 00                          
   112 : mov    [rax], [r8], r8         ; 4e 89 04 00                          
   113 : mov    [rdi], [rax], rax       ; 48 89 04 07                          
   114 : mov    [r8], [rax], rax        ; 49 89 04 00                          
   115 : mov    [r8], [rax], r8         ; 4d 89 04 00                          
   116 : mov    [r8], [r8], rax         ; 4b 89 04 00                          
   117 : mov    [r8], [r8], r8          ; 4f 89 04 00                          
   118 : mov    [rax], [rax], eax       ; 89 04 00                             
   119 : mov    [rax], [rax], eax       ; 89 04 00                             
   120 : mov    [rax], [rax], edi       ; 89 3c 00                             
   121 : mov    [rax], [rax], r8d       ; 44 89 04 00                          
   122 : mov    [rax], [rdi], eax       ; 89 04 38                             
   123 : mov    [rax], [r8], eax        ; 42 89 04 00                          
   124 : mov    [rax], [r8], r8d        ; 46 89 04 00                          
   125 : mov    [rdi], [rax], eax       ; 89 04 07                             
   126 : mov    [r8], [rax], eax        ; 41 89 04 00                          
   127 : mov    [r8], [rax], r8d        ; 45 89 04 00                          
   128 : mov    [r8], [r8], eax         ; 43 89 04 00                          
   129 : mov    [r8], [r8], r8d         ; 47 89 04 00                          
   130 : mov    [r13], [rax], eax       ; 41 89 44 05 00                       
   131 : mov    [r13], [rax], r8d       ; 45 89 44 05 00                       
   132 : mov    [r13], [r8], eax        ; 43 89 44 05 00                       
   133 : mov    [r13], [r8], r8d        ; 47 89 44 05 00                       
   134 : mov    [rax], [rax], ax        ; 66 89 04 00                          
   135 : mov    [rax], [rax], ax        ; 66 89 04 00                          
   136 : mov    [rax], [rax], di        ; 66 89 3c 00                          
   137 : mov    [rax], [rax], r8w       ; 66 44 89 04 00                       
   138 : mov    [rax], [rdi], ax        ; 66 89 04 38                          
   139 : mov    [rax], [r8], ax         ; 66 42 89 04 00                       
   140 : mov    [rax], [r8], r8w        ; 66 46 89 04 00                       
   141 : mov    [rdi], [rax], ax        ; 66 89 04 07                          
   142 : mov    [r8], [rax], ax         ; 66 41 89 04 00                       
   143 : mov    [r8], [rax], r8w        ; 66 45 89 04 00                       
   144 : mov    [r8], [r8], ax          ; 66 43 89 04 00                       
   145 : mov    [r8], [r8], r8w         ; 66 47 89 04 00                       
   146 : mov    [r13], [rax], ax        ; 66 41 89 44 05 00                    
   147 : mov    [r13], [r8], ax         ; 66 43 89 44 05 00                    
   148 : mov    [r13], [rax], r8w       ; 66 45 89 44 05 00                    
   149 : mov    [r13], [r8], r8w        ; 66 47 89 44 05 00                    
   150 : mov    [rax], [rax], al        ; 88 04 00                             
   151 : mov    [rax], [rax], al        ; 88 04 00                             
   152 : mov    [rax], [rax], dil       ; 40 88 3c 00                          
   153 : mov    [rax], [rax], r8b       ; 44 88 04 00                          
   154 : mov    [rax], [rdi], al        ; 88 04 38                             
   155 : mov    [rax], [r8], al         ; 42 88 04 00                          
   156 : mov    [rax], [r8], dil        ; 42 88 3c 00                          
   157 : mov    [rax], [r8], r8b        ; 46 88 04 00                          
   158 : mov    [rdi], [rax], al        ; 88 04 07                             
   159 : mov    [r8], [rax], al         ; 41 88 04 00                          
   160 : mov    [r8], [rax], dil        ; 41 88 3c 00                          
   161 : mov    [r8], [rax], r8b        ; 45 88 04 00                          
   162 : mov    [r8], [r8], al          ; 43 88 04 00                          
   163 : mov    [r8], [r8], r8b         ; 47 88 04 00                          
   164 : mov    [r8], [r8], dil         ; 43 88 3c 00                          
   165 : mov    [r13], [rax], al        ; 41 88 44 05 00                       
   166 : mov    [r13], [rax], r8b       ; 45 88 44 05 00                       
   167 : mov    [r13], [r8], al         ; 43 88 44 05 00                       
   168 : mov    [r13], [r8], r8b        ; 47 88 44 05 00                       
   169 : mov    [rax], [#0x100], rax    ; 48 89 80 00 01 00 00                 
   170 : mov    [rax], [#0x100], rax    ; 48 89 80 00 01 00 00                 
   171 : mov    [rax], [#0x100], rdi    ; 48 89 b8 00 01 00 00                 
   172 : mov    [rax], [#0x100], r8     ; 4c 89 80 00 01 00 00                 
   173 : mov    [rdi], [#0x100], rax    ; 48 89 87 00 01 00 00                 
   174 : mov    [r8], [#0x100], rax     ; 49 89 80 00 01 00 00                 
   175 : mov    [r8], [#0x100], r8      ; 4d 89 80 00 01 00 00                 
   176 : mov    [r12], [#0x100], rax    ; 49 89 84 24 00 01 00 00              
   177 : mov    [r12], [#0x100], rdi    ; 49 89 bc 24 00 01 00 00              
   178 : mov    [r12], [#0x100], r8     ; 4d 89 84 24 00 01 00 00              
   179 : mov    [rax], [#0x100], eax    ; 89 80 00 01 00 00                    
   180 : mov    [rax], [#0x100], eax    ; 89 80 00 01 00 00                    
   181 : mov    [rax], [#0x100], edi    ; 89 b8 00 01 00 00                    
   182 : mov    [rax], [#0x100], r8d    ; 44 89 80 00 01 00 00                 
   183 : mov    [rdi], [#0x100], eax    ; 89 87 00 01 00 00                    
   184 : mov    [r8], [#0x100], eax     ; 41 89 80 00 01 00 00                 
   185 : mov    [r8], [#0x100], r8d     ; 45 89 80 00 01 00 00                 
   186 : mov    [r12], [#0x100], eax    ; 41 89 84 24 00 01 00 00              
   187 : mov    [r12], [#0x100], r8d    ; 45 89 84 24 00 01 00 00              
   188 : mov    [rax], [#0x100], ax     ; 66 89 80 00 01 00 00                 
   189 : mov    [rax], [#0x100], ax     ; 66 89 80 00 01 00 00                 
   190 : mov    [rax], [#0x100], di     ; 66 89 b8 00 01 00 00                 
   191 : mov    [rax], [#0x100], r8w    ; 66 44 89 80 00 01 00 00              
   192 : mov    [rdi], [#0x100], ax     ; 66 89 87 00 01 00 00                 
   193 : mov    [r8], [#0x100], ax      ; 66 41 89 80 00 01 00 00              
   194 : mov    [r8], [#0x100], r8w     ; 66 45 89 80 00 01 00 00              
   195 : mov    [r12], [#0x100], ax     ; 66 41 89 84 24 00 01 00 00           
   196 : mov    [r12], [#0x100], r8w    ; 66 45 89 84 24 00 01 00 00           
   197 : mov    [rax], [#0x100], al     ; 88 80 00 01 00 00                    
   198 : mov    [rax], [#0x100], al     ; 88 80 00 01 00 00                    
   199 : mov    [rax], [#0x100], dil    ; 40 88 b8 00 01 00 00                 
   200 : mov    [rax], [#0x100], r8b    ; 44 88 80 00 01 00 00                 
   201 : mov    [rdi], [#0x100], al     ; 88 87 00 01 00 00                    
   202 : mov    [r8], [#0x100], al      ; 41 88 80 00 01 00 00                 
   203 : mov    [r8], [#0x100], r8b     ; 45 88 80 00 01 00 00                 
   204 : mov    [r12], [#0x100], al     ; 41 88 84 24 00 01 00 00              
   205 : mov    [r12], [#0x100], r8b    ; 45 88 84 24 00 01 00 00              
   206 : mov    [rax], [rax], #0x100    ; 48 c7 04 00 00 01 00 00              
   207 : mov    [rax], [rax], #0x100    ; 48 c7 04 00 00 01 00 00              
   208 : mov    [rax], [rdi], #0x100    ; 48 c7 04 38 00 01 00 00              
   209 : mov    [rax], [r8], #0x100     ; 4a c7 04 00 00 01 00 00              
   210 : mov    [rdi], [rax], #0x100    ; 48 c7 04 07 00 01 00 00              
   211 : mov    [r8], [rax], #0x100     ; 49 c7 04 00 00 01 00 00              
   212 : mov    [r8], [r8], #0x100      ; 4b c7 04 00 00 01 00 00              
   213 : mov    [r13], [rax], #0x100    ; 49 c7 44 05 00 00 01 00 00           
   214 : mov    [r13], [r8], #0x100     ; 4b c7 44 05 00 00 01 00 00           
   215 : mov    [rax], [rax], #0x100    ; c7 04 00 00 01 00 00                 
   216 : mov    [rax], [rax], #0x100    ; c7 04 00 00 01 00 00                 
   217 : mov    [rax], [rdi], #0x100    ; c7 04 38 00 01 00 00                 
   218 : mov    [rax], [r8], #0x100     ; 42 c7 04 00 00 01 00 00              
   219 : mov    [rdi], [rax], #0x100    ; c7 04 07 00 01 00 00                 
   220 : mov    [r8], [rax], #0x100     ; 41 c7 04 00 00 01 00 00              
   221 : mov    [r8], [r8], #0x100      ; 43 c7 04 00 00 01 00 00              
   222 : mov    [r13], [rax], #0x100    ; 41 c7 44 05 00 00 01 00 00           
   223 : mov    [r13], [r8], #0x100     ; 43 c7 44 05 00 00 01 00 00           
   224 : mov    [rax], [rax], #0x100    ; 66 c7 04 00 00 01                    
   225 : mov    [rax], [rax], #0x100    ; 66 c7 04 00 00 01                    
   226 : mov    [rax], [rdi], #0x100    ; 66 c7 04 38 00 01                    
   227 : mov    [rax], [r8], #0x100     ; 66 42 c7 04 00 00 01                 
   228 : mov    [rdi], [rax], #0x100    ; 66 c7 04 07 00 01                    
   229 : mov    [r8], [rax], #0x100     ; 66 41 c7 04 00 00 01                 
   230 : mov    [r8], [r8], #0x100      ; 66 43 c7 04 00 00 01                 
   231 : mov    [r13], [rax], #0x100    ; 66 41 c7 44 05 00 00 01              
   232 : mov    [r13], [r8], #0x100     ; 66 43 c7 44 05 00 00 01              
   233 : mov    [rax], [rax], #0xff     ; c6 04 00 ff                          
   234 : mov    [rax], [rax], #0xff     ; c6 04 00 ff                          
   235 : mov    [rax], [rdi], #0xff     ; c6 04 38 ff                          
   236 : mov    [rax], [r8], #0xff      ; 42 c6 04 00 ff                       
   237 : mov    [rdi], [rax], #0xff     ; c6 04 07 ff                          
   238 : mov    [r8], [rax], #0xff      ; 41 c6 04 00 ff                       
   239 : mov    [r8], [r8], #0xff       ; 43 c6 04 00 ff                       
   240 : mov    [r13], [rax], #0xff     ; 41 c6 44 05 00 ff                    
   241 : mov    [r13], [r8], #0xff      ; 43 c6 44 05 00 ff                    
   242 : mov    [rax], [#0x101], #0x100 ; 48 c7 80 01 01 00 00 00 01 00 00     
   243 : mov    [rax], [#0x101], #0x100 ; 48 c7 80 01 01 00 00 00 01 00 00     
   244 : mov    [rdi], [#0x101], #0x100 ; 48 c7 87 01 01 00 00 00 01 00 00     
   245 : mov    [r8], [#0x101], #0x100  ; 49 c7 80 01 01 00 00 00 01 00 00     
   246 : mov    [r12], [#0x101], #0x100 ; 49 c7 84 24 01 01 00 00 00 01 00 00  
   247 : mov    [rax], [#0x101], #0x100 ; c7 80 01 01 00 00 00 01 00 00        
   248 : mov    [rax], [#0x101], #0x100 ; c7 80 01 01 00 00 00 01 00 00        
   249 : mov    [rdi], [#0x101], #0x100 ; c7 87 01 01 00 00 00 01 00 00        
   250 : mov    [r8], [#0x101], #0x100  ; 41 c7 80 01 01 00 00 00 01 00 00     
   251 : mov    [r12], [#0x101], #0x100 ; 41 c7 84 24 01 01 00 00 00 01 00 00  
   252 : mov    [rax], [#0x101], #0x100 ; 66 c7 80 01 01 00 00 00 01           
   253 : mov    [rax], [#0x101], #0x100 ; 66 c7 80 01 01 00 00 00 01           
   254 : mov    [rdi], [#0x101], #0x100 ; 66 c7 87 01 01 00 00 00 01           
   255 : mov    [r8], [#0x101], #0x100  ; 66 41 c7 80 01 01 00 00 00 01        
   256 : mov    [r12], [#0x101], #0x100 ; 66 41 c7 84 24 01 01 00 00 00 01     
   257 : mov    [rax], [#0x101], #0xff  ; c6 80 01 01 00 00 ff                 
   258 : mov    [rax], [#0x101], #0xff  ; c6 80 01 01 00 00 ff                 
   259 : mov    [rdi], [#0x101], #0xff  ; c6 87 01 01 00 00 ff                 
   260 : mov    [r8], [#0x101], #0xff   ; 41 c6 80 01 01 00 00 ff              
   261 : mov    [r12], [#0x101], #0xff  ; 41 c6 84 24 01 01 00 00 ff           
   262 : mov    rax, [rax]              ; 48 8b 00                             
   263 : mov    rax, [rax]              ; 48 8b 00                             
   264 : mov    rdi, [rax]              ; 48 8b 38                             
   265 : mov    rax, [rdi]              ; 48 8b 07                             
   266 : mov    rax, [rax]              ; 48 8b 00                             
   267 : mov    r8, [rax]               ; 4c 8b 00                             
   268 : mov    rax, [r8]               ; 49 8b 00                             
   269 : mov    r8, [r8]                ; 4d 8b 00                             
   270 : mov    eax, [rax]              ; 8b 00                                
   271 : mov    edi, [rax]              ; 8b 38                                
   272 : mov    eax, [rdi]              ; 8b 07                                
   273 : mov    eax, [rax]              ; 8b 00                                
   274 : mov    r8d, [rax]              ; 44 8b 00                             
   275 : mov    eax, [r8]               ; 41 8b 00                             
   276 : mov    r8d, [r8]               ; 45 8b 00                             
   277 : movzx  rax, ax                 ; 48 0f b7 00                          
   278 : movzx  rdi, ax                 ; 48 0f b7 38                          
   279 : movzx  rax, di                 ; 48 0f b7 07                          
   280 : movzx  r8, ax                  ; 4c 0f b7 00                          
   281 : movzx  rax, r8w                ; 49 0f b7 00                          
   282 : movzx  r8, r8w                 ; 4d 0f b7 00                          
   283 : movsx  rax, ax                 ; 48 0f bf 00                          
   284 : movsx  rdi, ax                 ; 48 0f bf 38                          
   285 : movsx  rax, di                 ; 48 0f bf 07                          
   286 : movsx  r8, ax                  ; 4c 0f bf 00                          
   287 : movsx  rax, r8w                ; 49 0f bf 00                          
   288 : movsx  r8, r8w                 ; 4d 0f bf 00                          
   289 : movzx  rax, al                 ; 48 0f b6 00                          
   290 : movzx  rdi, al                 ; 48 0f b6 38                          
   291 : movzx  rax, dil                ; 48 0f b6 07                          
   292 : movzx  r8, al                  ; 4c 0f b6 00                          
   293 : movzx  rax, r8b                ; 49 0f b6 00                          
   294 : movzx  r8, r8b                 ; 4d 0f b6 00                          
   295 : movsx  rax, al                 ; 48 0f be 00                          
   296 : movsx  rdi, al                 ; 48 0f be 38                          
   297 : movsx  rax, dil                ; 48 0f be 07                          
   298 : movsx  r8, al                  ; 4c 0f be 00                          
   299 : movsx  rax, r8b                ; 49 0f be 00                          
   300 : movsx  r8, r8b                 ; 4d 0f be 00                          
   301 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   302 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   303 : mov    rdi, [rax], [#0x100]    ; 48 8b b8 00 01 00 00                 
   304 : mov    rax, [rdi], [#0x100]    ; 48 8b 87 00 01 00 00                 
   305 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   306 : mov    r8, [rax], [#0x100]     ; 4c 8b 80 00 01 00 00                 
   307 : mov    rax, [r8], [#0x100]     ; 49 8b 80 00 01 00 00                 
   308 : mov    r8, [r8], [#0x100]      ; 4d 8b 80 00 01 00 00                 
   309 : mov    eax, [rax], [#0x100]    ; 8b 80 00 01 00 00                    
   310 : mov    edi, [rax], [#0x100]    ; 8b b8 00 01 00 00                    
   311 : mov    eax, [rdi], [#0x100]    ; 8b 87 00 01 00 00                    
   312 : mov    r8d, [rax], [#0x100]    ; 44 8b 80 00 01 00 00                 
   313 : mov    eax, [r8], [#0x100]     ; 41 8b 80 00 01 00 00                 
   314 : mov    r8d, [r8], [#0x100]     ; 45 8b 80 00 01 00 00                 
   315 : movsxd rax, rax, #0x100        ; 48 63 80 00 01 00 00                 
   316 : movsxd rdi, rax, #0x100        ; 48 63 b8 00 01 00 00                 
   317 : movsxd rax, rdi, #0x100        ; 48 63 87 00 01 00 00                 
   318 : movsxd r8, rax, #0x100         ; 4c 63 80 00 01 00 00                 
   319 : movsxd rax, r8, #0x100         ; 49 63 80 00 01 00 00                 
   320 : movsxd r8, r8, #0x100          ; 4d 63 80 00 01 00 00                 
   321 : movzx  rax, ax, #0x100         ; 48 0f b7 80 00 01 00 00              
   322 : movzx  rdi, ax, #0x100         ; 48 0f b7 b8 00 01 00 00              
   323 : movzx  rax, di, #0x100         ; 48 0f b7 87 00 01 00 00              
   324 : movzx  r8, ax, #0x100          ; 4c 0f b7 80 00 01 00 00              
   325 : movzx  rax, r8w, #0x100        ; 49 0f b7 80 00 01 00 00              
   326 : movzx  r8, r8w, #0x100         ; 4d 0f b7 80 00 01 00 00              
   327 : movsx  rax, ax, #0x100         ; 48 0f bf 80 00 01 00 00              
   328 : movsx  rdi, ax, #0x100         ; 48 0f bf b8 00 01 00 00              
   329 : movsx  rax, di, #0x100         ; 48 0f bf 87 00 01 00 00              
   330 : movsx  r8, ax, #0x100          ; 4c 0f bf 80 00 01 00 00              
   331 : movsx  rax, r8w, #0x100        ; 49 0f bf 80 00 01 00 00              
   332 : movsx  r8, r8w, #0x100         ; 4d 0f bf 80 00 01 00 00              
   333 : movzx  rax, al, #0x100         ; 48 0f b6 80 00 01 00 00              
   334 : movzx  rdi, al, #0x100         ; 48 0f b6 b8 00 01 00 00              
   335 : movzx  rax, dil, #0x100        ; 48 0f b6 87 00 01 00 00              
   336 : movzx  r8, al, #0x100          ; 4c 0f b6 80 00 01 00 00              
   337 : movzx  rax, r8b, #0x100        ; 49 0f b6 80 00 01 00 00              
   338 : movzx  r8, r8b, #0x100         ; 4d 0f b6 80 00 01 00 00              
   339 : movsx  rax, al, #0x100         ; 48 0f be 80 00 01 00 00              
   340 : movsx  rdi, al, #0x100         ; 48 0f be b8 00 01 00 00              
   341 : movsx  rax, dil, #0x100        ; 48 0f be 87 00 01 00 00              
   342 : movsx  r8, al, #0x100          ; 4c 0f be 80 00 01 00 00              
   343 : movsx  rax, r8b, #0x100        ; 49 0f be 80 00 01 00 00              
   344 : movsx  r8, r8b, #0x100         ; 4d 0f be 80 00 01 00 00              
   345 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   346 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   347 : mov    rdi, [rax], [rax]       ; 48 8b 3c 00                          
   348 : mov    rax, [rdi], [rax]       ; 48 8b 04 07                          
   349 : mov    rax, [rax], [rdi]       ; 48 8b 04 38                          
   350 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   351 : mov    r8, [rax], [rax]        ; 4c 8b 04 00                          
   352 : mov    rax, [r8], [rax]        ; 49 8b 04 00                          
   353 : mov    r8, [r8], [rax]         ; 4d 8b 04 00                          
   354 : mov    rax, [rax], [r8]        ; 4a 8b 04 00                          
   355 : mov    r8, [rax], [r8]         ; 4e 8b 04 00                          
   356 : mov    rax, [r8], [r8]         ; 4b 8b 04 00                          
   357 : mov    r8, [r8], [r8]          ; 4f 8b 04 00                          
   358 : mov    eax, [rax], [rax]       ; 8b 04 00                             
   359 : mov    edi, [rax], [rax]       ; 8b 3c 00                             
   360 : mov    eax, [rdi], [rax]       ; 8b 04 07                             
   361 : mov    eax, [rax], [rdi]       ; 8b 04 38                             
   362 : mov    r8d, [rax], [rax]       ; 44 8b 04 00                          
   363 : mov    eax, [r8], [rax]        ; 41 8b 04 00                          
   364 : mov    r8d, [r8], [rax]        ; 45 8b 04 00                          
   365 : mov    eax, [rax], [r8]        ; 42 8b 04 00                          
   366 : mov    r8d, [rax], [r8]        ; 46 8b 04 00                          
   367 : mov    eax, [r8], [r8]         ; 43 8b 04 00                          
   368 : mov    r8d, [r8], [r8]         ; 47 8b 04 00                          
   369 : movzx  rax, ax, rax            ; 48 0f b7 04 00                       
   370 : movzx  rdi, ax, rax            ; 48 0f b7 3c 00                       
   371 : movzx  rax, di, rax            ; 48 0f b7 04 07                       
   372 : movzx  rax, ax, rdi            ; 48 0f b7 04 38                       
   373 : movzx  r8, ax, rax             ; 4c 0f b7 04 00                       
   374 : movzx  rax, r8w, rax           ; 49 0f b7 04 00                       
   375 : movzx  r8, r8w, rax            ; 4d 0f b7 04 00                       
   376 : movzx  rax, ax, r8             ; 4a 0f b7 04 00                       
   377 : movzx  r8, ax, r8              ; 4e 0f b7 04 00                       
   378 : movzx  rax, r8w, r8            ; 4b 0f b7 04 00                       
   379 : movzx  r8, r8w, r8             ; 4f 0f b7 04 00                       
   380 : movsx  rax, ax, rax            ; 48 0f bf 04 00                       
   381 : movsx  rdi, ax, rax            ; 48 0f bf 3c 00                       
   382 : movsx  rax, di, rax            ; 48 0f bf 04 07                       
   383 : movsx  rax, ax, rdi            ; 48 0f bf 04 38                       
   384 : movsx  r8, ax, rax             ; 4c 0f bf 04 00                       
   385 : movsx  rax, r8w, rax           ; 49 0f bf 04 00                       
   386 : movsx  r8, r8w, rax            ; 4d 0f bf 04 00                       
   387 : movsx  rax, ax, r8             ; 4a 0f bf 04 00                       
   388 : movsx  r8, ax, r8              ; 4e 0f bf 04 00                       
   389 : movsx  rax, r8w, r8            ; 4b 0f bf 04 00                       
   390 : movsx  r8, r8w, r8             ; 4f 0f bf 04 00                       
   391 : movzx  rax, al, rax            ; 48 0f b6 04 00                       
   392 : movzx  rdi, al, rax            ; 48 0f b6 3c 00                       
   393 : movzx  rax, dil, rax           ; 48 0f b6 04 07                       
   394 : movzx  rax, al, rdi            ; 48 0f b6 04 38                       
   395 : movzx  r8, al, rax             ; 4c 0f b6 04 00                       
   396 : movzx  rax, r8b, rax           ; 49 0f b6 04 00                       
   397 : movzx  r8, r8b, rax            ; 4d 0f b6 04 00                       
   398 : movzx  rax, al, r8             ; 4a 0f b6 04 00                       
   399 : movzx  r8, al, r8              ; 4e 0f b6 04 00                       
   400 : movzx  rax, r8b, r8            ; 4b 0f b6 04 00                       
   401 : movzx  r8, r8b, r8             ; 4f 0f b6 04 00                       
   402 : movsx  rax, al, rax            ; 48 0f be 04 00                       
   403 : movsx  rdi, al, rax            ; 48 0f be 3c 00                       
   404 : movsx  rax, dil, rax           ; 48 0f be 04 07                       
   405 : movsx  rax, al, rdi            ; 48 0f be 04 38                       
   406 : movsx  r8, al, rax             ; 4c 0f be 04 00                       
   407 : movsx  rax, r8b, rax           ; 49 0f be 04 00                       
   408 : movsx  r8, r8b, rax            ; 4d 0f be 04 00                       
   409 : movsx  rax, al, r8             ; 4a 0f be 04 00                       
   410 : movsx  r8, al, r8              ; 4e 0f be 04 00                       
   411 : movsx  rax, r8b, r8            ; 4b 0f be 04 00                       
   412 : movsx  r8, r8b, r8             ; 4f 0f be 04 00                       
   413 : xchg   rax, rax                ; 48 90                                
   414 : xchg   rax, rdi                ; 48 97                                
   415 : xchg   rax, r8                 ; 49 90                                
   416 : xchg   rcx, rcx                ; 48 87 c9                             
   417 : xchg   rdi, rcx                ; 48 87 cf                             
   418 : xchg   rcx, rdi                ; 48 87 f9                             
   419 : xchg   rcx, rcx                ; 48 87 c9                             
   420 : xchg   r8, rcx                 ; 49 87 c8                             
   421 : xchg   rcx, r8                 ; 4c 87 c1                             
   422 : xchg   r8, r8                  ; 4d 87 c0                             
   423 : xchg   rcx, [rsp+#0x100]       ; 48 87 8c 24 00 01 00 00              
   424 : xchg   rdi, [rsp+#0x100]       ; 48 87 bc 24 00 01 00 00              
   425 : xchg   r8, [rsp+#0x100]        ; 4c 87 84 24 00 01 00 00              
   426 : xchg   r15, [rsp+#0x100]       ; 4c 87 bc 24 00 01 00 00              
   427 : xchg   [rsp+#0x100], rcx       ; 48 87 8c 24 00 01 00 00              
   428 : xchg   [rsp+#0x100], rdi       ; 48 87 bc 24 00 01 00 00              
   429 : xchg   [rsp+#0x100], r8        ; 4c 87 84 24 00 01 00 00              
   430 : xchg   [rsp+#0x100], r15       ; 4c 87 bc 24 00 01 00 00              
   431 : shl    rax, #0x0f              ; 48 c1 e0 0f                          
   432 : shl    rdi, #0x0f              ; 48 c1 e7 0f                          
   433 : shl    r8, #0x0f               ; 49 c1 e0 0f                          
   434 : shl    [rsp+#0x100], #0x0f     ; 48 c1 a4 24 00 01 00 00 0f           
   435 : shl    rax, cl                 ; 48 d3 e0                             
   436 : shl    rdi, cl                 ; 48 d3 e7                             
   437 : shl    r8, cl                  ; 49 d3 e0                             
   438 : shl    [rsp+#0x100], cl        ; 48 d3 a4 24 00 01 00 00              
   439 : shr    rax, #0x0f              ; 48 c1 e8 0f                          
   440 : shr    rdi, #0x0f              ; 48 c1 ef 0f                          
   441 : shr    r8, #0x0f               ; 49 c1 e8 0f                          
   442 : shr    [rsp+#0x100], #0x0f     ; 48 c1 ac 24 00 01 00 00 0f           
   443 : shr    rax, cl                 ; 48 d3 e8                             
   444 : shr    rdi, cl                 ; 48 d3 ef                             
   445 : shr    r8, cl                  ; 49 d3 e8                             
   446 : shr    [rsp+#0x100], cl        ; 48 d3 ac 24 00 01 00 00              
   447 : sar    rax, #0x0f              ; 48 c1 f8 0f                          
   448 : sar    rdi, #0x0f              ; 48 c1 ff 0f                          
   449 : sar    r8, #0x0f               ; 49 c1 f8 0f                          
   450 : sar    [rsp+#0x100], #0x0f     ; 48 c1 bc 24 00 01 00 00 0f           
   451 : sar    rax, cl                 ; 48 d3 f8                             
   452 : sar    rdi, cl                 ; 48 d3 ff                             
   453 : sar    r8, cl                  ; 49 d3 f8                             
   454 : sar    [rsp+#0x100], cl        ; 48 d3 bc 24 00 01 00 00              
   455 : and    rax, rax                ; 48 21 c0                             
   456 : and    rax, rdi                ; 48 21 f8                             
   457 : and    rdi, rax                ; 48 21 c7                             
   458 : and    rax, r8                 ; 4c 21 c0                             
   459 : and    r8, rax                 ; 49 21 c0                             
   460 : and    r8, r8                  ; 4d 21 c0                             
   461 : and    rax, [rsp+#0x100]       ; 48 23 84 24 00 01 00 00              
   462 : and    rdi, [rsp+#0x100]       ; 48 23 bc 24 00 01 00 00              
   463 : and    r8, [rsp+#0x100]        ; 4c 23 84 24 00 01 00 00              
   464 : and    rax, #0x100             ; 48 25 00 01 00 00                    
   465 : and    rcx, #0x100             ; 48 81 e1 00 01 00 00                 
   466 : and    rdi, #0x100             ; 48 81 e7 00 01 00 00                 
   467 : and    r8, #0x100              ; 49 81 e0 00 01 00 00                 
   468 : and    [rsp+#0x100], rax       ; 48 21 84 24 00 01 00 00              
   469 : and    [rsp+#0x100], rdi       ; 48 21 bc 24 00 01 00 00              
   470 : and    [rsp+#0x100], r8        ; 4c 21 84 24 00 01 00 00              
   471 : and    [rsp+#0x100], #0x101    ; 48 81 a4 24 00 01 00 00 01 01 00 00  
   472 : or     rax, rax                ; 48 09 c0                             
   473 : or     rax, rdi                ; 48 09 f8                             
   474 : or     rdi, rax                ; 48 09 c7                             
   475 : or     rax, r8                 ; 4c 09 c0                             
   476 : or     r8, rax                 ; 49 09 c0                             
   477 : or     [rsp+#0x100], rax       ; 48 09 84 24 00 01 00 00              
   478 : or     [rsp+#0x100], r8        ; 4c 09 84 24 00 01 00 00              
   479 : or     rax, [rsp+#0x100]       ; 48 0b 84 24 00 01 00 00              
   480 : or     r8, [rsp+#0x100]        ; 4c 0b 84 24 00 01 00 00              
   481 : or     rax, #0x100             ; 48 0d 00 01 00 00                    
   482 : or     r8, #0x100              ; 49 81 c8 00 01 00 00                 
   483 : or     [rsp+#0x100], #0x101    ; 48 81 8c 24 00 01 00 00 01 01 00 00  
   484 : xor    rax, rax                ; 48 31 c0                             
   485 : xor    rax, rdi                ; 48 31 f8                             
   486 : xor    rdi, rax                ; 48 31 c7                             
   487 : xor    rax, r8                 ; 4c 31 c0                             
   488 : xor    r8, rax                 ; 49 31 c0                             
   489 : xor    r8, r8                  ; 4d 31 c0                             
   490 : xor    rax, [rsp+#0x100]       ; 48 33 84 24 00 01 00 00              
   491 : xor    rdi, [rsp+#0x100]       ; 48 33 bc 24 00 01 00 00              
   492 : xor    r8, [rsp+#0x100]        ; 4c 33 84 24 00 01 00 00              
   493 : xor    rax, #0x100             ; 48 35 00 01 00 00                    
   494 : xor    rcx, #0x100             ; 48 81 f1 00 01 00 00                 
   495 : xor    rdi, #0x100             ; 48 81 f7 00 01 00 00                 
   496 : xor    r8, #0x100              ; 49 81 f0 00 01 00 00                 
   497 : xor    [rsp+#0x100], rax       ; 48 31 84 24 00 01 00 00              
   498 : xor    [rsp+#0x100], rdi       ; 48 31 bc 24 00 01 00 00              
   499 : xor    [rsp+#0x100], r8        ; 4c 31 84 24 00 01 00 00              
   500 : xor    [rsp+#0x100], #0x101    ; 48 81 b4 24 00 01 00 00 01 01 00 00  
   501 : cmove  rax, rax                ; 48 0f 44 c0                          
   502 : cmovne rax, rax                ; 48 0f 45 c0                          
   503 : cmovl  rax, rax                ; 48 0f 4c c0                          
   504 : cmovg  rax, rax                ; 48 0f 4f c0                          
   505 : cmovle rax, rax                ; 48 0f 4e c0                          
   506 : cmovge rax, rax                ; 48 0f 4d c0                          
   507 : cmovs  rax, rax                ; 48 0f 48 c0                          
   508 : cmovns rax, rax                ; 48 0f 49 c0                          
   509 : cmove  rdi, rax                ; 48 0f 44 f8                          
   510 : cmove  rax, rdi                ; 48 0f 44 c7                          
   511 : cmove  r8, rax                 ; 4c 0f 44 c0                          
   512 : cmove  rax, r8                 ; 49 0f 44 c0                          
   513 : cmove  r8, r8                  ; 4d 0f 44 c0                          
   514 : cmove  rax, [rsp+#0x100]       ; 48 0f 44 84 24 00 01 00 00           
   515 : cmove  rdi, [rsp+#0x100]       ; 48 0f 44 bc 24 00 01 00 00           
   516 : cmove  r8, [rsp+#0x100]        ; 4c 0f 44 84 24 00 01 00 00           
   517 : sete   rax                     ; 0f 94 c0                             
   518 : setne  rax                     ; 0f 95 c0                             
   519 : setl   rax                     ; 0f 9c c0                             
   520 : setg   rax                     ; 0f 9f c0                             
   521 : setle  rax                     ; 0f 9e c0                             
   522 : setge  rax                     ; 0f 9d c0                             
   523 : sets   rax                     ; 0f 98 c0                             
   524 : setns  rax                     ; 0f 99 c0                             
   525 : sete   rdi                     ; 40 0f 94 c7                          
   526 : sete   r8                      ; 41 0f 94 c0                          
   527 : sete   [rsp+#0x100]            ; 0f 94 84 24 00 01 00 00              
   528 : adc    rax, rax                ; 48 11 c0                             
   529 : adc    rdi, rax                ; 48 11 c7                             
   530 : adc    rax, rdi                ; 48 11 f8                             
   531 : adc    r8, rax                 ; 49 11 c0                             
   532 : adc    rax, r8                 ; 4c 11 c0                             
   533 : adc    r8, r8                  ; 4d 11 c0                             
   534 : adc    rax, #0x100             ; 48 15 00 01 00 00                    
   535 : adc    rcx, #0x100             ; 48 81 d1 00 01 00 00                 
   536 : adc    rdi, #0x100             ; 48 81 d7 00 01 00 00                 
   537 : adc    r8, #0x100              ; 49 81 d0 00 01 00 00                 
   538 : adc    rax, [rsp+#0x100]       ; 48 13 84 24 00 01 00 00              
   539 : adc    rdi, [rsp+#0x100]       ; 48 13 bc 24 00 01 00 00              
   540 : adc    r8, [rsp+#0x100]        ; 4c 13 84 24 00 01 00 00              
   541 : adc    [rsp+#0x100], rax       ; 48 11 84 24 00 01 00 00              
   542 : adc    [rsp+#0x100], rdi       ; 48 11 bc 24 00 01 00 00              
   543 : adc    [rsp+#0x100], r8        ; 4c 11 84 24 00 01 00 00              
   544 : adc    [rsp+#0x100], #0x100    ; 48 81 94 24 00 01 00 00 00 01 00 00  
   545 : cmp    rax, [rsp+#0xfff8]      ; 48 3b 84 24 f8 ff 00 00              
   546 : cmp    rdi, [rsp+#0xfff8]      ; 48 3b bc 24 f8 ff 00 00              
   547 : cmp    r8, [rsp+#0xfff8]       ; 4c 3b 84 24 f8 ff 00 00              
   548 : cmp    r15, [rsp+#0xfff8]      ; 4c 3b bc 24 f8 ff 00 00              
   549 : cmp    [rsp+#0xfff8], rax      ; 48 39 84 24 f8 ff 00 00              
   550 : cmp    [rsp+#0xfff8], rdi      ; 48 39 bc 24 f8 ff 00 00              
   551 : cmp    [rsp+#0xfff8], r8       ; 4c 39 84 24 f8 ff 00 00              
   552 : cmp    [rsp+#0xfff8], r15      ; 4c 39 bc 24 f8 ff 00 00              
   553 : cmp    [rsp+#0xfff8], #0x8888  ; 48 81 bc 24 f8 ff 00 00 88 88 00 00  
   554 : call   [rax]                   ; ff d0                                
   555 : call   [rdi]                   ; ff d7                                
   556 : call   [r8]                    ; 41 ff d0                             
   557 : call   [r15]                   ; 41 ff d7                             
   558 : call   [[rsp+#0x100]]          ; ff 94 24 00 01 00 00                 
