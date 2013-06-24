/*
*/

#include "types.h"


//
//	This funny implementation avoids having to copy the filename into a temp
//	buffer and byte swap the words.
//

#define	CHAR_TO_INT(a,b,c)	((((INT) (c)) << 16) | (((INT) (b)) << 8) | (INT) (a))

// "a:/Audible/AudibleActivation.sys"
const INT ActivationFilename[] = {
	CHAR_TO_INT( 'a', ':', '/' ),
	CHAR_TO_INT( 'A', 'u', 'd' ),
	CHAR_TO_INT( 'i', 'b', 'l' ),
	CHAR_TO_INT( 'e', '/', 'A' ),
	CHAR_TO_INT( 'u', 'd', 'i' ),
	CHAR_TO_INT( 'b', 'l', 'e' ),
	CHAR_TO_INT( 'A', 'c', 't' ),
	CHAR_TO_INT( 'i', 'v', 'a' ),
	CHAR_TO_INT( 't', 'i', 'o' ),
	CHAR_TO_INT( 'n', '.', 's' ),
	CHAR_TO_INT( 'y', 's', '\0' )
};
