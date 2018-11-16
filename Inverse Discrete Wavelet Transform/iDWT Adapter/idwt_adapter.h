
#include "xidwt.h"


// Image size
#define COLS 2048
#define ROWS 2048

#define NUM_BLOCKS 16

#define SEND_BLOCK (ROWS*COLS/NUM_BLOCKS/4)
#define RECV_BLOCK (SEND_BLOCK*4)


// Reserved Memory <0x38000000 0x8000000> = 128MB
// Mapping to reserved DDR3 mmeory
static const unsigned int SOURCE_DDR_BUFFER_0 = 0x38000000;
static const unsigned int SOURCE_DDR_BUFFER_1 = 0x39000000;
static const unsigned int SOURCE_DDR_BUFFER_2 = 0x3A000000;
static const unsigned int SOURCE_DDR_BUFFER_3 = 0x3B000000;
static const unsigned int DEST_DDR_BUFFER = 0x3C000000;

// Mapping address range IN:64MB - OUT:16MB * 4
static const unsigned int SOURCE_DDR_BUFFER_SIZE = 0x01000000;
static const unsigned int DEST_DDR_BUFFER_SIZE = ( SOURCE_DDR_BUFFER_SIZE * 4 );


// IP Core Name in the Device-tree
static const char* instanceName = "idwt";


// Methods
int idwt(float *inImage0, float *inImage1, float *inImage2, float *inImage3, float *outImage);


