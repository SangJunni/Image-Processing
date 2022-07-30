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
	inputFile2 = fopen("Pokemon.bmp", "rb");
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

	unsigned char* inputImg1 = NULL, * inputImg2 = NULL,* outputImg = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);

	int bit_num[512];
	unsigned char* y1, * y2;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			y1[j * width + i] = inputImg1[j * stride + 3 * i + 0];
		}
	}
	for (int j = 0; j < 255; j++)
	{
		bit_num[2 * j] = 0;
		bit_num[2 * j + 1] = 0;
		for (int i = 0; i < height * width; i++)
		{
			if (y1[i] == j)
			{
				bit_num[2*j] += 1;
				bit_num[2 * j + 1] += 1;
			}
		}
		printf("%d: %d\n", j, bit_num[2 * j]);
		bit_num[2 * j] /= 10;
		bit_num[2 * j + 1] /= 10;
	}
	/*for (int j = 0; j < height; j++)//threshold 하기위한 경우
	{
		for (int i = 0; i < width; i++)
		{

			outputImg[j * stride + 3 * i + 0] = (inputImg1[j * stride + 3 * i + 0] > 250) ? 255 : 0;
			outputImg[j * stride + 3 * i + 1] = (inputImg1[j * stride + 3 * i + 0] > 250) ? 255 : 0;
			outputImg[j * stride + 3 * i + 2] = (inputImg1[j * stride + 3 * i + 0] > 250) ? 255 : 0;
		}
	}*/
	for (int j = height-1; j >= 0; j--)//hsitogram 하기위한 경우
	{
		for (int i = 0; i < width; i++)//파일에서는 위아래 invert 라는 점 유의!!
		{
			outputImg[j * stride + 3 * i + 0] = j < bit_num[i] ? 0 : 255;
			outputImg[j * stride + 3 * i + 1] = j < bit_num[i] ? 0 : 255;
			outputImg[j * stride + 3 * i + 2] = j < bit_num[i] ? 0 : 255;
		}
	}
	FILE* outputFile = fopen("Output_Histogram.bmp", "wb");//Output: copy pictures, Output_RED: red pictures, Output_Blue: blue pictures, Output_GREEN: green pictures
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