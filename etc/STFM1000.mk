#file stfm1000.mk
#
MODULELIST= $(MODULELIST)\
            $(OUTPUT_RSRC_PATH)\TunerModule.src
            
#ifndef TUNER_STFM1000 
#AS_OPTS=$(AS_OPTS) -DTUNER_STFM1000
#CC_OPTS=$(CC_OPTS) -DTUNER_STFM1000
#endif

RESOURCELIST = $(RESOURCELIST)\
               $(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src


FUNCLETSRCLIST=$(FUNCLETSRCLIST)\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src\
             $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src\
             $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src\
             $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src\
             $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src\
             $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src\
            $(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\STFM1000.obj\
          $(OBJ_PATH)\TunerModule.obj\
          $(OBJ_PATH)\STFM1000PowerDown.obj 
                    
          

"$(OBJ_PATH)\STFM1000.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000.c
"$(OBJ_PATH)\STFM1000.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000.h
"$(OBJ_PATH)\STFM1000.obj" : $(HW_MSGMODULES_PATH)\Tuner\tunerdriver.h
"$(OBJ_PATH)\STFM1000.obj" : $(OUTPUT_INCLUDE_PATH)\project.h

"$(OBJ_PATH)\STFM1000PowerDown.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000PowerDown.c
"$(OBJ_PATH)\STFM1000PowerDown.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000.h
"$(OBJ_PATH)\STFM1000PowerDown.obj" : $(HW_MSGMODULES_PATH)\Tuner\tunerdriver.h
"$(OBJ_PATH)\STFM1000PowerDown.obj" : $(OUTPUT_INCLUDE_PATH)\project.h


"$(OBJ_PATH)\tunermodule.obj" : $(HW_MSGMODULES_PATH)\Tuner\tunermodule.c
"$(OBJ_PATH)\tunermodule.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000.h
"$(OBJ_PATH)\tunermodule.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h

"$(OUTPUT_RSRC_PATH)\TunerModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract TunerModule 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptexttunermodule,.ptextstfm1000 -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract STFM1000NonOverlay 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.pSTFM1000_NO_OVERLAY_P -f$(OUTPUT_PATH)\$(PROJ).map

######################################################
# Create the resource for funclets
######################################################        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverInit 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_INIT_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000RegPowerUp1 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_REG_POWER_UP1_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000RegPowerUp2 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_REG_POWER_UP2_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000FindRssiThreshold 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_FIND_RSSI_THRESHOLD_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000DataPathOff 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_DATA_PATH_OFF_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000ContinueSearching 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_CONTINUE_SEARCHING_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverIncUp 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_INC_UP_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverIncDown 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_INC_DOWN_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverStandyby 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_SET_STANDBY_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverReplacePreset 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_REPLACE_PRESET_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverErasePreset 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_ERASE_PRESET_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverTuneToPreset 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_TUNE_TO_PRESET_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverSearchDown 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_SEARCH_DOWN_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000SafePowerDown 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_SAFE_POWER_DOWN_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000OptimizeChannel 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_TUNER_OPTIMIZE_CHANNEL_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000I2CInit 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_I2C_INIT_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverScanStations 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_SCAN_STATION_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverSetMonoStereoMode
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_TUNER_DRIVER_SET_MONO_STEREO_MODE_P -f$(OUTPUT_PATH)\$(PROJ).map
 
#"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000SetLnaBias.src"          : $(OUTPUT_PATH)\$(PROJ).abs
#        @echo Extract Funclet_STFM1000SetLnaBias  
#        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_SET_LNA_BIAS_P -f$(OUTPUT_PATH)\$(PROJ).map
 
#"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000LookForPilot.src"          : $(OUTPUT_PATH)\$(PROJ).abs
#        @echo Extract Funclet_STFM1000LookForPilot  
#        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_LOOK_FOR_PILOT_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src"          : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_STFM1000TunerSearchEnded  
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_TUNER_SEARCH_ENDED_P -f$(OUTPUT_PATH)\$(PROJ).map
         
