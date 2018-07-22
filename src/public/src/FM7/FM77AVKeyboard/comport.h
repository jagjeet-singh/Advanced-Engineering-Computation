#ifndef COMPORT_IS_INCLUDED
#define COMPORT_IS_INCLUDED
/* { */



#include <vector>

class YsCOMPort
{
public:
	class PortHandle;

private:
	/*  Platform-dependent code must be written so that portPtr is nullptr when it is disconnected.
	*/
	PortHandle *portPtr;
	std::vector <unsigned char> sendBuf;
	std::vector <unsigned char> receiveBuf;
	int portNumber;

public:
	YsCOMPort();
	~YsCOMPort();
	void CleanUp(void);

	static std::vector <int> FindAvailablePort(void);

	/*! 
	*/
	bool Open(int portNumber);

	/*! Returns true if port is connected.
	*/
	bool IsConnected(void) const;

	/*! Returns connected port number.
	*/
	int GetPort(void) const;

	/*! 
	*/
	void Close(void);

	/*! Call this function regularly to update the COM port status.
	*/
	bool Update(void);

	/*! Sends byte data and returns number of bytes sent.
	    It may just have copied data to the send buffer, 
	    in which case the return value will be the number of bytes copied to the send buffer.
	*/
	long long int Send(long long int nDat,const unsigned char dat[]);

	/*! Flush the write-buffer.
	*/
	void FlushWriteBuffer(void);

	/*! Reads byte data from the port.
	*/
	std::vector <unsigned char> Receive(void);
};



/* } */
#endif
