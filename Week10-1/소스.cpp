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
	FILE* output = fopen("bitstream.txt","w");
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
	int value;
	int n1 = 0, n2 = 1;
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
			value = y1[j * width + i];
			value = value / 16;
			value = value * 16;
			y2[j * width + i] = value;
		}
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (y2[j * width + i] == 0)
			{
				fprintf(output, "%d%d%d%d", n1, n1, n1, n1);
			}
			else if (y2[j * width + i] == 16)
			{
				fprintf(output, "%d%d%d%d", n1, n1, n1, n2);
			}
			else if (y2[j * width + i] == 32)
			{
				fprintf(output, "%d%d%d%d", n1, n1, n2, n1);
			}
			else if (y2[j * width + i] == 48)
			{
				fprintf(output, "%d%d%d%d", n1, n1, n2, n2);
			}
			else if (y2[j * width + i] == 64)
			{
				fprintf(output, "%d%d%d%d", n1, n2, n1, n1);
			}
			else if (y2[j * width + i] == 80)
			{
				fprintf(output, "%d%d%d%d", n1, n2, n1, n2);
			}
			else if (y2[j * width + i] == 96)
			{
				fprintf(output, "%d%d%d%d", n1, n2, n2, n1);
			}
			else if (y2[j * width + i] == 112)
			{
				fprintf(output, "%d%d%d%d", n1, n2, n2, n2);
			}
			else if (y2[j * width + i] == 128)
			{
				fprintf(output, "%d%d%d%d", n2, n1, n1, n1);
			}
			else if (y2[j * width + i] == 144)
			{
				fprintf(output, "%d%d%d%d", n2, n1, n1, n2);
			}
			else if (y2[j * width + i] == 160)
			{
				fprintf(output, "%d%d%d%d", n2, n1, n2, n1);
			}
			else if (y2[j * width + i] == 176)
			{
				fprintf(output, "%d%d%d%d", n2, n1, n2, n2);
			}
			else if (y2[j * width + i] == 192)
			{
				fprintf(output, "%d%d%d%d", n2, n2, n1, n1);
			}
			else if (y2[j * width + i] == 208)
			{
				fprintf(output, "%d%d%d%d", n2, n2, n1, n2);
			}
			else if (y2[j * width + i] == 224)
			{
				fprintf(output, "%d%d%d%d", n2, n2, n2, n1);
			}
			else if (y2[j * width + i] == 240)
			{
				fprintf(output, "%d%d%d%d", n2, n2, n2, n2);
			}
		}
	}

	free(y1);
	free(y2);
	free(output);
	free(inputImg1);
	fclose(inputFile1);
}