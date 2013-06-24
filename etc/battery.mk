#first error check (only one battery type allowed)
ifdef D_BATTERY_LI_ION
include $(ETC_PATH)\liion.mk
else
ifdef D_BATTERY_NIMH
include $(ETC_PATH)\nimh.mk
else
ifdef D_BATTERY_ALKALINE
include $(ETC_PATH)\alkaline.mk
else
#this is the default
D_BATTERY_ALKALINE = TRUE
include $(ETC_PATH)\alkaline.mk
endif
endif
endif
