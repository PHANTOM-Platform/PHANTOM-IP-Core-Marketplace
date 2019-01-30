
#include "idwt_adapter.h"

int wildcmp(const char *wild, const char *string);

int idwt(float *inImage0, float *inImage1, float *inImage2, float *inImage3, float *outImage)
{
	XIdwt idwt;
	int memFd;
	float *inBuff0;
	float *inBuff1;
	float *inBuff2;
	float *inBuff3;
	float *outBuff;
	
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


	//printf("\nIDWT IP Core Started\n");
	
	
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



	const unsigned int source_ddr_buffer_0 = uio_addr_int; 
	const unsigned int source_ddr_buffer_1 = source_ddr_buffer_0 + source_ddr_buffer_size;
	const unsigned int source_ddr_buffer_2 = source_ddr_buffer_1 + source_ddr_buffer_size;
	const unsigned int source_ddr_buffer_3 = source_ddr_buffer_2 + source_ddr_buffer_size;
	const unsigned int dest_ddr_buffer = source_ddr_buffer_3 + source_ddr_buffer_size;



	// ---------- Prepare data
	if( ( memFd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		return -1;
	}


	// mmap DMA to IP Buffer
	inBuff0 = (float*)mmap( NULL, source_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, source_ddr_buffer_0 );
	if( inBuff0 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap DMA to IP Buffer
	inBuff1 = (float*)mmap( NULL, source_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, source_ddr_buffer_1 );
	if( inBuff1 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap DMA to IP Buffer
	inBuff2 = (float*)mmap( NULL, source_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, source_ddr_buffer_2 );
	if( inBuff2 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap DMA to IP Buffer
	inBuff3 = (float*)mmap( NULL, source_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, source_ddr_buffer_3 );
	if( inBuff3 == MAP_FAILED ) {
		close( memFd );
		return -1;
	}

	// mmap IP to DMA Buffer
	outBuff = (float*)mmap( NULL, dest_ddr_buffer_size, ( PROT_READ | PROT_WRITE ), MAP_SHARED, memFd, dest_ddr_buffer );
	if( outBuff == MAP_FAILED ) {
		close( memFd );
		return -1;
	}


	u32 inbuff_0_ptr = source_ddr_buffer_0;
	u32 inbuff_1_ptr = source_ddr_buffer_1;
	u32 inbuff_2_ptr = source_ddr_buffer_2;
	u32 inbuff_3_ptr = source_ddr_buffer_3;
	u32 outbuff_ptr = dest_ddr_buffer;

	const unsigned int rows = ROWS;
	const unsigned int cols = COLS;
	const unsigned int halfRows = rows/2;
	const unsigned int halfCols = cols/2;


	// Initialize IP Core iDWT
	XIdwt_Initialize(&idwt, uio_slave_interface);


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
		
			if(XIdwt_IsDone(&idwt) == 1) {
				printf("Block %d Done - %d\n", i, j);
				break;
			}
			else if(j == 10000000 - 1) {
				printf("Block %d Timeout - IP Core stuck\n", i);
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
	if( munmap( inBuff0, source_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( inBuff1, source_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( inBuff2, source_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( inBuff3, source_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}
	if( munmap( outBuff, dest_ddr_buffer_size ) != 0 ) {
		close( memFd );
		return -1;
	}

	// Make the pointers point to NULL
	inBuff0 = 0;
	inBuff1 = 0;
	inBuff2 = 0;
	inBuff3 = 0;
	outBuff = 0;
	
	delete [] uio_slave_interface;
	delete [] uio_master_interface;
	delete [] uio_path;
	uio_slave_interface = NULL;
	uio_master_interface = NULL;
	uio_path = NULL;

	close( memFd );
	XIdwt_Release(&idwt);

	//printf("\nIP Core finished\n");

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

