    include "project.inc"

    org y,".yconstresource":

    global VERSION_MAJOR
    global VERSION_MIDDLE
    global VERSION_MINOR
    
VERSION_MAJOR           equ         2
VERSION_MIDDLE          equ         0
VERSION_MINOR           equ         313

ResourceStart:
    include "resource.asm"
ResourceStop:
