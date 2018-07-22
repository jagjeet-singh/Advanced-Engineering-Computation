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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fslazywindow.h>
#include <ysglfontdata.h>


#include "bintree.h"


class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
	BinaryTree <int,int> tree;
	BinaryTree <int,int>::NodeHandle highlightNdHd;

	void DrawNode(BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y,int dy,int prevX,int prevY) const;
	BinaryTree <int,int>::NodeHandle GetMouseOnNode(int mx,int my,BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y,int dy) const;

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

void FsLazyWindowApplication::DrawNode(BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y,int dy,int prevX,int prevY) const
{
	if(true==ndHd.IsNull())
	{
		return;
	}

	if(ndHd==highlightNdHd)
	{
		glColor3ub(0,0,255);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x0,y);
		glVertex2i(x1,y);
		glVertex2i(x1,y+dy);
		glVertex2i(x0,y+dy);
		glEnd();
	}

	int cx=(x0+x1)/2;
	int cy=y+dy/2;

	if(true==tree.Up(ndHd).IsNotNull())
	{
		glColor3ub(0,255,0);
		glBegin(GL_LINES);
		glVertex2i(cx,cy);
		glVertex2i(prevX,prevY);
		glEnd();
	}

	DrawNode(tree.Left(ndHd),x0,cx,y+dy,dy,cx,cy);
	DrawNode(tree.Right(ndHd),cx,x1,y+dy,dy,cx,cy);

	char str[256];
	sprintf(str,"%d(%d)",tree.GetKey(ndHd),tree.GetHeight(ndHd));
	glColor3ub(0,0,0);
	glRasterPos2i(cx,cy);
	YsGlDrawFontBitmap8x12(str);
}

BinaryTree <int,int>::NodeHandle FsLazyWindowApplication::GetMouseOnNode(int mx,int my,BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y,int dy) const
{
	if(true==ndHd.IsNull())
	{
		return tree.Null();
	}

	if(x0<=mx && mx<x1 && y<=my && my<y+dy)
	{
		return ndHd;
	}

	int cx=(x0+x1)/2;
	{
		auto ndHdLeft=GetMouseOnNode(mx,my,tree.Left(ndHd),x0,cx,y+dy,dy);
		if(true==ndHdLeft.IsNotNull())
		{
			return ndHdLeft;
		}
	}

	{
		auto ndHdRight=GetMouseOnNode(mx,my,tree.Right(ndHd),cx,x1,y+dy,dy);
		if(true==ndHdRight.IsNotNull())
		{
			return ndHdRight;
		}
	}

	return tree.Null();
}

FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;
	highlightNdHd.Nullify();
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
	srand((int)time(nullptr));
	for(int i=0; i<50; ++i)
	{
		auto key=rand()%100;
		tree.Insert(key,0);
	}
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}

	if(FSKEY_DEL==key && true==highlightNdHd.IsNotNull())
	{
		tree.Delete(highlightNdHd);
		highlightNdHd.Nullify();
	}
	if(FSKEY_SPACE==key)
	{
		auto key=rand()%100;
		tree.Insert(key,0);
	}
	if(FSKEY_L==key && true==highlightNdHd.IsNotNull())
	{
		tree.RotateLeft(highlightNdHd);
	}
	if(FSKEY_R==key && true==highlightNdHd.IsNotNull())
	{
		tree.RotateRight(highlightNdHd);
	}
	if(FSKEY_V==key)
	{
		tree.TreeToVine();
	}
	if(FSKEY_T==key)
	{
		tree.VineToTree();
	}


	int wid,hei;
	FsGetWindowSize(wid,hei);

	int mx,my,lb,mb,rb;
	FsGetMouseEvent(lb,mb,rb,mx,my);
	highlightNdHd=GetMouseOnNode(mx,my,tree.RootNode(),0,wid,0,40);

	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glViewport(0,0,wid,hei);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,wid-1,hei-1,0,-1,1);

	DrawNode(tree.RootNode(),0,wid,0,40,0,0);
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
