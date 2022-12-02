///////////////////////////////////////////////////////////////////
// Prepared for BCIT ELEX4618, April 2022, by Craig Hennessey
///////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream>

#include "Client.h"

#ifdef WIN4618
#pragma comment(lib, "ws2_32.lib")
#endif

#define BUFF_SIZE 65535

CClient::CClient()
{
  _socket = 0;

#ifdef WIN4618
  if (WSAStartup(0x0101, &_wsdat))
  {
    WSACleanup();
    return;
  }
#endif
}

bool CClient::setblocking(SOCKET fd, bool blocking)
{
  if (fd < 0) return false;

#ifdef WIN4618
  unsigned long mode = blocking ? 0 : 1;
  return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) return false;
  flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
  return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

void CClient::connect_socket(std::string addr, int port)
{
  sockaddr_in ipaddr;

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket == SOCKET_ERROR)
  {
#ifdef WIN4618
    WSACleanup();
#endif
    return;
  }

  if (setblocking(_socket, false) == false)
  {
#ifdef WIN4618
    WSACleanup();
#endif
    return;
  }

  ipaddr.sin_family = AF_INET;
  ipaddr.sin_port = htons(port);
  ipaddr.sin_addr.s_addr = inet_addr(addr.c_str());

  connect(_socket, (struct sockaddr*)&ipaddr, sizeof(ipaddr));
}

void CClient::close_socket()
{
#ifdef WIN4618
  closesocket(_socket);
#endif
#ifdef PI4618
  close(_socket);
#endif

  _socket = 0;
}

CClient::~CClient()
{
  close_socket();

#ifdef WIN4618
  WSACleanup();
#endif
}

void CClient::tx_str (std::string str)
{
  send(_socket, str.c_str(), str.length(), 0);
}

bool CClient::rx_str (std::string &str)
{
	char rxbuff[BUFF_SIZE];
  int rxbytes = 0;
  double start_time = cv::getTickCount();

  do
  {
    rxbytes = recv(_socket, rxbuff, BUFF_SIZE - 1, 0); // recvfrom

    if (rxbytes > 0)
    {
      rxbuff[rxbytes] = 0; // Add NULL
      str = rxbuff;
      return true;
    }
  }
  while (rxbytes == -1 && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0);  // Timeout after 1 second

  return false;
}

bool CClient::rx_im(cv::Mat &im)
{
	char rxbuff[BUFF_SIZE];
	int rxbytes = -1;
  int imagebytes = 0;
  int minimum_image_size = 5000;

  double start_time = cv::getTickCount();

  // Store incoming data into byte array
  do
	{
		rxbytes = recv(_socket, rxbuff, BUFF_SIZE, 0);
	} 
  while (rxbytes == -1 && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0);  // Timeout after 1 second

  std::cout << "\nRXbytes = " << rxbytes;

  // If all the bytes were recieved, decode JPEG data to image (assumes image size minimum is 5kB)
	if (rxbytes > minimum_image_size)
	{
		im = imdecode(cv::Mat(rxbytes, 1, CV_8U, rxbuff), cv::IMREAD_UNCHANGED);
		return true;
	}

	return false;
}
