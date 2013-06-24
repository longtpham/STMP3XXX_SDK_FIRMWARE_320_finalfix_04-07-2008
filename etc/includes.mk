#includes.mk

ifndef INCLUDES_MK
INCLUDES_MK= TRUE



ifndef __CONVERT_MESSAGES_H
__CONVERT_MESSAGES_H=TRUE
ifdef STMP_BUILD_PLAYER 
$(OUTPUT_INCLUDE_PATH)\messages.h :$(INC_PATH)\msgequ.inc
else
$(OUTPUT_INCLUDE_PATH)\messages.h :$(INC_PATH)\messages.inc
endif
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __RESOURCE_H
__RESOURCE_H=TRUE
$(OUTPUT_INCLUDE_PATH)\resource.h :$(OUTPUT_INCLUDE_PATH)\resource.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef CONVERT_PROJECT_H
CONVERT_PROJECT_H=TRUE
$(OUTPUT_INCLUDE_PATH)\project.h :$(MAIN_PATH)\project.inc 
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __ERRORS_H
__ERRORS_H=TRUE
$(OUTPUT_INCLUDE_PATH)\errors.h : $(INC_PATH)\errors.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef __SYSEQU_H
__SYSEQU_H=TRUE
$(OUTPUT_INCLUDE_PATH)\sysequ.h : $(INC_PATH)\sysequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

endif 