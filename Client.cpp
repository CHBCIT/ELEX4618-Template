///////////////////////////////////////////////////////////////////
// Prepared for BCIT ELEX4618, April 2017, by Craig Hennessey
///////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream>

#include "Client.h"

#pragma comment(lib, "ws2_32.lib")

#define BUFF_SIZE 65535

Client::Client(int port, std::string addr)
{
  _socket = NULL;

  sockaddr_in ipaddr;
  SOCKET subsock = NULL;

  u_long polling = 1;

  if (WSAStartup(0x0101, &_wsdat))
  {
    WSACleanup();
    return;
  }

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket == SOCKET_ERROR)
  {
    WSACleanup();
    return;
  }

  if (ioctlsocket(_socket, FIONBIO, &polling) == SOCKET_ERROR)
  {
    WSACleanup();
    return;
  }

  ipaddr.sin_family = AF_INET;
  ipaddr.sin_port = htons(port);
  ipaddr.sin_addr.S_un.S_addr = inet_addr(addr.c_str());

  connect(_socket, (struct sockaddr*)&ipaddr, sizeof(ipaddr));  
}

Client::~Client()
{
	if (_socket != NULL) 
	{
		closesocket(_socket);
	}

	Sleep (50);
	WSACleanup();
}

void Client::tx_str (std::string str)
{
  send(_socket, str.c_str(), str.length(), 0);
}

bool Client::rx_str (std::string &str)
{
	char rxbuff[BUFF_SIZE];
	int rxbytes = recv(_socket, rxbuff, BUFF_SIZE-1, 0); // recvfrom
  
  if (rxbytes > 0)
  {
		rxbuff[rxbytes] = 0; // Add NULL
    str = rxbuff;
    return true;
  }

  return false;
}

// Note probably would work just as well in blocking mode and no need for do/while loops (but then could not do auto-reconnect)
bool Client::rx_im(cv::Mat &im)
{
	char rxbuff[BUFF_SIZE];
	int rxbytes = -1;
	float elapsedtime = 0;
	int64 start_time = cv::getTickCount();

	do
	{
		// Store incoming data into byte array
		rxbytes = recv(_socket, rxbuff, BUFF_SIZE, 0);

		elapsedtime = (float)(cv::getTickCount() - start_time) / (float)cv::getTickFrequency();
	} while (rxbytes == 0 && elapsedtime < 1.0);  // Timeout after 1 second

	// If all the bytes were recieved, decode JPEG data to image
	if (rxbytes > 0)
	{
		im = imdecode(cv::Mat(rxbytes, 1, CV_8U, rxbuff), cv::IMREAD_UNCHANGED);
		return true;
	}

	return false;
}
