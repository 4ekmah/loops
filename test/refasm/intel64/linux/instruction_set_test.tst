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
    26 : mov    [rdi], rax              ; 48 89 07                             
    27 : mov    [rax], rdi              ; 48 89 38                             
    28 : mov    [r8], rax               ; 49 89 00                             
    29 : mov    [rax], r8               ; 4c 89 00                             
    30 : mov    [r8], r8                ; 4d 89 00                             
    31 : mov    [r12], rax              ; 49 89 04 24                          
    32 : mov    [r13], rax              ; 49 89 45 00                          
    33 : mov    [r12], r8               ; 4d 89 04 24                          
    34 : mov    [r13], r8               ; 4d 89 45 00                          
    35 : mov    [rax], ax               ; 66 89 00                             
    36 : mov    [rax], ax               ; 66 89 00                             
    37 : mov    [rdi], ax               ; 66 89 07                             
    38 : mov    [rax], di               ; 66 89 38                             
    39 : mov    [rax], ax               ; 66 89 00                             
    40 : mov    [r8], ax                ; 66 41 89 00                          
    41 : mov    [rax], r8w              ; 66 44 89 00                          
    42 : mov    [r8], r8w               ; 66 45 89 00                          
    43 : mov    [r8], ax                ; 66 41 89 00                          
    44 : mov    [r12], ax               ; 66 41 89 04 24                       
    45 : mov    [r12], di               ; 66 41 89 3c 24                       
    46 : mov    [r13], ax               ; 66 41 89 45 00                       
    47 : mov    [r13], di               ; 66 41 89 7d 00                       
    48 : mov    [r13], ax               ; 66 41 89 45 00                       
    49 : mov    [r8], r8w               ; 66 45 89 00                          
    50 : mov    [r12], r8w              ; 66 45 89 04 24                       
    51 : mov    [r13], r8w              ; 66 45 89 45 00                       
    52 : mov    [rax], al               ; 88 00                                
    53 : mov    [rax], al               ; 88 00                                
    54 : mov    [rdi], al               ; 88 07                                
    55 : mov    [rax], dil              ; 40 88 38                             
    56 : mov    [rax], al               ; 88 00                                
    57 : mov    [r8], al                ; 41 88 00                             
    58 : mov    [rax], r8b              ; 44 88 00                             
    59 : mov    [r8], r8b               ; 45 88 00                             
    60 : mov    [r12], al               ; 41 88 04 24                          
    61 : mov    [r12], dil              ; 41 88 3c 24                          
    62 : mov    [r12], r8b              ; 45 88 04 24                          
    63 : mov    [r13], al               ; 41 88 45 00                          
    64 : mov    [r13], dil              ; 41 88 7d 00                          
    65 : mov    [r13], r8b              ; 45 88 45 00                          
    66 : mov    [rax], [rax], rax       ; 48 89 04 00                          
    67 : mov    [rax], [rax], rax       ; 48 89 04 00                          
    68 : mov    [rdi], [rax], rax       ; 48 89 04 07                          
    69 : mov    [rax], [rdi], rax       ; 48 89 04 38                          
    70 : mov    [rax], [rax], rdi       ; 48 89 3c 00                          
    71 : mov    [rax], [rax], rax       ; 48 89 04 00                          
    72 : mov    [r8], [rax], rax        ; 49 89 04 00                          
    73 : mov    [rax], [r8], rax        ; 4a 89 04 00                          
    74 : mov    [r8], [r8], rax         ; 4b 89 04 00                          
    75 : mov    [rax], [rax], r8        ; 4c 89 04 00                          
    76 : mov    [r8], [rax], r8         ; 4d 89 04 00                          
    77 : mov    [rax], [r8], r8         ; 4e 89 04 00                          
    78 : mov    [r8], [r8], r8          ; 4f 89 04 00                          
    79 : mov    [rax], [rax], eax       ; 89 04 00                             
    80 : mov    [rax], [rax], eax       ; 89 04 00                             
    81 : mov    [rdi], [rax], eax       ; 89 04 07                             
    82 : mov    [rax], [rdi], eax       ; 89 04 38                             
    83 : mov    [rax], [rax], edi       ; 89 3c 00                             
    84 : mov    [rax], [rax], eax       ; 89 04 00                             
    85 : mov    [r8], [rax], eax        ; 41 89 04 00                          
    86 : mov    [rax], [r8], eax        ; 42 89 04 00                          
    87 : mov    [r8], [r8], eax         ; 43 89 04 00                          
    88 : mov    [rax], [rax], r8d       ; 44 89 04 00                          
    89 : mov    [r8], [rax], r8d        ; 45 89 04 00                          
    90 : mov    [rax], [r8], r8d        ; 46 89 04 00                          
    91 : mov    [r8], [r8], r8d         ; 47 89 04 00                          
    92 : mov    [r13], [rax], eax       ; 41 89 44 05 00                       
    93 : mov    [r13], [r8], eax        ; 43 89 44 05 00                       
    94 : mov    [r13], [rax], r8d       ; 45 89 44 05 00                       
    95 : mov    [r13], [r8], r8d        ; 47 89 44 05 00                       
    96 : mov    [rax], [rax], ax        ; 66 89 04 00                          
    97 : mov    [rax], [rax], ax        ; 66 89 04 00                          
    98 : mov    [rdi], [rax], ax        ; 66 89 04 07                          
    99 : mov    [rax], [rdi], ax        ; 66 89 04 38                          
   100 : mov    [rax], [rax], di        ; 66 89 3c 00                          
   101 : mov    [rax], [rax], ax        ; 66 89 04 00                          
   102 : mov    [r8], [rax], ax         ; 66 41 89 04 00                       
   103 : mov    [rax], [r8], ax         ; 66 42 89 04 00                       
   104 : mov    [r8], [r8], ax          ; 66 43 89 04 00                       
   105 : mov    [rax], [rax], r8w       ; 66 44 89 04 00                       
   106 : mov    [r8], [rax], r8w        ; 66 45 89 04 00                       
   107 : mov    [rax], [r8], r8w        ; 66 46 89 04 00                       
   108 : mov    [r8], [r8], r8w         ; 66 47 89 04 00                       
   109 : mov    [r13], [rax], ax        ; 66 41 89 44 05 00                    
   110 : mov    [r13], [r8], ax         ; 66 43 89 44 05 00                    
   111 : mov    [r13], [rax], r8w       ; 66 45 89 44 05 00                    
   112 : mov    [r13], [r8], r8w        ; 66 47 89 44 05 00                    
   113 : mov    [rax], [rax], al        ; 88 04 00                             
   114 : mov    [rax], [rax], al        ; 88 04 00                             
   115 : mov    [rdi], [rax], al        ; 88 04 07                             
   116 : mov    [rax], [rdi], al        ; 88 04 38                             
   117 : mov    [rax], [rax], dil       ; 40 88 3c 00                          
   118 : mov    [rax], [rax], al        ; 88 04 00                             
   119 : mov    [r8], [rax], al         ; 41 88 04 00                          
   120 : mov    [rax], [r8], al         ; 42 88 04 00                          
   121 : mov    [r8], [r8], al          ; 43 88 04 00                          
   122 : mov    [rax], [rax], r8b       ; 44 88 04 00                          
   123 : mov    [r8], [rax], r8b        ; 45 88 04 00                          
   124 : mov    [rax], [r8], r8b        ; 46 88 04 00                          
   125 : mov    [r8], [r8], r8b         ; 47 88 04 00                          
   126 : mov    [r13], [rax], al        ; 41 88 44 05 00                       
   127 : mov    [r13], [r8], al         ; 43 88 44 05 00                       
   128 : mov    [r13], [rax], r8b       ; 45 88 44 05 00                       
   129 : mov    [r13], [r8], r8b        ; 47 88 44 05 00                       
   130 : mov    [rax], [rax], dil       ; 40 88 3c 00                          
   131 : mov    [r8], [rax], dil        ; 41 88 3c 00                          
   132 : mov    [rax], [r8], dil        ; 42 88 3c 00                          
   133 : mov    [r8], [r8], dil         ; 43 88 3c 00                          
   134 : mov    [rax], [#0x100], rax    ; 48 89 80 00 01 00 00                 
   135 : mov    [rax], [#0x100], rax    ; 48 89 80 00 01 00 00                 
   136 : mov    [rdi], [#0x100], rax    ; 48 89 87 00 01 00 00                 
   137 : mov    [rax], [#0x100], rdi    ; 48 89 b8 00 01 00 00                 
   138 : mov    [rax], [#0x100], rax    ; 48 89 80 00 01 00 00                 
   139 : mov    [r8], [#0x100], rax     ; 49 89 80 00 01 00 00                 
   140 : mov    [rax], [#0x100], r8     ; 4c 89 80 00 01 00 00                 
   141 : mov    [r8], [#0x100], r8      ; 4d 89 80 00 01 00 00                 
   142 : mov    [r12], [#0x100], rax    ; 49 89 84 24 00 01 00 00              
   143 : mov    [r12], [#0x100], rdi    ; 49 89 bc 24 00 01 00 00              
   144 : mov    [r12], [#0x100], r8     ; 4d 89 84 24 00 01 00 00              
   145 : mov    [rax], [#0x100], eax    ; 89 80 00 01 00 00                    
   146 : mov    [rax], [#0x100], eax    ; 89 80 00 01 00 00                    
   147 : mov    [rdi], [#0x100], eax    ; 89 87 00 01 00 00                    
   148 : mov    [rax], [#0x100], edi    ; 89 b8 00 01 00 00                    
   149 : mov    [rax], [#0x100], eax    ; 89 80 00 01 00 00                    
   150 : mov    [r8], [#0x100], eax     ; 41 89 80 00 01 00 00                 
   151 : mov    [rax], [#0x100], r8d    ; 44 89 80 00 01 00 00                 
   152 : mov    [r8], [#0x100], r8d     ; 45 89 80 00 01 00 00                 
   153 : mov    [r12], [#0x100], eax    ; 41 89 84 24 00 01 00 00              
   154 : mov    [r12], [#0x100], r8d    ; 45 89 84 24 00 01 00 00              
   155 : mov    [rax], [#0x100], ax     ; 66 89 80 00 01 00 00                 
   156 : mov    [rax], [#0x100], ax     ; 66 89 80 00 01 00 00                 
   157 : mov    [rdi], [#0x100], ax     ; 66 89 87 00 01 00 00                 
   158 : mov    [rax], [#0x100], di     ; 66 89 b8 00 01 00 00                 
   159 : mov    [rax], [#0x100], ax     ; 66 89 80 00 01 00 00                 
   160 : mov    [r8], [#0x100], ax      ; 66 41 89 80 00 01 00 00              
   161 : mov    [rax], [#0x100], r8w    ; 66 44 89 80 00 01 00 00              
   162 : mov    [r8], [#0x100], r8w     ; 66 45 89 80 00 01 00 00              
   163 : mov    [r12], [#0x100], ax     ; 66 41 89 84 24 00 01 00 00           
   164 : mov    [r12], [#0x100], r8w    ; 66 45 89 84 24 00 01 00 00           
   165 : mov    [rax], [#0x100], al     ; 88 80 00 01 00 00                    
   166 : mov    [rax], [#0x100], al     ; 88 80 00 01 00 00                    
   167 : mov    [rdi], [#0x100], al     ; 88 87 00 01 00 00                    
   168 : mov    [rax], [#0x100], dil    ; 40 88 b8 00 01 00 00                 
   169 : mov    [rax], [#0x100], al     ; 88 80 00 01 00 00                    
   170 : mov    [r8], [#0x100], al      ; 41 88 80 00 01 00 00                 
   171 : mov    [rax], [#0x100], r8b    ; 44 88 80 00 01 00 00                 
   172 : mov    [r8], [#0x100], r8b     ; 45 88 80 00 01 00 00                 
   173 : mov    [r12], [#0x100], al     ; 41 88 84 24 00 01 00 00              
   174 : mov    [r12], [#0x100], r8b    ; 45 88 84 24 00 01 00 00              
   175 : mov    [rax], [rax], #0x100    ; 48 c7 04 00 00 01 00 00              
   176 : mov    [rax], [rax], #0x100    ; 48 c7 04 00 00 01 00 00              
   177 : mov    [rdi], [rax], #0x100    ; 48 c7 04 07 00 01 00 00              
   178 : mov    [rax], [rdi], #0x100    ; 48 c7 04 38 00 01 00 00              
   179 : mov    [rax], [rax], #0x100    ; 48 c7 04 00 00 01 00 00              
   180 : mov    [r8], [rax], #0x100     ; 49 c7 04 00 00 01 00 00              
   181 : mov    [rax], [r8], #0x100     ; 4a c7 04 00 00 01 00 00              
   182 : mov    [r8], [r8], #0x100      ; 4b c7 04 00 00 01 00 00              
   183 : mov    [r13], [rax], #0x100    ; 49 c7 44 05 00 00 01 00 00           
   184 : mov    [r13], [r8], #0x100     ; 4b c7 44 05 00 00 01 00 00           
   185 : mov    [rax], [rax], #0x100    ; c7 04 00 00 01 00 00                 
   186 : mov    [rax], [rax], #0x100    ; c7 04 00 00 01 00 00                 
   187 : mov    [rdi], [rax], #0x100    ; c7 04 07 00 01 00 00                 
   188 : mov    [rax], [rdi], #0x100    ; c7 04 38 00 01 00 00                 
   189 : mov    [rax], [rax], #0x100    ; c7 04 00 00 01 00 00                 
   190 : mov    [r8], [rax], #0x100     ; 41 c7 04 00 00 01 00 00              
   191 : mov    [rax], [r8], #0x100     ; 42 c7 04 00 00 01 00 00              
   192 : mov    [r8], [r8], #0x100      ; 43 c7 04 00 00 01 00 00              
   193 : mov    [r13], [rax], #0x100    ; 41 c7 44 05 00 00 01 00 00           
   194 : mov    [r13], [r8], #0x100     ; 43 c7 44 05 00 00 01 00 00           
   195 : mov    [rax], [rax], #0x100    ; 66 c7 04 00 00 01                    
   196 : mov    [rax], [rax], #0x100    ; 66 c7 04 00 00 01                    
   197 : mov    [rdi], [rax], #0x100    ; 66 c7 04 07 00 01                    
   198 : mov    [rax], [rdi], #0x100    ; 66 c7 04 38 00 01                    
   199 : mov    [rax], [rax], #0x100    ; 66 c7 04 00 00 01                    
   200 : mov    [r8], [rax], #0x100     ; 66 41 c7 04 00 00 01                 
   201 : mov    [rax], [r8], #0x100     ; 66 42 c7 04 00 00 01                 
   202 : mov    [r8], [r8], #0x100      ; 66 43 c7 04 00 00 01                 
   203 : mov    [r13], [rax], #0x100    ; 66 41 c7 44 05 00 00 01              
   204 : mov    [r13], [r8], #0x100     ; 66 43 c7 44 05 00 00 01              
   205 : mov    [rax], [rax], #0xff     ; c6 04 00 ff                          
   206 : mov    [rax], [rax], #0xff     ; c6 04 00 ff                          
   207 : mov    [rdi], [rax], #0xff     ; c6 04 07 ff                          
   208 : mov    [rax], [rdi], #0xff     ; c6 04 38 ff                          
   209 : mov    [rax], [rax], #0xff     ; c6 04 00 ff                          
   210 : mov    [r8], [rax], #0xff      ; 41 c6 04 00 ff                       
   211 : mov    [rax], [r8], #0xff      ; 42 c6 04 00 ff                       
   212 : mov    [r8], [r8], #0xff       ; 43 c6 04 00 ff                       
   213 : mov    [r13], [rax], #0xff     ; 41 c6 44 05 00 ff                    
   214 : mov    [r13], [r8], #0xff      ; 43 c6 44 05 00 ff                    
   215 : mov    [rax], [#0x101], #0x100 ; 48 c7 80 01 01 00 00 00 01 00 00     
   216 : mov    [rax], [#0x101], #0x100 ; 48 c7 80 01 01 00 00 00 01 00 00     
   217 : mov    [rdi], [#0x101], #0x100 ; 48 c7 87 01 01 00 00 00 01 00 00     
   218 : mov    [rax], [#0x101], #0x100 ; 48 c7 80 01 01 00 00 00 01 00 00     
   219 : mov    [r8], [#0x101], #0x100  ; 49 c7 80 01 01 00 00 00 01 00 00     
   220 : mov    [r12], [#0x101], #0x100 ; 49 c7 84 24 01 01 00 00 00 01 00 00  
   221 : mov    [rax], [#0x101], #0x100 ; c7 80 01 01 00 00 00 01 00 00        
   222 : mov    [rax], [#0x101], #0x100 ; c7 80 01 01 00 00 00 01 00 00        
   223 : mov    [rdi], [#0x101], #0x100 ; c7 87 01 01 00 00 00 01 00 00        
   224 : mov    [r8], [#0x101], #0x100  ; 41 c7 80 01 01 00 00 00 01 00 00     
   225 : mov    [r12], [#0x101], #0x100 ; 41 c7 84 24 01 01 00 00 00 01 00 00  
   226 : mov    [rax], [#0x101], #0x100 ; 66 c7 80 01 01 00 00 00 01           
   227 : mov    [rax], [#0x101], #0x100 ; 66 c7 80 01 01 00 00 00 01           
   228 : mov    [rdi], [#0x101], #0x100 ; 66 c7 87 01 01 00 00 00 01           
   229 : mov    [r8], [#0x101], #0x100  ; 66 41 c7 80 01 01 00 00 00 01        
   230 : mov    [r12], [#0x101], #0x100 ; 66 41 c7 84 24 01 01 00 00 00 01     
   231 : mov    [rax], [#0x101], #0xff  ; c6 80 01 01 00 00 ff                 
   232 : mov    [rax], [#0x101], #0xff  ; c6 80 01 01 00 00 ff                 
   233 : mov    [rdi], [#0x101], #0xff  ; c6 87 01 01 00 00 ff                 
   234 : mov    [r8], [#0x101], #0xff   ; 41 c6 80 01 01 00 00 ff              
   235 : mov    [r12], [#0x101], #0xff  ; 41 c6 84 24 01 01 00 00 ff           
   236 : mov    rax, [rax]              ; 48 8b 00                             
   237 : mov    rax, [rax]              ; 48 8b 00                             
   238 : mov    rdi, [rax]              ; 48 8b 38                             
   239 : mov    rax, [rdi]              ; 48 8b 07                             
   240 : mov    rax, [rax]              ; 48 8b 00                             
   241 : mov    r8, [rax]               ; 4c 8b 00                             
   242 : mov    rax, [r8]               ; 49 8b 00                             
   243 : mov    r8, [r8]                ; 4d 8b 00                             
   244 : mov    eax, [rax]              ; 8b 00                                
   245 : mov    edi, [rax]              ; 8b 38                                
   246 : mov    eax, [rdi]              ; 8b 07                                
   247 : mov    eax, [rax]              ; 8b 00                                
   248 : mov    r8d, [rax]              ; 44 8b 00                             
   249 : mov    eax, [r8]               ; 41 8b 00                             
   250 : mov    r8d, [r8]               ; 45 8b 00                             
   251 : movzx  rax, ax                 ; 48 0f b7 00                          
   252 : movzx  rdi, ax                 ; 48 0f b7 38                          
   253 : movzx  rax, di                 ; 48 0f b7 07                          
   254 : movzx  r8, ax                  ; 4c 0f b7 00                          
   255 : movzx  rax, r8w                ; 49 0f b7 00                          
   256 : movzx  r8, r8w                 ; 4d 0f b7 00                          
   257 : movsx  rax, ax                 ; 48 0f bf 00                          
   258 : movsx  rdi, ax                 ; 48 0f bf 38                          
   259 : movsx  rax, di                 ; 48 0f bf 07                          
   260 : movsx  r8, ax                  ; 4c 0f bf 00                          
   261 : movsx  rax, r8w                ; 49 0f bf 00                          
   262 : movsx  r8, r8w                 ; 4d 0f bf 00                          
   263 : movzx  rax, al                 ; 48 0f b6 00                          
   264 : movzx  rdi, al                 ; 48 0f b6 38                          
   265 : movzx  rax, dil                ; 48 0f b6 07                          
   266 : movzx  r8, al                  ; 4c 0f b6 00                          
   267 : movzx  rax, r8b                ; 49 0f b6 00                          
   268 : movzx  r8, r8b                 ; 4d 0f b6 00                          
   269 : movsx  rax, al                 ; 48 0f be 00                          
   270 : movsx  rdi, al                 ; 48 0f be 38                          
   271 : movsx  rax, dil                ; 48 0f be 07                          
   272 : movsx  r8, al                  ; 4c 0f be 00                          
   273 : movsx  rax, r8b                ; 49 0f be 00                          
   274 : movsx  r8, r8b                 ; 4d 0f be 00                          
   275 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   276 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   277 : mov    rdi, [rax], [#0x100]    ; 48 8b b8 00 01 00 00                 
   278 : mov    rax, [rdi], [#0x100]    ; 48 8b 87 00 01 00 00                 
   279 : mov    rax, [rax], [#0x100]    ; 48 8b 80 00 01 00 00                 
   280 : mov    r8, [rax], [#0x100]     ; 4c 8b 80 00 01 00 00                 
   281 : mov    rax, [r8], [#0x100]     ; 49 8b 80 00 01 00 00                 
   282 : mov    r8, [r8], [#0x100]      ; 4d 8b 80 00 01 00 00                 
   283 : mov    eax, [rax], [#0x100]    ; 8b 80 00 01 00 00                    
   284 : mov    edi, [rax], [#0x100]    ; 8b b8 00 01 00 00                    
   285 : mov    eax, [rdi], [#0x100]    ; 8b 87 00 01 00 00                    
   286 : mov    r8d, [rax], [#0x100]    ; 44 8b 80 00 01 00 00                 
   287 : mov    eax, [r8], [#0x100]     ; 41 8b 80 00 01 00 00                 
   288 : mov    r8d, [r8], [#0x100]     ; 45 8b 80 00 01 00 00                 
   289 : movsxd rax, rax, #0x100        ; 48 63 80 00 01 00 00                 
   290 : movsxd rdi, rax, #0x100        ; 48 63 b8 00 01 00 00                 
   291 : movsxd rax, rdi, #0x100        ; 48 63 87 00 01 00 00                 
   292 : movsxd r8, rax, #0x100         ; 4c 63 80 00 01 00 00                 
   293 : movsxd rax, r8, #0x100         ; 49 63 80 00 01 00 00                 
   294 : movsxd r8, r8, #0x100          ; 4d 63 80 00 01 00 00                 
   295 : movzx  rax, ax, #0x100         ; 48 0f b7 80 00 01 00 00              
   296 : movzx  rdi, ax, #0x100         ; 48 0f b7 b8 00 01 00 00              
   297 : movzx  rax, di, #0x100         ; 48 0f b7 87 00 01 00 00              
   298 : movzx  r8, ax, #0x100          ; 4c 0f b7 80 00 01 00 00              
   299 : movzx  rax, r8w, #0x100        ; 49 0f b7 80 00 01 00 00              
   300 : movzx  r8, r8w, #0x100         ; 4d 0f b7 80 00 01 00 00              
   301 : movsx  rax, ax, #0x100         ; 48 0f bf 80 00 01 00 00              
   302 : movsx  rdi, ax, #0x100         ; 48 0f bf b8 00 01 00 00              
   303 : movsx  rax, di, #0x100         ; 48 0f bf 87 00 01 00 00              
   304 : movsx  r8, ax, #0x100          ; 4c 0f bf 80 00 01 00 00              
   305 : movsx  rax, r8w, #0x100        ; 49 0f bf 80 00 01 00 00              
   306 : movsx  r8, r8w, #0x100         ; 4d 0f bf 80 00 01 00 00              
   307 : movzx  rax, al, #0x100         ; 48 0f b6 80 00 01 00 00              
   308 : movzx  rdi, al, #0x100         ; 48 0f b6 b8 00 01 00 00              
   309 : movzx  rax, dil, #0x100        ; 48 0f b6 87 00 01 00 00              
   310 : movzx  r8, al, #0x100          ; 4c 0f b6 80 00 01 00 00              
   311 : movzx  rax, r8b, #0x100        ; 49 0f b6 80 00 01 00 00              
   312 : movzx  r8, r8b, #0x100         ; 4d 0f b6 80 00 01 00 00              
   313 : movsx  rax, al, #0x100         ; 48 0f be 80 00 01 00 00              
   314 : movsx  rdi, al, #0x100         ; 48 0f be b8 00 01 00 00              
   315 : movsx  rax, dil, #0x100        ; 48 0f be 87 00 01 00 00              
   316 : movsx  r8, al, #0x100          ; 4c 0f be 80 00 01 00 00              
   317 : movsx  rax, r8b, #0x100        ; 49 0f be 80 00 01 00 00              
   318 : movsx  r8, r8b, #0x100         ; 4d 0f be 80 00 01 00 00              
   319 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   320 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   321 : mov    rdi, [rax], [rax]       ; 48 8b 3c 00                          
   322 : mov    rax, [rdi], [rax]       ; 48 8b 04 07                          
   323 : mov    rax, [rax], [rdi]       ; 48 8b 04 38                          
   324 : mov    rax, [rax], [rax]       ; 48 8b 04 00                          
   325 : mov    r8, [rax], [rax]        ; 4c 8b 04 00                          
   326 : mov    rax, [r8], [rax]        ; 49 8b 04 00                          
   327 : mov    r8, [r8], [rax]         ; 4d 8b 04 00                          
   328 : mov    rax, [rax], [r8]        ; 4a 8b 04 00                          
   329 : mov    r8, [rax], [r8]         ; 4e 8b 04 00                          
   330 : mov    rax, [r8], [r8]         ; 4b 8b 04 00                          
   331 : mov    r8, [r8], [r8]          ; 4f 8b 04 00                          
   332 : mov    eax, [rax], [rax]       ; 8b 04 00                             
   333 : mov    edi, [rax], [rax]       ; 8b 3c 00                             
   334 : mov    eax, [rdi], [rax]       ; 8b 04 07                             
   335 : mov    eax, [rax], [rdi]       ; 8b 04 38                             
   336 : mov    r8d, [rax], [rax]       ; 44 8b 04 00                          
   337 : mov    eax, [r8], [rax]        ; 41 8b 04 00                          
   338 : mov    r8d, [r8], [rax]        ; 45 8b 04 00                          
   339 : mov    eax, [rax], [r8]        ; 42 8b 04 00                          
   340 : mov    r8d, [rax], [r8]        ; 46 8b 04 00                          
   341 : mov    eax, [r8], [r8]         ; 43 8b 04 00                          
   342 : mov    r8d, [r8], [r8]         ; 47 8b 04 00                          
   343 : movzx  rax, ax, rax            ; 48 0f b7 04 00                       
   344 : movzx  rdi, ax, rax            ; 48 0f b7 3c 00                       
   345 : movzx  rax, di, rax            ; 48 0f b7 04 07                       
   346 : movzx  rax, ax, rdi            ; 48 0f b7 04 38                       
   347 : movzx  r8, ax, rax             ; 4c 0f b7 04 00                       
   348 : movzx  rax, r8w, rax           ; 49 0f b7 04 00                       
   349 : movzx  r8, r8w, rax            ; 4d 0f b7 04 00                       
   350 : movzx  rax, ax, r8             ; 4a 0f b7 04 00                       
   351 : movzx  r8, ax, r8              ; 4e 0f b7 04 00                       
   352 : movzx  rax, r8w, r8            ; 4b 0f b7 04 00                       
   353 : movzx  r8, r8w, r8             ; 4f 0f b7 04 00                       
   354 : movsx  rax, ax, rax            ; 48 0f bf 04 00                       
   355 : movsx  rdi, ax, rax            ; 48 0f bf 3c 00                       
   356 : movsx  rax, di, rax            ; 48 0f bf 04 07                       
   357 : movsx  rax, ax, rdi            ; 48 0f bf 04 38                       
   358 : movsx  r8, ax, rax             ; 4c 0f bf 04 00                       
   359 : movsx  rax, r8w, rax           ; 49 0f bf 04 00                       
   360 : movsx  r8, r8w, rax            ; 4d 0f bf 04 00                       
   361 : movsx  rax, ax, r8             ; 4a 0f bf 04 00                       
   362 : movsx  r8, ax, r8              ; 4e 0f bf 04 00                       
   363 : movsx  rax, r8w, r8            ; 4b 0f bf 04 00                       
   364 : movsx  r8, r8w, r8             ; 4f 0f bf 04 00                       
   365 : movzx  rax, al, rax            ; 48 0f b6 04 00                       
   366 : movzx  rdi, al, rax            ; 48 0f b6 3c 00                       
   367 : movzx  rax, dil, rax           ; 48 0f b6 04 07                       
   368 : movzx  rax, al, rdi            ; 48 0f b6 04 38                       
   369 : movzx  r8, al, rax             ; 4c 0f b6 04 00                       
   370 : movzx  rax, r8b, rax           ; 49 0f b6 04 00                       
   371 : movzx  r8, r8b, rax            ; 4d 0f b6 04 00                       
   372 : movzx  rax, al, r8             ; 4a 0f b6 04 00                       
   373 : movzx  r8, al, r8              ; 4e 0f b6 04 00                       
   374 : movzx  rax, r8b, r8            ; 4b 0f b6 04 00                       
   375 : movzx  r8, r8b, r8             ; 4f 0f b6 04 00                       
   376 : movsx  rax, al, rax            ; 48 0f be 04 00                       
   377 : movsx  rdi, al, rax            ; 48 0f be 3c 00                       
   378 : movsx  rax, dil, rax           ; 48 0f be 04 07                       
   379 : movsx  rax, al, rdi            ; 48 0f be 04 38                       
   380 : movsx  r8, al, rax             ; 4c 0f be 04 00                       
   381 : movsx  rax, r8b, rax           ; 49 0f be 04 00                       
   382 : movsx  r8, r8b, rax            ; 4d 0f be 04 00                       
   383 : movsx  rax, al, r8             ; 4a 0f be 04 00                       
   384 : movsx  r8, al, r8              ; 4e 0f be 04 00                       
   385 : movsx  rax, r8b, r8            ; 4b 0f be 04 00                       
   386 : movsx  r8, r8b, r8             ; 4f 0f be 04 00                       
   387 : xchg   rax, rax                ; 48 90                                
   388 : xchg   rax, rdi                ; 48 97                                
   389 : xchg   rax, r8                 ; 49 90                                
   390 : xchg   rcx, rcx                ; 48 87 c9                             
   391 : xchg   rdi, rcx                ; 48 87 cf                             
   392 : xchg   rcx, rdi                ; 48 87 f9                             
   393 : xchg   rcx, rcx                ; 48 87 c9                             
   394 : xchg   r8, rcx                 ; 49 87 c8                             
   395 : xchg   rcx, r8                 ; 4c 87 c1                             
   396 : xchg   r8, r8                  ; 4d 87 c0                             
   397 : xchg   rcx, [rsp+#0x100]       ; 48 87 8c 24 00 01 00 00              
   398 : xchg   rdi, [rsp+#0x100]       ; 48 87 bc 24 00 01 00 00              
   399 : xchg   r8, [rsp+#0x100]        ; 4c 87 84 24 00 01 00 00              
   400 : xchg   r15, [rsp+#0x100]       ; 4c 87 bc 24 00 01 00 00              
   401 : xchg   [rsp+#0x100], rcx       ; 48 87 8c 24 00 01 00 00              
   402 : xchg   [rsp+#0x100], rdi       ; 48 87 bc 24 00 01 00 00              
   403 : xchg   [rsp+#0x100], r8        ; 4c 87 84 24 00 01 00 00              
   404 : xchg   [rsp+#0x100], r15       ; 4c 87 bc 24 00 01 00 00              
   405 : shl    rax, #0x0f              ; 48 c1 e0 0f                          
   406 : shl    rdi, #0x0f              ; 48 c1 e7 0f                          
   407 : shl    r8, #0x0f               ; 49 c1 e0 0f                          
   408 : shl    [rsp+#0x100], #0x0f     ; 48 c1 a4 24 00 01 00 00 0f           
   409 : shl    rax, cl                 ; 48 d3 e0                             
   410 : shl    rdi, cl                 ; 48 d3 e7                             
   411 : shl    r8, cl                  ; 49 d3 e0                             
   412 : shl    [rsp+#0x100], cl        ; 48 d3 a4 24 00 01 00 00              
   413 : shr    rax, #0x0f              ; 48 c1 e8 0f                          
   414 : shr    rdi, #0x0f              ; 48 c1 ef 0f                          
   415 : shr    r8, #0x0f               ; 49 c1 e8 0f                          
   416 : shr    [rsp+#0x100], #0x0f     ; 48 c1 ac 24 00 01 00 00 0f           
   417 : shr    rax, cl                 ; 48 d3 e8                             
   418 : shr    rdi, cl                 ; 48 d3 ef                             
   419 : shr    r8, cl                  ; 49 d3 e8                             
   420 : shr    [rsp+#0x100], cl        ; 48 d3 ac 24 00 01 00 00              
   421 : sar    rax, #0x0f              ; 48 c1 f8 0f                          
   422 : sar    rdi, #0x0f              ; 48 c1 ff 0f                          
   423 : sar    r8, #0x0f               ; 49 c1 f8 0f                          
   424 : sar    [rsp+#0x100], #0x0f     ; 48 c1 bc 24 00 01 00 00 0f           
   425 : sar    rax, cl                 ; 48 d3 f8                             
   426 : sar    rdi, cl                 ; 48 d3 ff                             
   427 : sar    r8, cl                  ; 49 d3 f8                             
   428 : sar    [rsp+#0x100], cl        ; 48 d3 bc 24 00 01 00 00              
   429 : and    rax, rax                ; 48 21 c0                             
   430 : and    rax, rdi                ; 48 21 f8                             
   431 : and    rdi, rax                ; 48 21 c7                             
   432 : and    rax, r8                 ; 4c 21 c0                             
   433 : and    r8, rax                 ; 49 21 c0                             
   434 : and    r8, r8                  ; 4d 21 c0                             
   435 : and    rax, [rsp+#0x100]       ; 48 23 84 24 00 01 00 00              
   436 : and    rdi, [rsp+#0x100]       ; 48 23 bc 24 00 01 00 00              
   437 : and    r8, [rsp+#0x100]        ; 4c 23 84 24 00 01 00 00              
   438 : and    rax, #0x100             ; 48 25 00 01 00 00                    
   439 : and    rcx, #0x100             ; 48 81 e1 00 01 00 00                 
   440 : and    rdi, #0x100             ; 48 81 e7 00 01 00 00                 
   441 : and    r8, #0x100              ; 49 81 e0 00 01 00 00                 
   442 : and    [rsp+#0x100], rax       ; 48 21 84 24 00 01 00 00              
   443 : and    [rsp+#0x100], rdi       ; 48 21 bc 24 00 01 00 00              
   444 : and    [rsp+#0x100], r8        ; 4c 21 84 24 00 01 00 00              
   445 : and    [rsp+#0x100], #0x101    ; 48 81 a4 24 00 01 00 00 01 01 00 00  
   446 : or     rax, rax                ; 48 09 c0                             
   447 : or     rax, rdi                ; 48 09 f8                             
   448 : or     rdi, rax                ; 48 09 c7                             
   449 : or     rax, r8                 ; 4c 09 c0                             
   450 : or     r8, rax                 ; 49 09 c0                             
   451 : or     [rsp+#0x100], rax       ; 48 09 84 24 00 01 00 00              
   452 : or     [rsp+#0x100], r8        ; 4c 09 84 24 00 01 00 00              
   453 : or     rax, [rsp+#0x100]       ; 48 0b 84 24 00 01 00 00              
   454 : or     r8, [rsp+#0x100]        ; 4c 0b 84 24 00 01 00 00              
   455 : or     rax, #0x100             ; 48 0d 00 01 00 00                    
   456 : or     r8, #0x100              ; 49 81 c8 00 01 00 00                 
   457 : or     [rsp+#0x100], #0x101    ; 48 81 8c 24 00 01 00 00 01 01 00 00  
   458 : xor    rax, rax                ; 48 31 c0                             
   459 : xor    rax, rdi                ; 48 31 f8                             
   460 : xor    rdi, rax                ; 48 31 c7                             
   461 : xor    rax, r8                 ; 4c 31 c0                             
   462 : xor    r8, rax                 ; 49 31 c0                             
   463 : xor    r8, r8                  ; 4d 31 c0                             
   464 : xor    rax, [rsp+#0x100]       ; 48 33 84 24 00 01 00 00              
   465 : xor    rdi, [rsp+#0x100]       ; 48 33 bc 24 00 01 00 00              
   466 : xor    r8, [rsp+#0x100]        ; 4c 33 84 24 00 01 00 00              
   467 : xor    rax, #0x100             ; 48 35 00 01 00 00                    
   468 : xor    rcx, #0x100             ; 48 81 f1 00 01 00 00                 
   469 : xor    rdi, #0x100             ; 48 81 f7 00 01 00 00                 
   470 : xor    r8, #0x100              ; 49 81 f0 00 01 00 00                 
   471 : xor    [rsp+#0x100], rax       ; 48 31 84 24 00 01 00 00              
   472 : xor    [rsp+#0x100], rdi       ; 48 31 bc 24 00 01 00 00              
   473 : xor    [rsp+#0x100], r8        ; 4c 31 84 24 00 01 00 00              
   474 : xor    [rsp+#0x100], #0x101    ; 48 81 b4 24 00 01 00 00 01 01 00 00  
   475 : cmove  rax, rax                ; 48 0f 44 c0                          
   476 : cmovne rax, rax                ; 48 0f 45 c0                          
   477 : cmovl  rax, rax                ; 48 0f 4c c0                          
   478 : cmovg  rax, rax                ; 48 0f 4f c0                          
   479 : cmovle rax, rax                ; 48 0f 4e c0                          
   480 : cmovge rax, rax                ; 48 0f 4d c0                          
   481 : cmovs  rax, rax                ; 48 0f 48 c0                          
   482 : cmovns rax, rax                ; 48 0f 49 c0                          
   483 : cmove  rdi, rax                ; 48 0f 44 f8                          
   484 : cmove  rax, rdi                ; 48 0f 44 c7                          
   485 : cmove  r8, rax                 ; 4c 0f 44 c0                          
   486 : cmove  rax, r8                 ; 49 0f 44 c0                          
   487 : cmove  r8, r8                  ; 4d 0f 44 c0                          
   488 : cmove  rax, [rsp+#0x100]       ; 48 0f 44 84 24 00 01 00 00           
   489 : cmove  rdi, [rsp+#0x100]       ; 48 0f 44 bc 24 00 01 00 00           
   490 : cmove  r8, [rsp+#0x100]        ; 4c 0f 44 84 24 00 01 00 00           
   491 : adc    rax, rax                ; 48 11 c0                             
   492 : adc    rdi, rax                ; 48 11 c7                             
   493 : adc    rax, rdi                ; 48 11 f8                             
   494 : adc    r8, rax                 ; 49 11 c0                             
   495 : adc    rax, r8                 ; 4c 11 c0                             
   496 : adc    r8, r8                  ; 4d 11 c0                             
   497 : adc    rax, #0x100             ; 48 15 00 01 00 00                    
   498 : adc    rcx, #0x100             ; 48 81 d1 00 01 00 00                 
   499 : adc    rdi, #0x100             ; 48 81 d7 00 01 00 00                 
   500 : adc    r8, #0x100              ; 49 81 d0 00 01 00 00                 
   501 : adc    rax, [rsp+#0x100]       ; 48 13 84 24 00 01 00 00              
   502 : adc    rdi, [rsp+#0x100]       ; 48 13 bc 24 00 01 00 00              
   503 : adc    r8, [rsp+#0x100]        ; 4c 13 84 24 00 01 00 00              
   504 : adc    [rsp+#0x100], rax       ; 48 11 84 24 00 01 00 00              
   505 : adc    [rsp+#0x100], rdi       ; 48 11 bc 24 00 01 00 00              
   506 : adc    [rsp+#0x100], r8        ; 4c 11 84 24 00 01 00 00              
   507 : adc    [rsp+#0x100], #0x100    ; 48 81 94 24 00 01 00 00 00 01 00 00  
   508 : cmp    rax, [rsp+#0xfff8]      ; 48 3b 84 24 f8 ff 00 00              
   509 : cmp    rdi, [rsp+#0xfff8]      ; 48 3b bc 24 f8 ff 00 00              
   510 : cmp    r8, [rsp+#0xfff8]       ; 4c 3b 84 24 f8 ff 00 00              
   511 : cmp    r15, [rsp+#0xfff8]      ; 4c 3b bc 24 f8 ff 00 00              
   512 : cmp    [rsp+#0xfff8], rax      ; 48 39 84 24 f8 ff 00 00              
   513 : cmp    [rsp+#0xfff8], rdi      ; 48 39 bc 24 f8 ff 00 00              
   514 : cmp    [rsp+#0xfff8], r8       ; 4c 39 84 24 f8 ff 00 00              
   515 : cmp    [rsp+#0xfff8], r15      ; 4c 39 bc 24 f8 ff 00 00              
   516 : cmp    [rsp+#0xfff8], #0x8888  ; 48 81 bc 24 f8 ff 00 00 88 88 00 00  
   517 : add    rax, [rsp+#0xfff8]      ; 48 03 84 24 f8 ff 00 00              
   518 : add    rdi, [rsp+#0xfff8]      ; 48 03 bc 24 f8 ff 00 00              
   519 : add    r8, [rsp+#0xfff8]       ; 4c 03 84 24 f8 ff 00 00              
   520 : add    r15, [rsp+#0xfff8]      ; 4c 03 bc 24 f8 ff 00 00              
   521 : add    [rsp+#0xfff8], rax      ; 48 01 84 24 f8 ff 00 00              
   522 : add    [rsp+#0xfff8], rdi      ; 48 01 bc 24 f8 ff 00 00              
   523 : add    [rsp+#0xfff8], r8       ; 4c 01 84 24 f8 ff 00 00              
   524 : add    [rsp+#0xfff8], r15      ; 4c 01 bc 24 f8 ff 00 00              
   525 : add    [rsp+#0xfff8], #0x8888  ; 48 81 84 24 f8 ff 00 00 88 88 00 00  
