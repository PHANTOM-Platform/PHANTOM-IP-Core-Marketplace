
#include "idwt_adapter.h"


int idwt(float *inImage0, float *inImage1, float *inImage2, float *inImage3, float *outImage)
{
	XIdwt idwt;
	int memFd;
	float *inBuff0;
	float *inBuff1;
	float *inBuff2;
	float *inBuff3;
	float *outBuff;

	//printf("\nIP Core Started\n");


	// ---------- Prepare data
	if( ( memFd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		return -1;
	}


	// mmap DMA to IP Buffer
	inBuff0 = (float*)mmap( NULL, SOURCE_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, SOURCE_DDR_BUFFER_0 );
	if( inBuff0 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap DMA to IP Buffer
	inBuff1 = (float*)mmap( NULL, SOURCE_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, SOURCE_DDR_BUFFER_1 );
	if( inBuff1 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap DMA to IP Buffer
	inBuff2 = (float*)mmap( NULL, SOURCE_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, SOURCE_DDR_BUFFER_2 );
	if( inBuff2 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap DMA to IP Buffer
	inBuff3 = (float*)mmap( NULL, SOURCE_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, SOURCE_DDR_BUFFER_3 );
	if( inBuff3 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap IP to DMA Buffer
	outBuff = (float*)mmap( NULL, DEST_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, DEST_DDR_BUFFER );
	if( outBuff == MAP_FAILED ) {
		close( memFd );
		return -1;
	}


	u32 inbuff_0_ptr = SOURCE_DDR_BUFFER_0;
	u32 inbuff_1_ptr = SOURCE_DDR_BUFFER_1;
	u32 inbuff_2_ptr = SOURCE_DDR_BUFFER_2;
	u32 inbuff_3_ptr = SOURCE_DDR_BUFFER_3;
	u32 outbuff_ptr = DEST_DDR_BUFFER;

	const unsigned int rows = ROWS;
	const unsigned int cols = COLS;
	const unsigned int halfRows = rows/2;
	const unsigned int halfCols = cols/2;


	// Initialize IP Core iDWT
	XIdwt_Initialize(&idwt, instanceName);


	// Fill Input Buffer
	for (unsigned int i = 0; i < halfRows * halfCols; i++) {
		inBuff0[i] = inImage0[i];
		inBuff1[i] = inImage1[i];
		inBuff2[i] = inImage2[i];
		inBuff3[i] = inImage3[i];
	}
	

	for ( uint32_t i = 0; i < NUM_BLOCKS; ++i ) {

		XIdwt_Set_imageIn0(&idwt, inbuff_0_ptr + i*SEND_BLOCK*sizeof(float));
		XIdwt_Set_imageIn1(&idwt, inbuff_1_ptr + i*SEND_BLOCK*sizeof(float));
		XIdwt_Set_imageIn2(&idwt, inbuff_2_ptr + i*SEND_BLOCK*sizeof(float));
		XIdwt_Set_imageIn3(&idwt, inbuff_3_ptr + i*SEND_BLOCK*sizeof(float));
		XIdwt_Set_imageOut(&idwt, outbuff_ptr + i*RECV_BLOCK*sizeof(float));
		
		//printf("isDone: %d - isReady: %d - isIdle: %d\n", XIdwt_IsDone(&idwt), XIdwt_IsReady(&idwt), XIdwt_IsIdle(&idwt));

		XIdwt_Start(&idwt);

		//printf("isDone: %d - isReady: %d - isIdle: %d\n", XIdwt_IsDone(&idwt), XIdwt_IsReady(&idwt), XIdwt_IsIdle(&idwt));

		uint32_t time = 0;

		for ( uint32_t j = 0;  j < 1000000; ++j ) {
			if(i == 1000000 - 1) {
				printf("%d - Timeout - IP Core stuck\n", i);
				break;
			}
			if(XIdwt_IsDone(&idwt) == 1) {
				//printf("%d - isDone - %d\n", i, j);
				break;
			}

			++time;
		}

		//printf("isDone: %d - isReady: %d - isIdle: %d\n", XIdwt_IsDone(&idwt), XIdwt_IsReady(&idwt), XIdwt_IsIdle(&idwt));
	}


	// Copy results to Output Buffer
	for (unsigned int i = 0; i < rows * cols; i++) {
		outImage[i] = outBuff[i];
	}


	// Clean up memory mapping and exit

	// Unmap Buffers
	if( munmap( inBuff0, SOURCE_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( inBuff1, SOURCE_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( inBuff2, SOURCE_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( inBuff3, SOURCE_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff, DEST_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}

	// Make the pointers point to NULL
	inBuff0 = 0;
	inBuff1 = 0;
	inBuff2 = 0;
	inBuff3 = 0;
	outBuff = 0;

	close( memFd );
	XIdwt_Release(&idwt);

	//printf("\nIP Core finished\n");

	return 0;
}


