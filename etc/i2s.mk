HWEQUCOMMON = $(HWEQUCOMMON)\
            $(OUTPUT_INCLUDE_PATH)\regscd.h\

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\i2s_dma_receive.obj

"$(OBJ_PATH)\i2s_dma_receive.obj" : $(DD_PATH)\Audio\i2s\i2s_dma_receive.c
"$(OBJ_PATH)\i2s_dma_receive.obj" : $(DD_PATH)\Audio\i2s\i2s_dma_receive.h


