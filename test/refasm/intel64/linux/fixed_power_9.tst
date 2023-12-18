fixed_power_9(i0)
     0 : mov  rsi, rdi ; 48 89 fe     
     1 : imul rsi, rdi ; 48 0f af f7  
     2 : imul rsi, rsi ; 48 0f af f6  
     3 : imul rsi, rsi ; 48 0f af f6  
     4 : imul rdi, rsi ; 48 0f af fe  
     5 : mov  rax, rdi ; 48 89 f8     
     6 : ret           ; c3           
