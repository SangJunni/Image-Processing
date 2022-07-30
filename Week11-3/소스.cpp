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
	FILE* output = fopen("bitstream.txt", "r");

	int width = bmpInfo1.biWidth;
	int height = bmpInfo1.biHeight;
	int size = bmpInfo1.biSizeImage;
	int bitCnt = bmpInfo1.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

	unsigned char* outputImg = NULL;
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	unsigned char* y1, * y2;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	int n1, n2, n3, n4, n5, value;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			fscanf(output, "%1d%1d%1d%1d%1d", &n1, &n2, &n3, &n4, &n5);
			//printf("%d %d %d %d\n", n1, n2, n3, n4);
			value = n1 * 16 + n2 * 8 + n3 * 4 + n2 * 2 + n1 * 1 - 16;
			value = value * 8;
			value = value + 128;
			y1[j * width + i] = value;
		}
	}
	printf("Finish\n");
	for (int j = 0; j < height; j++)//결과 출력
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = y1[j * width + i];
			outputImg[j * stride + 3 * i + 1] = y1[j * width + i];
			outputImg[j * stride + 3 * i + 2] = y1[j * width + i];
		}
	}
	FILE* outputFile = fopen("Output_Decopmression.bmp", "wb");
	bmpInfo1.biWidth = width;
	bmpInfo1.biHeight = height;
	bmpInfo1.biSizeImage = size;
	fwrite(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(y1);
	free(y2);
	fclose(output);
	free(outputImg);
	fclose(inputFile1);
	fclose(outputFile);
}