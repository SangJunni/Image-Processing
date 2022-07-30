#include<stdio.h>
#include<windows.h> 
int main()
{
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	inputFile = fopen("AICenter.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (j < 100 && i < 100)
			{
				outputImg[j * stride + 3 * i + 0] = 0;
				outputImg[j * stride + 3 * i + 1] = 0;
				outputImg[j * stride + 3 * i + 2] = 0;
				continue;
			}
			if (j > 400 && i > 400)
			{
				outputImg[j * stride + 3 * i + 0] = 255;
				outputImg[j * stride + 3 * i + 1] = 255;
				outputImg[j * stride + 3 * i + 2] = 255;
				continue;
			}
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
			
		}
	}
	FILE* outputFile = fopen("Output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}