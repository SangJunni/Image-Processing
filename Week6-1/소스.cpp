#include<stdio.h>
#include<windows.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	BITMAPFILEHEADER bmpFile1;
	BITMAPINFOHEADER bmpInfo1;
	FILE* inputFile1 = NULL;
	inputFile1 = fopen("AICenterY_Org.bmp", "rb");
	fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	int width = bmpInfo1.biWidth;
	int height = bmpInfo1.biHeight;
	int size = bmpInfo1.biSizeImage;
	int bitCnt = bmpInfo1.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	int ratio = 2;
	//int width2 = bmpInfo1.biWidth >> ratio;
	int width2 = bmpInfo1.biWidth / 2;
	//int height2 = bmpInfo1.biHeight >> ratio;
	int height2 = bmpInfo1.biHeight / 2;
	int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
	int size2 = stride2 * height2;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width2, stride2, height2, size2, bitCnt);
	unsigned char* inputImg1 = NULL,* outputImg = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);

	unsigned char* y1, * y2;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width2 * height2, sizeof(unsigned char));
	double sum;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			y1[j * width + i] = inputImg1[j * stride + 3 * i + 0];
		}
	}
	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++)
		{
			//y2[j * width2 + i] = y1[(j << ratio) * width + (i << ratio)];
			//sum = (y1[(j << ratio) * width + (i << ratio)] + y1[(j << ratio) * width + (i << ratio) + 1] + y1[((j << ratio)+1) * width + (i << ratio)] + y1[((j << ratio) + 1) * width + (i << ratio) + 1]);
			sum = (y1[(j * 2) * width + (i * 2)] + y1[(j * 2) * width + (i * 2) + 1] + y1[((j * 2) + 1) * width + (i * 2)] + y1[((j * 2) + 1) * width + (i * 2) + 1]);
			y2[j * width2 + i] = sum / 4;
		}
	}
	for (int j = 0; j < height2; j++)//enhance 결과 출력
	{
		for (int i = 0; i < width2; i++)
		{
			outputImg[j * stride2 + 3 * i + 0] = y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 1] = y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 2] = y2[j * width2 + i];
		}
	}
	FILE* outputFile = fopen("Output_downsampling256_average.bmp", "wb");
	bmpInfo1.biWidth = width2;
	bmpInfo1.biHeight = height2;
	bmpInfo1.biSizeImage = size2;
	bmpFile1.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
	fwrite(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

	free(y1);
	free(y2);
	free(outputImg);
	free(inputImg1);
	fclose(inputFile1);
	fclose(outputFile);
}