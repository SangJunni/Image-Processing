#include<stdio.h>
#include<windows.h>
void Ycbcr(double* Y, double* Cb, double* Cr, unsigned char* inputImg, int i, int j, int stride);
void Ycbcr2RGB(double Y, double Cb, double Cr, double* R, double* G, double* B);
int main()
{
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile1 = NULL, * inputFile2 = NULL,* inputFile3 = NULL;
	inputFile1 = fopen("AICenter.bmp", "rb");
	inputFile2 = fopen("SejongMark.bmp", "rb");
	inputFile3 = fopen("FilteringMask.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg1 = NULL, * outputImg = NULL, * inputImg2 = NULL,* inputImg3 = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg3 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);
	fread(inputImg3, sizeof(unsigned char), size, inputFile3);
	double Y, Cb, Cr, Y1, Y2;
	double I, R, G, B;
	double Diff_Y;
	double a, b;
	//scanf("%lf %lf", &a, &b);
	for (int j = 0; j < height; j++)
	{
		/*for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];//data about pictures of blue image
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];// data about pictures of green image
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];//data about pictures of red image
		}
		for (int i = 0; i < width; i++)//for ICbCr and HSI
		{
			Ycbcr(&Y, &Cb, &Cr, inputImg, i, j, stride);
			Ycbcr2RGB(Y, Cb, Cr, &R, &G, &B);
			I = (inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 1] + inputImg[j * stride + 3 * i + 0]) / 3;
			//Diff_Y = (inputImg1[j * stride + 3 * i + 2] - inputImg[j * stride + 3 * i + 2]) * (inputImg1[j * stride + 3 * i + 2] - inputImg[j * stride + 3 * i + 2]);
			Diff_Y = (Y - I) * (Y - I);
			Diff_Y = (Diff_Y > 255) ? 255 : Diff_Y;
			outputImg[j * stride + 3 * i + 0] = (unsigned char)Diff_Y;
			outputImg[j * stride + 3 * i + 1] = (unsigned char)Diff_Y;
			outputImg[j * stride + 3 * i + 2] = (unsigned char)Diff_Y;
		}*/
		//--------------------------------------------------------------------------------------------------------------------
		/*for (int i = 0; i < width; i++)
		{
			Y1 = 0.299 * inputImg1[j * stride + 3 * i + 2] + 0.587 * inputImg1[j * stride + 3 * i + 1] + 0.114 * inputImg1[j * stride + 3 * i + 0];
			Y2 = 0.299 * inputImg2[j * stride + 3 * i + 2] + 0.587 * inputImg2[j * stride + 3 * i + 1] + 0.114 * inputImg2[j * stride + 3 * i + 0];
			Y = Y1 / a + Y2 / b;
			Y = (Y > 255) ? 255 : Y;
			outputImg[j * stride + 3 * i + 0] = (unsigned char)Y;
			outputImg[j * stride + 3 * i + 1] = (unsigned char)Y;
			outputImg[j * stride + 3 * i + 2] = (unsigned char)Y;
		}*/
		for (int i = 0; i < width; i++)//mask1
		{
			inputImg3[j * stride + 3 * i + 0] = inputImg3[j * stride + 3 * i + 0] == 255 ? 1 : 0;
			outputImg[j * stride + 3 * i + 0] = (inputImg1[j * stride + 3 * i + 0] * inputImg3[j * stride + 3 * i + 0]);
				outputImg[j * stride + 3 * i + 1] = (inputImg1[j * stride + 3 * i + 1] * inputImg3[j * stride + 3 * i + 0]);
				outputImg[j * stride + 3 * i + 2] = (inputImg1[j * stride + 3 * i + 2] * inputImg3[j * stride + 3 * i + 0]);
		}
		/*for (int i = 0; i < width; i++)//mask2
		{
			inputImg3[j * stride + 3 * i + 0] = inputImg3[j * stride + 3 * i + 0] == 255 ? 0 : 1;
			outputImg[j * stride + 3 * i + 0] = (inputImg1[j * stride + 3 * i + 0] * inputImg3[j * stride + 3 * i + 0]);
			outputImg[j * stride + 3 * i + 1] = (inputImg1[j * stride + 3 * i + 1] * inputImg3[j * stride + 3 * i + 0]);
			outputImg[j * stride + 3 * i + 2] = (inputImg1[j * stride + 3 * i + 2] * inputImg3[j * stride + 3 * i + 0]);
		}*/
	}
	FILE* outputFile = fopen("Output_Mask1.bmp", "wb");//Output: copy pictures, Output_RED: red pictures, Output_Blue: blue pictures, Output_GREEN: green pictures
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg1);
	free(inputImg2);
	fclose(inputFile1);
	fclose(inputFile2);
	fclose(outputFile);
}
void Ycbcr2RGB(double Y, double Cb, double Cr, double *R, double *G, double *B)
{
	*R = ((Y + 1.402 * Cr) > 255) ? 255 : (Y + 1.402 * Cr);//0~255사이 존재하도록 하기 위한 식
	*R = (*R < 0) ? 0 : *R;
	*G = ((Y - 0.714 * Cr - 0.344 * Cb) > 255) ? 255 : (Y - 0.714 * Cr - 0.344 * Cb);
	*G = (*G < 0) ? 0 : *G;
	*B = ((Y + 1.772 * Cb) > 255) ? 255 : (Y + 1.772 * Cb);
	*B = (*B < 0) ? 0 : *B;

}
void Ycbcr(double* Y, double* Cb, double* Cr, unsigned char* inputImg, int i, int j, int stride)
{
	*Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
	*Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
	*Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];
}
