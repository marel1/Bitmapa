#include"BitmapaDllCpp/DllCpp.h"
#include <iostream>
#include <fstream>
#include<windows.h>

using namespace std;
int matrix[3][3] = {-1,0,1,
					-1,1,1,
					-1,0,1 };
int matrix2[3][3] = { 0,0,0,
					0,0,0,
					0,0,0 };

int SumMatrix = (matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[2][0] + matrix[2][1] + matrix[2][2]);

extern "C" int _stdcall MyProc1(RGB *rgb, RGB *rgb2,  int height,int width);

int main()
{
	ifstream ifs("Zyra.bmp", ios::in | ios::binary);
	if (ifs.is_open() == false)
	{

		return false;
	}

	char* temp = new char[sizeof(BITMAPFILEHEADER)];
	ifs.read(temp, sizeof(BITMAPFILEHEADER));
	BITMAPFILEHEADER*bfh = (BITMAPFILEHEADER*)(temp);

	temp = new char[sizeof(BITMAPINFOHEADER)];
	ifs.read(temp, sizeof(BITMAPINFOHEADER));
	BITMAPINFOHEADER* bih = (BITMAPINFOHEADER*)(temp);

	ifs.seekg(bfh->bfOffBits, ios::beg);
	RGB*rgb = new RGB[bih->biWidth*bih->biHeight];
	RGB*rgb2 = new RGB[bih->biWidth*bih->biHeight];
	if (bih->biWidth % 4 == 0)
	{
		temp = new char[3 * bih->biWidth*bih->biHeight];
		ifs.read(temp, 3 * bih->biWidth*bih->biHeight);
		for (auto i = 0; i < bih->biWidth*bih->biHeight; i++)
		{
			memcpy(&rgb[i], &temp[3 * i], 3);
			memcpy(&rgb2[i], &temp[3 * i], 3);
		}
	}
	else
	{
		auto wyrowanie = 4 - ((bih->biWidth * 3) % 4);
		temp = new char[(3 * bih->biWidth + wyrowanie)*bih->biHeight];
		ifs.read(temp, (3 * bih->biWidth+wyrowanie)*bih->biHeight);
		for (auto y = 0; y < bih->biHeight; y++)
		{
			for (auto x = 0; x < bih->biWidth; x++)
			{
				memcpy(&rgb[y*bih->biHeight + x], &temp[(y*bih->biHeight + x) * 3 + y * wyrowanie], 3);
				memcpy(&rgb2[y*bih->biHeight + x], &temp[(y*bih->biHeight + x) * 3 + y * wyrowanie], 3);
			}
		}
	}
	
	/*int width = bih->biWidth;
	if (width % 4) width += 4 - (width % 4); // piksele w bitmapie s¹ wyrównywane do 4 bajtów}

	temp = new char[3*bih->biHeight*width];
	ifs.read(temp, 3*bih->biHeight*width);

	RGB*rgb = (RGB*)(temp);

	char*temp2= new char[3 * bih->biHeight*width];
	for (int i = 0; i < 3 * bih->biHeight*width; i++)
	{
		temp2[i] = temp[i];
	}
	RGB*rgb2 = (RGB*)(temp2);*/
	ifs.close();

	//miejsce na operacje na bitmapie
	//do_smth(rgb,rgb2, matrix,bih, SumMatrix);
	//MyProc1(rgb, rgb2, bih->biWidth, bih->biHeight);
	//zapis do 
	BITMAPFILEHEADER file = {bfh->bfType,54+bih->biWidth*bih->biHeight*4,bfh->bfReserved1,bfh->bfReserved2,bfh->bfOffBits};
	BITMAPINFOHEADER info = {bih->biSize,bih->biWidth,bih->biHeight,bih->biPlanes,32,bih->biCompression,bih->biSizeImage,bih->biXPelsPerMeter,bih->biYPelsPerMeter,bih->biClrUsed,bih->biClrImportant};
	ofstream ofs("Test_programu.bmp", ios::binary);
	ofs.write((const char*)&file, sizeof(BITMAPFILEHEADER));
	ofs.write((const char*)&info, sizeof(BITMAPINFOHEADER));
	unsigned char alpha = 255;
	for (auto y = 0; y < bih->biHeight; y++)
	{
		for (auto x = 0; x < bih->biWidth; x++)
		{
			ofs.write((char*)&rgb[y*bih->biWidth + x], 1);
			ofs.write((char*)&rgb[y*bih->biWidth + x], 1);
			ofs.write((char*)&rgb[y*bih->biWidth + x], 1);
			ofs.write((char*)&alpha, 1);
		}
	}


	/*temp = (char*)(rgb2);
	ofs.write(temp2, 3*bih->biHeight*width);*/
	system("pause");
	return 0;
}