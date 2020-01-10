#include"BitmapaDllCpp/DllCpp.h"
#include <iostream>
#include <fstream>
#include<windows.h>

using namespace std;

uint16_t matrix[3][3] = {-1,0,1,
					-1,1,1,
					-1,0,1 };
uint16_t matrix2[3][3] = { 0,0,0,
					0,0,0,
					0,0,0 };

int SumMatrix = (matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[2][0] + matrix[2][1] + matrix[2][2]);

extern "C" int _stdcall MyProc1(RGB *rgb, RGB *rgb2, uint16_t matrix[3][3], uint16_t height,int width, int SumMatrix);

int main()
{
	ifstream ifs("tak.bmp", ios::in | ios::binary);
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
	
	int width = bih->biWidth;
	if (width % 4) width += 4 - (width % 4); // piksele w bitmapie s¹ wyrównywane do 4 bajtów}

	temp = new char[3*bih->biHeight*width];
	ifs.read(temp, 3*bih->biHeight*width);

	RGB*rgb = (RGB*)(temp);

	char*temp2= new char[3 * bih->biHeight*width];
	for (int i = 0; i < 3 * bih->biHeight*width; i++)
	{
		temp2[i] = temp[i];
	}
	RGB*rgb2 = (RGB*)(temp2);
	ifs.close();

	//miejsce na operacje na bitmapie
	//do_smth(rgb,rgb2, matrix,bih, SumMatrix);
	MyProc1(rgb, rgb2, matrix, bih->biHeight,bih->biWidth, SumMatrix);
	//zapis do pliku
	ofstream ofs("Test_programu.bmp", ios::binary);
	temp = (char*)(bfh);
	ofs.write(temp, sizeof(BITMAPFILEHEADER));

	temp = (char*)(bih);
	ofs.write(temp, sizeof(BITMAPINFOHEADER));

	temp = (char*)(rgb2);
	ofs.write(temp2, 3*bih->biHeight*width);
	system("pause");
	return 0;
}