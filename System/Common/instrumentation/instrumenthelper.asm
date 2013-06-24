    include "sysmacro.asm"
    

    global PushAandB    
    global PopAandB
    global PushA
    global PopA

    org p,".ptextinstrumenthelper":
    
PushAandB    
    push a2
    push a1
    push a0
    push b2
    push b1
    push b0
    rts
    
PushA
    push a2
    push a1
    push a0
    rts    
    
PopAandB
    pop b0
    pop b1
    pop b2
PopA
    pop a0
    pop a1
    pop a2
    rts