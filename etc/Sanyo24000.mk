MODULELIST= $(MODULELIST)\
            $(OUTPUT_RSRC_PATH)\TunerModule.src

FUNCLETSRCLIST=$(FUNCLETSRCLIST)\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerScanStations.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSetToPreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSetMonoStereo.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSetStandby.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSetPreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerErasePreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSetIf.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerWriteIfOSc.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerGetFieldStrength.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerInitRfCapOsc.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmStation.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmPreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSetRegion.src            

AS_OPTS=$(AS_OPTS) -DTUNER_LV24000
CC_OPTS=$(CC_OPTS) -DTUNER_LV24000

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\Sanyo24000.obj\
          $(OBJ_PATH)\TunerModule.obj

"$(OBJ_PATH)\Sanyo24000.obj" : $(HW_MSGMODULES_PATH)\Tuner\Sanyo24000\Sanyo24000.c
"$(OBJ_PATH)\Sanyo24000.obj" : $(HW_MSGMODULES_PATH)\Tuner\Sanyo24000\Sanyo24000.h
"$(OBJ_PATH)\Sanyo24000.obj" : $(HW_MSGMODULES_PATH)\Tuner\tunerdriver.h
"$(OBJ_PATH)\Sanyo24000.obj" : $(OUTPUT_INCLUDE_PATH)\project.h

"$(OBJ_PATH)\tunermodule.obj" : $(HW_MSGMODULES_PATH)\Tuner\tunermodule.c
"$(OBJ_PATH)\tunermodule.obj" : $(HW_MSGMODULES_PATH)\Tuner\Sanyo24000\Sanyo24000.h
"$(OBJ_PATH)\tunermodule.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h

######################################################
# Create reource for tuner module
######################################################
"$(OUTPUT_RSRC_PATH)\TunerModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract TunerModule 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptexttunermodule,.ptextsanyo24000 -f$(OUTPUT_PATH)\$(PROJ).map

######################################################
# Create the resource for funclets
######################################################        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverInit 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_INIT_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerScanStations.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerScanStations 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SCAN_STATIONS_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetToPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetToPreset 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SET_TO_PRESET_P -f$(OUTPUT_PATH)\$(PROJ).map
        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetMonoStereo.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetMonoStereo 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SET_MONO_STEREO_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetStandby.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetStandby 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SET_STANDBY_P -f$(OUTPUT_PATH)\$(PROJ).map
        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetPreset 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SET_PRESET_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerErasePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerErasePreset 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_ERASE_PRESET_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetIf.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetIf 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SET_IF_P -f$(OUTPUT_PATH)\$(PROJ).map
        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerWriteIfOSc.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerWriteIfOSc 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_WRITE_IF_OSC_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerGetFieldStrength.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerGetFieldStrength 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_GET_FIELD_STRENGTH_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerInitRfCapOsc.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerInitRfCapOsc 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_INIT_RF_CAP_OSC_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmStation.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerStoreFmStation 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_STORE_FM_STATION_P -f$(OUTPUT_PATH)\$(PROJ).map
        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerStoreFmPreset 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_STORE_FM_PRESET_P -f$(OUTPUT_PATH)\$(PROJ).map
        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetRegion.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetRegion 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SET_REGION_P -f$(OUTPUT_PATH)\$(PROJ).map
        
