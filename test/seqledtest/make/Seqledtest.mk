#########################################
# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
#########################################

PROJ                = Seqledtest

# relative pointer to SDK root directory \Mp3BaseCc
RELBASE_PATH        = ..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\test\$(PROJ)
PROJSUPPORT_PATH    = $(ROOT)\test\testsupport
PROJSYS_PATH        = $(PROJSUPPORT_PATH)\system
PROJRSRC_PATH       = $(PROJSUPPORT_PATH)\rsrc
HWEQU_H             = $(OUTPUT_INCLUDE_PATH)\hwequ.h

#########################################
include $(ROOT)\etc\standard.mk
#########################################

#########################################
include $(ROOT)\etc\registers.mk
#########################################

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL=-I$(PROJRSRC_PATH);$(PROJ_PATH);$(PROJSYS_PATH);$(OUTPUT_PATH);$(INC_ALL);$(PROJSUPPORT_PATH)
INC_ALL_C = -Wc-I$(PROJ_PATH) -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH) -Wc-I$(PROJSUPPORT_PATH)

### $(BLD) is environment variable; set in batch file
AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD) 
LK_OPTS= -L$(MP3_PATH)\tasking -L$(BIN_DRIVE)\2410\lib\2410 -w8
CC_OPTS= -Wc-si $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL)

HWEQUCOMMON = $(HWEQUCOMMON)\
            $(OUTPUT_INCLUDE_PATH)\hwequ.h\
            $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h\
            $(OUTPUT_INCLUDE_PATH)\regscore.h\
            $(OUTPUT_INCLUDE_PATH)\regscodec.h\
            $(OUTPUT_INCLUDE_PATH)\regsdcdc.h\
            $(OUTPUT_INCLUDE_PATH)\regsemc.h\
            $(OUTPUT_INCLUDE_PATH)\regsgpio.h\
            $(OUTPUT_INCLUDE_PATH)\regsi2c.h\
            $(OUTPUT_INCLUDE_PATH)\regsi2s.h\
            $(OUTPUT_INCLUDE_PATH)\regsicoll.h\
            $(OUTPUT_INCLUDE_PATH)\regslradc.h\
            $(OUTPUT_INCLUDE_PATH)\regsrevision.h\
            $(OUTPUT_INCLUDE_PATH)\regsrtc.h\
            $(OUTPUT_INCLUDE_PATH)\regssdram.h\
            $(OUTPUT_INCLUDE_PATH)\regsspare.h\
            $(OUTPUT_INCLUDE_PATH)\regsspi.h\
            $(OUTPUT_INCLUDE_PATH)\regsswizzle.h\
            $(OUTPUT_INCLUDE_PATH)\regstb.h\
            $(OUTPUT_INCLUDE_PATH)\regstimer.h\

#Create hwequ list based on device
ifdef D_D3500
HWEQULIST = $(HWEQULIST)\
            $(HWEQUCOMMON)\
            $(OUTPUT_INCLUDE_PATH)\regsecc.h\
            $(OUTPUT_INCLUDE_PATH)\regsgpflash.h\
            $(OUTPUT_INCLUDE_PATH)\regsfilco.h\
            $(OUTPUT_INCLUDE_PATH)\regspwm.h\
            $(OUTPUT_INCLUDE_PATH)\regsusb20.h\
            $(OUTPUT_INCLUDE_PATH)\regsusb20phy.h
else
#3410 list
HWEQULIST = $(HWEQULIST)\
            $(HWEQUCOMMON)\
            $(OUTPUT_INCLUDE_PATH)\regsusb.h
endif

OBJLIST =   $(OBJ_PATH)\sysmem.obj\
            $(OBJ_PATH)\sysmisc.obj\
            $(OBJ_PATH)\systime.obj\
            $(OBJ_PATH)\systimers.obj\
            $(OBJ_PATH)\syspll.obj\
            $(OBJ_PATH)\bufdesc.obj\
            $(OBJ_PATH)\stack.obj\
            $(OBJ_PATH)\stackmem.obj\
            $(OBJ_PATH)\const.obj\
            $(OBJ_PATH)\led.obj\
            $(OBJ_PATH)\ledtable.obj\
            $(OBJ_PATH)\modulemsg.obj\
            $(OBJ_PATH)\seqledtest.obj\
            $(OBJ_PATH)\syspostmsg.obj\
            $(OBJ_PATH)\utilasm.obj\
            $(OBJ_PATH)\divide.obj\
            $(OBJ_PATH)\incr.obj\
            $(OBJ_PATH)\sysmodule4test.obj\
            $(OBJ_PATH)\resourcey.obj\

RESOURCELIST = $(RESOURCELIST)\

ALL : $(OUTPUT_PATH)\resource.asm $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).s  

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : $(CPU_PATH)\testdflt.dsc $(HWEQULIST) $(OBJLIST) $(PROJ).mk
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) 

"$(OUTPUT_PATH)\$(PROJ).abs"            : "$(OUTPUT_PATH)\$(PROJ).out" $(CPU_PATH)\testdflt.dsc 
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////
$(OUTPUT_PATH)\resource.asm : $(PROJ_PATH)\resource.inc
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(PROJ_PATH)\resource.inc \
    -i$(ROOT)\fonts\iso8859\
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(RESOURCE_PATH)\shingyih\New_Type_LCD \
    -i$(OUTPUT_RSRC_PATH) -i$(PROJRSRC_PATH) \
    -o$(OUTPUT_PATH)\resource.bin \
    -v$(VERSION) -a 
        @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt

#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\sysmem.obj : $(SYS_COMMON_PATH)\sysmem.asm

$(OBJ_PATH)\sysmisc.obj : $(SYS_COMMON_PATH)\sysmisc.asm

$(OBJ_PATH)\systime.obj : $(SYS_COMMON_PATH)\systime.asm

$(OBJ_PATH)\divide.obj : "$(PROJSYS_PATH)\divide.asm"

$(OBJ_PATH)\incr.obj : "$(PROJSYS_PATH)\incr.asm"

$(OBJ_PATH)\systimers.obj : $(SYS_COMMON_PATH)\systimers.asm

$(OBJ_PATH)\syspll.obj : $(SYS_COMMON_PATH)\syspll.asm

$(OBJ_PATH)\bufdesc.obj : $(SYS_COMMON_PATH)\bufdesc.asm

$(OBJ_PATH)\sysmodule4test.obj : $(SYS_EXEC_PATH)\sysmodule4test.asm

$(OBJ_PATH)\stack.obj : $(SYS_PATH)\data_management\stack\stack.asm

$(OBJ_PATH)\stackmem.obj : $(SYS_COMMON_PATH)\stackmem.asm

$(OBJ_PATH)\modulemsg.obj : $(PROJ_PATH)\modulemsg.asm

$(OBJ_PATH)\utilasm.obj : $(PROJSYS_PATH)\utilasm.asm 

$(OBJ_PATH)\resourcey.obj : $(PROJSYS_PATH)\resourcey.asm

"$(OUTPUT_INCLUDE_PATH)\systimers.h":$(PROJSUPPORT_PATH)\systimers.inc 
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

"$(OUTPUT_INCLUDE_PATH)\messages.h":$(INC_PATH)\msgequ.inc 
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\errors.h :$(INC_PATH)\errors.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

"$(OBJ_PATH)\seqledtest.obj" : $(PROJ_PATH)\Seqledtest.c
"$(OBJ_PATH)\seqledtest.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h
"$(OBJ_PATH)\seqledtest.obj" : "$(OUTPUT_INCLUDE_PATH)\systimers.h" 
"$(OBJ_PATH)\seqledtest.obj" : "$(PROJSYS_PATH)\syspostmsg.xref"
"$(OBJ_PATH)\seqledtest.obj" : $(SYS_COMMON_PATH)\systime.asm
"$(OBJ_PATH)\seqledtest.obj" : $(XREF_PATH)\stackmem.xref
"$(OBJ_PATH)\seqledtest.obj" : $(XREF_PATH)\led.xref
"$(OBJ_PATH)\seqledtest.obj" : $(XREF_PATH)\systime.xref
"$(OBJ_PATH)\seqledtest.obj" : $(PROJ_PATH)\Seqledtest.h
"$(OBJ_PATH)\seqledtest.obj" : $(HW_MSGMODULES_PATH)\led\led.asm

$(OBJ_PATH)\const.obj : $(SYS_PATH)\data_management\const.asm

$(OBJ_PATH)\led.obj : $(HW_MSGMODULES_PATH)\led\led.asm

"$(OBJ_PATH)\ledtable.obj" : $(PROJ_PATH)\ledtable.asm

"$(OBJ_PATH)\syspostmsg.obj" : "$(SYS_COMMON_PATH)\syspostmsg.asm"

$(HWEQU_H) : $(HW_REGS_INC_PATH)\hwequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

#///////////////////////////////////////////////////////////////////////////////
# Module code extraction
#///////////////////////////////////////////////////////////////////////////////

"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=P_clstr,X_clstr,Y_clstr
        copy /y $(OUTPUT_PATH)\$(PROJ).s $(OUTPUT_PATH)\stmpsys.s
        @$(BIN_PATH)\stosbcon $(OUTPUT_PATH)\stmpsys.s $(OUTPUT_PATH)\stmpsys.sb

