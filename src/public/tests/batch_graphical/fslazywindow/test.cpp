/* ////////////////////////////////////////////////////////////

File Name: test.cpp
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
#include <stdlib.h>

class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
	bool testCompleted;
	int nFail;

	int intervalCount; // Let's wait for a few iterations.
	mutable bool passedBeforeEverything;
	mutable bool passedGetOpenWindowOption;
	mutable bool passedInitialize;
	mutable bool passedInterval;
	mutable bool passedBeforeTerminate;
	mutable bool passedNeedRedraw;
	mutable bool passedDraw;


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
	testCompleted=false;
	nFail=0;

	intervalCount=0;
	passedBeforeEverything=false;
	passedGetOpenWindowOption=false;
	passedInitialize=false;
	passedInterval=false;
	passedBeforeTerminate=false;
	passedNeedRedraw=false;
	passedDraw=false;
}

/* virtual */ void FsLazyWindowApplication::BeforeEverything(int argc,char *argv[])
{
	passedBeforeEverything=true;
}
/* virtual */ void FsLazyWindowApplication::GetOpenWindowOption(FsOpenWindowOption &opt) const
{
	opt.x0=0;
	opt.y0=0;
	opt.wid=320;
	opt.hei=240;

	passedGetOpenWindowOption=true;
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{
	passedInitialize=true;
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	++intervalCount;
	if(5<=intervalCount)
	{
		testCompleted=true;
	}

	auto key=FsInkey();
	if(FSKEY_ESC==key || true==testCompleted)
	{
		SetMustTerminate(true);
	}
	needRedraw=true;

	passedInterval=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	needRedraw=false;
	passedDraw=true;
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
	passedBeforeTerminate=true;


	if(true!=passedBeforeEverything)
	{
		++nFail;
	}
	if(true!=passedGetOpenWindowOption)
	{
		++nFail;
	}
	if(true!=passedInitialize)
	{
		++nFail;
	}
	if(true!=passedInterval)
	{
		++nFail;
	}
	if(true!=passedBeforeTerminate)
	{
		++nFail;
	}
	if(true!=passedNeedRedraw)
	{
		++nFail;
	}
	if(true!=passedDraw)
	{
		++nFail;
	}


	if(0<nFail)
	{
		exit(1);
	}
	exit(0);
}
/* virtual */ bool FsLazyWindowApplication::NeedRedraw(void) const
{
	passedNeedRedraw=true;
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
