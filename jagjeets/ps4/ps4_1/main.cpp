#include "simplebitmap.h"
#include <string>
#include <fstream>
#include <iostream>
#include "yspngenc.h"
using namespace std;


int main(int argc, char *argv[])
{
	int wid = 40;
	int hei = 40;
	SimpleBitmap sbp;
	
	if (argv[1]==NULL)
	{
		printf("\nUsage: ps4_1 <pngFileName.png>\n");
		return 0;
	}
	bool read = sbp.LoadPng(argv[1]);
	if (read == false)
	{
		printf("\nError: Failed to read a .PNG file.\n");
		return 0;	
	}
	int fileCounter = 0;

	for (int iy=0; iy<sbp.GetHeight(); iy+=hei)
	{
		for (int ix=0; ix<sbp.GetWidth(); ix+=wid)
		{

			FILE *pFile;
			auto cut = sbp.CutOut(ix, iy, wid, hei);
			std::string file_name = std::to_string(fileCounter)+".png";
			char *name = const_cast<char*>(file_name.c_str());
 			pFile = fopen(name,"wb");
			cut.SavePng(pFile);
			fileCounter++;
			if (fileCounter==200)
			{
				return 0;
			}
		}
	}
	return 0;
}