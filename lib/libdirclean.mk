###############################################################################
# SigmaTel Inc., 2003-2005
#
# lidirclean.mk
#
# This makefile is use to clean the lib directory.  It was created for
# invocation by ReleaseScripts\sdkbuildall.bat to ensure the libs dir
# is completely scrubbed when rebuilding the entire code tree. It's now just used
# by sdksource.bat and mtp_sdksource.bat during sdk extraction. 
###############################################################################

clean :
	erase /Q .\debug\MSC
	erase /Q .\debug\MSC\3500
	erase /Q .\debug\Player
	erase /Q .\debug\Player\3500
	erase /Q .\debug\sdk2400
	erase /Q .\retail\MSC
	erase /Q .\retail\MSC\3500
	erase /Q .\retail\Player
	erase /Q .\retail\Player\3500
	erase /Q .\retail\sdk2400


#warning: \libs\.  has 3 we won't delete: libfp.a librt.a ccr.a since they are not rebuilt in buildlibs script
