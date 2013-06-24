
OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\FrameBuffer.obj\
          $(OBJ_PATH)\AsmFrameBuffer.obj\


DISPLAY_SAL_SECTIONS=.ptextframebuffer

"$(OBJ_PATH)\framebuffer.obj" : $(HW_MSGMODULES_PATH)\Display\SAL\Framebuffer\framebuffer.c
"$(OBJ_PATH)\framebuffer.obj" : $(DISPLAY_DRIVER_PATH)\displaydriver.h
"$(OBJ_PATH)\framebuffer.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h

"$(OBJ_PATH)\framebuffer.obj" : $(INC_PATH)\sysresourceapi.h
"$(OBJ_PATH)\framebuffer.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h


"$(OBJ_PATH)\asmframebuffer.obj" : $(HW_MSGMODULES_PATH)\Display\SAL\Framebuffer\asmframebuffer.asm
