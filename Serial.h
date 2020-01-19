#pragma once

#include <string>
#include <windows.h>

typedef std::basic_string<TCHAR> tstring;

/** Serial.h
 *
 * A very simple serial port control class that does NOT require MFC/AFX.
 *
 * License: This source code can be used and/or modified without restrictions.
 * It is provided as is and the author disclaims all warranties, expressed
 * or implied, including, without limitation, the warranties of
 * merchantability and of fitness for any purpose. The user must assume the
 * entire risk of using the Software.
 *
 * @author Hans de Ruiter
 *
 * @version 0.1 -- 28 October 2008
 */
class Serial
{
private:
	HANDLE commHandle;

public:
	Serial();

	virtual ~Serial();

  bool open (std::string commPortName, int bitRate = 115200);
	bool is_open();

  /** Writes a string of bytes to the serial port.
	 *
	 * @param buffer pointer to the buffer containing the bytes
	 * @param buffLen the number of bytes in the buffer
	 *
	 * @return int the number of bytes written
	 */
	int write(const char *buffer, int buffLen);

	/** Reads a string of bytes from the serial port.
	 *
	 * @param buffer pointer to the buffer to be written to
	 * @param buffLen the size of the buffer
	 *
	 * @return int the number of bytes read
	 */
	int read(char *buffer, int buffLen);

	// Flushes everything from the serial port's read buffer
	void flush();
};
