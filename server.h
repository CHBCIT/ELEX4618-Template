///////////////////////////////////////////////////////////////////
// Prepared for BCIT ELEX4618 by Craig Hennessey
///////////////////////////////////////////////////////////////////
#pragma once

#define WIN4618
//#define PI4618

#include <iostream>
#include <string>
#include <mutex>
#include <vector>

#ifdef PI4618
#include <opencv2/opencv.hpp>
#endif

class Server
{
private:
  bool _exit;
  cv::Mat _txim;
  
  std::mutex _immutex;
  std::mutex _cmdmutex;
  std::vector<std::string> _cmd_list;
  
public:
  Server();
	~Server();

  // Start server listening (probably best to do in a separate thread)
  void start(int port);

  // Set the image to transmit
  void set_txim (cv::Mat &im);

  // Get a vector of the commands the server has read
  void get_cmd (std::vector<std::string> &cmds);
};


