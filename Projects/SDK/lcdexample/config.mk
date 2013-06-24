# Filename: config.mk for stmp35xx LCDexample player

# Default Shingyi LCD is changed to a new model June 2005. 
#   If you want to continue using the previous Shingyi LCD that's out of production, make 
#   contrast range changes described in \mp3firmware\resources\shingyih\readme.txt

#Uncomment the following line to enable the USB throughput to be shown while connected to the PC.
#BLD=$(BLD) -DUSB_THROUGHPUT

#Uncomment the following line to for the device to power off for USB Suspend
#BLD=$(BLD) -DUSB_SHUTDOWN_ON_SUSPEND_ENABLE

#Uncomment the following line to force hardware (swizzle) copies when receiving USB data
#BLD=$(BLD) -DUSB_HARDWARE_COPY_FROM_ENDPOINT_BUFFER

#Uncomment the following line to enable the USB SOF interrupt
#BLD=$(BLD) -DUSB_SOF_INTERRUPT

#Uncomment the following line to keep tabs on USB SOF timing
#BLD=$(BLD) -DUSB_MONITOR_SOF_TIMING

#This is needed to use the RTC for system time instead of a timer interrupt.
#BLD=$(BLD) -DUSE_RTC_FOR_TIME


#########   Battery choices ################################################
# If no battery below is chosen, then config will default                  
# to alkaline in battery.mk  Iff you have a STMP3550 or above with internal 
# charger, select battery usbmsc.bat option at build time & select Li-Ion or NiMHY 
# config parameters in battery_config.h. Do not edit this battery section.
ifdef D_LIION
# Chooses LiIon with battery recharge.
D_BATTERY_LI_ION=1

# BATTERY_CHARGE is not needed in Updater anyway.  Removing it saves P memory
ifndef D_UPDATER
BUILD_BATTERY_CHARGE=1
endif  

else
ifdef D_NIMHY
# This is to choose Nickle Metal-Hydride with battery recharge             
D_BATTERY_NIMH=1

# BATTERY_CHARGE is not needed in Updater anyway.  Removing it saves P memory
ifndef D_UPDATER
BUILD_BATTERY_CHARGE=1
endif

endif
endif
# This is to choose Alkaline battery (no recharge). If none chosen, this will default
#D_BATTERY_ALKALINE=1
###########################################################################

################## 5V to DCDC Power Transfer ######################
# 5V to DCDC will be on by default.  Comment out next line to disable.
DCDC_POWER_TRANSFER=1
#
ifdef DCDC_POWER_TRANSFER
BLD=$(BLD) -DDCDC_POWER_TRANSFER
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DDCDC_POWER_TRANSFER
endif
####################################################################

##### BACKLIGHT ############
#uncomment the next line to include the Backlight in the build
BACKLIGHT=TRUE
ifdef BACKLIGHT
BLD=$(BLD) -DBACKLIGHT 
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DBACKLIGHT
endif
##########################


#####  144-Pin Package #####
#uncomment the next line for 144 pin packages specific build options such as FM Tuner as LINE2
#144PIN_PACKAGE=TRUE
ifdef 144PIN_PACKAGE
BLD=$(BLD) -DFMTUNER_ON_LINE2_IN
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFMTUNER_ON_LINE2_IN
endif
############################

###### DRM #################
#enbled in player.bat via command line
ifdef D_DRM
else
D_WMAAPI_NO_DRM=TRUE
BLD=$(BLD) -DWMAAPI_NO_DRM
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DWMAAPI_NO_DRM
endif
############################

##### EVAL BD PLATFORM ######
#comment out the following lines if using a 3400 only legacy engineering board (fmtuner lines rerouted) 
REVB_ENGR_BD=TRUE
ifdef REVB_ENGR_BD
BLD=$(BLD) -DREVB_ENGR_BD
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DREVB_ENGR_BD
endif
###########################

##### DEMO PLATFORM ######
# volume buttons switched
# diff contrast range/default if using DC/DC VDDD voltages
#Uncomment the following line if building for demo hardware
#DEMO_HW=TRUE
ifdef DEMO_HW
BLD=$(BLD) -DDEMO_HW
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DDEMO_HW
endif
###########################
##### SPECTRUM ANALYZER ######
#this will be defined by the command line options
#Since we only allow up to 9 command line options
#you may want to explicitly define SPECTRUM_ANAL
#in this file.  In that case comment out the
#first ifdef and endif below.
ifdef D_SPECTRUM 	
SPECTRUM_ANAL=TRUE
endif

ifdef SPECTRUM_ANAL
BLD=$(BLD) -DSPECTRUM_ANAL
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSPECTRUM_ANAL
endif
###########################

################################################################################
# uncomment following line to enable color display.
# Note that C_LCD is also an input to player.bat so you don't have to uncomment this line
# to get a color lcd build but if you do, it should always result in color lcd support by default. 
################################################################################
#D_C_LCD=TRUE

################################################################################
# Choose the display driver for the LCD
################################################################################
###########DISPLAY DRIVER###############
ifdef D_C_LCD
S6B33B0A_LCD=TRUE
#LDS514_LCD=TRUE
#ILI814_LCD=TRUE
#ML9341_LCD=TRUE
#SSD1332_LCD=TRUE
else
SED15XX_LCD=TRUE
endif
######################################

ifdef D_C_LCD #if D_C_LCD
################################################################################
# Please choose either one of the following for your color LCD
################################################################################
#uncomment the next line if you would like 262k color support instead of the default 4096 color support
#COLOR_262K=TRUE
#uncomment the next line if you would like 65k color support instead of the default 4096 color support
COLOR_65K=TRUE

#if C_LCD_16BIT is not declare outside, 8-bit lcd interface is assumed.
ifdef D_C_LCD_16BIT
BLD=$(BLD) -DCLCD_16BIT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCLCD_16BIT
else
BLD=$(BLD) -DCLCD_8BIT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCLCD_8BIT
endif

################################################################################
# Color LCD compilation option settings
################################################################################
D_CLCD=TRUE

BLD=$(BLD) -DCLCD
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCLCD

ifdef COLOR_262K
BLD=$(BLD) -DCOLOR_262K
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCOLOR_262K
else
ifdef COLOR_65K
BLD=$(BLD) -DCOLOR_65K
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCOLOR_65K
endif
endif

ifdef S6B33B0A_LCD
BLD=$(BLD) -DS6B33B0A_LCD -DBACKLIGHT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DS6B33B0A_LCD -Wa-DBACKLIGHT
endif

ifdef LDS514_LCD
BLD=$(BLD) -DLDS514_LCD -DBACKLIGHT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DLDS514_LCD -Wa-DBACKLIGHT
endif

ifdef ILI814_LCD
BLD=$(BLD) -DILI814_LCD -DBACKLIGHT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DILI814_LCD -Wa-DBACKLIGHT
endif

ifdef ML9341_LCD
BLD=$(BLD) -DML9341_LCD -DBACKLIGHT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DML9341_LCD -Wa-DBACKLIGHT
endif

ifdef SSD1332_LCD
BLD=$(BLD) -DSSD1332_LCD -DBACKLIGHT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSSD1332_LCD -Wa-DBACKLIGHT
endif

else  #else D_C_LCD

################################################################################
# Monochrome LCD compilation option settings
################################################################################
ifdef SED15XX_LCD
BLD=$(BLD) -DSED15XX_LCD
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSED15XX_LCD
endif

endif #end D_C_LCD

ifdef STMP_BUILD_PLAYER
################################################################################
# uncomment following line to enable lyric support.
################################################################################
D_LYRIC_TOOL=TRUE

################################################################################
# uncomment following lines to enable the specific multimedia applications
# Note :
#     Following applications only applicable when D_C_LCD is defined. 
#     note that these are also build options in player.bat so you do not have to uncomment the lines below to get the option, just type it as input into player.bat
################################################################################
#D_JPG_TOOL=TRUE       #Jpeg application
#D_SMV_TOOL=TRUE       #Motion Video application
#D_ALBUMART_TOOL=TRUE  #Album art application

include ../../../../../etc/app_conf.mk
endif #STMP_BUILD_PLAYER
