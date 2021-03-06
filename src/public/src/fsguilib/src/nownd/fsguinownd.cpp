/* ////////////////////////////////////////////////////////////

File Name: fsguinownd.cpp
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
#include <string.h>
#include <stddef.h>
#include <time.h>

#include <ysclass.h>

#ifdef _WIN32
	#ifndef _WINSOCKAPI_
		// Prevent inclusion of winsock.h
		#define _WINSOCKAPI_
		#include <windows.h>
		#undef _WINSOCKAPI_
	#else
		// Too late.  Just include it.
		#include <windows.h>
	#endif
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif


#include <ysbitmap.h>
#include "fsgui.h"
#include "fsguicommondrawing.h"


void FsGuiCommonDrawing::DrawLine(double,double,double,double,const YsColor &)
{
}

void FsGuiCommonDrawing::DrawLineStrip(int,const double [],const YsColor &)
{
}

void FsGuiCommonDrawing::DrawRect(double,double,double,double,const YsColor &,YSBOOL)
{
}

void FsGuiCommonDrawing::DrawCircle(double,double,double,const YsColor &,YSBOOL)
{
}

void FsGuiCommonDrawing::DrawPolygon(int,const double [],const YsColor &)
{
}

void FsGuiCommonDrawing::DrawGradationPolygon(int,const double [],const YsColor [])
{
}

void FsGuiCommonDrawing::DrawBmp(const YsBitmap &,double,double)
{
}

void FsGuiCommonDrawing::DrawBmpArray(const YsBitmapArray &,double,double)
{
}

void FsGuiCommonDrawing::DrawMask(const YsColor &,const YsColor &,double,double,double,double)
{
}

void FsGuiCommonDrawing::SetClipRect(double,double,double,double)
{
}

void FsGuiCommonDrawing::ClearClipRect(void)
{
}

