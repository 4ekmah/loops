fixed_power_9(i0)
     0 : mov  rdx, rcx ; 48 89 ca     
     1 : imul rdx, rcx ; 48 0f af d1  
     2 : imul rdx, rdx ; 48 0f af d2  
     3 : imul rdx, rdx ; 48 0f af d2  
     4 : imul rcx, rdx ; 48 0f af ca  
     5 : mov  rax, rcx ; 48 89 c8     
     6 : ret           ; c3           
