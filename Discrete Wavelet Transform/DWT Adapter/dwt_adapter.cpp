
#include "dwt_adapter.h"


int dwt(float *inImage, float *outImage0, float *outImage1, float *outImage2, float *outImage3)
{
	XDwt dwt;
	int memFd;
	float *inBuff;
	float *outBuff0;
	float *outBuff1;
	float *outBuff2;
	float *outBuff3;

	//printf("\nIP Core Started\n");


	// ---------- Prepare data
	if( ( memFd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		return -1;
	}


	// mmap DMA to IP Buffer
	inBuff = (float*)mmap( NULL, SOURCE_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, SOURCE_DDR_BUFFER );
	if( inBuff == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap IP to DMA Buffer
	outBuff0 = (float*)mmap( NULL, DEST_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, DEST_DDR_BUFFER_0 );

	if( outBuff0 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}
	outBuff1 = (float*)mmap( NULL, DEST_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, DEST_DDR_BUFFER_1 );

	if( outBuff1 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}
	outBuff2 = (float*)mmap( NULL, DEST_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, DEST_DDR_BUFFER_2 );

	if( outBuff2 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}
	outBuff3 = (float*)mmap( NULL, DEST_DDR_BUFFER_SIZE, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, DEST_DDR_BUFFER_3 );

	if( outBuff3 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	u32 inbuff_ptr = SOURCE_DDR_BUFFER;
	u32 outbuff0_ptr = DEST_DDR_BUFFER_0;
	u32 outbuff1_ptr = DEST_DDR_BUFFER_1;
	u32 outbuff2_ptr = DEST_DDR_BUFFER_2;
	u32 outbuff3_ptr = DEST_DDR_BUFFER_3;

	const unsigned int rows = ROWS;
	const unsigned int cols = COLS;
	const unsigned int halfRows = rows/2;
	const unsigned int halfCols = cols/2;


	// Initialize IP Core DWT
	XDwt_Initialize(&dwt, instanceName);


	// Fill Input Buffer
	for (unsigned int i = 0; i < rows * cols; i++) {
		inBuff[i] = inImage[i];
	}
	

	for ( uint32_t i = 0; i < NUM_BLOCKS; ++i ) {

		XDwt_Set_imageIn(&dwt, inbuff_ptr + i*SEND_BLOCK*sizeof(float));
		XDwt_Set_imageOut0(&dwt, outbuff0_ptr + i*RECV_BLOCK*sizeof(float));
		XDwt_Set_imageOut1(&dwt, outbuff1_ptr + i*RECV_BLOCK*sizeof(float));
		XDwt_Set_imageOut2(&dwt, outbuff2_ptr + i*RECV_BLOCK*sizeof(float));
		XDwt_Set_imageOut3(&dwt, outbuff3_ptr + i*RECV_BLOCK*sizeof(float));

		//printf("isDone: %d - isReady: %d - isIdle: %d\n", XDwt_IsDone(&dwt), XDwt_IsReady(&dwt), XDwt_IsIdle(&dwt));

		XDwt_Start(&dwt);

		//printf("isDone: %d - isReady: %d - isIdle: %d\n", XDwt_IsDone(&dwt), XDwt_IsReady(&dwt), XDwt_IsIdle(&dwt));

		uint32_t time = 0;

		for ( uint32_t j = 0;  j < 1000000; ++j ) {
			if(i == 1000000 - 1) {
				printf("%d - Timeout - IP Core stuck\n", i);
				break;
			}
			if(XDwt_IsDone(&dwt) == 1) {
				//printf("%d - isDone - %d\n", i, j);
				break;
			}

			++time;
		}

		//printf("isDone: %d - isReady: %d - isIdle: %d\n", XDwt_IsDone(&dwt), XDwt_IsReady(&dwt), XDwt_IsIdle(&dwt));
	}


	// Copy results to Output Buffer
	for (unsigned int i = 0; i < halfRows * halfCols; i++) {
		outImage0[i] = outBuff0[i];
		outImage1[i] = outBuff1[i];
		outImage2[i] = outBuff2[i];
		outImage3[i] = outBuff3[i];
	}


	// Clean up memory mapping and exit

	// Unmap Buffers
	if( munmap( inBuff, SOURCE_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff0, DEST_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff1, DEST_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff2, DEST_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff3, DEST_DDR_BUFFER_SIZE ) != 0 ) {
		close( memFd );
		return -1;
	}

	// Make the pointers point to NULL
	inBuff = 0;
	outBuff0 = 0;
	outBuff1 = 0;
	outBuff2 = 0;
	outBuff3 = 0;

	close( memFd );
	XDwt_Release(&dwt);

	//printf("\nIP Core Finished\n");

	return 0;
}

