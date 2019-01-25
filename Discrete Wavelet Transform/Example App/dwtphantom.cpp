#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "EasyBMP.h"
#include "dwt_adapter.h"


// ---------- Main ------------------------------------------------------------
int main(int argc, char *argv[])
{
	printf("\n  --- DWT FPGA DEMO  v1.0.0 ---\n\n");

	/*
	if(argc < 2) {
		printf("Usage: ./dwtphantom <ipcore UIO name>\n\n");
		return -1;
	}*/

	unsigned int rows = ROWS;
	unsigned int cols = COLS;
	unsigned int halfRows = ROWS/2;
	unsigned int halfCols = COLS/2;

    float *inImage   = new float[rows*cols];
    float *outImage0 = new float[rows*cols/4];
	float *outImage1 = new float[rows*cols/4];
	float *outImage2 = new float[rows*cols/4];
	float *outImage3 = new float[rows*cols/4];

	BMP inputImage;
 	inputImage.ReadFromFile("testImage-gray.bmp");

 	BMP outputImage0;
 	BMP outputImage1;
 	BMP outputImage2;
	BMP outputImage3;

 	outputImage0.SetSize( inputImage.TellWidth()/2 , inputImage.TellHeight()/2 );
 	outputImage1.SetSize( inputImage.TellWidth()/2 , inputImage.TellHeight()/2 );	
 	outputImage2.SetSize( inputImage.TellWidth()/2 , inputImage.TellHeight()/2 );
 	outputImage3.SetSize( inputImage.TellWidth()/2 , inputImage.TellHeight()/2 );


	printf("ROWS: %d - COLS: %d\n\n", rows, cols);


	// Fill Input Buffer
	printf("Filling DWT IP Core Input Buffer\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
			inImage[i*cols + j] = (inputImage.GetPixel(i,j).Red + inputImage.GetPixel(i,j).Green +
						inputImage.GetPixel(i,j).Blue) / 3.0;
		}
	}


	// Run IP Core
    printf("Running DWT IP Core\n\n");
    int result = dwt(inImage, outImage0, outImage1, outImage2, outImage3);

	if ( result != 0 ) {
		printf("\nDWT Failed\n\n");
		return -1;
	}
	printf("DWT IP Core Finished\n\n");


	// Copy results to Output Buffer
	printf("Copying DWT IP Core Output Buffers\n");

	for (unsigned int i = 0; i < halfRows; i++) {
		for (unsigned int j = 0; j < halfCols; j++) {
			unsigned int idx = i*halfCols + j;

			outputImage0(i,j)->Blue = outImage0[idx];
			outputImage0(i,j)->Green = outImage0[idx];
			outputImage0(i,j)->Red = outImage0[idx];

			outputImage1(i,j)->Blue = outImage1[idx];
			outputImage1(i,j)->Green = outImage1[idx];
			outputImage1(i,j)->Red = outImage1[idx];

			outputImage2(i,j)->Blue = outImage2[idx];
			outputImage2(i,j)->Green = outImage2[idx];
			outputImage2(i,j)->Red = outImage2[idx];

			outputImage3(i,j)->Blue = outImage3[idx];
			outputImage3(i,j)->Green = outImage3[idx];
			outputImage3(i,j)->Red = outImage3[idx];
		}
	}

	printf("Saving Output Images...\n");

	outputImage0.SetBitDepth( 8 );
   	CreateGrayscaleColorTable( outputImage0 );
	outputImage1.SetBitDepth( 8 );
   	CreateGrayscaleColorTable( outputImage1 );
	outputImage2.SetBitDepth( 8 );
   	CreateGrayscaleColorTable( outputImage2 );
	outputImage3.SetBitDepth( 8 );
   	CreateGrayscaleColorTable( outputImage3 );
	
	outputImage0.WriteToFile( "dwtFPGA_output0.bmp" );
	outputImage1.WriteToFile( "dwtFPGA_output1.bmp" );
	outputImage2.WriteToFile( "dwtFPGA_output2.bmp" );
	outputImage3.WriteToFile( "dwtFPGA_output3.bmp" );

	delete [] inImage;
    delete [] outImage0;
	delete [] outImage1;
	delete [] outImage2;
	delete [] outImage3;

	printf("Finished!\n\n");

	return 0;
}

