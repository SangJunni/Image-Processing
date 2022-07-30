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
	inputFile2 = fopen("AICenterY_Dark.bmp", "rb");
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

	unsigned char* inputImg1 = NULL, * inputImg2 = NULL, * outputImg = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);

	int bit_num[512];
	unsigned char* y1, * y2;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	double gamma = 2.5;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			y1[j * width + i] = inputImg1[j * stride + 3 * i + 0];
			y2[j * width + i] = (unsigned char)(255 * pow(y1[j * width + i] / 255.0, gamma));
			y2[j * width + i] = (unsigned char)(255 * pow(y2[j * width + i] / 255.0, 1/gamma));
		}
	}
	for (int j = 0; j < height; j++)//enhance 결과 출력
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = y2[j * width + i];
		}
	}
	FILE* outputFile = fopen("Output_Gamma_Correction.bmp", "wb");
	fwrite(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg1);
	free(inputImg2);
	fclose(inputFile1);
	fclose(inputFile2);
	fclose(outputFile);
}