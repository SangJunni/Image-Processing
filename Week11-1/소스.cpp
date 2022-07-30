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
	FILE* output = fopen("bitstream.txt", "w");
	inputFile1 = fopen("AICenterY_Org.bmp", "rb");
	fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	int width = bmpInfo1.biWidth;
	int height = bmpInfo1.biHeight;
	int size = bmpInfo1.biSizeImage;
	int bitCnt = bmpInfo1.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);


	unsigned char* inputImg1 = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);

	unsigned char* y1, * y2;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	int *value;
	value = (int*)calloc(width * height, sizeof(int));
	int n1 = 0, n2[5], n3, power;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			y1[j * width + i] = inputImg1[j * stride + 3 * i + 0];
		}
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			value[j * width + i] = y1[j * width + i];
			value[j * width + i] = value[j * width + i] - 128;
			value[j * width + i] = value[j * width + i] / 8;
		}
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			n1 = value[j * width + i] + 16;
			for (int k = 0; k < 5; k++)
			{
				n3 = 4 - k;
				power = pow(2, n3);
				n2[k] = n1 / power;
				n1 = n1 - n2[k] * power;
			}
			fprintf(output, "%d%d%d%d%d", n2[0], n2[1], n2[2], n2[3], n2[4]);
		}
	}

	free(y1);
	free(y2);
	free(output);
	free(inputImg1);
	fclose(inputFile1);
}