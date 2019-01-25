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
#include "idwt_adapter.h"


// ---------- Main ------------------------------------------------------------
int main(int argc, char *argv[])
{
	printf("\n  --- iDWT FPGA DEMO v1.0.0 ---\n\n");

    /*
	if(argc < 2) {
		printf("Usage: ./idwtphantom <ipcore UIO name>\n\n");
		return -1;
	}*/

	unsigned int rows = ROWS;
	unsigned int cols = COLS;
	unsigned int halfRows = ROWS/2;
	unsigned int halfCols = COLS/2;

    float *inImage0 = new float[rows*cols/4];
	float *inImage1 = new float[rows*cols/4];
	float *inImage2 = new float[rows*cols/4];
	float *inImage3 = new float[rows*cols/4];
    float *outImage = new float[rows*cols];

	BMP inputImage0;
 	inputImage0.ReadFromFile("dwtFPGA_output0.bmp");

	BMP inputImage1;
 	inputImage1.ReadFromFile("dwtFPGA_output1.bmp");

	BMP inputImage2;
 	inputImage2.ReadFromFile("dwtFPGA_output2.bmp");

	BMP inputImage3;
 	inputImage3.ReadFromFile("dwtFPGA_output3.bmp");

 	BMP outputImage;
 	outputImage.SetSize( inputImage0.TellWidth()*2 , inputImage0.TellHeight()*2 );


	printf("ROWS: %d - COLS: %d\n\n", rows, cols);


	// Fill Input Buffer
	printf("Filling IDWT IP Core Input Buffer\n");

	for (unsigned int i = 0; i < halfRows; i++) {
		for (unsigned int j = 0; j < halfCols; j++) {
			unsigned int idx = i*halfCols + j;
			inImage0[idx] = (inputImage0.GetPixel(i,j).Red + inputImage0.GetPixel(i,j).Green + 
						inputImage0.GetPixel(i,j).Blue) / 3.0;

			inImage1[idx] = (inputImage1.GetPixel(i,j).Red + inputImage1.GetPixel(i,j).Green +
						inputImage1.GetPixel(i,j).Blue) / 3.0;

			inImage2[idx] = (inputImage2.GetPixel(i,j).Red + inputImage2.GetPixel(i,j).Green +
						inputImage2.GetPixel(i,j).Blue) / 3.0;

			inImage3[idx] = (inputImage3.GetPixel(i,j).Red + inputImage3.GetPixel(i,j).Green + 
						inputImage3.GetPixel(i,j).Blue) / 3.0;
		}
	}


    // Run IP Core
    printf("Running IDWT IP Core\n\n");
	int result = idwt(inImage0, inImage1, inImage2, inImage3, outImage);
	
	if ( result != 0 ) {
		printf("\nIDWT Failed\n\n");
        return -1;
    }
	printf("IDWT IP Core Finished\n\n");


	// Copy results to Output Buffer
	printf("Copying IDWT IP Core Output Buffers\n");

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
            		unsigned int idx = i*cols + j;
			outputImage(i,j)->Blue = outImage[idx];
			outputImage(i,j)->Green = outImage[idx];
			outputImage(i,j)->Red = outImage[idx];
		}
	}


	printf("Saving Output Image...\n");

	outputImage.SetBitDepth( 8 );
   	CreateGrayscaleColorTable( outputImage );
	outputImage.WriteToFile( "idwtFPGA_output.bmp" );
	
	delete [] inImage0;
    delete [] inImage1;
	delete [] inImage2;
	delete [] inImage3;
	delete [] outImage;

	printf("Finished!\n\n");

	return 0;
}

