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
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);


	unsigned char* inputImg1 = NULL, * outputImg = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);

	unsigned char* y1, * y2;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	int x, y;
	double Gx, Gy, G, Gx2, Gx3, Gy2, Gy3;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			y1[j * width + i] = inputImg1[j * stride + 3 * i + 0];
		}
	}
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			Gx = ((y1[(j - 1) * width + i - 1] - y1[(j - 1) * width + i + 1])+ 2 * (y1[j * width + i - 1] - y1[j * width + i + 1]) + (y1[(j + 1) * width + i - 1] - y1[(j + 1) * width + i + 1]));
			Gy = ((y1[(j - 1) * width + i - 1] - y1[(j + 1) * width + i - 1]) + 2 * (y1[(j - 1) * width + i] - y1[(j + 1) * width + i]) + (y1[(j - 1) * width + i - 1] - y1[(j + 1) * width + i + 1]));
			G = sqrt(Gx * Gx + Gy * Gy);
			if (G >= 200)
			{
				y2[j * width + i] = (unsigned char)(G > 255 ? 255 : G);
			}
		}
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = y2[j * width + i];
		}
	}
	FILE* outputFile = fopen("Output_Edge_Sobel_200.bmp", "wb");
	fwrite(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(y1);
	free(y2);
	free(outputImg);
	free(inputImg1);
	fclose(inputFile1);
	fclose(outputFile);
}