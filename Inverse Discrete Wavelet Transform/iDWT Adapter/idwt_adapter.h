
#include "xidwt.h"


// Image size
#define COLS 2048
#define ROWS 2048

#define NUM_BLOCKS 16

#define SEND_BLOCK (ROWS*COLS/NUM_BLOCKS/4)
#define RECV_BLOCK (SEND_BLOCK*4)


// Reserved DDR3 Memory <0x38000000 0x8000000> = 128MB
// Mapping address range IN:16MB * 4 - OUT:64MB
static const unsigned int total_ddr_size 	 = 0x08000000;
static const unsigned int source_ddr_buffer_size = 0x01000000;
static const unsigned int dest_ddr_buffer_size 	 = 0x04000000;


// IP Core UIO interfaces
static const char * uio_slave = "phantom_??_slave_xilinx.com,hls,idwt,1.0_0";
static const char * uio_master = "phantom_??_master_xilinx.com,hls,idwt,1.0_0";



// Methods
int idwt(float *inImage0, float *inImage1, float *inImage2, float *inImage3, float *outImagezzzz);


