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
	float Y, Cb, Cr; 
	float I, R, G, B;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{			
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];//data about pictures of blue image
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];// data about pictures of green image
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];//data about pictures of red image
		}
		for (int i = 0; i < width; i++)//for ICbCr and HSI
		{
			Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]+30;
			Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
			Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];
			R = ((Y + 1.402 * Cr) > 255) ? 255 : (Y + 1.402 * Cr);//0~255사이 존재하도록 하기 위한 식
			R = (R < 0) ? 0 : R;
			G = ((Y - 0.714 * Cr - 0.344 * Cb) > 255) ? 255 : (Y - 0.714 * Cr - 0.344 * Cb);
			G = (G < 0) ? 0 : G;
			B = ((Y + 1.772 * Cb) > 255) ? 255 : (Y + 1.772 * Cb);
			B = (B < 0) ? 0 : B;
			I = (inputImg[j * stride + 3 * i + 2] +inputImg[j * stride + 3 * i + 1] + inputImg[j * stride + 3 * i + 0])/3;
			outputImg[j * stride + 3 * i + 0] = (unsigned char)B;
			outputImg[j * stride + 3 * i + 1] = (unsigned char)G;
			outputImg[j * stride + 3 * i + 2] = (unsigned char)R;
		}
	}
	FILE* outputFile = fopen("Output_YCbCrRGB.bmp", "wb");//Output: copy pictures, Output_RED: red pictures, Output_Blue: blue pictures, Output_GREEN: green pictures
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}