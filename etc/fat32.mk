#This is the makefile for the FAT32 filesystem.
#
#This file should be included into the project makefile and

FAT32_PATH= $(ROOT)\FileSystem\Fat32
FSLIB_PATH  =$(FAT32_PATH)\lib
JANUS_PATH  = $(ROOT)\Algorithms\DRM\Janus\System
INC_FS = $(FAT32_PATH)\h
INC_ALL=$(INC_ALL);$(INC_FS)
#INC_ALL_C = $(INC_ALL_C) -Wc-I$(INC_FS)

ifndef PLAYER
#include the perf enhancement switch for all non player FAT32 FS builds. Player had mem limits.
# Since standard.mk does not use C_ASM_OPTS directly, & since they are included into CC_OPTS  
# in several apps, we currently only need to define this for CC_OPTS (avoids redef warnings).
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-Dk_opt_performance_enhancement
CC_OPTS=$(CC_OPTS) -Wc-Dk_opt_performance_enhancement
endif

# TOVERIFY TODO: which apps would eventually get this? player, non player, mtp, tests, etc?
CC_OPTS=$(CC_OPTS) -Wc-Dk_opt_single_fat
# May be enabled later but more dev to do since 3.05 development has assumed 2 fat tables so 
# rework of JJN's code is needed before enabling single FAT there. This define was moved here from player.mk 
# and mtp.mk but not tested while being defined here yet in any sdk.

# Line removed since no MMC/SD. This is imported from application mk files but not tested when defined from here.
# This is not needed unless MMC/SD.
# CC_OPTS=$(CC_OPTS) -Wc-Dk_opt_dynamic_cache



CMP_PATH = $(FAT32_PATH)\cmp
CMP_STDTYPE_PATH = $(CMP_PATH)\cmp_stdtype
CMP_DEBUG_PATH = $(CMP_PATH)\cmp_debug
CMP_FILE_SYSTEM_PATH = $(CMP_PATH)\cmp_file_system
CMP_MEDIA_NAND_PATH = $(ROOT)\DeviceDriver\Media\cmp\cmp_media_nand
NAND_HAL_PATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\include

INC_ALL_C =\
  $(INC_ALL_C)\
  -Wc-I$(INC_FS)\
  -Wc-I$(CMP_STDTYPE_PATH)\
  -Wc-I$(CMP_DEBUG_PATH)\
  -Wc-I$(CMP_FILE_SYSTEM_PATH)\
  -Wc-I$(CMP_MEDIA_NAND_PATH)\
  -Wc-I$(NAND_HAL_PATH)


ifdef D_PLIBS
ifdef D_FUNCLETS
FAT32LIB = $(OBJ_PATH)\fat32lib_funclets.a
else
FAT32LIB = $(OBJ_PATH)\fat32lib.a
endif
else
ifdef D_FUNCLETS
FAT32LIB = $(LIB_PATH)\fat32lib_funclets.a
else
FAT32LIB = $(LIB_PATH)\fat32lib.a
endif
endif

ifndef MTP_BUILD
MODULELIST =\
  $(MODULELIST)\
  $(OUTPUT_RSRC_PATH)\fatwritep.src
else
MODULELIST =\
  $(MODULELIST)\
  $(OUTPUT_RSRC_PATH)\fatwritep.src\
  $(OUTPUT_RSRC_PATH)\writep.src\
  $(OUTPUT_RSRC_PATH)\mtpbootinit.src
endif  

FAT32LIST =\
  $(OUTPUT_PATH)\OBJ\cacheutil.obj \
  $(OUTPUT_PATH)\OBJ\readsector.obj \
  $(OUTPUT_PATH)\OBJ\writesector.obj \
  $(OUTPUT_PATH)\OBJ\flushsector.obj \
  $(OUTPUT_PATH)\OBJ\platform.obj\
  $(OUTPUT_PATH)\OBJ\Fsinit.obj\
			$(OUTPUT_PATH)\OBJ\fsdriveinit.obj\
			$(OUTPUT_PATH)\OBJ\setcwdhandle.obj\
			$(OUTPUT_PATH)\OBJ\fsclearBuf.obj\
  $(OUTPUT_PATH)\OBJ\FsShutdown.obj\
  $(OUTPUT_PATH)\OBJ\Handle.obj\
  $(OUTPUT_PATH)\OBJ\HandleRes.obj\
  $(OUTPUT_PATH)\OBJ\FgetFastHandle.obj\
  $(OUTPUT_PATH)\OBJ\Fread.obj\
  $(OUTPUT_PATH)\OBJ\Fseek.obj\
  $(OUTPUT_PATH)\OBJ\Fwrite.obj\
  $(OUTPUT_PATH)\OBJ\Fcreate.obj\
  $(OUTPUT_PATH)\OBJ\ArrangeFileName.obj\
  $(OUTPUT_PATH)\OBJ\IsShortNameValid.obj\
  $(OUTPUT_PATH)\OBJ\DiscardTrailigPeriods.obj\
  $(OUTPUT_PATH)\OBJ\Getchar.obj\
  $(OUTPUT_PATH)\OBJ\Putchar.obj\
  $(OUTPUT_PATH)\OBJ\Convert_itoa.obj\
  $(OUTPUT_PATH)\OBJ\ArrangeLongFileName.obj\
  $(OUTPUT_PATH)\OBJ\Setshortfilename.obj\
  $(OUTPUT_PATH)\OBJ\CreateShortDirRecord.obj\
  $(OUTPUT_PATH)\OBJ\ClearCluster.obj\
  $(OUTPUT_PATH)\OBJ\filegetattrib.obj\
  $(OUTPUT_PATH)\OBJ\filesetattrib.obj\
  $(OUTPUT_PATH)\OBJ\filesetdate.obj\
  $(OUTPUT_PATH)\OBJ\filegetdate.obj\
  $(OUTPUT_PATH)\OBJ\CreateDirectory.obj\
  $(OUTPUT_PATH)\OBJ\FastOpen.obj\
  $(OUTPUT_PATH)\OBJ\getshortfilename.obj\
  $(OUTPUT_PATH)\OBJ\GetFileSize.obj\
  $(OUTPUT_PATH)\OBJ\UpdateFileSize.obj\
  $(OUTPUT_PATH)\OBJ\Fflush.obj\
  $(OUTPUT_PATH)\OBJ\ReadRootdirRecord.obj\
  $(OUTPUT_PATH)\OBJ\DeleteRecord.obj\
  $(OUTPUT_PATH)\OBJ\DeleteContent.obj\
  $(OUTPUT_PATH)\OBJ\CheckspaceinRootdir.obj\
  $(OUTPUT_PATH)\OBJ\FATSector.obj\
  $(OUTPUT_PATH)\OBJ\FileSystempresent.obj\
  $(OUTPUT_PATH)\OBJ\FindNextCluster.obj\
  $(OUTPUT_PATH)\OBJ\FindnextFreecurrentcluster.obj\
  $(OUTPUT_PATH)\OBJ\FirstfreeAndallocate.obj\
  $(OUTPUT_PATH)\OBJ\Firstsectorofcluster.obj\
  $(OUTPUT_PATH)\OBJ\GetNewcluster.obj\
  $(OUTPUT_PATH)\OBJ\ReadFAT12entry.obj\
  $(OUTPUT_PATH)\OBJ\ReadFATentry.obj\
  $(OUTPUT_PATH)\OBJ\Totalfreecluster.obj\
  $(OUTPUT_PATH)\OBJ\WriteFATentry.obj\
  $(OUTPUT_PATH)\OBJ\Readdevicerecord.obj\
  $(OUTPUT_PATH)\OBJ\Changepath.obj\
  $(OUTPUT_PATH)\OBJ\Chdir.obj\
  $(OUTPUT_PATH)\OBJ\FindFirst.obj\
  $(OUTPUT_PATH)\OBJ\Findnext.obj\
  $(OUTPUT_PATH)\OBJ\Fopen.obj\
  $(OUTPUT_PATH)\OBJ\Fopenw.obj\
  $(OUTPUT_PATH)\OBJ\getvolumelabel.obj\
  $(OUTPUT_PATH)\OBJ\setvolumelabel.obj\
  $(OUTPUT_PATH)\OBJ\Fremove.obj\
  $(OUTPUT_PATH)\OBJ\FremoveW.obj\
  $(OUTPUT_PATH)\OBJ\Ftell.obj\
  $(OUTPUT_PATH)\OBJ\Feof.obj\
  $(OUTPUT_PATH)\OBJ\GetCwd.obj\
  $(OUTPUT_PATH)\OBJ\MkDIr.obj\
  $(OUTPUT_PATH)\OBJ\MkDIrw.obj\
  $(OUTPUT_PATH)\OBJ\longfilename.obj\
  $(OUTPUT_PATH)\OBJ\RmDIr.obj\
  $(OUTPUT_PATH)\OBJ\RmDIrW.obj\
  $(OUTPUT_PATH)\OBJ\SearchDirectory.obj\
  $(OUTPUT_PATH)\OBJ\SetHandleforsearch.obj\
  $(OUTPUT_PATH)\OBJ\CreateDirectoryRecord.obj\
  $(OUTPUT_PATH)\OBJ\Extractfilename.obj\
  $(OUTPUT_PATH)\OBJ\ExtractPath.obj\
  $(OUTPUT_PATH)\OBJ\FindfreeRecord.obj\
  $(OUTPUT_PATH)\OBJ\Strlength.obj\
  $(OUTPUT_PATH)\OBJ\IsDirectoryEmpty.obj\
  $(OUTPUT_PATH)\OBJ\Shortdirmatch.obj\
  $(OUTPUT_PATH)\OBJ\Longdirmatch.obj\
  $(OUTPUT_PATH)\OBJ\SetCwd.obj\
  $(OUTPUT_PATH)\OBJ\Fstrcpy.obj\
  $(OUTPUT_PATH)\OBJ\GetName.obj\
  $(OUTPUT_PATH)\OBJ\MatchdirRecord.obj\
  $(OUTPUT_PATH)\OBJ\FindNextSector.obj\
  $(OUTPUT_PATH)\OBJ\Fgetc.obj\
  $(OUTPUT_PATH)\OBJ\Fgets.obj\
  $(OUTPUT_PATH)\OBJ\Fputc.obj\
  $(OUTPUT_PATH)\OBJ\Fputs.obj\
  $(OUTPUT_PATH)\OBJ\Fclose.obj\
  $(OUTPUT_PATH)\OBJ\ReadDirectoryRecord.obj\
  $(OUTPUT_PATH)\OBJ\ChangeToRootdirectory.obj\
  $(OUTPUT_PATH)\OBJ\DeleteTree.obj\
  $(OUTPUT_PATH)\OBJ\Computefreeclusters.obj\
  $(OUTPUT_PATH)\OBJ\strcpyUCS3_2.obj\
  $(OUTPUT_PATH)\OBJ\fsddinit.obj\
  $(OUTPUT_PATH)\OBJ\fsddread.obj\
  $(OUTPUT_PATH)\OBJ\fsddwrite.obj\
  $(OUTPUT_PATH)\OBJ\mtpauth.obj\

OBJLIST = \
  $(OBJLIST)\
  $(OBJ_PATH)\fat32_resource.obj


#///////////////////////////////////////////////////////////////////////////////
# FAT32 File System
#///////////////////////////////////////////////////////////////////////////////
FILESYSLIB =\
  $(FILESYSLIB)\
  $(FAT32LIB)

###### BUILD FAT LIB ###########################################################
# Link against library because not all functions are used
#   in all projects. By using the lib only the functions used will be
#   included in the build
################################################################################
#///////////////////////////////////////////////////////////////////////////////
# Archive build : libraries (*.a) from object modules (*.obj)
#///////////////////////////////////////////////////////////////////////////////
ifdef D_PLIBS
"$(FAT32LIB)": $(FAT32LIST) $(ETC_PATH)\fat32.mk
  @echo Creating IEEE archive library file for FAT32 File System $@
  @$(AR_EXE) -r $@ $(match .obj $!)

#///////////////////////////////////////////////////////////////////////////////
# FAT32 Files
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\OBJ\platform.obj": $(FAT32_PATH)\platform\platform.asm
"$(OUTPUT_PATH)\OBJ\cacheutil.obj": $(FAT32_PATH)\cache\cacheutil.c $(FAT32_PATH)\cache\cacheutil.h
"$(OUTPUT_PATH)\OBJ\readsector.obj": $(FAT32_PATH)\cache\readsector.c $(FAT32_PATH)\cache\cacheutil.h
"$(OUTPUT_PATH)\OBJ\writesector.obj": $(FAT32_PATH)\cache\writesector.c $(FAT32_PATH)\cache\cacheutil.h
"$(OUTPUT_PATH)\OBJ\flushsector.obj": $(FAT32_PATH)\cache\flushsector.c  $(OUTPUT_INCLUDE_PATH)\errors.h
"$(OUTPUT_PATH)\OBJ\Fread.obj": $(FAT32_PATH)\Base\Fread.c
"$(OUTPUT_PATH)\OBJ\Fwrite.obj": $(FAT32_PATH)\Base\Fwrite.c
"$(OUTPUT_PATH)\OBJ\Fseek.obj": $(FAT32_PATH)\Base\Fseek.c
"$(OUTPUT_PATH)\OBJ\GetFileSize.obj": $(FAT32_PATH)\Base\GetFileSize.c
"$(OUTPUT_PATH)\OBJ\UpdateFileSize.obj": $(FAT32_PATH)\Base\UpdateFileSize.c
"$(OUTPUT_PATH)\OBJ\Fflush.obj": $(FAT32_PATH)\Base\Fflush.c
"$(OUTPUT_PATH)\OBJ\Fcreate.obj": $(FAT32_PATH)\Base\Fcreate.c
"$(OUTPUT_PATH)\OBJ\CreateDirectory.obj": $(FAT32_PATH)\Base\CreateDirectory.c
"$(OUTPUT_PATH)\OBJ\FastOpen.obj": $(FAT32_PATH)\Base\FastOpen.c
"$(OUTPUT_PATH)\OBJ\getshortfilename.obj": $(FAT32_PATH)\Base\getshortfilename.c
"$(OUTPUT_PATH)\OBJ\ReadRootdirRecord.obj": $(FAT32_PATH)\Base\ReadRootdirRecord.c
"$(OUTPUT_PATH)\OBJ\ArrangeFileName.obj": $(FAT32_PATH)\Base\ArrangeFileName.c
"$(OUTPUT_PATH)\OBJ\IsShortNameValid.obj": $(FAT32_PATH)\Utility\IsShortNameValid.c
"$(OUTPUT_PATH)\OBJ\mtpauth.obj": $(FAT32_PATH)\Utility\mtpauth.c
"$(OUTPUT_PATH)\OBJ\DiscardTrailigPeriods.obj": $(FAT32_PATH)\Utility\DiscardTrailigPeriods.c
"$(OUTPUT_PATH)\OBJ\Getchar.obj": $(FAT32_PATH)\Utility\Getchar.c
"$(OUTPUT_PATH)\OBJ\Putchar.obj": $(FAT32_PATH)\Utility\Putchar.c
"$(OUTPUT_PATH)\OBJ\Convert_itoa.obj": $(FAT32_PATH)\Utility\Convert_itoa.c
"$(OUTPUT_PATH)\OBJ\ArrangeLongFileName.obj": $(FAT32_PATH)\Utility\ArrangeLongFileName.c
"$(OUTPUT_PATH)\OBJ\Setshortfilename.obj": $(FAT32_PATH)\Utility\Setshortfilename.c
"$(OUTPUT_PATH)\OBJ\CreateShortDirRecord.obj": $(FAT32_PATH)\Utility\CreateShortDirRecord.c
"$(OUTPUT_PATH)\OBJ\ClearCluster.obj": $(FAT32_PATH)\Base\ClearCluster.c
"$(OUTPUT_PATH)\OBJ\DeleteContent.obj": $(FAT32_PATH)\Base\DeleteContent.c
"$(OUTPUT_PATH)\OBJ\DeleteRecord.obj": $(FAT32_PATH)\Base\DeleteRecord.c
"$(OUTPUT_PATH)\OBJ\filegetattrib.obj": $(FAT32_PATH)\Base\filegetattrib.c
"$(OUTPUT_PATH)\OBJ\filesetattrib.obj": $(FAT32_PATH)\Base\filesetattrib.c
"$(OUTPUT_PATH)\OBJ\filesetdate.obj": $(FAT32_PATH)\Base\filesetdate.c
"$(OUTPUT_PATH)\OBJ\filegetdate.obj": $(FAT32_PATH)\Base\filegetdate.c
"$(OUTPUT_PATH)\OBJ\Fsinit.obj": $(FAT32_PATH)\Common\Fsinit.c
"$(OUTPUT_PATH)\OBJ\fsdriveinit.obj" 	        : $(FAT32_PATH)\Common\fsdriveinit.c
"$(OUTPUT_PATH)\OBJ\setcwdhandle.obj" 	        : $(FAT32_PATH)\Common\setcwdhandle.c
"$(OUTPUT_PATH)\OBJ\fsclearBuf.obj" 	        : $(FAT32_PATH)\Common\fsclearBuf.c
"$(OUTPUT_PATH)\OBJ\FsShutdown.obj": $(FAT32_PATH)\Common\FsShutdown.c
"$(OUTPUT_PATH)\OBJ\fsddinit.obj": $(FAT32_PATH)\Fsddi\fsddinit.c $(OUTPUT_INCLUDE_PATH)\errors.h
"$(OUTPUT_PATH)\OBJ\fsddread.obj": $(FAT32_PATH)\Fsddi\fsddread.c
"$(OUTPUT_PATH)\OBJ\fsddwrite.obj": $(FAT32_PATH)\Fsddi\fsddwrite.c
"$(OUTPUT_PATH)\OBJ\Handle.obj": $(FAT32_PATH)\Handle\Handle.c
"$(OUTPUT_PATH)\OBJ\HandleRes.obj": $(FAT32_PATH)\Handle\HandleRes.c
"$(OUTPUT_PATH)\OBJ\FgetFastHandle.obj": $(FAT32_PATH)\Handle\FgetFastHandle.c
"$(OUTPUT_PATH)\OBJ\Readdevicerecord.obj": $(FAT32_PATH)\Device\Readdevicerecord.c
"$(OUTPUT_PATH)\OBJ\CheckspaceinRootdir.obj": $(FAT32_PATH)\Fatapi\CheckspaceinRootdir.c
"$(OUTPUT_PATH)\OBJ\FATSector.obj": $(FAT32_PATH)\Fatapi\FATSector.c
"$(OUTPUT_PATH)\OBJ\FileSystempresent.obj": $(FAT32_PATH)\Fatapi\FileSystempresent.c
"$(OUTPUT_PATH)\OBJ\FindNextCluster.obj": $(FAT32_PATH)\Fatapi\FindNextCluster.c
"$(OUTPUT_PATH)\OBJ\FindnextFreecurrentcluster.obj": $(FAT32_PATH)\Fatapi\FindnextFreecurrentcluster.c
"$(OUTPUT_PATH)\OBJ\Firstsectorofcluster.obj": $(FAT32_PATH)\Fatapi\Firstsectorofcluster.c
"$(OUTPUT_PATH)\OBJ\GetNewcluster.obj": $(FAT32_PATH)\Fatapi\GetNewcluster.c
"$(OUTPUT_PATH)\OBJ\FirstfreeAndallocate.obj": $(FAT32_PATH)\Fatapi\FirstfreeAndallocate.c $(OUTPUT_INCLUDE_PATH)\errors.h
"$(OUTPUT_PATH)\OBJ\ReadFATentry.obj": $(FAT32_PATH)\Fatapi\ReadFATentry.c
"$(OUTPUT_PATH)\OBJ\ReadFAT12entry.obj": $(FAT32_PATH)\Fatapi\ReadFAT12entry.c
"$(OUTPUT_PATH)\OBJ\Totalfreecluster.obj": $(FAT32_PATH)\Fatapi\Totalfreecluster.c
"$(OUTPUT_PATH)\OBJ\WriteFATentry.obj": $(FAT32_PATH)\Fatapi\WriteFATentry.c
"$(OUTPUT_PATH)\OBJ\Computefreeclusters.obj": $(FAT32_PATH)\Fatapi\Computefreeclusters.c
"$(OUTPUT_PATH)\OBJ\CreateDirectoryRecord.obj": $(FAT32_PATH)\Utility\CreateDirectoryRecord.c
"$(OUTPUT_PATH)\OBJ\FindfreeRecord.obj": $(FAT32_PATH)\Utility\FindfreeRecord.c
"$(OUTPUT_PATH)\OBJ\Shortdirmatch.obj": $(FAT32_PATH)\Utility\Shortdirmatch.c
"$(OUTPUT_PATH)\OBJ\Longdirmatch.obj": $(FAT32_PATH)\Utility\Longdirmatch.c
"$(OUTPUT_PATH)\OBJ\Extractfilename.obj": $(FAT32_PATH)\Utility\Extractfilename.c
"$(OUTPUT_PATH)\OBJ\ExtractPath.obj": $(FAT32_PATH)\Utility\ExtractPath.c
"$(OUTPUT_PATH)\OBJ\GetName.obj": $(FAT32_PATH)\Utility\GetName.c
"$(OUTPUT_PATH)\OBJ\MatchdirRecord.obj": $(FAT32_PATH)\Utility\MatchdirRecord.c
"$(OUTPUT_PATH)\OBJ\Strlength.obj": $(FAT32_PATH)\Utility\Strlength.c
"$(OUTPUT_PATH)\OBJ\FindNextSector.obj": $(FAT32_PATH)\Utility\FindNextSector.c
"$(OUTPUT_PATH)\OBJ\Fstrcpy.obj": $(FAT32_PATH)\Utility\Fstrcpy.c
"$(OUTPUT_PATH)\OBJ\Fgetc.obj": $(FAT32_PATH)\StandardApi\Fgetc.c
"$(OUTPUT_PATH)\OBJ\Fgets.obj": $(FAT32_PATH)\StandardApi\Fgets.c
"$(OUTPUT_PATH)\OBJ\Fputc.obj": $(FAT32_PATH)\StandardApi\Fputc.c
"$(OUTPUT_PATH)\OBJ\Fputs.obj": $(FAT32_PATH)\StandardApi\Fputs.c
"$(OUTPUT_PATH)\OBJ\Fclose.obj": $(FAT32_PATH)\StandardApi\Fclose.c
"$(OUTPUT_PATH)\OBJ\ReadDirectoryRecord.obj": $(FAT32_PATH)\Standardapi\ReadDirectoryRecord.c
"$(OUTPUT_PATH)\OBJ\SetHandleforsearch.obj": $(FAT32_PATH)\HigherApi\SetHandleforsearch.c
"$(OUTPUT_PATH)\OBJ\Fopen.obj": $(FAT32_PATH)\HigherApi\Fopen.c
"$(OUTPUT_PATH)\OBJ\FopenW.obj": $(FAT32_PATH)\HigherApi\FopenW.c
"$(OUTPUT_PATH)\OBJ\getvolumelabel.obj": $(FAT32_PATH)\HigherApi\getvolumelabel.c
"$(OUTPUT_PATH)\OBJ\setvolumelabel.obj": $(FAT32_PATH)\HigherApi\setvolumelabel.c
"$(OUTPUT_PATH)\OBJ\SearchDirectory.obj": $(FAT32_PATH)\HigherApi\SearchDirectory.c
"$(OUTPUT_PATH)\OBJ\Feof.obj": $(FAT32_PATH)\HigherApi\Feof.c
"$(OUTPUT_PATH)\OBJ\Ftell.obj": $(FAT32_PATH)\HigherApi\Ftell.c
"$(OUTPUT_PATH)\OBJ\FindFirst.obj": $(FAT32_PATH)\HigherApi\FindFirst.c
"$(OUTPUT_PATH)\OBJ\FindNext.obj": $(FAT32_PATH)\HigherApi\FindNext.c
"$(OUTPUT_PATH)\OBJ\Feof.obj": $(FAT32_PATH)\higherapi\Feof.c
"$(OUTPUT_PATH)\OBJ\Ftell.obj": $(FAT32_PATH)\higherapi\Ftell.c
"$(OUTPUT_PATH)\OBJ\Changepath.obj": $(FAT32_PATH)\HigherApi\Changepath.c
"$(OUTPUT_PATH)\OBJ\ChDir.obj": $(FAT32_PATH)\HigherApi\ChDir.c
"$(OUTPUT_PATH)\OBJ\MkDir.obj": $(FAT32_PATH)\higherapi\MkDir.c
"$(OUTPUT_PATH)\OBJ\MkDirw.obj" 	: $(FAT32_PATH)\higherapi\MkDirw.c
"$(OUTPUT_PATH)\OBJ\longfilename.obj": $(FAT32_PATH)\higherapi\longfilename.c
"$(OUTPUT_PATH)\OBJ\RMDir.obj": $(FAT32_PATH)\higherapi\RMDir.c
"$(OUTPUT_PATH)\OBJ\RMDirW.obj": $(FAT32_PATH)\higherapi\RMDirW.c
"$(OUTPUT_PATH)\OBJ\GetCwd.obj": $(FAT32_PATH)\higherapi\GetCwd.c
"$(OUTPUT_PATH)\OBJ\Fremove.obj": $(FAT32_PATH)\higherapi\Fremove.c
"$(OUTPUT_PATH)\OBJ\FremoveW.obj": $(FAT32_PATH)\higherapi\FremoveW.c
"$(OUTPUT_PATH)\OBJ\SetCwd.obj": $(FAT32_PATH)\Utility\SetCwd.c
"$(OUTPUT_PATH)\OBJ\ChangeToRootdirectory.obj": $(FAT32_PATH)\Base\ChangeToRootdirectory.c
"$(OUTPUT_PATH)\OBJ\DeleteTree.obj": $(FAT32_PATH)\HigherApi\DeleteTree.c
"$(OUTPUT_PATH)\OBJ\Isdirectoryempty.obj": $(FAT32_PATH)\Utility\Isdirectoryempty.c
"$(OUTPUT_PATH)\OBJ\strcpyUCS3_2.obj": $(FAT32_PATH)\Utility\strcpyUCS3_2.c
endif 

"$(OBJ_PATH)\fat32_resource.obj": $(SYS_COMMON_PATH)\symbols\fat32_resource.asm

#
# In case a special project file is not used, just use the default PROJ make 
# variable.  This maintains compatibility with many other projects that use
# this makefile.
#    TOVERIFY2   PROJ_FATWRITEP below added by sdk2.6. Verify it is compatible in next 30 lines and in all of sdk.
ifndef PROJ_FATWRITEP
PROJ_FATWRITEP = $(PROJ)
endif

# Note: the *mlc.loc files below are not used in the SDK 2.6xx currently. Instead we use fatwrite.loc and fatwritemmc.loc for mmc build.
ifdef D_MLC
ifdef D_MMC
"$(OUTPUT_RSRC_PATH)\fatwritep.src" : $(OUTPUT_PATH)\$(PROJ_FATWRITEP).abs $(LOC_PATH)\fatwritemmmcmlc.loc
        @echo Extract WriteFAT32 P
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ_FATWRITEP).map -P$(OUTPUT_RSRC_PATH)\

else
"$(OUTPUT_RSRC_PATH)\fatwritep.src" : $(OUTPUT_PATH)\$(PROJ_FATWRITEP).abs $(LOC_PATH)\fatwritemlc.loc
        @echo Extract WriteFAT32 P
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ_FATWRITEP).map -P$(OUTPUT_RSRC_PATH)\

endif 
else
ifndef MTP_BUILD
ifdef D_MMC
"$(OUTPUT_RSRC_PATH)\fatwritep.src": $(OUTPUT_PATH)\$(PROJ_FATWRITEP).abs $(LOC_PATH)\fatwritemmc.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ_FATWRITEP).map -P$(OUTPUT_RSRC_PATH)\

else
"$(OUTPUT_RSRC_PATH)\fatwritep.src": $(OUTPUT_PATH)\$(PROJ_FATWRITEP).abs $(LOC_PATH)\fatwrite.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ_FATWRITEP).map -P$(OUTPUT_RSRC_PATH)\

endif 
else
ifdef D_MMC

"$(OUTPUT_RSRC_PATH)\fatwritep.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwriteMTPmmc.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
  
"$(OUTPUT_RSRC_PATH)\writep.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwriteMTPmmc.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
  
"$(OUTPUT_RSRC_PATH)\mtpbootinit.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwriteMTPmmc.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\


else
"$(OUTPUT_RSRC_PATH)\fatwritep.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwriteMTP.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
  
"$(OUTPUT_RSRC_PATH)\writep.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwriteMTP.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
  
"$(OUTPUT_RSRC_PATH)\mtpbootinit.src": $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\fatwriteMTP.loc
  @echo Extract WriteFAT32 P
  @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

endif 
endif 
endif
