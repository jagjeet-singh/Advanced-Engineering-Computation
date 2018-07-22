#include <windows.h>

#include "comport.h"



class YsCOMPort::PortHandle
{
public:
	HANDLE hComm;
};




/* static */ std::vector <int> YsCOMPort::FindAvailablePort(void)
{
	std::vector <int> available;
	for(int port=1; port<10; ++port)
	{
		char fn[256];
		sprintf(fn,"COM%d",port);
		printf("Testing %s\n",fn);

		HANDLE hComm=CreateFile(
			fn,
			GENERIC_READ|GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if(INVALID_HANDLE_VALUE!=hComm)
		{
			printf("Found Port %d.\n",port);
			available.push_back(port);
			CloseHandle(hComm);
		}
	}
	return available;
}
bool YsCOMPort::Open(int portNumber)
{
	Close();

	char fn[256];
	sprintf(fn,"COM%d",portNumber);

	HANDLE hComm=CreateFile(
		fn,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if(INVALID_HANDLE_VALUE==hComm)
	{
		return false;
	}

	this->portNumber=portNumber;
	this->portPtr=new PortHandle;
	this->portPtr->hComm=hComm;


	// Thanks Mr. Apollo!
	// FM77AV keyboard takes 12.5us on and 12.5us off cycle.
	// One on or off cycle needs two bytes of data.
	// 16 bits for 12.5us -> 80000x16=1280000 bits-per-second.
	// Win32 API tells it can go up to 1600000 bits-per-second, but the question is if it is real??

	DCB dcb;
	if(TRUE==GetCommState(hComm,&dcb))
	{
		// Set up polling-based operation.
		// Based on the information
		// https://groups.google.com/forum/#!topic/comp.os.ms-windows.programmer.win32/SotVc2_Eiig
		COMMTIMEOUTS timeOut;
		ZeroMemory(&timeOut,sizeof(timeOut));
		timeOut.ReadIntervalTimeout=MAXDWORD;
		timeOut.ReadTotalTimeoutMultiplier=0;
		timeOut.ReadTotalTimeoutConstant=0;
		timeOut.WriteTotalTimeoutMultiplier=1000;
		timeOut.WriteTotalTimeoutConstant=0;
		SetCommTimeouts(hComm,&timeOut);
		printf("Set up for polling-mode.  I need to be fast!\n");

		printf("Default Baud Rate=%d\n",dcb.BaudRate);

		dcb.BaudRate=921600;
		if(0==SetCommState(hComm,&dcb))
		{
			printf("Failed to set 921600 bits-per-second.\n");
		}

		dcb.BaudRate=1280000;
		if(0==SetCommState(hComm,&dcb))
		{
			printf("Failed to set 1280000 bits-per-second.\n");
		}

		dcb.BaudRate=1600000;
		if(0==SetCommState(hComm,&dcb))
		{
			printf("Failed to set 1600000 bits-per-second.\n");
		}

		GetCommState(hComm,&dcb);
		printf("Speed set to %d bits-per-second.\n",dcb.BaudRate);
	}
	else
	{
		Close();
		return false;
	}

	return true;
}

void YsCOMPort::Close(void)
{
	if(nullptr!=portPtr)
	{
		CloseHandle(portPtr->hComm);
		delete portPtr;
		portPtr=nullptr;
		portNumber=-1;
	}
}

bool YsCOMPort::Update(void)
{
	return true;
}

long long int YsCOMPort::Send(long long int nDat,const unsigned char dat[])
{
	if(nullptr!=this->portPtr && INVALID_HANDLE_VALUE!=this->portPtr->hComm)
	{
		DWORD byteWritten=0;
		WriteFile(this->portPtr->hComm,dat,(DWORD)nDat,&byteWritten,NULL);
		return byteWritten;
	}
	return 0;
}

void YsCOMPort::FlushWriteBuffer(void)
{
	if(nullptr!=this->portPtr && INVALID_HANDLE_VALUE!=this->portPtr->hComm)
	{
		FlushFileBuffers(this->portPtr->hComm);
	}
}

std::vector <unsigned char> YsCOMPort::Receive(void)
{
	std::vector <unsigned char> dat;

	if(nullptr!=this->portPtr && INVALID_HANDLE_VALUE!=this->portPtr->hComm)
	{
		const int maxNByte=256;
		unsigned char readBuf[maxNByte];

		for(;;)
		{
			DWORD byteRead=0;
			if(TRUE==ReadFile(this->portPtr->hComm,readBuf,maxNByte,&byteRead,NULL))
			{
				for(int i=0; i<byteRead; ++i)
				{
					dat.push_back(readBuf[i]);
				}
			}
			else
			{
				break;
			}
			if(0==byteRead)
			{
				break;
			}
		}
	}

	return dat;
}
