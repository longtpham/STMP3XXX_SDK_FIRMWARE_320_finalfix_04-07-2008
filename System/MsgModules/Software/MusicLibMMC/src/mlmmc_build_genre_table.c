#ifndef _PC_SIMULATION_
#define _RUNNING_IN_EMBEDDED_
#endif

#ifdef	_RUNNING_IN_EMBEDDED_
#define COMP_OPT _reentrant static
#define COMP_OPT_G
#else
#define COMP_OPT
#endif

#include	"..\hdr\musiclib_hdr.h"

BYTE _X genre_table[][LEN_OF_ID3V1_GENRE] = {
	"Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", 
	"Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies", "Other", 
	"Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial", 
	"Alternative", "Ska", "Death Metal", "Pranks", "Soundtrack", 
	"Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion", 
	"Trance", "Classical", "Instrumental", "Acid", "House", "Game", 
	"Sound Clip", "Gospel", "Noise", "AlternRock", "Bass", "Soul", 
	"Punk", "Space", "Meditative", "Instrum. Pop", "Instrum. Rock", 
	"Ethnic", "Gothic", "Darkwave", "Techno-Indust.", "Electronic", 
	"Pop-Folk", "Eurodance", "Dream", "Southern Rock", "Comedy", 
	"Cult", "Gangsta", "Top 40", "Christian Rap", "Pop/Funk", "Jungle", 
	"Native American", "Cabaret", "New Wave", "Psychadelic", "Rave", 
	"Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", 
	"Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock",
	// Total 80 genre (Ref. ID3v1 spec)

/*
	"Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", 
	"Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies", "Other", 
	"Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial", 
	"Alternative", "Ska", "Death Metal", "Pranks", "Soundtrack", 
	"Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion", 
	"Trance", "Classical", "Instrumental", "Acid", "House", "Game", 
	"Sound Clip", "Gospel", "Noise", "Alt. Rock", "Bass", "Soul", 
	"Punk", "Space", "Meditative", "Instrum. Pop", "Instrum. Rock", 
	"Ethnic", "Gothic", "Darkwave", "Techno-Indust.", "Electronic", 
	"Pop-Folk", "Eurodance", "Dream", "Southern Rock", "Comedy", 
	"Cult", "Gangsta", "Top 40", "Christian Rap", "Pop/Funk", "Jungle", 
	"Native American", "Cabaret", "New Wave", "Psychadelic", "Rave", 
	"Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", 
	"Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock", 
	// WinAmp extend
	"Folk", 
	"Folk/Rock", "National Folk", "Swing", "Fusion", "Bebob", "Latin", 
	"Revival", "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock", 
	"Progress. Rock", "Psychadel. Rock", "Symphonic Rock", "Slow Rock", 
	"Big Band", "Chorus", "Easy Listening", "Acoustic", "Humour", 
	"Speech", "Chanson", "Opera", "Chamber Music", "Sonata", "Symphony", 
	"Booty Bass", "Primus", "Porn Groove", "Satire", "Slow Jam", 
	"Club", "Tango", "Samba", "Folklore", "Ballad", "Power Ballad", 
	"Rhythmic Soul", "Freestyle", "Duet", "Punk Rock", "Drum Solo", 
	"A Capella", "Euro-House", "Dance Hall", "Goa", "Drum & Bass", 
	"Club-House", "Hardcore", "Terror", "Indie", "BritPop", "Negerpunk", 
	"Polsk Punk", "Beat", "Christian Gangsta Rap", "Heavy Metal", 
	"Black Metal", "Crossover", "Contemporary Christian", "Christian Rock",
	"Merengue", "Salsa", "Thrash Metal", "Anime", "Jpop", "Synthpop" 
// total 147 genre
*/
};
