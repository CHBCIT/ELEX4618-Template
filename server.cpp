///////////////////////////////////////////////////////////////////
// Prepared for BCIT ELEX4618, May 2017, by Craig Hennessey
///////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "server.h"

#ifdef WIN4618
#include "Winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef PI4618
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#define RECV_BUFF_SIZE 256

#define BACKLOG 5
#define BUFFER 16000

bool setblocking(int fd, bool blocking)
{
   if (fd < 0) return false;

#ifdef WIN4618
   unsigned long mode = blocking ? 0 : 1;
   return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
   int flags = fcntl(fd, F_GETFL, 0);
   if (flags < 0) return false;
   flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

Server::Server()
{
  _txim = cv::Mat::zeros(10,10,CV_8UC3);
}

Server::~Server()
{
  _exit = true;
  cv::waitKey(150);
}

void Server::set_txim (cv::Mat &im)
{
  if (im.empty() == false)
  {
    _immutex.lock();
    im.copyTo(_txim);
    _immutex.unlock();
  }
}

void Server::start(int port)
{
  cv::Mat frame;

  // Image compression parameters
  std::vector<unsigned char> image_buffer;
  std::vector<int> compression_params;
  compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
  compression_params.push_back(30); // 1 to 100 (100 = highest quality/largest image)

  int ret;
  struct sockaddr_in server_addr, client_addr;
  SOCKET serversock = 0;
  SOCKET clientsock = 0;
 
#ifdef WIN4618
  WSADATA wsdat;
 int addressSize = sizeof(server_addr);
#endif

#ifdef PI4618
  unsigned int addressSize = sizeof(server_addr);
#endif

  char buff[BUFFER + 1]; // +1 for null

#ifdef WIN4618
  if (WSAStartup(0x0101, &wsdat))
  {
    WSACleanup();
    return;
  }
#endif

  serversock = socket(AF_INET, SOCK_STREAM, 0);
  if (serversock == SOCKET_ERROR)
  {
#ifdef WIN4618
    WSACleanup();
#endif
    std::cout << "Server Exit: socket error";
    return;
  }

  if (setblocking(serversock, false) == SOCKET_ERROR)
  {
    std::cout << "Server Exit: failed to set non-blocking";
#ifdef WIN4618
    WSACleanup();
#endif

    return;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(serversock, (sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
  {
#ifdef WIN4618
    closesocket(serversock);
    WSACleanup();
#endif
#ifdef PI4618
    close(serversock);
#endif  
    std::cout << "Server Exit: bind error";
    return;
  }

  listen(serversock, BACKLOG);

  while (_exit == false)
  {
    cv::waitKey(100);
    
    clientsock = accept(serversock, (struct sockaddr *) &client_addr, &addressSize);

    if (clientsock != INVALID_SOCKET)
    {
      setblocking(clientsock, false);
      
      do
      {
        ret = recv(clientsock, buff, BUFFER, 0);

        // If socket was shut down orderly (client disconnected)
        if (ret == 0)
        {
#ifdef PI4618
          close(clientsock);
#endif
#ifdef WIN4618
          closesocket(clientsock);
#endif
          clientsock = INVALID_SOCKET;
        }
        // Else some other error
        else if (ret == SOCKET_ERROR)
        {
#ifdef PI4618
          //unsigned int len = sizeof(errno);
          //getsockopt(clientsock, SOL_SOCKET, SO_ERROR, &errno, &len);

          if (errno == EWOULDBLOCK)
          {
            // no data to recieve, go check again
          }
          else
          {
            std::cout << "\nSO_ERROR";
            close(clientsock);
            clientsock = INVALID_SOCKET;
          }
#endif

#ifdef WIN4618
          if (WSAGetLastError() == WSAEWOULDBLOCK)
          { 
            // no data to recieve, go check again
          } 
          else
          {
            closesocket(clientsock);
            clientsock = INVALID_SOCKET;
          }
#endif
        }
        else
				{
					if (ret < BUFFER)
					{
						// Add NULL terminator to string
						buff[ret] = 0;

						// Processing incoming data
						std::string str = buff;
						//std::cout << "\nServer RX: " << str;

						// The client sent "im" as a message
						if (str == "im")
						{
							std::cout << "\nReceived 'im' command";

							_immutex.lock();
							_txim.copyTo(frame);
							_immutex.unlock();

							image_buffer.clear();
							if (frame.empty() == false)
							{
								// Compress image to reduce size
								cv::imencode("image.jpg", frame, image_buffer, compression_params);
							}

							// Send image
							send(clientsock, reinterpret_cast<char*>(&image_buffer[0]), image_buffer.size(), 0);
						}
						// The client sent a message, add to cmd list queue
						else if (str == "a")
						{
							std::cout << "\nReceived 'a' command";

							// Remove the following two lines in the final version
							std::string reply = "Hi there from Server";
							send(clientsock, reply.c_str(), reply.length(), 0);
						}
						else
						{
							_cmdmutex.lock();
							_cmd_list.push_back(str);
							_cmdmutex.unlock();

							std::cout << "\nReceived other command";

							// Remove the following two lines in the final version
							std::string reply = "Response to other command";
							send(clientsock, reply.c_str(), reply.length(), 0);
						}
					}
        }
      } 
      while (clientsock != INVALID_SOCKET && _exit != true);
    }
  }

#ifdef WIN4618
  closesocket(serversock);
  WSACleanup();
#endif

#ifdef PI4618
  close(serversock);
#endif  
}

void Server::get_cmd (std::vector<std::string> &cmds)
{
  cmds.clear();
  
  // Copy command list into return list
  _cmdmutex.lock();
  _cmd_list.swap(cmds);
  _cmdmutex.unlock();
}

