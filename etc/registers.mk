#registers.mk

ifndef REGISTERS_MK
REGISTERS_MK= TRUE

ifndef __CONVERT_REGSCD
__CONVERT_REGSCD=TRUE
$(OUTPUT_INCLUDE_PATH)\regscd.h :$(HW_REGS_INC_PATH)\regscd.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSCLKCTRL
__CONVERT_REGSCLKCTRL=TRUE
$(OUTPUT_INCLUDE_PATH)\regsclkctrl.h :$(HW_REGS_INC_PATH)\regsclkctrl.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $(HW_REGS_INC_PATH)\regsclkctrl.inc $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h
endif

ifndef __CONVERT_REGSCODEC
__CONVERT_REGSCODEC=TRUE
$(OUTPUT_INCLUDE_PATH)\regscodec.h :$(HW_REGS_INC_PATH)\regscodec.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSCORE
__CONVERT_REGSCORE=TRUE
$(OUTPUT_INCLUDE_PATH)\regscore.h :$(HW_REGS_INC_PATH)\regscore.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSDCDC
__CONVERT_REGSDCDC=TRUE
$(OUTPUT_INCLUDE_PATH)\regsdcdc.h :$(HW_REGS_INC_PATH)\regsdcdc.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSECC
__CONVERT_REGSECC=TRUE
$(OUTPUT_INCLUDE_PATH)\regsecc.h :$(HW_REGS_INC_PATH)\regsecc.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSEMC
__CONVERT_REGSEMC=TRUE
$(OUTPUT_INCLUDE_PATH)\regsemc.h :$(HW_REGS_INC_PATH)\regsemc.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSESPI
__CONVERT_REGSESPI=TRUE
$(OUTPUT_INCLUDE_PATH)\regsespi.h :$(HW_REGS_INC_PATH)\regsespi.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSFILCO
__CONVERT_REGSFILCO=TRUE
$(OUTPUT_INCLUDE_PATH)\regsfilco.h :$(HW_REGS_INC_PATH)\regsfilco.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSGPFLASH
__CONVERT_REGSGPFLASH=TRUE
$(OUTPUT_INCLUDE_PATH)\regsgpflash.h :$(HW_REGS_INC_PATH)\regsgpflash.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $(HW_REGS_INC_PATH)\regsgpflash.inc $(OUTPUT_INCLUDE_PATH)\regsgpflash.h
endif

ifndef __CONVERT_REGSGPIO
__CONVERT_REGSGPIO=TRUE
$(OUTPUT_INCLUDE_PATH)\regsgpio.h :$(HW_REGS_INC_PATH)\regsgpio.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSI2C
__CONVERT_REGSI2C=TRUE
$(OUTPUT_INCLUDE_PATH)\regsi2c.h :$(HW_REGS_INC_PATH)\regsi2c.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSI2S
__CONVERT_REGSI2S=TRUE
$(OUTPUT_INCLUDE_PATH)\regsi2s.h :$(HW_REGS_INC_PATH)\regsi2s.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSICOLL
__CONVERT_REGSICOLL=TRUE
$(OUTPUT_INCLUDE_PATH)\regsicoll.h :$(HW_REGS_INC_PATH)\regsicoll.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSLRADC
__CONVERT_REGSLRADC=TRUE
$(OUTPUT_INCLUDE_PATH)\regslradc.h :$(HW_REGS_INC_PATH)\regslradc.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSPWM
__CONVERT_REGSPWM=TRUE
$(OUTPUT_INCLUDE_PATH)\regspwm.h :$(HW_REGS_INC_PATH)\regspwm.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSREVISION
__CONVERT_REGSREVISION=TRUE
$(OUTPUT_INCLUDE_PATH)\regsrevision.h :$(HW_REGS_INC_PATH)\regsrevision.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSRTC
__CONVERT_REGSRTC=TRUE
$(OUTPUT_INCLUDE_PATH)\regsrtc.h :$(HW_REGS_INC_PATH)\regsrtc.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSSDRAM
__CONVERT_REGSSDRAM=TRUE
$(OUTPUT_INCLUDE_PATH)\regssdram.h :$(HW_REGS_INC_PATH)\regssdram.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSSPARE
__CONVERT_REGSSPARE=TRUE
$(OUTPUT_INCLUDE_PATH)\regsspare.h :$(HW_REGS_INC_PATH)\regsspare.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSPI
__CONVERT_REGSSPI=TRUE
$(OUTPUT_INCLUDE_PATH)\regsspi.h :$(HW_REGS_INC_PATH)\regsspi.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSSWIZZLE
__CONVERT_REGSSWIZZLE=TRUE
$(OUTPUT_INCLUDE_PATH)\regsswizzle.h :$(HW_REGS_INC_PATH)\regsswizzle.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSSYTEM
__CONVERT_REGSSYSTEM=TRUE
$(OUTPUT_INCLUDE_PATH)\regssystem.h :$(HW_REGS_INC_PATH)\regssystem.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSTB
__CONVERT_REGSTB=TRUE
$(OUTPUT_INCLUDE_PATH)\regstb.h :$(HW_REGS_INC_PATH)\regstb.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSTIMER
__CONVERT_REGSTIMER=TRUE
$(OUTPUT_INCLUDE_PATH)\regstimer.h :$(HW_REGS_INC_PATH)\regstimer.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSUSB
__CONVERT_REGSUSB=TRUE
$(OUTPUT_INCLUDE_PATH)\regsusb.h :$(HW_REGS_INC_PATH)\regsusb.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSUSB20
__CONVERT_REGSUSB20=TRUE
$(OUTPUT_INCLUDE_PATH)\regsusb20.h :$(HW_REGS_INC_PATH)\regsusb20.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __CONVERT_REGSUSB20PHY
__CONVERT_REGSUSB20PHY=TRUE
$(OUTPUT_INCLUDE_PATH)\regsusb20phy.h :$(HW_REGS_INC_PATH)\regsusb20phy.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

endif 