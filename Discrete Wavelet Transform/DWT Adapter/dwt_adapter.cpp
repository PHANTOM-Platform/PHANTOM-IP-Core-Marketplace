
#include "dwt_adapter.h"

int wildcmp(const char *wild, const char *string);

int dwt(float *inImage, float *outImage0, float *outImage1, float *outImage2, float *outImage3)
{
	XDwt dwt;
	int memFd;
	float *inBuff;
	float *outBuff0;
	float *outBuff1;
	float *outBuff2;
	float *outBuff3;
	
	FILE* fp;
	int ret = 0;
	int bytes = 0;

	unsigned int uio_addr_int = 0;
	unsigned int uio_size_int = 0;
	int uio_master_number = -1;
	int uio_slave_number = -1;
	char* uio_slave_interface = new char[100];
	char* uio_master_interface = new char[100];
	char* uio_path = new char[50];


	//printf("\nDWT IP Core Started\n");
	
	
	// Find UIO Slave interface
	for (int i = 0; i < 32; ++i ) {
		int bytes = snprintf(uio_path, 40, "/sys/class/uio/uio%d/name", i);		
		if ( bytes > 0 ) {
			fp = fopen(uio_path, "r");
			if (fp) {
				ret = fscanf(fp, "%s", uio_slave_interface);
				fclose(fp);
				if (ret >= 0) {
					if ( wildcmp(uio_slave, uio_slave_interface) ) {
						printf("Found IP Core slave interface: UIO%d: %s\n", i, uio_slave_interface);
						uio_slave_number = i;
						break;
					}
		        }
			}
		}
	}

	if ( uio_slave_number < 0 ) {
		printf("Could not find slave interface\n");
		return -1;
	}


	// Find UIO Master interface
	for (int i = 0; i < 32; ++i ) {
		bytes = snprintf(uio_path, 40, "/sys/class/uio/uio%d/name", i);		
		if ( bytes > 0 ) {
			fp = fopen(uio_path, "r");
			if (fp) {
				ret = fscanf(fp, "%s", uio_master_interface);
				fclose(fp);
				if (ret >= 0) {
					if ( wildcmp(uio_master, uio_master_interface) ) {
						printf("Found IP Core master interface: UIO%d: %s\n", i, uio_master_interface);
						uio_master_number = i;
						break;
					}
				}
			}
		}
	}
	
	if ( uio_master_number < 0 ) {
		printf("Could not find master interface\n");
		return -1;
	}

	// Get UIO Master Addr
	bytes = snprintf(uio_path, 40, "/sys/class/uio/uio%d/maps/map0/addr", uio_master_number);		
	if ( bytes > 0 ) {
		fp = fopen(uio_path, "r");
		if (!fp) {
			printf("Could not read master interface address\n");
			return -1;
		} else {
			ret = fscanf(fp, "0x%x", &uio_addr_int);
			if (ret < 0) {
				printf("Could not read master interface address\n");
				return -1;
			} else
				printf("UIO master interface addr: 0x%08X\n", uio_addr_int);
			fclose(fp);
		}
	} else {
		printf("Could not read master interface address\n");
		return -1;
	}

	// Get UIO Master Size
	bytes = snprintf(uio_path, 40, "/sys/class/uio/uio%d/maps/map0/size", uio_master_number);		
	if ( bytes > 0 ) {		
		fp = fopen(uio_path, "r");
		if (!fp) {
			printf("Could not read master interface size\n");
			return -1;
		}
		else {
			ret = fscanf(fp, "0x%x", &uio_size_int);
			if (ret < 0) {
				printf("Could not read master interface size\n");
				return -1;
			} else
				printf("UIO master interface size: 0x%08X\n", uio_size_int);
			fclose(fp);
		}
	} else {
		printf("Could not read master interface size\n");
		return -1;
	}

	if ( uio_size_int < total_ddr_size ) {
		printf("Master interface does not have enough memory available\n");
		printf("IP Core needs: 0x%08X bytes and master interface only has: 0x%08X bytes\n", total_ddr_size, uio_size_int);
		return -1;
	}



	const unsigned int source_ddr_buffer = uio_addr_int; 
	const unsigned int dest_ddr_buffer_0 = source_ddr_buffer + source_ddr_buffer_size;
	const unsigned int dest_ddr_buffer_1 = dest_ddr_buffer_0 + dest_ddr_buffer_size;
	const unsigned int dest_ddr_buffer_2 = dest_ddr_buffer_1 + dest_ddr_buffer_size;
	const unsigned int dest_ddr_buffer_3 = dest_ddr_buffer_2 + dest_ddr_buffer_size;



	// ---------- Prepare memory
	if( ( memFd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		return -1;
	}


	// mmap DMA to IP Buffer
	inBuff = (float*)mmap( NULL, source_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, source_ddr_buffer );
	if( inBuff == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap IP to DMA Buffer
	outBuff0 = (float*)mmap( NULL, dest_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, dest_ddr_buffer_0 );

	if( outBuff0 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}
	outBuff1 = (float*)mmap( NULL, dest_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, dest_ddr_buffer_1 );

	if( outBuff1 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}
	outBuff2 = (float*)mmap( NULL, dest_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, dest_ddr_buffer_2 );

	if( outBuff2 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}
	outBuff3 = (float*)mmap( NULL, dest_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, dest_ddr_buffer_3 );

	if( outBuff3 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	u32 inbuff_ptr = source_ddr_buffer;
	u32 outbuff0_ptr = dest_ddr_buffer_0;
	u32 outbuff1_ptr = dest_ddr_buffer_1;
	u32 outbuff2_ptr = dest_ddr_buffer_2;
	u32 outbuff3_ptr = dest_ddr_buffer_3;

	const unsigned int rows = ROWS;
	const unsigned int cols = COLS;
	const unsigned int halfRows = rows/2;
	const unsigned int halfCols = cols/2;


	// Initialize IP Core DWT
	XDwt_Initialize(&dwt, uio_slave_interface);


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

		//printf("%d - isDone: %d - isReady: %d - isIdle: %d\n", i, XDwt_IsDone(&dwt), XDwt_IsReady(&dwt), XDwt_IsIdle(&dwt));

		XDwt_Start(&dwt);

		//printf("%d - isDone: %d - isReady: %d - isIdle: %d\n", i, XDwt_IsDone(&dwt), XDwt_IsReady(&dwt), XDwt_IsIdle(&dwt));

		uint32_t time = 0;

		for ( uint32_t j = 0;  j < 10000000; ++j ) {
			if(j == 10000000 - 1) {
				printf("%d - Timeout - IP Core stuck\n", i);
				break;
			}
			if(XDwt_IsDone(&dwt) == 1) {
				//printf("%d - isDone - %d\n", i, j);
				break;
			}

			++time;
		}

		//printf("%d - isDone: %d - isReady: %d - isIdle: %d - Time: %d\n", i, XDwt_IsDone(&dwt), XDwt_IsReady(&dwt), XDwt_IsIdle(&dwt), time);
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
	if( munmap( inBuff, source_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff0, dest_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff1, dest_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff2, dest_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff3, dest_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}

	// Make the pointers point to NULL
	inBuff = 0;
	outBuff0 = 0;
	outBuff1 = 0;
	outBuff2 = 0;
	outBuff3 = 0;

	delete [] uio_slave_interface;
	delete [] uio_master_interface;
	delete [] uio_path;
	uio_slave_interface = NULL;
	uio_master_interface = NULL;
	uio_path = NULL;

	close( memFd );
	XDwt_Release(&dwt);

	//printf("\nIP Core Finished\n");

	return 0;
}


int wildcmp(const char *wild, const char *string) {
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

