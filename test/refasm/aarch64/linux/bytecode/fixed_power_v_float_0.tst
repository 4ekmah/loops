fixed_power_v_float_0(i0, i1, i2)
     0 : sub            i31, i31, 16   
     1 : spill          0, i25         
     2 : mov            i3, 0          
     3 : mov            i25, 4         
     4 : mul            i2, i2, i25    
     5 : label 0:                      
     6 : cmp            i3, i2         
     7 : jmp_ge 2                      
     8 : vld.fp32       v0, i0, i3     
     9 : mov            i25, 0         
    10 : arm_movk       i25, 16256, 16 
    11 : broadcast.fp32 v0, i25        
    12 : vst.fp32       i1, i3, v0     
    13 : add            i3, i3, 16     
    14 : jmp            0              
    15 : label 2:                      
    16 : mov            i0, 0          
    17 : unspill        i25, 0         
    18 : add            i31, i31, 16   
    19 : ret                           
