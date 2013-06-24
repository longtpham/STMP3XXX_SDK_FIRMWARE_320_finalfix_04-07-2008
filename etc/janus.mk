#This is the makefile for the Janus.
#
#This file should be included into the project makefile and 


GENERALLIB = $(GENERALLIB)\
			 $(SYSLIBLIST) 


ifdef D_JANUS


SYSLIBLIST = $(JANUS_LIB_PATH)\janus.a\
			$(JANUS_LIB_PATH)\janus_runtime.a\
			$(JANUS_LIB_PATH)\janussystem.a\
			



endif          

ifdef D_JANUS

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\DoPlay_resources.obj \ 
		  $(OBJ_PATH)\pkcrypto_resource.obj\
          $(OBJ_PATH)\janus_resource.obj\
          $(OBJ_PATH)\oemgroupcert.obj \
          $(OBJ_PATH)\oem_data.obj
          
else
OBJLIST = $(OBJLIST)\          
          $(OBJ_PATH)\janus_resource.obj
          
endif   



MODULELIST = $(MODULELIST)\
             $(JANUSMODULERSRCLIST)

                        
JANUSMODULERSRCLIST=    $(OUTPUT_RSRC_PATH)\pkcrypto_p.src\                     
                        $(OUTPUT_RSRC_PATH)\pkcrypto_ovl_p.src\
                        $(OUTPUT_RSRC_PATH)\doplay_p.src\
                        $(OUTPUT_RSRC_PATH)\janus_tables.src\
                        $(OUTPUT_RSRC_PATH)\janus_constants.src\
                        $(OUTPUT_RSRC_PATH)\janus_xmem.src\
                        $(OUTPUT_RSRC_PATH)\januscommon.src\
                        $(OUTPUT_RSRC_PATH)\januswmasupport.src\
                        $(OUTPUT_RSRC_PATH)\aes_enc.src\
                        $(OUTPUT_RSRC_PATH)\aes_dec_table.src\
                        $(OUTPUT_RSRC_PATH)\aes_key_table.src\
                        $(OUTPUT_RSRC_PATH)\aes_enc_table.src\
                        $(OUTPUT_RSRC_PATH)\const_pkcrypto.src\
                        $(OUTPUT_RSRC_PATH)\const_y.src\
                        $(OUTPUT_RSRC_PATH)\janusy_data.src\
                        $(OUTPUT_RSRC_PATH)\devcert_template.src\
                        $(OUTPUT_RSRC_PATH)\device_cert.src\
                        $(OUTPUT_RSRC_PATH)\janusp.src\
                        $(OUTPUT_RSRC_PATH)\janusx.src\
						$(OUTPUT_RSRC_PATH)\oem_data.src\
						$(OUTPUT_RSRC_PATH)\gpk_p.src\
						$(OUTPUT_RSRC_PATH)\key_data.src\
						$(OUTPUT_RSRC_PATH)\hdsimplcommon.src\
						$(OUTPUT_RSRC_PATH)\hdsimpl_p.src


ifdef D_JANUS


"$(OUTPUT_RSRC_PATH)\pkcrypto_p.src"      : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\pkcrypto_ovl_p.src"      : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\pkcrypto_section.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)


"$(OUTPUT_RSRC_PATH)\doplay_p.src"        : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\hdsimplcommon.src"        : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\hdsimpl_p.src"        : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\doplay_section.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\janus_tables.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janus_constants.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janus_xmem.src"                      : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\const_pkcrypto.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\const_y.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janusy_data.src"                     : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\devcert_template.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\device_cert.src"                     : $(OUTPUT_PATH)\$(PROJ).abs


        @echo Extract Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\Janus_Mem.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\januscommon.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\januswmasupport.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract WMA Decoder Sections"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\januscommon_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

"$(OUTPUT_RSRC_PATH)\aes_enc.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\aes_dec_table.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\aes_key_table.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\aes_enc_table.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract WMA Decoder Sections"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\aes_enc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
        
"$(OUTPUT_RSRC_PATH)\janusp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\janusx.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_data.src" 						  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\gpk_p.src" 						  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\key_data.src" 						  : $(OUTPUT_PATH)\$(PROJ).abs


        @echo "Extract MP3 Enc X & MP3 Enc Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\janus_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
        
        

else

"$(OUTPUT_RSRC_PATH)\pkcrypto_p.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\pkcrypto_p.src /Y
"$(OUTPUT_RSRC_PATH)\pkcrypto_ovl_p.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\pkcrypto_ovl_p.src /Y
"$(OUTPUT_RSRC_PATH)\oem_data.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\oem_data.src /Y
"$(OUTPUT_RSRC_PATH)\gpk_p.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\gpk_p.src /Y
"$(OUTPUT_RSRC_PATH)\key_data.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\key_data.src /Y
"$(OUTPUT_RSRC_PATH)\doplay_p.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\doplay_p.src /Y
"$(OUTPUT_RSRC_PATH)\janus_tables.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\janus_tables.src /Y
"$(OUTPUT_RSRC_PATH)\janus_constants.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\janus_constants.src /Y
"$(OUTPUT_RSRC_PATH)\janus_xmem.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\janus_xmem.src /Y
"$(OUTPUT_RSRC_PATH)\januscommon.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\januscommon.src /Y
"$(OUTPUT_RSRC_PATH)\januswmasupport.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\januswmasupport.src /Y
"$(OUTPUT_RSRC_PATH)\janusp.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\janusp.src /Y
"$(OUTPUT_RSRC_PATH)\janusx.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\janusx.src /Y
"$(OUTPUT_RSRC_PATH)\aes_enc.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\aes_enc.src /Y
"$(OUTPUT_RSRC_PATH)\aes_dec_table.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\aes_dec_table.src /Y
"$(OUTPUT_RSRC_PATH)\aes_key_table.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\aes_key_table.src /Y
"$(OUTPUT_RSRC_PATH)\aes_enc_table.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\aes_enc_table.src /Y
"$(OUTPUT_RSRC_PATH)\const_pkcrypto.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\const_pkcrypto.src /Y
"$(OUTPUT_RSRC_PATH)\const_y.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\const_y.src /Y
"$(OUTPUT_RSRC_PATH)\janusy_data.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\janusy_data.src /Y
"$(OUTPUT_RSRC_PATH)\devcert_template.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\devcert_template.src /Y
"$(OUTPUT_RSRC_PATH)\device_cert.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\device_cert.src /Y
"$(OUTPUT_RSRC_PATH)\hdsimplcommon.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\hdsimplcommon.src /Y
"$(OUTPUT_RSRC_PATH)\hdsimpl_p.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\hdsimpl_p.src /Y
endif

#///////////////////////////////////////////////////////////////////////////////
# janus support file
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\DoPlay_resources.obj"  : $(SYS_COMMON_PATH)\symbols\DoPlay_resources.asm
"$(OBJ_PATH)\pkcrypto_resource.obj"  : $(SYS_COMMON_PATH)\symbols\pkcrypto_resource.asm
"$(OBJ_PATH)\oemgroupcert.obj"  : $(DRM_PATH)\oemgroupcert.asm
"$(OBJ_PATH)\janus_resource.obj" : $(SYS_COMMON_PATH)\symbols\janus_resource.asm 
"$(OBJ_PATH)\oem_data.obj"			: $(OUTPUT_RSRC_PATH)\oem_data.c



