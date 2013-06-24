#This is the makefile for the filesystem portion to be used in the DCC
#
#This file should be included into the project makefile

FAT12_PATH= $(ROOT)\FileSystem\Fat12
FSMAC_PATH=$(FAT12_PATH)\macro;$(FAT12_PATH)\directory\macro
FSLIB_PATH  =$(FAT12_PATH)\lib
FS_PATH     =$(FAT12_PATH)\filesystem
FSDIR_PATH  =$(FS_PATH)\directory\include

INC_FS=$(FSMAC_PATH);$(FSDIR_PATH);$(FS_PATH);$(FSLIB_PATH);$(FAT12_PATH);$(FS_PATH)\handle\macro;$(FSLIB_PATH)\include
INC_ALL=$(INC_ALL);$(INC_FS)

FILESYSTEM_DCC_PATH = $(ROOT)\dcc\FileSystem

FAT12LIST = $(FAT12_LIB) $(FAT12_READ)

#///////////////////////////////////////////////////////////////////////////////
# FAT12 File System
#///////////////////////////////////////////////////////////////////////////////

FAT12_LIB= \
    "$(OBJ_PATH)\WORDputbyte.OBJ"\
    "$(OBJ_PATH)\modulo.obj"\
    "$(OBJ_PATH)\divide.obj"
    
    
    
FAT12_READ= \
    "$(OBJ_PATH)\Init.obj"\ 
    "$(OBJ_PATH)\MediaMapper.obj"      \
       

OBJLIST=$(OBJLIST)\
    $(FAT12_LIB)\
    $(FAT12_READ)



#///////////////////////////////////////////////////////////////////////////////
# FAT12 Files
#///////////////////////////////////////////////////////////////////////////////
#FAT12_LIB
"$(OBJ_PATH)\GetWordByte.OBJ"    :  "$(FSLIB_PATH)\ASM\GetWordByte.asm" 
"$(OBJ_PATH)\WORDputbyte.OBJ"    :  "$(FSLIB_PATH)\ASM\WORDputbyte.asm"
"$(OBJ_PATH)\modulo.obj"         :  "$(FSLIB_PATH)\ASM\modulo.asm" 
"$(OBJ_PATH)\divide.obj"         :  "$(FSLIB_PATH)\ASM\divide.asm" 
"$(OBJ_PATH)\MediaMapper.obj"    :"$(FAT12_PATH)\filesystem\MediaMapper.Asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\Init.obj"         :  "$(FILESYSTEM_DCC_PATH)\init.asm"

