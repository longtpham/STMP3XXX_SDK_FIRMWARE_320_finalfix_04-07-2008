#This is the makefile for the Janus.
#
#This file should be included into the project makefile and 


GENERALLIB = $(GENERALLIB) $(SYSLIBLIST)

SYSLIBLIST = $(JANUS_LIB_PATH)\janusmtp.a\
			 $(JANUS_LIB_PATH)\janusmtp_runtime.a\
			 $(JANUS_LIB_PATH)\mtpsystem.a\



OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\mtp_system_resource.obj\
		  $(OBJ_PATH)\janus_resource.obj\
		  $(OBJ_PATH)\oemgroupcert.obj


#///////////////////////////////////////////////////////////////////////////////
# janus support file
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\mtp_system_resource.obj" : $(SYS_COMMON_PATH)\symbols\mtp_system_resource.asm 
"$(OBJ_PATH)\janus_resource.obj" : $(SYS_COMMON_PATH)\symbols\janus_resource.asm 
"$(OBJ_PATH)\oemgroupcert.obj"  : $(DRM_PATH)\oemgroupcert.asm





