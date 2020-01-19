/** Serial.cpp
 *
 * A very simple serial port control class that does NOT require MFC/AFX.
 *
 * @author Hans de Ruiter
 *
 * @version 0.1 -- 28 October 2008
 */

#include "stdafx.h"
#include <iostream>

using namespace std;

#include "Serial.h"

#define FLUSH_BUFFSIZE 10

std::wstring s2ws(const std::string& s)
{
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
 wchar_t* buf = new wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 return r;
}

Serial::Serial()
{
}

bool Serial::open(string commPortName, int bitRate)
{
  commPortName = "\\\\.\\" + commPortName;

	commHandle = CreateFile(commPortName.c_str(), GENERIC_READ|GENERIC_WRITE, 0,NULL, OPEN_EXISTING, 0, NULL);

	if(commHandle == INVALID_HANDLE_VALUE) 
	{
		//throw("ERROR: Could not open com port");
	  return false;
  }
	else 
	{
		// set timeouts
		COMMTIMEOUTS cto = { MAXDWORD, 0, 0, 0, 0};
		DCB dcb;
		if(!SetCommTimeouts(commHandle,&cto))
		{
			Serial::~Serial();
			//throw("ERROR: Could not set com port time-outs");
      return false;
		}

		// set DCB
		memset(&dcb,0,sizeof(dcb));
		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = bitRate;
		dcb.fBinary = 1;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;

		if(!SetCommState(commHandle,&dcb))
		{
			Serial::~Serial();
			//throw("ERROR: Could not set com port parameters");
      return false;
		}
	}

  return true;
}

Serial::~Serial()
{
	CloseHandle(commHandle);
}

bool Serial::is_open()
{
	if (commHandle == NULL)
	{
		return false;
	}

	return true;
}

int Serial::write(const char *buffer, int buffLen)
{
	DWORD numWritten;
	WriteFile(commHandle, buffer, buffLen, &numWritten, NULL); 

	return numWritten;
}

int Serial::read(char *buffer, int buffLen)
{
	DWORD numRead;

	BOOL ret = ReadFile(commHandle, buffer, buffLen, &numRead, NULL);

	if(!ret)
	{
		return 0;
	}

	return numRead;
}

void Serial::flush()
{
	char buffer[FLUSH_BUFFSIZE];
	int numBytes = read(buffer, FLUSH_BUFFSIZE);
	while(numBytes != 0)
	{
		numBytes = read(buffer, FLUSH_BUFFSIZE);
	}
}
