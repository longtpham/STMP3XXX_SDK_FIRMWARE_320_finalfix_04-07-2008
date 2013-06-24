;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2002-2003
;
;  File        : componentversion.asm   (was version.asm)
;  Description : Firmware component version major and minor words
;///////////////////////////////////////////////////////////////////////////////

    section ComponentVersion
                                            
    GLOBAL VERSION_MAJOR
    GLOBAL VERSION_MIDDLE
    GLOBAL VERSION_MINOR

    GLOBAL Fg_wVersionMajor
    GLOBAL Fg_wVersionMiddle
    GLOBAL Fg_wVersionMinor

    GLOBAL g_wVersionMajor
    GLOBAL g_wVersionMiddle
    GLOBAL g_wVersionMinor
        
    include "resource.inc"
    
    ORG     y,"ComponentVersion_y":

Fg_wVersionMajor
g_wVersionMajor     DC  VERSION_MAJOR

Fg_wVersionMiddle
g_wVersionMiddle     DC  VERSION_MIDDLE


Fg_wVersionMinor
g_wVersionMinor     DC  VERSION_MINOR

    endsec
