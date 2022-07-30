#include<stdio.h>
#include<windows.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	BITMAPFILEHEADER bmpFile1, bmpFile2;
	BITMAPINFOHEADER bmpInfo1, bmpInfo2;
	FILE* inputFile1 = NULL, * inputFile2 = NULL;
	inputFile1 = fopen("AICenterY_Org.bmp", "rb");
	inputFile2 = fopen("Output_upsampling512_Bilinear.bmp", "rb");
	fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);
	fread(&bmpFile2, sizeof(BITMAPFILEHEADER), 1, inputFile2);
	fread(&bmpInfo2, sizeof(BITMAPINFOHEADER), 1, inputFile2);

	int width = bmpInfo1.biWidth;
	int height = bmpInfo1.biHeight;
	int size = bmpInfo1.biSizeImage;
	int bitCnt = bmpInfo1.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg1 = NULL, * inputImg2 = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);

	double Y, Cb, Cr;
	double Y1, Y2;
	double I, R, G, B;
	double Diff_Y;
	double mse = 0, psnr;
	unsigned char *y1, *y2;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)//case of luminance
		{
			y1[j * width + i] = inputImg1[j * stride + 3 * i + 0];
			y2[j * width + i] = inputImg2[j * stride + 3 * i + 0];
			mse += (double)((y2[j * width + i] - y1[j * width + i]) * (y2[j * width + i] - y1[j * width + i]));
		}
		/*for (int i = 0; i < width; i++)//case of RGB
		{
			Y1 = 0.299 * inputImg1[j * stride + 3 * i + 2] + 0.587 * inputImg1[j * stride + 3 * i + 1] + 0.114 * inputImg1[j * stride + 3 * i + 0];
			y1[j * width + i] = (unsigned char)(Y1 > 255) ? 255 : (Y1 > 0 ? Y1 : 0);
			y2[j * width + i] = inputImg2[j * stride + 3 * i + 0];
			mse += (double)((y2[j * width + i] - y1[j * width + i]) * (y2[j * width + i] - y1[j * width + i]));
		}*/
	}
	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);

	free(inputImg1);
	free(inputImg2);
	fclose(inputFile1);
	fclose(inputFile2);
}