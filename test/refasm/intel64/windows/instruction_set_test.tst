instruction_set_test()
     0 : mov    [rax], #0x100           ; 48 c7 00 00 01 00 00                 
     1 : mov    [rax], #0x100           ; 48 c7 00 00 01 00 00                 
     2 : mov    [rdi], #0x100           ; 48 c7 07 00 01 00 00                 
     3 : mov    [r8], #0x100            ; 49 c7 00 00 01 00 00                 
     4 : mov    [r12], #0x100           ; 49 c7 04 24 00 01 00 00              
     5 : mov    [r13], #0x100           ; 49 c7 45 00 00 01 00 00              
     6 : mov    [rax], #0x100           ; c7 00 00 01 00 00                    
     7 : mov    [rax], #0x100           ; c7 00 00 01 00 00                    
     8 : mov    [rdi], #0x100           ; c7 07 00 01 00 00                    
     9 : mov    [r8], #0x100            ; 41 c7 00 00 01 00 00                 
    10 : mov    [r12], #0x100           ; 41 c7 04 24 00 01 00 00              
    11 : mov    [r13], #0x100           ; 41 c7 45 00 00 01 00 00              
    12 : mov    [rax], #0x100           ; 66 c7 00 00 01                       
    13 : mov    [rax], #0x100           ; 66 c7 00 00 01                       
    14 : mov    [rdi], #0x100           ; 66 c7 07 00 01                       
    15 : mov    [r8], #0x100            ; 66 41 c7 00 00 01                    
    16 : mov    [r12], #0x100           ; 66 41 c7 04 24 00 01                 
    17 : mov    [r13], #0x100           ; 66 41 c7 45 00 00 01                 
    18 : mov    [rax], #0xff            ; c6 00 ff                             
    19 : mov    [rax], #0xff            ; c6 00 ff                             
    20 : mov    [rdi], #0xff            ; c6 07 ff                             
    21 : mov    [r8], #0xff             ; 41 c6 00 ff                          
    22 : mov    [r12], #0xff            ; 41 c6 04 24 ff                       
    23 : mov    [r13], #0xff            ; 41 c6 45 00 ff                       
    24 : mov    [rax], rax              ; 48 89 00                             
    25 : mov    [rax], rax              ; 48 89 00                             
    26 : mov    [rax], rdi              ; 48 89 38                             
    27 : mov    [rax], r8               ; 4c 89 00                             
    28 : mov    [rdi], rax              ; 48 89 07                             
    29 : mov    [r8], rax               ; 49 89 00                             
    30 : mov    [r8], r8                ; 4d 89 00                             
    31 : mov    [r12], rax              ; 49 89 04 24                          
    32 : mov    [r12], rdi              ; 49 89 3c 24                          
    33 : mov    [r12], r8               ; 4d 89 04 24                          
    34 : mov    [r13], rax              ; 49 89 45 00                          
    35 : mov    [r13], rdi              ; 49 89 7d 00                          
    36 : mov    [r13], r8               ; 4d 89 45 00                          
    37 : mov    [rax], ax               ; 66 89 00                             
    38 : mov    [rax], ax               ; 66 89 00                             
    39 : mov    [rax], di               ; 66 89 38                             
    40 : mov    [rax], r8w              ; 66 44 89 00                          
    41 : mov    [rdi], ax               ; 66 89 07                             
    42 : mov    [r8], ax                ; 66 41 89 00                          
    43 : mov    [r8], r8w               ; 66 45 89 00                          
    44 : mov    [r12], ax               ; 66 41 89 04 24                       
    45 : mov    [r12], di               ; 66 41 89 3c 24                       
    46 : mov    [r12], r8w              ; 66 45 89 04 24                       
    47 : mov    [r13], ax               ; 66 41 89 45 00                       
    48 : mov    [r13], di               ; 66 41 89 7d 00                       
    49 : mov    [r13], r8w              ; 66 45 89 45 00                       
    50 : mov    [rax], al               ; 88 00                                
    51 : mov    [rax], al               ; 88 00                                
    52 : mov    [rax], dil              ; 40 88 38                             
    53 : mov    [rax], r8b              ; 44 88 00                             
    54 : mov    [rdi], al               ; 88 07                                
    55 : mov    [r8], al                ; 41 88 00                             
    56 : mov    [r8], r8b               ; 45 88 00                             
    57 : mov    [r12], al               ; 41 88 04 24                          
    58 : mov    [r12], dil              ; 41 88 3c 24                          
    59 : mov    [r12], r8b              ; 45 88 04 24                          
    60 : mov    [r13], al               ; 41 88 45 00                          
    61 : mov    [r13], dil              ; 41 88 7d 00                          
    62 : mov    [r13], r8b              ; 45 88 45 00                          
    63 : mov    [rax], [rax], rax       ; 48 89 04 00                          
    64 : mov    [rax], [rax], rax       ; 48 89 04 00                          
    65 : mov    [rax], [rax], rdi       ; 48 89 3c 00                          
    66 : mov    [rax], [rax], r8        ; 4c 89 04 00                          
    67 : mov    [rax], [rdi], rax       ; 48 89 04 38                          
    68 : mov    [rax], [r8], rax        ; 4a 89 04 00                          
    69 : mov    [rax], [r8], r8         ; 4e 89 04 00                          
    70 : mov    [rdi], [rax], rax       ; 48 89 04 07                          
    71 : mov    [r8], [rax], rax        ; 49 89 04 00                          
    72 : mov    [r8], [rax], r8         ; 4d 89 04 00                          
    73 : mov    [r8], [r8], rax         ; 4b 89 04 00                          
    74 : mov    [r8], [r8], r8          ; 4f 89 04 00                          
    75 : mov    [rax], [rax], eax       ; 89 04 00                             
    76 : mov    [rax], [rax], eax       ; 89 04 00                             
    77 : mov    [rax], [rax], edi       ; 89 3c 00                             
    78 : mov    [rax], [rax], r8d       ; 44 89 04 00                          
    79 : mov    [rax], [rdi], eax       ; 89 04 38                             
    80 : mov    [rax], [r8], eax        ; 42 89 04 00                          
    81 : mov    [rax], [r8], r8d        ; 46 89 04 00                          
    82 : mov    [rdi], [rax], eax       ; 89 04 07                             
    83 : mov    [r8], [rax], eax        ; 41 89 04 00                          
    84 : mov    [r8], [rax], r8d        ; 45 89 04 00                          
    85 : mov    [r8], [r8], eax         ; 43 89 04 00                          
    86 : mov    [r8], [r8], r8d         ; 47 89 04 00                          
    87 : mov    [r13], [rax], eax       ; 41 89 44 05 00                       
    88 : mov    [r13], [rax], r8d       ; 45 89 44 05 00                       
    89 : mov    [r13], [r8], eax        ; 43 89 44 05 00                       
    90 : mov    [r13], [r8], r8d        ; 47 89 44 05 00                       
    91 : mov    [rax], [rax], ax        ; 66 89 04 00                          
    92 : mov    [rax], [rax], ax        ; 66 89 04 00                          
    93 : mov    [rax], [rax], di        ; 66 89 3c 00                          
    94 : mov    [rax], [rax], r8w       ; 66 44 89 04 00                       
    95 : mov    [rax], [rdi], ax        ; 66 89 04 38                          
    96 : mov    [rax], [r8], ax         ; 66 42 89 04 00                       
    97 : mov    [rax], [r8], r8w        ; 66 46 89 04 00                       
    98 : mov    [rdi], [rax], ax        ; 66 89 04 07                          
    99 : mov    [r8], [rax], ax         ; 66 41 89 04 00                       
   100 : mov    [r8], [rax], r8w        ; 66 45 89 04 00                       
   101 : mov    [r8], [r8], ax          ; 66 43 89 04 00                       
   102 : mov    [r8], [r8], r8w         ; 66 47 89 04 00                       
   103 : mov    [r13], [rax], ax        ; 66 41 89 44 05 00                    
   104 : mov    [r13], [r8], ax         ; 66 43 89 44 05 00                    
   105 : mov    [r13], [rax], r8w       ; 66 45 89 44 05 00                    
   106 : mov    [r13], [r8], r8w        ; 66 47 89 44 05 00                    
   107 : mov    [rax], [rax], al        ; 88 04 00                             
   108 : mov    [rax], [rax], al        ; 88 04 00                             
   109 : mov    [rax], [rax], dil       ; 40 88 3c 00                          
   110 : mov    [rax], [rax], r8b       ; 44 88 04 00                          
   111 : mov    [rax], [rdi], al        ; 88 04 38                             
   112 : mov    [rax], [r8], al         ; 42 88 04 00                          
   113 : mov    [rax], [r8], dil        ; 42 88 3c 00                          
   114 : mov    [rax], [r8], r8b        ; 46 88 04 00                          
   115 : mov    [rdi], [rax], al        ; 88 04 07                             
   116 : mov    [r8], [rax], al         ; 41 88 04 00                          
   117 : mov    [r8], [rax], dil        ; 41 88 3c 00                          
   118 : mov    [r8], [rax], r8b        ; 45 88 04 00                          
   119 : mov    [r8], [r8], al          ; 43 88 04 00                          
   120 : mov    [r8], [r8], r8b         ; 47 88 04 00                          
   121 : mov    [r8], [r8], dil         ; 43 88 3c 00                          
   122 : mov    [r13], [rax], al        ; 41 88 44 05 00                       
   123 : mov    [r13], [rax], r8b       ; 45 88 44 05 00                       
   124 : mov    [r13], [r8], al         ; 43 88 44 05 00                       
   125 : mov    [r13], [r8], r8b        ; 47 88 44 05 00                       
   126 : mov    [rax], [#0x100], rax    ; 48 89 80 00 01 00 00                 
   127 : mov    [rax], [#0x100], rax    ; 48 89 80 00 01 00 00                 
   128 : mov    [rax], [#0x100], rdi    ; 48 89 b8 00 01 00 00                 
   129 : mov    [rax], [#0x100], r8     ; 4c 89 80 00 01 00 00                 
   130 : mov    [rdi], [#0x100], rax    ; 48 89 87 00 01 00 00                 
   131 : mov    [r8], [#0x100], rax     ; 49 89 80 00 01 00 00                 
   132 : mov    [r8], [#0x100], r8      ; 4d 89 80 00 01 00 00                 
   133 : mov    [r12], [#0x100], rax    ; 49 89 84 24 00 01 00 00              
   134 : mov    [r12], [#0x100], rdi    ; 49 89 bc 24 00 01 00 00              
   135 : mov    [r12], [#0x100], r8     ; 4d 89 84 24 00 01 00 00              
   136 : mov    [rax], [#0x100], eax    ; 89 80 00 01 00 00                    
   137 : mov    [rax], [#0x100], eax    ; 89 80 00 01 00 00                    
   138 : mov    [rax], [#0x100], edi    ; 89 b8 00 01 00 00                    
   139 : mov    [rax], [#0x100], r8d    ; 44 89 80 00 01 00 00                 
   140 : mov    [rdi], [#0x100], eax    ; 89 87 00 01 00 00                    
   141 : mov    [r8], [#0x100], eax     ; 41 89 80 00 01 00 00                 
   142 : mov    [r8], [#0x100], r8d     ; 45 89 80 00 01 00 00                 
   143 : mov    [r12], [#0x100], eax    ; 41 89 84 24 00 01 00 00              
   144 : mov    [r12], [#0x100], r8d    ; 45 89 84 24 00 01 00 00              
   145 : mov    [rax], [#0x100], ax     ; 66 89 80 00 01 00 00                 
   146 : mov    [rax], [#0x100], ax     ; 66 89 80 00 01 00 00                 
   147 : mov    [rax], [#0x100], di     ; 66 89 b8 00 01 00 00                 
   148 : mov    [rax], [#0x100], r8w    ; 66 44 89 80 00 01 00 00              
   149 : mov    [rdi], [#0x100], ax     ; 66 89 87 00 01 00 00                 
   150 : mov    [r8], [#0x100], ax      ; 66 41 89 80 00 01 00 00              
   151 : mov    [r8], [#0x100], r8w     ; 66 45 89 80 00 01 00 00              
   152 : mov    [r12], [#0x100], ax     ; 66 41 89 84 24 00 01 00 00           
   153 : mov    [r12], [#0x100], r8w    ; 66 45 89 84 24 00 01 00 00           
   154 : mov    [rax], [#0x100], al     ; 88 80 00 01 00 00                    
   155 : mov    [rax], [#0x100], al     ; 88 80 00 01 00 00                    
   156 : mov    [rax], [#0x100], dil    ; 40 88 b8 00 01 00 00                 
   157 : mov    [rax], [#0x100], r8b    ; 44 88 80 00 01 00 00                 
   158 : mov    [rdi], [#0x100], al     ; 88 87 00 01 00 00                    
   159 : mov    [r8], [#0x100], al      ; 41 88 80 00 01 00 00                 
   160 : mov    [r8], [#0x100], r8b     ; 45 88 80 00 01 00 00                 
   161 : mov    [r12], [#0x100], al     ; 41 88 84 24 00 01 00 00              
   162 : mov    [r12], [#0x100], r8b    ; 45 88 84 24 00 01 00 00              
   163 : mov    [rax], [rax], #0x100    ; 48 c7 04 00 00 01 00 00              
   164 : mov    [rax], [rax], #0x100    ; 48 c7 04 00 00 01 00 00              
   165 : mov    [rax], [rdi], #0x100    ; 48 c7 04 38 00 01 00 00              
   166 : mov    [rax], [r8], #0x100     ; 4a c7 04 00 00 01 00 00              
   167 : mov    [rdi], [rax], #0x100    ; 48 c7 04 07 00 01 00 00              
   168 : mov    [r8], [rax], #0x100     ; 49 c7 04 00 00 01 00 00              
   169 : mov    [r8], [r8], #0x100      ; 4b c7 04 00 00 01 00 00              
   170 : mov    [r13], [rax], #0x100    ; 49 c7 44 05 00 00 01 00 00           
   171 : mov    [r13], [r8], #0x100     ; 4b c7 44 05 00 00 01 00 00           
   172 : mov    [rax], [rax], #0x100    ; c7 04 00 00 01 00 00                 
   173 : mov    [rax], [rax], #0x100    ; c7 04 00 00 01 00 00                 
   174 : mov    [rax], [rdi], #0x100    ; c7 04 38 00 01 00 00                 
   175 : mov    [rax], [r8], #0x100     ; 42 c7 04 00 00 01 00 00              
   176 : mov    [rdi], [rax], #0x100    ; c7 04 07 00 01 00 00                 
   177 : mov    [r8], [rax], #0x100     ; 41 c7 04 00 00 01 00 00              
   178 : mov    [r8], [r8], #0x100      ; 43 c7 04 00 00 01 00 00              
   179 : mov    [r13], [rax], #0x100    ; 41 c7 44 05 00 00 01 00 00           
   180 : mov    [r13], [r8], #0x100     ; 43 c7 44 05 00 00 01 00 00           
   181 : mov    [rax], [rax], #0x100    ; 66 c7 04 00 00 01                    
   182 : mov    [rax], [rax], #0x100    ; 66 c7 04 00 00 01                    
   183 : mov    [rax], [rdi], #0x100    ; 66 c7 04 38 00 01                    
   184 : mov    [rax], [r8], #0x100     ; 66 42 c7 04 00 00 01                 
   185 : mov    [rdi], [rax], #0x100    ; 66 c7 04 07 00 01                    
   186 : mov    [r8], [rax], #0x100     ; 66 41 c7 04 00 00 01                 
   187 : mov    [r8], [r8], #0x100      ; 66 43 c7 04 00 00 01                 
   188 : mov    [r13], [rax], #0x100    ; 66 41 c7 44 05 00 00 01              
   189 : mov    [r13], [r8], #0x100     ; 66 43 c7 44 05 00 00 01              
   190 : mov    [rax], [rax], #0xff     ; c6 04 00 ff                          
   191 : mov    [rax], [rax], #0xff     ; c6 04 00 ff                          
   192 : mov    [rax], [rdi], #0xff     ; c6 04 38 ff                          
   193 : mov    [rax], [r8], #0xff      ; 42 c6 04 00 ff                       
   194 : mov    [rdi], [rax], #0xff     ; c6 04 07 ff                          
   195 : mov    [r8], [rax], #0xff      ; 41 c6 04 00 ff                       
   196 : mov    [r8], [r8], #0xff       ; 43 c6 04 00 ff                       
   197 : mov    [r13], [rax], #0xff     ; 41 c6 44 05 00 ff                    
   198 : mov    [r13], [r8], #0xff      ; 43 c6 44 05 00 ff                    
   199 : mov    [rax], [#0x101], #0x100 ; 48 c7 80 01 01 00 00 00 01 00 00     
   200 : mov    [rax], [#0x101], #0x100 ; 48 c7 80 01 01 00 00 00 01 00 00     
   201 : mov    [rdi], [#0x101], #0x100 ; 48 c7 87 01 01 00 00 00 01 00 00     
   202 : mov    [r8], [#0x101], #0x100  ; 49 c7 80 01 01 00 00 00 01 00 00     
   203 : mov    [r12], [#0x101], #0x100 ; 49 c7 84 24 01 01 00 00 00 01 00 00  
   204 : mov    [rax], [#0x101], #0x100 ; c7 80 01 01 00 00 00 01 00 00        
   205 : mov    [rax], [#0x101], #0x100 ; c7 80 01 01 00 00 00 01 00 00        
   206 : mov    [rdi], [#0x101], #0x100 ; c7 87 01 01 00 00 00 01 00 00        
   207 : mov    [r8], [#0x101], #0x100  ; 41 c7 80 01 01 00 00 00 01 00 00     
   208 : mov    [r12], [#0x101], #0x100 ; 41 c7 84 24 01 01 00 00 00 01 00 00  
   209 : mov    [rax], [#0x101], #0x100 ; 66 c7 80 01 01 00 00 00 01           
   210 : mov    [rax], [#0x101], #0x100 ; 66 c7 80 01 01 00 00 00 01           
   211 : mov    [rdi], [#0x101], #0x100 ; 66 c7 87 01 01 00 00 00 01           
   212 : mov    [r8], [#0x101], #0x100  ; 66 41 c7 80 01 01 00 00 00 01        
   213 : mov    [r12], [#0x101], #0x100 ; 66 41 c7 84 24 01 01 00 00 00 01     
   214 : mov    [rax], [#0x101], #0xff  ; c6 80 01 01 00 00 ff                 
   215 : mov    [rax], [#0x101], #0xff  ; c6 80 01 01 00 00 ff                 
   216 : mov    [rdi], [#0x101], #0xff  ; c6 87 01 01 00 00 ff                 
   217 : mov    [r8], [#0x101], #0xff   ; 41 c6 80 01 01 00 00 ff              
   218 : mov    [r12], [#0x101], #0xff  ; 41 c6 84 24 01 01 00 00 ff           
   219 : mov    rax, [rax]              ; 48 8b 00                             
   220 : mov    rax, [rax]              ; 48 8b 00                             
   221 : mov    rdi, [rax]              ; 48 8b 38                             
   222 : mov    rax, [rdi]              ; 48 8b 07                             
   223 : mov    rax, [rax]              ; 48 8b 00                             
   224 : mov    r8, [rax]               ; 4c 8b 00                             
   225 : mov    rax, [r8]               ; 49 8b 00                             
   226 : mov    r8, [r8]                ; 4d 8b 00                             
   227 : mov    eax, [rax]              ; 8b 00                                
   228 : mov    edi, [rax]              ; 8b 38                                
   229 : mov    eax, [rdi]              ; 8b 07                                
   230 : mov    eax, [rax]              ; 8b 00                                
   231 : mov    r8d, [rax]              ; 44 8b 00                             
   232 : mov    eax, [r8]               ; 41 8b 00                             
   233 : mov    r8d, [r8]               ; 45 8b 00                             
   234 : movzx  rax, ax                 ; 48 0f b7 00                          
   235 : movzx  rdi, ax                 ; 48 0f b7 38                          
   236 : movzx  rax, di                 ; 48 0f b7 07                          
   237 : movzx  r8, ax                  ; 4c 0f b7 00                          
   238 : movzx  rax, r8w                ; 49 0f b7 00                          
   239 : movzx  r8, r8w                 ; 4d 0f b7 00                          
   240 : movsx  rax, ax                 ; 48 0f bf 00                          
   241 : movsx  rdi, ax                 ; 48 0f bf 38                          
   242 : movsx  rax, di                 ; 48 0f bf 07                          
   243 : movsx  r8, ax                  ; 4c 0f bf 00                          
   244 : movsx  rax, r8w                ; 49 0f bf 00                          
   245 : movsx  r8, r8w                 ; 4d 0f bf 00                          
   246 : movzx  rax, al                 ; 48 0f b6 00                          
   247 : movzx  rdi, al                 ; 48 0f b6 38                          
   248 : movzx  rax, dil                ; 48 0f b6 07                          
   249 : movzx  r8, al                  ; 4c 0f b6 00                          
   250 : movzx  rax, r8b                ; 49 0f b6 00                          
   251 : movzx  r8, r8b                 ; 4d 0f b6 00                          
   252 : movsx  rax, al                 ; 48 0f be 00                          
   253 : movsx  rdi, al                 ; 48 0f be 38                          
   254 : movsx  rax, dil                ; 48 0f be 07                          
   255 : movsx  r8, al                  ; 4c 0f be 00                          
   256 : movsx  rax, r8b                ; 49 0f be 00                          
   257 : movsx  r8, r8b                 ; 4d 0f be 00                          
   258 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   259 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   260 : mov    rdi, [rax], [#0x100]    ; 48 8b b8 00 01 00 00                 
   261 : mov    rax, [rdi], [#0x100]    ; 48 8b 87 00 01 00 00                 
   262 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   263 : mov    r8, [rax], [#0x100]     ; 4c 8b 80 00 01 00 00                 
   264 : mov    rax, [r8], [#0x100]     ; 49 8b 80 00 01 00 00                 
   265 : mov    r8, [r8], [#0x100]      ; 4d 8b 80 00 01 00 00                 
   266 : mov    eax, [rax], [#0x100]    ; 8b 80 00 01 00 00                    
   267 : mov    edi, [rax], [#0x100]    ; 8b b8 00 01 00 00                    
   268 : mov    eax, [rdi], [#0x100]    ; 8b 87 00 01 00 00                    
   269 : mov    r8d, [rax], [#0x100]    ; 44 8b 80 00 01 00 00                 
   270 : mov    eax, [r8], [#0x100]     ; 41 8b 80 00 01 00 00                 
   271 : mov    r8d, [r8], [#0x100]     ; 45 8b 80 00 01 00 00                 
   272 : movsxd rax, rax, #0x100        ; 48 63 80 00 01 00 00                 
   273 : movsxd rdi, rax, #0x100        ; 48 63 b8 00 01 00 00                 
   274 : movsxd rax, rdi, #0x100        ; 48 63 87 00 01 00 00                 
   275 : movsxd r8, rax, #0x100         ; 4c 63 80 00 01 00 00                 
   276 : movsxd rax, r8, #0x100         ; 49 63 80 00 01 00 00                 
   277 : movsxd r8, r8, #0x100          ; 4d 63 80 00 01 00 00                 
   278 : movzx  rax, ax, #0x100         ; 48 0f b7 80 00 01 00 00              
   279 : movzx  rdi, ax, #0x100         ; 48 0f b7 b8 00 01 00 00              
   280 : movzx  rax, di, #0x100         ; 48 0f b7 87 00 01 00 00              
   281 : movzx  r8, ax, #0x100          ; 4c 0f b7 80 00 01 00 00              
   282 : movzx  rax, r8w, #0x100        ; 49 0f b7 80 00 01 00 00              
   283 : movzx  r8, r8w, #0x100         ; 4d 0f b7 80 00 01 00 00              
   284 : movsx  rax, ax, #0x100         ; 48 0f bf 80 00 01 00 00              
   285 : movsx  rdi, ax, #0x100         ; 48 0f bf b8 00 01 00 00              
   286 : movsx  rax, di, #0x100         ; 48 0f bf 87 00 01 00 00              
   287 : movsx  r8, ax, #0x100          ; 4c 0f bf 80 00 01 00 00              
   288 : movsx  rax, r8w, #0x100        ; 49 0f bf 80 00 01 00 00              
   289 : movsx  r8, r8w, #0x100         ; 4d 0f bf 80 00 01 00 00              
   290 : movzx  rax, al, #0x100         ; 48 0f b6 80 00 01 00 00              
   291 : movzx  rdi, al, #0x100         ; 48 0f b6 b8 00 01 00 00              
   292 : movzx  rax, dil, #0x100        ; 48 0f b6 87 00 01 00 00              
   293 : movzx  r8, al, #0x100          ; 4c 0f b6 80 00 01 00 00              
   294 : movzx  rax, r8b, #0x100        ; 49 0f b6 80 00 01 00 00              
   295 : movzx  r8, r8b, #0x100         ; 4d 0f b6 80 00 01 00 00              
   296 : movsx  rax, al, #0x100         ; 48 0f be 80 00 01 00 00              
   297 : movsx  rdi, al, #0x100         ; 48 0f be b8 00 01 00 00              
   298 : movsx  rax, dil, #0x100        ; 48 0f be 87 00 01 00 00              
   299 : movsx  r8, al, #0x100          ; 4c 0f be 80 00 01 00 00              
   300 : movsx  rax, r8b, #0x100        ; 49 0f be 80 00 01 00 00              
   301 : movsx  r8, r8b, #0x100         ; 4d 0f be 80 00 01 00 00              
   302 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   303 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   304 : mov    rdi, [rax], [rax]       ; 48 8b 3c 00                          
   305 : mov    rax, [rdi], [rax]       ; 48 8b 04 07                          
   306 : mov    rax, [rax], [rdi]       ; 48 8b 04 38                          
   307 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   308 : mov    r8, [rax], [rax]        ; 4c 8b 04 00                          
   309 : mov    rax, [r8], [rax]        ; 49 8b 04 00                          
   310 : mov    r8, [r8], [rax]         ; 4d 8b 04 00                          
   311 : mov    rax, [rax], [r8]        ; 4a 8b 04 00                          
   312 : mov    r8, [rax], [r8]         ; 4e 8b 04 00                          
   313 : mov    rax, [r8], [r8]         ; 4b 8b 04 00                          
   314 : mov    r8, [r8], [r8]          ; 4f 8b 04 00                          
   315 : mov    eax, [rax], [rax]       ; 8b 04 00                             
   316 : mov    edi, [rax], [rax]       ; 8b 3c 00                             
   317 : mov    eax, [rdi], [rax]       ; 8b 04 07                             
   318 : mov    eax, [rax], [rdi]       ; 8b 04 38                             
   319 : mov    r8d, [rax], [rax]       ; 44 8b 04 00                          
   320 : mov    eax, [r8], [rax]        ; 41 8b 04 00                          
   321 : mov    r8d, [r8], [rax]        ; 45 8b 04 00                          
   322 : mov    eax, [rax], [r8]        ; 42 8b 04 00                          
   323 : mov    r8d, [rax], [r8]        ; 46 8b 04 00                          
   324 : mov    eax, [r8], [r8]         ; 43 8b 04 00                          
   325 : mov    r8d, [r8], [r8]         ; 47 8b 04 00                          
   326 : movzx  rax, ax, rax            ; 48 0f b7 04 00                       
   327 : movzx  rdi, ax, rax            ; 48 0f b7 3c 00                       
   328 : movzx  rax, di, rax            ; 48 0f b7 04 07                       
   329 : movzx  rax, ax, rdi            ; 48 0f b7 04 38                       
   330 : movzx  r8, ax, rax             ; 4c 0f b7 04 00                       
   331 : movzx  rax, r8w, rax           ; 49 0f b7 04 00                       
   332 : movzx  r8, r8w, rax            ; 4d 0f b7 04 00                       
   333 : movzx  rax, ax, r8             ; 4a 0f b7 04 00                       
   334 : movzx  r8, ax, r8              ; 4e 0f b7 04 00                       
   335 : movzx  rax, r8w, r8            ; 4b 0f b7 04 00                       
   336 : movzx  r8, r8w, r8             ; 4f 0f b7 04 00                       
   337 : movsx  rax, ax, rax            ; 48 0f bf 04 00                       
   338 : movsx  rdi, ax, rax            ; 48 0f bf 3c 00                       
   339 : movsx  rax, di, rax            ; 48 0f bf 04 07                       
   340 : movsx  rax, ax, rdi            ; 48 0f bf 04 38                       
   341 : movsx  r8, ax, rax             ; 4c 0f bf 04 00                       
   342 : movsx  rax, r8w, rax           ; 49 0f bf 04 00                       
   343 : movsx  r8, r8w, rax            ; 4d 0f bf 04 00                       
   344 : movsx  rax, ax, r8             ; 4a 0f bf 04 00                       
   345 : movsx  r8, ax, r8              ; 4e 0f bf 04 00                       
   346 : movsx  rax, r8w, r8            ; 4b 0f bf 04 00                       
   347 : movsx  r8, r8w, r8             ; 4f 0f bf 04 00                       
   348 : movzx  rax, al, rax            ; 48 0f b6 04 00                       
   349 : movzx  rdi, al, rax            ; 48 0f b6 3c 00                       
   350 : movzx  rax, dil, rax           ; 48 0f b6 04 07                       
   351 : movzx  rax, al, rdi            ; 48 0f b6 04 38                       
   352 : movzx  r8, al, rax             ; 4c 0f b6 04 00                       
   353 : movzx  rax, r8b, rax           ; 49 0f b6 04 00                       
   354 : movzx  r8, r8b, rax            ; 4d 0f b6 04 00                       
   355 : movzx  rax, al, r8             ; 4a 0f b6 04 00                       
   356 : movzx  r8, al, r8              ; 4e 0f b6 04 00                       
   357 : movzx  rax, r8b, r8            ; 4b 0f b6 04 00                       
   358 : movzx  r8, r8b, r8             ; 4f 0f b6 04 00                       
   359 : movsx  rax, al, rax            ; 48 0f be 04 00                       
   360 : movsx  rdi, al, rax            ; 48 0f be 3c 00                       
   361 : movsx  rax, dil, rax           ; 48 0f be 04 07                       
   362 : movsx  rax, al, rdi            ; 48 0f be 04 38                       
   363 : movsx  r8, al, rax             ; 4c 0f be 04 00                       
   364 : movsx  rax, r8b, rax           ; 49 0f be 04 00                       
   365 : movsx  r8, r8b, rax            ; 4d 0f be 04 00                       
   366 : movsx  rax, al, r8             ; 4a 0f be 04 00                       
   367 : movsx  r8, al, r8              ; 4e 0f be 04 00                       
   368 : movsx  rax, r8b, r8            ; 4b 0f be 04 00                       
   369 : movsx  r8, r8b, r8             ; 4f 0f be 04 00                       
   370 : xchg   rax, rax                ; 48 90                                
   371 : xchg   rax, rdi                ; 48 97                                
   372 : xchg   rax, r8                 ; 49 90                                
   373 : xchg   rcx, rcx                ; 48 87 c9                             
   374 : xchg   rdi, rcx                ; 48 87 cf                             
   375 : xchg   rcx, rdi                ; 48 87 f9                             
   376 : xchg   rcx, rcx                ; 48 87 c9                             
   377 : xchg   r8, rcx                 ; 49 87 c8                             
   378 : xchg   rcx, r8                 ; 4c 87 c1                             
   379 : xchg   r8, r8                  ; 4d 87 c0                             
   380 : xchg   rcx, [rsp+#0x100]       ; 48 87 8c 24 00 01 00 00              
   381 : xchg   rdi, [rsp+#0x100]       ; 48 87 bc 24 00 01 00 00              
   382 : xchg   r8, [rsp+#0x100]        ; 4c 87 84 24 00 01 00 00              
   383 : xchg   r15, [rsp+#0x100]       ; 4c 87 bc 24 00 01 00 00              
   384 : xchg   [rsp+#0x100], rcx       ; 48 87 8c 24 00 01 00 00              
   385 : xchg   [rsp+#0x100], rdi       ; 48 87 bc 24 00 01 00 00              
   386 : xchg   [rsp+#0x100], r8        ; 4c 87 84 24 00 01 00 00              
   387 : xchg   [rsp+#0x100], r15       ; 4c 87 bc 24 00 01 00 00              
   388 : shl    rax, #0x0f              ; 48 c1 e0 0f                          
   389 : shl    rdi, #0x0f              ; 48 c1 e7 0f                          
   390 : shl    r8, #0x0f               ; 49 c1 e0 0f                          
   391 : shl    [rsp+#0x100], #0x0f     ; 48 c1 a4 24 00 01 00 00 0f           
   392 : shl    rax, cl                 ; 48 d3 e0                             
   393 : shl    rdi, cl                 ; 48 d3 e7                             
   394 : shl    r8, cl                  ; 49 d3 e0                             
   395 : shl    [rsp+#0x100], cl        ; 48 d3 a4 24 00 01 00 00              
   396 : shr    rax, #0x0f              ; 48 c1 e8 0f                          
   397 : shr    rdi, #0x0f              ; 48 c1 ef 0f                          
   398 : shr    r8, #0x0f               ; 49 c1 e8 0f                          
   399 : shr    [rsp+#0x100], #0x0f     ; 48 c1 ac 24 00 01 00 00 0f           
   400 : shr    rax, cl                 ; 48 d3 e8                             
   401 : shr    rdi, cl                 ; 48 d3 ef                             
   402 : shr    r8, cl                  ; 49 d3 e8                             
   403 : shr    [rsp+#0x100], cl        ; 48 d3 ac 24 00 01 00 00              
   404 : sar    rax, #0x0f              ; 48 c1 f8 0f                          
   405 : sar    rdi, #0x0f              ; 48 c1 ff 0f                          
   406 : sar    r8, #0x0f               ; 49 c1 f8 0f                          
   407 : sar    [rsp+#0x100], #0x0f     ; 48 c1 bc 24 00 01 00 00 0f           
   408 : sar    rax, cl                 ; 48 d3 f8                             
   409 : sar    rdi, cl                 ; 48 d3 ff                             
   410 : sar    r8, cl                  ; 49 d3 f8                             
   411 : sar    [rsp+#0x100], cl        ; 48 d3 bc 24 00 01 00 00              
   412 : and    rax, rax                ; 48 21 c0                             
   413 : and    rax, rdi                ; 48 21 f8                             
   414 : and    rdi, rax                ; 48 21 c7                             
   415 : and    rax, r8                 ; 4c 21 c0                             
   416 : and    r8, rax                 ; 49 21 c0                             
   417 : and    r8, r8                  ; 4d 21 c0                             
   418 : and    rax, [rsp+#0x100]       ; 48 23 84 24 00 01 00 00              
   419 : and    rdi, [rsp+#0x100]       ; 48 23 bc 24 00 01 00 00              
   420 : and    r8, [rsp+#0x100]        ; 4c 23 84 24 00 01 00 00              
   421 : and    rax, #0x100             ; 48 25 00 01 00 00                    
   422 : and    rcx, #0x100             ; 48 81 e1 00 01 00 00                 
   423 : and    rdi, #0x100             ; 48 81 e7 00 01 00 00                 
   424 : and    r8, #0x100              ; 49 81 e0 00 01 00 00                 
   425 : and    [rsp+#0x100], rax       ; 48 21 84 24 00 01 00 00              
   426 : and    [rsp+#0x100], rdi       ; 48 21 bc 24 00 01 00 00              
   427 : and    [rsp+#0x100], r8        ; 4c 21 84 24 00 01 00 00              
   428 : and    [rsp+#0x100], #0x101    ; 48 81 a4 24 00 01 00 00 01 01 00 00  
   429 : or     rax, rax                ; 48 09 c0                             
   430 : or     rax, rdi                ; 48 09 f8                             
   431 : or     rdi, rax                ; 48 09 c7                             
   432 : or     rax, r8                 ; 4c 09 c0                             
   433 : or     r8, rax                 ; 49 09 c0                             
   434 : or     [rsp+#0x100], rax       ; 48 09 84 24 00 01 00 00              
   435 : or     [rsp+#0x100], r8        ; 4c 09 84 24 00 01 00 00              
   436 : or     rax, [rsp+#0x100]       ; 48 0b 84 24 00 01 00 00              
   437 : or     r8, [rsp+#0x100]        ; 4c 0b 84 24 00 01 00 00              
   438 : or     rax, #0x100             ; 48 0d 00 01 00 00                    
   439 : or     r8, #0x100              ; 49 81 c8 00 01 00 00                 
   440 : or     [rsp+#0x100], #0x101    ; 48 81 8c 24 00 01 00 00 01 01 00 00  
   441 : xor    rax, rax                ; 48 31 c0                             
   442 : xor    rax, rdi                ; 48 31 f8                             
   443 : xor    rdi, rax                ; 48 31 c7                             
   444 : xor    rax, r8                 ; 4c 31 c0                             
   445 : xor    r8, rax                 ; 49 31 c0                             
   446 : xor    r8, r8                  ; 4d 31 c0                             
   447 : xor    rax, [rsp+#0x100]       ; 48 33 84 24 00 01 00 00              
   448 : xor    rdi, [rsp+#0x100]       ; 48 33 bc 24 00 01 00 00              
   449 : xor    r8, [rsp+#0x100]        ; 4c 33 84 24 00 01 00 00              
   450 : xor    rax, #0x100             ; 48 35 00 01 00 00                    
   451 : xor    rcx, #0x100             ; 48 81 f1 00 01 00 00                 
   452 : xor    rdi, #0x100             ; 48 81 f7 00 01 00 00                 
   453 : xor    r8, #0x100              ; 49 81 f0 00 01 00 00                 
   454 : xor    [rsp+#0x100], rax       ; 48 31 84 24 00 01 00 00              
   455 : xor    [rsp+#0x100], rdi       ; 48 31 bc 24 00 01 00 00              
   456 : xor    [rsp+#0x100], r8        ; 4c 31 84 24 00 01 00 00              
   457 : xor    [rsp+#0x100], #0x101    ; 48 81 b4 24 00 01 00 00 01 01 00 00  
   458 : cmove  rax, rax                ; 48 0f 44 c0                          
   459 : cmovne rax, rax                ; 48 0f 45 c0                          
   460 : cmovl  rax, rax                ; 48 0f 4c c0                          
   461 : cmovg  rax, rax                ; 48 0f 4f c0                          
   462 : cmovle rax, rax                ; 48 0f 4e c0                          
   463 : cmovge rax, rax                ; 48 0f 4d c0                          
   464 : cmovs  rax, rax                ; 48 0f 48 c0                          
   465 : cmovns rax, rax                ; 48 0f 49 c0                          
   466 : cmove  rdi, rax                ; 48 0f 44 f8                          
   467 : cmove  rax, rdi                ; 48 0f 44 c7                          
   468 : cmove  r8, rax                 ; 4c 0f 44 c0                          
   469 : cmove  rax, r8                 ; 49 0f 44 c0                          
   470 : cmove  r8, r8                  ; 4d 0f 44 c0                          
   471 : cmove  rax, [rsp+#0x100]       ; 48 0f 44 84 24 00 01 00 00           
   472 : cmove  rdi, [rsp+#0x100]       ; 48 0f 44 bc 24 00 01 00 00           
   473 : cmove  r8, [rsp+#0x100]        ; 4c 0f 44 84 24 00 01 00 00           
   474 : sete   rax                     ; 0f 94 c0                             
   475 : setne  rax                     ; 0f 95 c0                             
   476 : setl   rax                     ; 0f 9c c0                             
   477 : setg   rax                     ; 0f 9f c0                             
   478 : setle  rax                     ; 0f 9e c0                             
   479 : setge  rax                     ; 0f 9d c0                             
   480 : sets   rax                     ; 0f 98 c0                             
   481 : setns  rax                     ; 0f 99 c0                             
   482 : sete   rdi                     ; 40 0f 94 c7                          
   483 : sete   r8                      ; 41 0f 94 c0                          
   484 : sete   [rsp+#0x100]            ; 0f 94 84 24 00 01 00 00              
   485 : adc    rax, rax                ; 48 11 c0                             
   486 : adc    rdi, rax                ; 48 11 c7                             
   487 : adc    rax, rdi                ; 48 11 f8                             
   488 : adc    r8, rax                 ; 49 11 c0                             
   489 : adc    rax, r8                 ; 4c 11 c0                             
   490 : adc    r8, r8                  ; 4d 11 c0                             
   491 : adc    rax, #0x100             ; 48 15 00 01 00 00                    
   492 : adc    rcx, #0x100             ; 48 81 d1 00 01 00 00                 
   493 : adc    rdi, #0x100             ; 48 81 d7 00 01 00 00                 
   494 : adc    r8, #0x100              ; 49 81 d0 00 01 00 00                 
   495 : adc    rax, [rsp+#0x100]       ; 48 13 84 24 00 01 00 00              
   496 : adc    rdi, [rsp+#0x100]       ; 48 13 bc 24 00 01 00 00              
   497 : adc    r8, [rsp+#0x100]        ; 4c 13 84 24 00 01 00 00              
   498 : adc    [rsp+#0x100], rax       ; 48 11 84 24 00 01 00 00              
   499 : adc    [rsp+#0x100], rdi       ; 48 11 bc 24 00 01 00 00              
   500 : adc    [rsp+#0x100], r8        ; 4c 11 84 24 00 01 00 00              
   501 : adc    [rsp+#0x100], #0x100    ; 48 81 94 24 00 01 00 00 00 01 00 00  
   502 : cmp    rax, [rsp+#0xfff8]      ; 48 3b 84 24 f8 ff 00 00              
   503 : cmp    rdi, [rsp+#0xfff8]      ; 48 3b bc 24 f8 ff 00 00              
   504 : cmp    r8, [rsp+#0xfff8]       ; 4c 3b 84 24 f8 ff 00 00              
   505 : cmp    r15, [rsp+#0xfff8]      ; 4c 3b bc 24 f8 ff 00 00              
   506 : cmp    [rsp+#0xfff8], rax      ; 48 39 84 24 f8 ff 00 00              
   507 : cmp    [rsp+#0xfff8], rdi      ; 48 39 bc 24 f8 ff 00 00              
   508 : cmp    [rsp+#0xfff8], r8       ; 4c 39 84 24 f8 ff 00 00              
   509 : cmp    [rsp+#0xfff8], r15      ; 4c 39 bc 24 f8 ff 00 00              
   510 : cmp    [rsp+#0xfff8], #0x8888  ; 48 81 bc 24 f8 ff 00 00 88 88 00 00  
   511 : add    rax, [rsp+#0xfff8]      ; 48 03 84 24 f8 ff 00 00              
   512 : add    rdi, [rsp+#0xfff8]      ; 48 03 bc 24 f8 ff 00 00              
   513 : add    r8, [rsp+#0xfff8]       ; 4c 03 84 24 f8 ff 00 00              
   514 : add    r15, [rsp+#0xfff8]      ; 4c 03 bc 24 f8 ff 00 00              
   515 : add    [rsp+#0xfff8], rax      ; 48 01 84 24 f8 ff 00 00              
   516 : add    [rsp+#0xfff8], rdi      ; 48 01 bc 24 f8 ff 00 00              
   517 : add    [rsp+#0xfff8], r8       ; 4c 01 84 24 f8 ff 00 00              
   518 : add    [rsp+#0xfff8], r15      ; 4c 01 bc 24 f8 ff 00 00              
   519 : add    [rsp+#0xfff8], #0x8888  ; 48 81 84 24 f8 ff 00 00 88 88 00 00  
