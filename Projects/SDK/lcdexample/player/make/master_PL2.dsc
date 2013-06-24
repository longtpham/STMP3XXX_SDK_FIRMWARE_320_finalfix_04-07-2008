// For this abs build, MP3_COPY are marked as copy (don't load) sections-
// for indirect purpose of having extractcluster know to extract
// those sections from this abs
#define WMA_COPY	attr=i
#define DRM_COPY	attr=i  
#define MP3_COPY  	attr=i 
#define PK_COPY   	attr=i
#define WMDRM_COPY  attr=i 
#define WMDRM_COPY1 attr=i 
#define WMDRM_COPY2 attr=i 
#define MP3ENC_COPY attr=i


//#include "stmp3500.dsc"
#include "stmp3500_PL2.dsc"
