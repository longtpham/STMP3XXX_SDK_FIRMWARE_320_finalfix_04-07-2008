MODULELIST= $(MODULELIST)\
            $(OUTPUT_RSRC_PATH)\TunerModule.src

RESOURCELIST = $(RESOURCELIST)\
               $(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src
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
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSetRegion.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\Philips5757.obj\
          $(OBJ_PATH)\TunerModule.obj

"$(OBJ_PATH)\Philips5757.obj" : $(HW_MSGMODULES_PATH)\Tuner\Philips5757\Philips5757.c
"$(OBJ_PATH)\Philips5757.obj" : $(HW_MSGMODULES_PATH)\Tuner\Philips5757\Philips5757.h
"$(OBJ_PATH)\Philips5757.obj" : $(HW_MSGMODULES_PATH)\Tuner\tunerdriver.h

"$(OBJ_PATH)\tunermodule.obj" : $(HW_MSGMODULES_PATH)\Tuner\tunermodule.c
"$(OBJ_PATH)\tunermodule.obj" : $(HW_MSGMODULES_PATH)\Tuner\Philips5757\Philips5757.h
"$(OBJ_PATH)\tunermodule.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h

"$(OUTPUT_RSRC_PATH)\TunerModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract TunerModule 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptexttunermodule,.ptextphilips5757 -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract STFM1000NonOverlay 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src
#### create dummy resources so there are no changes to resource.inc 
"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerDriverInit 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000RegPowerUp1 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000RegPowerUp2 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000FindRssiThreshold 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000DataPathOff 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerScanStations.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerScanStations 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerScanStations.src


"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetToPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetToPreset 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSetToPreset.src

        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetMonoStereo.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetMonoStereo 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSetMonoStereo.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetStandby.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetStandby 
       copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSetStandby.src
       
"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetPreset 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSetPreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerErasePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerErasePreset 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerErasePreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetIf.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetIf 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSetIf.src
"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSTFM1000ContinueSearching 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src

        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerWriteIfOSc.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerWriteIfOSc 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerWriteIfOSc.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerGetFieldStrength.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerGetFieldStrength 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerGetFieldStrength.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerInitRfCapOsc.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerInitRfCapOsc 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerInitRfCapOsc.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmStation.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerStoreFmStation 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmStation.src
        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerStoreFmPreset 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerStoreFmPreset.src
        
"$(OUTPUT_RSRC_PATH)\Funclet_TunerSetRegion.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_TunerSetRegion 
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSetRegion.src
        
