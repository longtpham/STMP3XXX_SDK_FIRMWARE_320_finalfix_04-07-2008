#This is the makefile for the FAT12 filesystem.
#
#This file should be included into the project makefile and 

FAT12_PATH= $(ROOT)\FileSystem\Fat12
FSMAC_PATH=$(FAT12_PATH)\macro;$(FAT12_PATH)\directory\macro
FSLIB_PATH  =$(FAT12_PATH)\lib
DDI_PATH     =$(ROOT)\devicedriver
FS_PATH     =$(FAT12_PATH)\filesystem
FSDIR_PATH  =$(FS_PATH)\directory\include

INC_FS=$(FSMAC_PATH);$(FSDIR_PATH);$(FS_PATH);$(FSLIB_PATH);$(FAT12_PATH);$(FS_PATH)\handle\macro;$(FSLIB_PATH)\include
INC_ALL=$(INC_ALL);$(INC_FS)


ifdef D_LIBS
ifdef D_FUNCLETS
FAT12LIB  = $(OBJ_PATH)\fat12lib_funclets.a
else
FAT12LIB  = $(OBJ_PATH)\fat12lib.a
endif
else
ifdef D_FUNCLETS
FAT12LIB  = $(LIB_PATH)\fat12lib_funclets.a
else
FAT12LIB  = $(LIB_PATH)\fat12lib.a
endif
endif

FILESYSLIB   = $(FILESYSLIB)\
            $(FAT12LIB)

FAT12LIST = $(FAT12_LIB)\
            $(FAT12_READ)\
            $(FAT12_WRITE)\
            $(PARTITION_MAP)
            
MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\fatwritep.src\
             $(OUTPUT_RSRC_PATH)\fatwritex.src\
             $(OUTPUT_RSRC_PATH)\fatwritey.src

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\fat12_resource.obj \
    $(OBJ_PATH)\dir_x.obj\
    $(OBJ_PATH)\dir_y.obj\
    $(OBJ_PATH)\fatdef_x.obj\
    $(OBJ_PATH)\fatdef_y.obj\
    $(OBJ_PATH)\databuffer_x.obj\
    $(OBJ_PATH)\fcb_y.obj\
    $(OBJ_PATH)\hndldef_y.obj\
    $(OBJ_PATH)\devicedef_y.obj\
    $(OBJ_PATH)\mtdef_y.obj\
  
#    $(OBJ_PATH)\fcbdef_y.obj\


#///////////////////////////////////////////////////////////////////////////////
# FAT12 File System
#///////////////////////////////////////////////////////////////////////////////
PARTITION_MAP = $(PARTITION_MAP)\
        $(OBJ_PATH)\fsddwrapper.obj\
        $(OBJ_PATH)\fsddread.obj\
        $(OBJ_PATH)\fsdatadrivegetsize.obj\
        $(OBJ_PATH)\fsdatadrivegetserialnumber.obj\
        $(OBJ_PATH)\fsddwrite.obj\
        $(OBJ_PATH)\fsdatadriveinit.obj\
        $(OBJ_PATH)\fsgetdatadrivepbslocation.obj\
        $(OBJ_PATH)\fspartitionentrycopy.obj


#        $(OBJ_PATH)\fsddinit.obj\
        


FAT12_COMMON_DEPENDANCIES= \
    "$(INC_PATH)\ddildl.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"\
  "$(FAT12_PATH)\filesystem.inc" "$(FAT12_PATH)\errors.inc" "$(FAT12_PATH)\SystemDefs.inc" \
  "$(FAT12_PATH)\filesystem\FSDefs.inc"  "$(FAT12_PATH)\lib\include\libdefs.inc" "$(FAT12_PATH)\filesystem\directory\include\directorydef.inc" \
  "$(SYS_COMMON_PATH)\sysmacro.asm" "$(INC_PATH)\sysequ.inc" "$(XREF_PATH)\const.xref" "$(INC_PATH)\sysequ.inc" \
   $(FAT12_PATH)\macro\convertmemtopointer.asm $(FAT12_PATH)\macro\decm.asm    $(FAT12_PATH)\macro\decr.asm    \
   $(FAT12_PATH)\macro\decxy.asm       $(FAT12_PATH)\macro\div24p2.asm \
   $(FAT12_PATH)\macro\endofchain.asm      $(FAT12_PATH)\macro\getb0.asm   \
   $(FAT12_PATH)\macro\getb0s.asm      $(FAT12_PATH)\macro\getb1.asm   \
   $(FAT12_PATH)\macro\getb1s.asm      $(FAT12_PATH)\macro\getb2.asm   \
   $(FAT12_PATH)\macro\getb2s.asm      $(FAT12_PATH)\macro\getbasedevice.asm   \
   $(FAT12_PATH)\macro\getbyte.asm     $(FAT12_PATH)\macro\getdevicevalue.asm  \
   $(FAT12_PATH)\macro\getpagesize.asm     $(FAT12_PATH)\macro\getpointeraddress.asm   \
   $(FAT12_PATH)\macro\getpointermemoryspace.asm       $(FAT12_PATH)\macro\gets0.asm   \
   $(FAT12_PATH)\macro\gets1x.asm      $(FAT12_PATH)\macro\gets2.asm   \
   $(FAT12_PATH)\macro\incm.asm        $(FAT12_PATH)\macro\incr.asm    \
   $(FAT12_PATH)\macro\incxy.asm       $(FAT12_PATH)\macro\isbigdev.asm    \
   $(FAT12_PATH)\macro\mod24p2.asm     $(FAT12_PATH)\macro\putbyte.asm \
   $(FAT12_PATH)\macro\setdevicevalue.asm      $(FAT12_PATH)\macro\setpointeraddress.asm   \
   $(FAT12_PATH)\macro\setpointermemoryspace.asm       $(FAT12_PATH)\macro\submem.asm  \
   $(FAT12_PATH)\filesystem\handle\macro\getdev.asm        $(FAT12_PATH)\filesystem\handle\macro\getmode.asm   \
   $(FAT12_PATH)\filesystem\handle\macro\getsect.asm       $(FAT12_PATH)\filesystem\handle\macro\setdev.asm    \
   $(FAT12_PATH)\filesystem\handle\macro\setmode.asm       $(FAT12_PATH)\filesystem\handle\macro\setsect.asm   \
   $(FAT12_PATH)\filesystem\fat12\macro\cachenext.asm      $(FAT12_PATH)\filesystem\fat12\macro\cacheprev.asm  \
   $(FAT12_PATH)\filesystem\fat12\macro\clearfat.asm       $(FAT12_PATH)\filesystem\fat12\macro\curruptfat.asm \
   $(FAT12_PATH)\filesystem\fat12\macro\decrcxcount.asm        $(FAT12_PATH)\filesystem\fat12\macro\incrcxcount.asm    \
   $(FAT12_PATH)\filesystem\fat12\macro\loaddevvar.asm     $(FAT12_PATH)\filesystem\fat12\macro\nextcluster.asm    \
   $(FAT12_PATH)\filesystem\fat12\macro\storedevvar.asm        $(FAT12_PATH)\filesystem\fat12\macro\tstbounds.asm  \
   $(FAT12_PATH)\filesystem\fat12\macro\tstcurrupt.asm     $(FAT12_PATH)\filesystem\fat12\macro\writefatsect.asm   \
   $(FAT12_PATH)\filesystem\fat12\include\fatdef.inc       $(FAT12_PATH)\filesystem\handle\include\hndldef.inc \
   $(FAT12_PATH)\filesystem\media\include\media.inc        $(FAT12_PATH)\filesystem\api\include\apidef.inc \
   $(FAT12_PATH)\filesystem\api\include\external.inc       $(INC_PATH)\smdddefs.inc \
   $(FAT12_PATH)\filesystem\fsdefs.inc       $(FAT12_PATH)\filesystem\directory\include\directorydef.inc   \
   $(FAT12_PATH)\filesystem\directory\include\search.inc     $(FAT12_PATH)\filesystem\handle\include\fcb_def.inc   \
   $(FAT12_PATH)\lib\include\libdefs.inc     $(FAT12_PATH)\lib\include\stringsdef.inc  


FAT12_LIB= \
    "$(OBJ_PATH)\STRING_X.OBJ"\
    "$(OBJ_PATH)\VAR_y.OBJ"\
    "$(OBJ_PATH)\Word.OBJ"\
    "$(OBJ_PATH)\divide.OBJ"\
    "$(OBJ_PATH)\modulo.OBJ"\
    "$(OBJ_PATH)\Math32.OBJ"\
    "$(OBJ_PATH)\Chrono.OBJ"\
    "$(OBJ_PATH)\WORDputbyte.OBJ"\
    "$(OBJ_PATH)\Pointer.OBJ"\
    "$(OBJ_PATH)\string.OBJ"\
    "$(OBJ_PATH)\FileAndPathString.OBJ"\
    "$(OBJ_PATH)\GetDevVal.OBJ"\
    "$(OBJ_PATH)\GetBaseDev.OBJ"




FAT12_WRITE=        \
    "$(OBJ_PATH)\WRITEbuffer.obj" \
    "$(OBJ_PATH)\CreateDIR.obj" \
    "$(OBJ_PATH)\DeleteDIR.obj" \
    "$(OBJ_PATH)\FileDelete.obj" \
    "$(OBJ_PATH)\FileRename.obj" \
    "$(OBJ_PATH)\FileWrite.obj" \
    "$(OBJ_PATH)\FileAppend.obj" \
    "$(OBJ_PATH)\FileAttribute.obj" \
    "$(OBJ_PATH)\FileAttributeClear.obj" \
    "$(OBJ_PATH)\FileCreate.obj" \
    "$(OBJ_PATH)\DIRAddCluster.obj" \
    "$(OBJ_PATH)\DIREraseCluster.obj" \
    "$(OBJ_PATH)\FCBNew.obj" \
    "$(OBJ_PATH)\FileClose_Write.obj" \
    "$(OBJ_PATH)\chaindelete.obj" \
    "$(OBJ_PATH)\fcballocate.obj" \
    "$(OBJ_PATH)\cluster_write.obj" \
    "$(OBJ_PATH)\fat_write.obj"\
    $(OBJ_PATH)\sysfatwrite.obj
    
FAT12_READ=     \
    "$(OBJ_PATH)\Search_X.obj"\
    "$(OBJ_PATH)\search_y.obj"   \
    "$(OBJ_PATH)\Init.obj"      \
    "$(OBJ_PATH)\STREAMwordcopy.obj"    \
    "$(OBJ_PATH)\STREAMbytecopy.obj"    \
    "$(OBJ_PATH)\GetByteFromStream.obj"  \
    "$(OBJ_PATH)\GetWordFromStream.obj"  \
    "$(OBJ_PATH)\Media.obj"        \
    "$(OBJ_PATH)\CXtoSect.obj"      \
    "$(OBJ_PATH)\handlemacro.obj"         \
    "$(OBJ_PATH)\macro.obj"         \
    "$(OBJ_PATH)\fat.obj"           \
    "$(OBJ_PATH)\cluster.obj"       \
    "$(OBJ_PATH)\chain.obj"         \
    "$(OBJ_PATH)\handle.obj"        \
    "$(OBJ_PATH)\ReadBuffer.obj"    \
    "$(OBJ_PATH)\fcb.obj"           \
    "$(OBJ_PATH)\ReadDIRRecord.obj"   \
    "$(OBJ_PATH)\CacheDIRSector.obj"  \
    "$(OBJ_PATH)\FileSystemSetDirectory.obj" \
    "$(OBJ_PATH)\SearchDirectory.obj"     \
    "$(OBJ_PATH)\ChangeDirectory.obj"  \
    "$(OBJ_PATH)\Path.obj"             \
    "$(OBJ_PATH)\SaveFCB.obj"         \
    "$(OBJ_PATH)\RestoreFCB.obj"       \
    "$(OBJ_PATH)\FCBGetFields.obj"     \
    "$(OBJ_PATH)\FCBSetFields.obj"     \
    "$(OBJ_PATH)\FCBPopulate.obj"      \
    "$(OBJ_PATH)\FCBStoreToDir.obj"        \
    "$(OBJ_PATH)\fileseek.obj"         \
    "$(OBJ_PATH)\fileread.obj"         \
    "$(OBJ_PATH)\fileeof.obj"          \
    "$(OBJ_PATH)\fsutils.obj"          \
    "$(OBJ_PATH)\fileopen.obj"         \
    "$(OBJ_PATH)\fileclose.obj"        \
    "$(OBJ_PATH)\updatefilesystem.obj"\
    "$(OBJ_PATH)\filesystemstructs.obj" \
    "$(OBJ_PATH)\ftell.obj"



###### BUILD FAT LIB ###########################################################
# Link against library because not all functions are used  
#   in all projects. By using the lib only the functions used will be 
#   included in the build  
################################################################################
#///////////////////////////////////////////////////////////////////////////////
# Archive build : libraries (*.a) from object modules (*.obj)
#///////////////////////////////////////////////////////////////////////////////
ifdef D_LIBS
"$(FAT12LIB)"            : $(FAT12LIST) $(ETC_PATH)\fat12.mk
    @echo Creating IEEE archive library file for FAT12 File System $@
        @$(AR_EXE) -r $@ $(match .obj $!) $(FAT12_LIB) $(FAT12_READ) $(FAT12_WRITE)

#///////////////////////////////////////////////////////////////////////////////
# FAT12 Files
#///////////////////////////////////////////////////////////////////////////////
#FAT12_LIB
"$(OBJ_PATH)\STRING_X.OBJ"       :  "$(FSLIB_PATH)\X_MEMORY\STRING_X.ASM" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\VAR_y.OBJ"          :  "$(FSLIB_PATH)\Y_MEMORY\VAR_y.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\Word.OBJ"           :  "$(FSLIB_PATH)\ASM\Word.Asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\divide.OBJ"         :  "$(FSLIB_PATH)\ASM\divide.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\modulo.OBJ"         :  "$(FSLIB_PATH)\ASM\modulo.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\Math32.OBJ"         :  "$(FSLIB_PATH)\ASM\Math32.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\Chrono.OBJ"         :  "$(FSLIB_PATH)\ASM\Chrono.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\GetWordByte.OBJ"    :  "$(FSLIB_PATH)\ASM\GetWordByte.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\WORDputbyte.OBJ"    :  "$(FSLIB_PATH)\ASM\WORDputbyte.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\Pointer.OBJ"        :  "$(FSLIB_PATH)\ASM\Pointer.ASM" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\string.OBJ"         :  "$(FSLIB_PATH)\ASM\string.Asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\FileAndPathString.OBJ":"$(FSLIB_PATH)\ASM\FileAndPathString.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\GetDevVal.OBJ"      :  "$(FSLIB_PATH)\ASM\GetDevVal.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\GetBaseDev.OBJ"     :  "$(FSLIB_PATH)\ASM\GetBaseDev.asm" $(FAT12_COMMON_DEPENDANCIES)



#FAT12_WRITE
"$(OBJ_PATH)\WRITEbuffer.obj"        : "$(FAT12_PATH)\filesystem\HANDLE\ASM\WRITEbuffer.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\CreateDIR.obj"          : "$(FAT12_PATH)\filesystem\API\ASM\CreateDIR.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\DeleteDIR.obj"          : "$(FAT12_PATH)\filesystem\API\ASM\DeleteDIR.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileDelete.obj"         : "$(FAT12_PATH)\filesystem\API\ASM\FileDelete.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileRename.obj"         : "$(FAT12_PATH)\filesystem\API\ASM\FileRename.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileWrite.obj"          : "$(FAT12_PATH)\filesystem\API\ASM\FileWrite.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileAppend.obj"         : "$(FAT12_PATH)\filesystem\API\ASM\FileAppend.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileAttribute.obj"      : "$(FAT12_PATH)\filesystem\API\ASM\FileAttribute.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileAttributeClear.obj" : "$(FAT12_PATH)\filesystem\API\ASM\FileAttributeClear.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileCreate.obj"         : "$(FAT12_PATH)\filesystem\API\ASM\FileCreate.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\DIRAddCluster.obj"      : "$(FAT12_PATH)\filesystem\Directory\ASM\DIRAddCluster.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\DIREraseCluster.obj"    : "$(FAT12_PATH)\filesystem\Directory\ASM\DIREraseCluster.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FCBNew.obj"             : "$(FAT12_PATH)\filesystem\handle\ASM\FCBNew.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileClose_Write.obj"    : "$(FAT12_PATH)\filesystem\API\ASM\FileClose_Write.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\chaindelete.obj"        : "$(FAT12_PATH)\filesystem\FAT12\ASM\chaindelete.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fcballocate.obj"        : "$(FAT12_PATH)\filesystem\HANDLE\ASM\fcballocate.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\cluster_write.obj"      : "$(FAT12_PATH)\filesystem\FAT12\ASM\cluster_write.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fat_write.obj"          : "$(FAT12_PATH)\filesystem\FAT12\ASM\fat_write.asm" $(FAT12_COMMON_DEPENDANCIES)


#FAT12READRead
"$(OBJ_PATH)\Search_X.obj"              :"$(FAT12_PATH)\filesystem\Directory\X_Memory\Search_X.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\search_y.obj"              :"$(FAT12_PATH)\filesystem\directory\y_memory\search_y.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\Init.obj"                  :"$(FAT12_PATH)\filesystem\Init.Asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\STREAMwordcopy.obj"        :"$(FAT12_PATH)\filesystem\Stream\ASM\STREAMwordcopy.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\STREAMbytecopy.obj"        :"$(FAT12_PATH)\filesystem\Stream\ASM\STREAMbytecopy.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\GetByteFromStream.obj"     :"$(FAT12_PATH)\filesystem\Stream\ASM\GetByteFromStream.asm"  $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\GetWordFromStream.obj"     :"$(FAT12_PATH)\filesystem\Stream\ASM\GetWordFromStream.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\Media.obj"                 :"$(FAT12_PATH)\filesystem\MEDIA\ASM\Media.Asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\CXtoSect.obj"              :"$(FAT12_PATH)\filesystem\FAT12\ASM\CXtoSect.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\macro.obj"                 :"$(FAT12_PATH)\filesystem\FAT12\ASM\macro.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fat.obj"                   :"$(FAT12_PATH)\filesystem\FAT12\ASM\fat.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\cluster.obj"               :"$(FAT12_PATH)\filesystem\FAT12\ASM\cluster.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\chain.obj"                 :"$(FAT12_PATH)\filesystem\FAT12\ASM\chain.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\handle.obj"                :"$(FAT12_PATH)\filesystem\HANDLE\ASM\handle.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\handlemacro.obj"           :"$(FAT12_PATH)\filesystem\HANDLE\ASM\handlemacro.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fcb.obj"                   :"$(FAT12_PATH)\filesystem\HANDLE\ASM\fcb.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\ReadBuffer.obj"            :"$(FAT12_PATH)\filesystem\HANDLE\ASM\READbuffer.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\SaveFCB.obj"               :"$(FAT12_PATH)\filesystem\HANDLE\ASM\SaveFCB.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\RestoreFCB.obj"            :"$(FAT12_PATH)\filesystem\HANDLE\ASM\RestoreFCB.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FCBGetFields.obj"          :"$(FAT12_PATH)\filesystem\HANDLE\ASM\FCBGetFields.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FCBSetFields.obj"          :"$(FAT12_PATH)\filesystem\HANDLE\ASM\FCBSetFields.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FCBPopulate.obj"           :"$(FAT12_PATH)\filesystem\HANDLE\ASM\FCBPopulate.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FCBStoreToDir.obj"         :"$(FAT12_PATH)\filesystem\HANDLE\ASM\FCBStoreToDIR.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\ReadDIRRecord.obj"         :"$(FAT12_PATH)\filesystem\Directory\ASM\ReadDIRRecord.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\CacheDIRSector.obj"        :"$(FAT12_PATH)\filesystem\Directory\ASM\CacheDIRSector.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\FileSystemSetDirectory.obj" :"$(FAT12_PATH)\filesystem\Directory\ASM\FileSystemSetDirectory.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\SearchDirectory.obj"       :"$(FAT12_PATH)\filesystem\Directory\ASM\SearchDirectory.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\ChangeDirectory.obj"       :"$(FAT12_PATH)\filesystem\Directory\ASM\ChangeDirectory.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\Path.obj"                  :"$(FAT12_PATH)\filesystem\Directory\ASM\Path.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fileseek.obj"              :"$(FAT12_PATH)\filesystem\API\ASM\FileSeek.asm"      $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fileread.obj"              :"$(FAT12_PATH)\filesystem\API\ASM\FileRead.asm"      $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fileeof.obj"               :"$(FAT12_PATH)\filesystem\API\ASM\FileEOF.asm"       $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fsutils.obj"               :"$(FAT12_PATH)\filesystem\API\ASM\FSutils.asm"       $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fileopen.obj"              :"$(FAT12_PATH)\filesystem\API\ASM\FileOpen.asm"      $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\fileclose.obj"             :"$(FAT12_PATH)\filesystem\API\ASM\FileClose.asm"     $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\mediadetection.obj"        :"$(SYS_COMMON_PATH)\mediadetection.c" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\updatefilesystem.obj"      :"$(FS_PATH)\updatefilesystem.asm" $(FAT12_COMMON_DEPENDANCIES)
"$(OBJ_PATH)\filesystemstructs.obj"     :"$(FAT12_PATH)\filesystemstructs.c" $(FAT12_COMMON_DEPENDANCIES) $(MAIN_DEPENDENCIES)

$(OBJ_PATH)\ftell.obj                   : $(FAT12_PATH)\filesystem\API\ASM\ftell.asm


$(OBJ_PATH)\sysfatwrite.obj             : $(SYS_COMMON_PATH)\sysfatwrite.asm 


#partition mapper
$(OBJ_PATH)\fsddwrapper.obj    :$(FAT12_PATH)\filesystem\fsddwrapper.asm

$(OBJ_PATH)\fsddread.obj       :$(FAT12_PATH)\filesystem\fsddread.c
$(OBJ_PATH)\fsdatadrivegetsize.obj :$(FAT12_PATH)\filesystem\fsdatadrivegetsize.c
$(OBJ_PATH)\fsdatadrivegetserialnumber.obj : $(FAT12_PATH)\filesystem\fsdatadrivegetserialnumber.c

$(OBJ_PATH)\fsddwrite.obj      :$(FAT12_PATH)\filesystem\fsddwrite.c

#$(OBJ_PATH)\fsddinit.obj       :$(FAT12_PATH)\filesystem\fsddinit.c

$(OBJ_PATH)\fsdatadriveinit.obj       :$(FAT12_PATH)\filesystem\fsdatadriveinit.c
$(OBJ_PATH)\fsgetdatadrivepbslocation.obj       :$(FAT12_PATH)\filesystem\fsgetdatadrivepbslocation.c
$(OBJ_PATH)\fspartitionentrycopy.obj       :$(FAT12_PATH)\filesystem\fspartitionentrycopy.c

endif 

"$(OUTPUT_RSRC_PATH)\fatwritex.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fatwritey.src" : $(OUTPUT_PATH)\$(PROJ).abs

ifdef D_MMC
"$(OUTPUT_RSRC_PATH)\fatwritep.src" : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwritemmc.loc
        @echo Extract WriteFAT12 P
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

else
"$(OUTPUT_RSRC_PATH)\fatwritep.src" : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwrite.loc
        @echo Extract WriteFAT12 P
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

endif

$(OBJ_PATH)\fat12_resource.obj : $(SYS_COMMON_PATH)\symbols\fat12_resource.asm

"$(OBJ_PATH)\DIR_X.obj"                 :"$(FAT12_PATH)\filesystem\Directory\X_Memory\DIR_X.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\dir_y.obj"                 :"$(FAT12_PATH)\filesystem\directory\y_memory\dir_y.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\fatdef_x.obj"              :"$(FAT12_PATH)\filesystem\FAT12\X_MEMORY\fatdef_x.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\fatdef_y.obj"              :"$(FAT12_PATH)\filesystem\fat12\y_memory\fatdef_y.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\DataBuffer_x.obj"          :"$(FAT12_PATH)\filesystem\HANDLE\X_Memory\DataBuffer_x.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\fcb_y.obj"                 :"$(FAT12_PATH)\filesystem\handle\y_memory\fcb_y.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\hndldef_y.obj"             :"$(FAT12_PATH)\filesystem\handle\y_memory\hndldef_y.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\devicedef_y.obj"           :"$(FAT12_PATH)\filesystem\media\y_memory\devicedef_y.asm" $(FAT12_COMMON_DEPENDANCIES)

"$(OBJ_PATH)\mtdef_y.obj"               :"$(FAT12_PATH)\filesystem\media\y_memory\mtdef_y.asm" $(FAT12_COMMON_DEPENDANCIES)
