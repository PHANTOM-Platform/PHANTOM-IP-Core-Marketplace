
#include "xdwt.h"


// Image size
#define COLS 2048
#define ROWS 2048

#define NUM_BLOCKS 16

#define SEND_BLOCK (ROWS*COLS/NUM_BLOCKS)
#define RECV_BLOCK (SEND_BLOCK/4)


// Reserved DDR3 Memory <0x38000000 0x8000000> = 128MB
// Mapping address range IN:64MB - OUT:16MB * 4
static const unsigned int total_ddr_size 	 = 0x08000000;
static const unsigned int source_ddr_buffer_size = 0x04000000;
static const unsigned int dest_ddr_buffer_size 	 = 0x01000000;


// IP Core UIO interfaces
static const char * uio_slave  = "phantom??_slave_xilinx.com,hls,dwt,1.0_0";
static const char * uio_master = "phantom??_master_xilinx.com,hls,dwt,1.0_0";


// Methods
int dwt(float *inImage, float *outImage0, float *outImage1, float *outImage2, float *outImage3);

