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

	unsigned char* y1, * y2, * y3;
	y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	int x, y;
	const double PI = 3.1415926535;
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
			x = (cos(PI / 4) * (i - 256) - sin(PI / 4) * (j - 256) + 256);
			y = (sin(PI / 4) * (i - 256) + cos(PI / 4) * (j - 256) + 256);
			//x = (cos(PI / 4) * (i) - sin(PI / 4) * (j));
			//y = (sin(PI / 4) * (i) + cos(PI / 4) * (j));
			if (x > -1 && y > -1 && x<512 && y<512)
			{
				y2[y * width + x] = y1[j * width + i];
				y3[y * width + x] = 1;
			}
			else
			{
				y3[j * width + i] = 2;
			}
		}
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (y3[j * width + i] == 0)
			{
				if (y2[j * width + i - 1] != 0)
				{
					y2[j * width + i] = y2[j * width + i - 1];
				}
				else if (y2[j * width + i + 1] != 0)
				{
					y2[j * width + i] = y2[j * width + i + 1];
				}
				else if (y2[(j-1) * width + i] != 0)
				{
					y2[j * width + i] = y2[(j-1) * width + i];
				}
				else
				{
					y2[j * width + i] = y2[(j+1) * width + i];
				}
			}
		}
	}
	for (int j = 0; j < height; j++)//결과 출력
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = y2[j * width + i];
		}
	}
	FILE* outputFile = fopen("Output_Rotation_hf.bmp", "wb");
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