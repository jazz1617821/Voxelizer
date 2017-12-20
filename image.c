#include <memory.h>
#include "image.h"
#include <stdio.h>
#include <stdlib.h>


int loadBMP(char *fileName, int *width, int *height, unsigned char **data)
{
	FILE             *fp;
	BITMAPINFO bitmapInfo;
	BITMAPFILEHEADER   bitmapFileHeader;   // Bitmap file header 
	unsigned char     *bitmapImage;      // Bitmap image data 
	unsigned int       lInfoSize;         // Size of information 
	unsigned int       lBitSize;         // Size of bitmap 

	unsigned char change;
	int pixel;
	int p = 0;

	fp = fopen(fileName, "rb");
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);         //讀取 bitmap header 

	lInfoSize = bitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);   //Info的size 
	fread(&bitmapInfo, lInfoSize, 1, fp);


	lBitSize = bitmapInfo.bmiHeader.biSizeImage;                  //配置記憶體 
	bitmapImage = (BYTE*)calloc(lBitSize, sizeof(BYTE));
	fread(bitmapImage, 1, lBitSize, fp);                        //讀取影像檔 

	fclose(fp);

	//此時傳回bitmapImage的話，顏色會是BGR順序，下面迴圈會改順序為RGB 
	pixel = (bitmapInfo.bmiHeader.biWidth) * (bitmapInfo.bmiHeader.biHeight);
	/*
	for (int i = 0; i<pixel; i++, p += 3)
	{
		//交換bitmapImage[p]和bitmapImage[p+2]的值 
		change = bitmapImage[p];
		bitmapImage[p] = bitmapImage[p + 2];
		bitmapImage[p + 2] = change;
	}
	*/

	*data = bitmapImage;
	*width = bitmapInfo.bmiHeader.biWidth;
	*height = bitmapInfo.bmiHeader.biHeight;

	return 1;
}


void saveBMP(char *filename, int w, int h, unsigned char *data)
{
	unsigned int headers[13];
	FILE * outfile;
	int extrabytes;
	int paddedsize;
	int x; int y; int n;
	int red, green, blue;

	extrabytes = 4 - ((w * 3) % 4);                 // How many bytes of padding to add to each
													// horizontal line - the size of which must
													// be a multiple of 4 bytes.
	if (extrabytes == 4)
		extrabytes = 0;

	paddedsize = ((w * 3) + extrabytes) * h;

	// Headers...
	// Note that the "BM" identifier in bytes 0 and 1 is NOT included in these "headers".

	headers[0] = paddedsize + 54;      // bfSize (whole file size)
	headers[1] = 0;                    // bfReserved (both)
	headers[2] = 54;                   // bfOffbits
	headers[3] = 40;                   // biSize
	headers[4] = w;  // biWidth
	headers[5] = h; // biHeight

					// Would have biPlanes and biBitCount in position 6, but they're shorts.
					// It's easier to write them out separately (see below) than pretend
					// they're a single int, especially with endian issues...

	headers[7] = 0;                    // biCompression
	headers[8] = paddedsize;           // biSizeImage
	headers[9] = 0;                    // biXPelsPerMeter
	headers[10] = 0;                    // biYPelsPerMeter
	headers[11] = 0;                    // biClrUsed
	headers[12] = 0;                    // biClrImportant

	outfile = fopen(filename, "wb");

	//
	// Headers begin...
	// When printing ints and shorts, we write out 1 character at a time to avoid endian issues.
	//

	fprintf(outfile, "BM");

	for (n = 0; n <= 5; n++)
	{
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
	}

	// These next 4 characters are for the biPlanes and biBitCount fields.

	fprintf(outfile, "%c", 1);
	fprintf(outfile, "%c", 0);
	fprintf(outfile, "%c", 24);
	fprintf(outfile, "%c", 0);

	for (n = 7; n <= 12; n++)
	{
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
	}

	//
	// Headers done, now write the data...
	//

	for (y = h - 1; y >= 0; y--)     // BMP image format is written from bottom to top...
	{
		for (x = 0; x <= w - 1; x++)
		{

			red = data[x + y * w];
			green = data[x + y * w];
			blue = data[x + y * w];

			if (red > 255) red = 255; if (red < 0) red = 0;
			if (green > 255) green = 255; if (green < 0) green = 0;
			if (blue > 255) blue = 255; if (blue < 0) blue = 0;

			// Also, it's written in (b,g,r) format...

			fprintf(outfile, "%c", blue);
			fprintf(outfile, "%c", green);
			fprintf(outfile, "%c", red);
		}
		if (extrabytes)      // See above - BMP lines must be of lengths divisible by 4.
		{
			for (n = 1; n <= extrabytes; n++)
			{
				fprintf(outfile, "%c", 0);
			}
		}
	}

	fclose(outfile);
}


float triLinear(float v[8], float x, float y, float z)
{
	return v[0] * (1 - x) * (1 - y) * (1 - z) +
		v[1] * x * (1 - y) * (1 - z) +
		v[2] * (1 - x) * y * (1 - z) +
		v[4] * (1 - x) * (1 - y) * z +
		v[5] * x * (1 - y) * z +
		v[6] * (1 - x) * y * z +
		v[3] * x * y * (1 - z) +
		v[7] * x * y * z;
}

// unit sqaure (0,0) (1,0) (0,1) (1,1)
//
//  2┌─┐3
//   │  │
//  0└─┘1
//
float biLinear(float v[4], float x, float y)
{
	return v[0] * (1 - x) * (1 - y) + v[1] * x * (1 - y) + v[2] * (1 - x) * y + v[3] * x * y;
}