    include "sysmacro.asm"
    include "const.xref"
    
    extern Fg_RegionTable

    global  Kernel_GetRegion


    org p,".ptextkernel_getregion":


;   passed in the function pointer in R0
;   returns:     the region pointer in R0
;                the region index in a1


Kernel_GetRegion
    move    #Fg_RegionTable,r1      ;load up the region table
    move    y:<Const_ffffff,r4
    move    r0,y0 
    
_top_of_loop
    move    y:(r4)+
    move    y:(r1)+,r0               ;get a pointer to the first region in the table (and advance the pointer to the next)
    move    r0,a 
    tst     a   y:(r0)+
    ccerror eq                      ;if our pointer is 0, then we've reached the end of the region list without finding a hit.
                                    ;if you've hit this error, the function that is being called by syscallfunction is not in 
                                    ;a region and it needs to be.
                                    ;
    move    y:(r0)+,a               ;move the lower address into a
    move    y:(r0)-,b               ;move the higher address into b
    cmp     y0,a   y:(r0)-          ;is it less than the lower address?
    jgt     _top_of_loop
    cmp     y0,b                    ;higher than the top?
    jle     _top_of_loop
    move    r4,a
    rts
    
Kernel_    