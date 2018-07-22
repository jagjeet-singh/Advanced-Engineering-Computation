#include "comport.h"



YsCOMPort::YsCOMPort()
{
	portPtr=nullptr;
	portNumber=-1;
}
YsCOMPort::~YsCOMPort()
{
	CleanUp();
}
bool YsCOMPort::IsConnected(void) const
{
	return (nullptr!=portPtr);
}
int YsCOMPort::GetPort(void) const
{
	return portNumber;
}
void YsCOMPort::CleanUp(void)
{
	Close();
	sendBuf.clear();
	receiveBuf.clear();
}
