////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : Jpegapp.h
// Description : Jpeg application header file
////////////////////////////////////////////////////////////////////////////////
#ifndef SDK_PLATFORM
#include <stdio.h>
#include <stdlib.h>
#endif


#ifdef SDK_PLATFORM
//#define PROFILE_REQUIRED
#endif

#ifdef SDK_PLATFORM
#ifdef PROFILE_REQUIRED
#include "fstypes.h"
#include "fsdef.h"
#endif
#endif



#ifdef SDK_PLATFORM
#define _XMEM	_X
#else
#define _XMEM
#endif

#ifdef COLOR_262K
#define	JPEG_OUTPUT_COLOUR_TYPE		 4
#else // COLOR_65K
#define	JPEG_OUTPUT_COLOUR_TYPE		 3
#endif

struct Jpeg_dec_app_struct
{
    signed int zoom_no;
    signed int x_pos_zoom;
    signed int y_pos_zoom;
    unsigned int output_grayscale_flag;
    unsigned int valid_output;
    unsigned int Progress_percent;
	unsigned int output_image_width;
	unsigned int output_image_height;
	unsigned int image_width;
	unsigned int image_height;
};
typedef struct Jpeg_dec_app_struct *j_app_ptr;



/* Input Structure */
struct Jpeg_dec_input_buff_struct
{
	signed int Input_buffer_filled_space;
	unsigned int * pBuffPointer;
};
typedef	struct Jpeg_dec_input_buff_struct *j_inputbuf_ptr;

/* Output Structure */
struct Jpeg_dec_output_buff_struct
{
	unsigned int output_buffer_size;
	unsigned int * pBuffPointer;
};
typedef struct Jpeg_dec_output_buff_struct * j_outputbuf_ptr;

struct Jpeg_dec_metadata_struct
{
    unsigned int start_metadata;
    unsigned int done_metadata;
    unsigned char Image_description[50];
    unsigned int Orientation;
    unsigned int X_Resolution;
    unsigned int Y_Resolution;
    unsigned char DateTimeOri[50];
    unsigned long PixelXDimension;
    unsigned long PixelYDimension;
};
typedef struct Jpeg_dec_metadata_struct *j_metadata_ptr;



extern unsigned int Jpeg_extract_metadata (j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_metadata_ptr Jpeg_dec_metadata_struct);

extern unsigned int jpeg_init (j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct);

extern unsigned int jpeg_decode (j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct);

extern unsigned int	BMP_decode (j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct);

extern unsigned int reset_Decoder (j_app_ptr Jpeg_dec_app_struct);


extern unsigned int SmvJpeg_extract_metadata (j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_metadata_ptr Jpeg_dec_metadata_struct);

extern unsigned int smvjpeg_init (j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct);

extern unsigned int smvjpeg_decode (j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct);

extern unsigned int smvj_reset_Decoder (j_app_ptr Jpeg_dec_app_struct);

/* Error codes used by Jpeg Library */

#define JPEG_INIT_COMPLETE				0// Init section complete.
#define JPEG_INIT_MOREDATA_REQUIRED		1// Runtime section complete.

#define JPEG_RUNTIME_COMPLETE			0// Init section complete.
#define JPEG_RUNTIME_MOREDATA_REQUIRED	1// Runtime section complete.
#define JPEG_RUNTIME_MCU_BLOCK_DONE		2// Decoding of MCUs block of data done.

#define JERR_BAD_COMPONENT_ID			3// Illegal component ID
#define JERR_BAD_DCT_COEF				4// DCT coefficient out of range
#define JERR_BAD_DCTSIZE				5// Unexpected DCT size
#define JERR_BAD_HUFF_TABLE				6// Illegal entry in quantization table
#define	JERR_BAD_IN_COLORSPACE			7// Illegal input color space value
#define JERR_BAD_J_COLORSPACE			8// Illegal jpeg color space value
#define JERR_BAD_LENGTH					9// Illegal number of bytes
#define JERR_BAD_SAMPLING				10// Illegal sampling factor
#define JERR_BAD_STRUCT_SIZE			11// Structure mismatch
#define JERR_BUFFER_SIZE				12// Illegal Buffer size
#define JERR_COMPONENT_COUNT			13// Illegal color component count
#define JERR_DHT_INDEX					14// Huffman table index out of range
#define JERR_DQT_INDEX					15// Quantization table index out of range
#define JERR_EOI_EXPECTED				16// Illegal image size
#define JERR_MISMATCHED_QUANT_TABLE		17// Quantization table mismatch
#define JERR_ARITH_NOTIMPL				18// Arithmetic coding not implemented
#define JERR_NO_HUFF_TABLE				19// No Huffman table present
#define JERR_NO_SOI						20// No SOI marker found
#define JERR_NO_QUANT_TABLE				21// No Quantization table found
#define JERR_SOF_DUPLICATE				22// More than one SOF present
#define JERR_SOF_NO_SOS					23// No SOS marker found
#define JERR_SOF_UNSUPPORTED			24// Unsupported SOF marker present
#define JERR_SOI_DUPLICATE				25// More than one SOI present
#define JERR_TOO_LITTLE_DATA			26// Insufficient data
#define JERR_UNKNOWN_MARKER				27// Unknown marker present
#define JERR_PROGRESSIVE_UNSUPPORTED	28// Progressive feature not supported
#define JERR_PRECISION_UNSUPPORTED		29// Illegal data precision
#define JERR_INVALID_JPEG_FILE			30// Invalid jpeg file
#define JERR_INVALID_VERSION_NO			31// Invalid version number
#define JERR_METADATA_MARKER_MISSING	32// Metadata marker not present
#define JERR_METADATA_INFO_UNSUPPORTED	33// Requested metadata info not supported
#define JERR_ZOOM_LIMIT_REACHED			34// Zooming limit reached
#define JERR_UNKNOWN_ERROR				35// Unknown error
#define JERR_MARKER_READING				36// Marker reading problem in metadata
#define JERR_IMAGE_TOO_BIG				37// Image is higher than 5120 x 3840
#define JERR_EMPTY_IMAGE				38// Image height or width or number of components
										  // in image is lesser than 0
#define JERR_SOS_NO_SOF					39// SOS marker comes before SOF
#define JERR_BAD_STATE					40// Execution gone in some wrong state
#define JERR_BMP_UNSUPPORTED_FORMAT		41// Input BMP file format is other than 24 bits.


