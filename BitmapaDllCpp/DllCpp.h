#pragma once
#include<windows.h>
#pragma pack(push, 1)
struct RGB
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a{ 0xFF }; //uzupe³nienie do 4
	
};
#pragma pack(pop)
void do_smth(RGB *rgb, RGB *rgb2,int matrix[3][3], int biWidth, int biHeight, int SumMatrix);