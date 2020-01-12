#include"BitmapaDllCpp/DllCpp.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <future>
#include<windows.h>

using namespace std;
int matrix[3][3] = {-1,0,1,
					-1,1,1,
					-1,0,1 };

int SumMatrix = (matrix[0][0] + matrix[0][1] + matrix[0][2] + matrix[1][0] + matrix[1][1] + matrix[1][2] + matrix[2][0] + matrix[2][1] + matrix[2][2]);

extern "C" int _stdcall MyProc1(RGB *rgb, RGB *rgb2,  int width,int height);
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
				memcpy(&rgb[y*bih->biWidth + x], &temp[(y*bih->biWidth + x) * 3 + y * wyrowanie], 3);
			}
		}
	}
	ifs.close();
	int ThreadCount = 0;
	std::cout << "Podaj liczbe watkow od 1 do 64 (inna liczba spowoduje uruchomienie optymalnej liczby watkow): ";
	cin >> ThreadCount;
	if (ThreadCount > 64 || ThreadCount < 1)
	{
		ThreadCount = thread::hardware_concurrency();
	}
	std::cout << "Liczba watkow wynosi: " << ThreadCount << endl;
	auto Futures = new future<void>[ThreadCount]; //stworzenie w¹tków
	auto HeightByThread = bih->biHeight / ThreadCount; //wysokoœc obrazu dla w¹tku
	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	if (ThreadCount != 1)
	{

		/*do_smth(&rgb[0], &rgb2[0], matrix, bih->biWidth, HeightByThread + 1, SumMatrix);
		for (int i = 1; i < ThreadCount - 1; i++)
		{
			do_smth(&rgb[i * (HeightByThread - 1)*bih->biWidth], &rgb2[i * (HeightByThread - 1)*bih->biWidth], matrix, bih->biWidth, HeightByThread + 2, SumMatrix);
		}
		do_smth(&rgb[(ThreadCount - 1) * (HeightByThread - 1)*bih->biWidth], &rgb2[(ThreadCount - 1) * (HeightByThread - 1)*bih->biWidth], matrix, bih->biWidth, HeightByThread + 1, SumMatrix);*/
		Futures[0] = async(launch::async, [=] {do_smth(&rgb [0], &rgb2[ 0], matrix, bih->biWidth, HeightByThread + 1, SumMatrix); });
		for (int i = 1; i < ThreadCount - 1; i++)
		{
			Futures[i] = async(launch::async, [=] {do_smth(&rgb[ i * (HeightByThread - 1)*bih->biWidth], &rgb2[ i * (HeightByThread - 1)*bih->biWidth], matrix, bih->biWidth, HeightByThread + 2, SumMatrix);});
		}
		Futures[ThreadCount - 1] = async(launch::async, [=] {do_smth(&rgb[(ThreadCount - 1) * (HeightByThread - 1)*bih->biWidth],&rgb2 [ (ThreadCount - 1) * (HeightByThread - 1)*bih->biWidth], matrix, bih->biWidth, HeightByThread + 1, SumMatrix); });

		for (auto i = 0; i < ThreadCount; i++)
		{
			Futures[i].wait();
		}
		if (bih->biHeight%ThreadCount != 0)
		{
			do_smth(&rgb[(ThreadCount) * (HeightByThread - 1)*bih->biWidth], &rgb2[(ThreadCount) * (HeightByThread - 1)*bih->biWidth], matrix, bih->biWidth, (bih->biHeight - ThreadCount * HeightByThread)+1, SumMatrix);
		}
	}
	else
	{
		do_smth(&rgb [0], &rgb2 [0], matrix, bih->biWidth, HeightByThread , SumMatrix);
	}

	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	//pocz¹tek liczenia czasu
	//miejsce na operacje na bitmapie
	//do_smth(rgb,rgb2, matrix,bih->biWidth,bih->biHeight, SumMatrix);
	//MyProc1(rgb, rgb2, bih->biWidth, bih->biHeight);
	//do_smth(rgb3 =rgb+0, rgb4=rgb2+0, matrix, bih->biWidth, HeightByThread, SumMatrix);
	//do_smth(rgb3 = rgb+HeightByThread*bih->biWidth, rgb4 =rgb2+ HeightByThread*bih->biWidth, matrix, bih->biWidth, HeightByThread, SumMatrix);
	//do_smth(rgb3 = rgb + 2*HeightByThread * bih->biWidth, rgb4 = rgb2 + 2*HeightByThread * bih->biWidth, matrix, bih->biWidth, HeightByThread, SumMatrix);
	//koniec liczenia czasu
	std::cout << "Czas trwania algorytmu: "
		<< chrono::duration_cast<chrono::microseconds>(end - start).count()
		<< "us.\n";

	//zapis do pliku
	BITMAPFILEHEADER file = {bfh->bfType,54+bih->biWidth*bih->biHeight*4,bfh->bfReserved1,bfh->bfReserved2,bfh->bfOffBits};
	BITMAPINFOHEADER info = {bih->biSize,bih->biWidth,bih->biHeight,bih->biPlanes,32,bih->biCompression,bih->biSizeImage,bih->biXPelsPerMeter,bih->biYPelsPerMeter,bih->biClrUsed,bih->biClrImportant};
	ofstream ofs("Test_programu.bmp", ios::binary);
	ofs.write((const char*)&file, sizeof(BITMAPFILEHEADER));
	ofs.write((const char*)&info, sizeof(BITMAPINFOHEADER));
	for (auto y = 0; y < bih->biHeight; y++)
	{
		for (auto x = 0; x < bih->biWidth; x++)
		{
			ofs.write((char*)&rgb2[y*bih->biWidth + x].b, 1);
			ofs.write((char*)&rgb2[y*bih->biWidth + x].g, 1);
			ofs.write((char*)&rgb2[y*bih->biWidth + x].r, 1);
			ofs.write((char*)&rgb2[y*bih->biWidth + x].a, 1);
		}
	}

	system("pause");
	return 0;
}