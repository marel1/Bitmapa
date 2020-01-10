#include "DllCpp.h"
void do_smth(RGB *rgb, RGB *rgb2,int matrix[3][3],BITMAPINFOHEADER*bih, int SumMatrix)
{
	int sr, sg, sb;
	for (int j = 1; j < bih->biHeight - 1; j++) {
	for (int i = 1; i < bih->biWidth - 1; i++) {
			sr = sg = sb = 0;

			sr = matrix[0][0] * rgb[(j-1)*bih->biWidth+(i-1)].r + matrix[0][1] * rgb[j*bih->biWidth+i-1].r + matrix[0][2] * rgb[(j+1)*bih->biWidth + i - 1].r +
				matrix[1][0] * rgb[(j - 1)*bih->biWidth + i].r + matrix[1][1] * rgb[j*bih->biWidth + i].r + matrix[1][2] * rgb[(j + 1)*bih->biWidth + i].r +
				matrix[2][0] * rgb[(j - 1)*bih->biWidth + i + 1].r + matrix[2][1] * rgb[j*bih->biWidth + i+1].r + matrix[2][2] * rgb[(j + 1)*bih->biWidth + i+1].r;

			sg = matrix[0][0] * rgb[(j - 1)*bih->biWidth + i - 1].g + matrix[0][1] * rgb[j*bih->biWidth + i - 1].g + matrix[0][2] * rgb[(j + 1)*bih->biWidth + i - 1].g +
				matrix[1][0] * rgb[(j - 1)*bih->biWidth + i].g + matrix[1][1] * rgb[j*bih->biWidth + i].g + matrix[1][2] * rgb[(j + 1)*bih->biWidth + i].g +
				matrix[2][0] * rgb[(j - 1)*bih->biWidth + i + 1].g + matrix[2][1] * rgb[j*bih->biWidth + i + 1].g + matrix[2][2] * rgb[(j + 1)*bih->biWidth + i + 1].g;

			sb = matrix[0][0] * rgb[(j - 1)*bih->biWidth + i - 1].b + matrix[0][1] * rgb[j*bih->biWidth + i - 1].b + matrix[0][2] * rgb[(j + 1)*bih->biWidth + i - 1].b +
				matrix[1][0] * rgb[(j - 1)*bih->biWidth + i].b + matrix[1][1] * rgb[j*bih->biWidth + i].b + matrix[1][2] * rgb[(j + 1)*bih->biWidth + i].b +
				matrix[2][0] * rgb[(j - 1)*bih->biWidth + i + 1].b + matrix[2][1] * rgb[j*bih->biWidth + i + 1].b + matrix[2][2] * rgb[(j + 1)*bih->biWidth + i + 1].b;
			
			if (sr > 255) sr = 255;
			else if (sr < 0) sr = 0;
			if (sg > 255) sg = 255;
			else if (sg < 0) sg = 0;
			if (sb > 255) sb = 255;
			else if (sb < 0) sb = 0;
			
			rgb2[j*bih->biWidth + i].r = sr / SumMatrix;
			rgb2[j*bih->biWidth + i].g = sg / SumMatrix;
			rgb2[j*bih->biWidth + i].b = sb / SumMatrix;
		}

	}
}