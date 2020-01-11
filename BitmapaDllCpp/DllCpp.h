#pragma once
#include<windows.h>
#pragma pack(push, 1)
struct RGB
{
	unsigned char b, g, r, a{0xFF}; //uzupe³nienie do 4
	
};
#pragma pack(pop)
void do_smth(RGB *rgb, RGB *rgb2,int matrix[3][3], BITMAPINFOHEADER* bih, int SumMatrix);