    include "project.inc"

    org y,".yconstresource":

    global VERSION_MAJOR
    global VERSION_MINOR
    
VERSION_MAJOR           equ         1
VERSION_MINOR           equ         100

ResourceStart:
    include "resource.asm"
ResourceStop:
