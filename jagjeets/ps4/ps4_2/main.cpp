/* ////////////////////////////////////////////////////////////

File Name: main.cpp
Copyright (c) 2017 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include <fslazywindow.h>
#include <stdio.h>
#include "hashtable.h"
#include "simplebitmap.h"
#include <string>
#include <iostream>

using namespace std;

template <>
HashCommon::CodeType 
    HashBase<SimpleBitmap>::HashCode (const SimpleBitmap &key) const
{
	HashTable <SimpleBitmap,int>::CodeType sum=0;
	const unsigned int m[]={5,7,11,13,17,19};
	int counter=0;
	for(int iy=0;iy<key.GetHeight();++iy)
		for(int ix=0; ix<key.GetWidth(); ++ix)
		{	
			auto pixelPtr = key.GetPixelPointer(ix,iy);
			for(int p=0; p<6; p++)
			{
				sum+=(pixelPtr[p])*m[counter%6];
				counter++;
			}
		}
	return sum;
}


class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;

	HashTable <SimpleBitmap,int> table;
	SimpleBitmap sbp;
	int CutWid = 40;
	int CutHei = 40;

public:
	FsLazyWindowApplication();
	virtual void BeforeEverything(int argc,char *argv[]);
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const;
	virtual void Initialize(int argc,char *argv[]);
	virtual void Interval(void);
	virtual void BeforeTerminate(void);
	virtual void Draw(void);
	virtual bool UserWantToCloseProgram(void);
	virtual bool MustTerminate(void) const;
	virtual long long int GetMinimumSleepPerInterval(void) const;
	virtual bool NeedRedraw(void) const;
};

FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;
}

/* virtual */ void FsLazyWindowApplication::BeforeEverything(int argc,char *argv[])
{
}
/* virtual */ void FsLazyWindowApplication::GetOpenWindowOption(FsOpenWindowOption &opt) const
{
	opt.x0=0;
	opt.y0=0;
	opt.wid=1200;
	opt.hei=800;
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{
	sbp.LoadPng(argv[1]);
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}

	int fileCounter = 0;
	for (int iy=0; iy<sbp.GetHeight(); iy+=CutHei)
	{
		for (int ix=0; ix<sbp.GetWidth(); ix+=CutWid)
		{

			auto cut = sbp.CutOut(ix, iy, CutWid, CutHei);
			cut.Invert();
			table.Update(cut,fileCounter);
			fileCounter++;
		}
	}
	printf("Table Size: %lld\n",table.GetN() );

}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	needRedraw=true;

}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	int wid,hei;
    FsGetWindowSize(wid,hei);
	
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	int x = 0;
	int y = CutWid;
	int fileCounter = 0;
	for(auto hd=table.First(); true==hd.IsNotNull(); hd=table.Next(hd))
	{
		auto keyPtr=table[hd];
		if(nullptr!=keyPtr)
		{	

			glRasterPos2i(x,y);
			glDrawPixels(CutWid,CutHei,GL_RGBA,GL_UNSIGNED_BYTE,keyPtr->GetBitmapPointer());
			if (x>(wid-CutWid))
			{
				y+=CutHei;
				x=0;
			}
			else
			{
				x+=CutWid;
			}
		}
			
	}

	FsSwapBuffers();

	needRedraw=false;
}
/* virtual */ bool FsLazyWindowApplication::UserWantToCloseProgram(void)
{
	return true; // Returning true will just close the program.
}
/* virtual */ bool FsLazyWindowApplication::MustTerminate(void) const
{
	return FsLazyWindowApplicationBase::MustTerminate();
}
/* virtual */ long long int FsLazyWindowApplication::GetMinimumSleepPerInterval(void) const
{
	return 10;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
}
/* virtual */ bool FsLazyWindowApplication::NeedRedraw(void) const
{
	return needRedraw;
}


static FsLazyWindowApplication *appPtr=nullptr;

/* static */ FsLazyWindowApplicationBase *FsLazyWindowApplicationBase::GetApplication(void)
{
	if(nullptr==appPtr)
	{
		appPtr=new FsLazyWindowApplication;
	}
	return appPtr;
}
