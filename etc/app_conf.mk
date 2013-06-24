################################################################################
################################################################################
# Filename: app_conf.mk for stmp35xx LCDexample player
#
# NOTE:
# If user merely want to choose different multimedia applications, he should
# change to options in config.mk
#
# This file is not expected to be modified by the user except the user
# has the intension to change the lower level driver such as jpeg decoder.
################################################################################
################################################################################

ifdef D_LYRIC_TOOL
SYNC_LYRICS=TRUE
endif

ifdef D_CLCD

ifdef D_JPG_TOOL
JPEG_DECODER=TRUE
JPEG_APP=TRUE
PLAYLIST2_MULTIMEDIA=TRUE
endif

ifdef D_SMV_TOOL
SMVJPEG_DECODER=TRUE
SMV_ADPCM=TRUE
MOTION_VIDEO=TRUE
PLAYLIST2_MULTIMEDIA=TRUE
endif

ifdef D_ALBUMART_TOOL
JPEG_DECODER=TRUE
JPEG_ALBUM_ART=TRUE
endif

endif #D_CLCD

################################################################################
################################################################################
ifdef SYNC_LYRICS
BLD=$(BLD) -DSYNC_LYRICS
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSYNC_LYRICS -Wc-DSYNC_LYRICS
endif

ifdef JPEG_DECODER
BLD=$(BLD) -DJPEG_DECODER
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DJPEG_DECODER
endif

ifdef SMVJPEG_DECODER
BLD=$(BLD) -DSMVJPEG_DECODER
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSMVJPEG_DECODER
endif

ifdef JPEG_APP
BLD=$(BLD) -DJPEG_APP
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DJPEG_APP
endif

ifdef MOTION_VIDEO
BLD=$(BLD) -DMOTION_VIDEO
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMOTION_VIDEO
endif

ifdef JPEG_ALBUM_ART
BLD=$(BLD) -DJPEG_ALBUM_ART
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DJPEG_ALBUM_ART
endif

ifdef PLAYLIST2_MULTIMEDIA
BLD=$(BLD) -DPLAYLIST2_MULTIMEDIA
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DPLAYLIST2_MULTIMEDIA
endif
