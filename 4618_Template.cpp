////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated Dec 6, 2021
////////////////////////////////////////////////////////////////
#include "stdafx.h"

// Add simple GUI elements
#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include "Client.h"
#include "Server.h"

// Must include Windows.h after Winsock2.h, so Serial must be included after Client/Server
#include "Serial.h" 

#define CANVAS_NAME "Display Image"

void process_msg()
{
  MSG msg;
  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
}

////////////////////////////////////////////////////////////////
// Serial Communication
////////////////////////////////////////////////////////////////
void test_com()
{
  // Comport class (change port to your MSP device port)
  Serial com;
  com.open("COM3");

  // TX and RX strings
  std::string tx_str = "G 1 15\n";
  std::string rx_str;

  // temporary storage
  char buff[2];
  do
  {
    // Send TX string
		com.write(tx_str.c_str(), tx_str.length());
    Sleep(10); // wait for ADC conversion, etc. May not be needed?
  
    rx_str = "";
    // start timeout count
    double start_time = cv::getTickCount();

    buff[0] = 0;
		// Read 1 byte and if an End Of Line then exit loop
    // Timeout after 1 second, if debugging step by step this will cause you to exit the loop
    while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
    {
      if (com.read(buff, 1) > 0)
      {
        rx_str = rx_str + buff[0];
      }
    }

    printf ("\nRX: %s", rx_str.c_str());
    cv::waitKey(1);
  } 
  while (1);
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
  cv::Mat im;

  im = cv::imread("BCIT.jpg");

  srand(time(0));

  for (int i = 0; i < 500; i++)
  {
    float radius = 50 * rand() / RAND_MAX;
    cv::Point center = cv::Point(im.size().width*rand() / RAND_MAX, im.size().height*rand() / RAND_MAX);
    
    cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);
    
    im.at<char>(i,i) = 255;
    
    cv::imshow(CANVAS_NAME, im);
    cv::waitKey(1);
  }
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
  cv::VideoCapture vid;

  vid.open(0);

  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  bool do_canny = true;
  bool do_aruco = false;
  int canny_thresh = 30;
  cvui::init(CANVAS_NAME);
  cv::Point set_pt = cv::Point(10, 50);
  std::vector<cv::Scalar> color_vec;
  color_vec.push_back(cv::Scalar(255, 255, 255));
  color_vec.push_back(cv::Scalar(255, 0, 0));
  color_vec.push_back(cv::Scalar(0, 255, 0));
  color_vec.push_back(cv::Scalar(0, 0, 255));
  int color_index = 0;
  if (vid.isOpened() == TRUE)
  {
    do
    {
      cv::Mat frame, edges;
      vid >> frame;

      if (frame.empty() == false)
      { 
        if (do_aruco == true)
        {
          std::vector<int> ids;
          std::vector<std::vector<cv::Point2f> > corners;
          cv::aruco::detectMarkers(frame, dictionary, corners, ids);
          if (ids.size() > 0)
          {
            cv::aruco::drawDetectedMarkers(frame, corners, ids);
          }
        }

        if (do_canny == true)
        {
          cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
          cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
          cv::Canny(edges, edges, 0, canny_thresh, 3);
          cv::add(frame, color_vec.at(color_index), frame, edges);
        }

        cvui::window(frame, set_pt.x, set_pt.y, 200, 190, "Settings");
        cvui::checkbox(frame, set_pt.x + 5, set_pt.y + 25, "Canny Filter", &do_canny);
        cvui::checkbox(frame, set_pt.x + 5, set_pt.y + 50, "ArUco", &do_aruco);
        cvui::text(frame, set_pt.x + 5, set_pt.y + 75, "Canny Threshold");
        cvui::trackbar(frame, set_pt.x + 5, set_pt.y + 90, 180, &canny_thresh, 5, 120);
        if (cvui::button(frame, set_pt.x + 5, set_pt.y + 140, 100, 30, "Colour Switch"))
        {
          color_index++;
          if (color_index >= color_vec.size()) { color_index = 0; }
        }

        cvui::update();
        cv::imshow(CANVAS_NAME, frame);
      }
    }
    while (cv::waitKey(10) != ' ');
  }      
}		
    

////////////////////////////////////////////////////////////////
// Demo client server communication
////////////////////////////////////////////////////////////////
bool serverthreadexit = false;
Server serv;

// Send image to TCP server
void serverimagethread()
{
  cv::VideoCapture vid;

  vid.open(0);

  if (vid.isOpened() == true)
  {
    do
    {
      cv::Mat frame;
      vid >> frame;
      if (frame.empty() == false)
      {
        imshow("Server Image", frame);
        process_msg();
        serv.set_txim(frame);
      }
    }
    while (serverthreadexit == false);
  }
}

void serverthread()
{
  // Start server
  serv.start(4618);
}

void server()
{
  char inputchar;
  std::vector<std::string> cmds;

  // Start image send to server thread
  std::thread t1(&serverimagethread);
  t1.detach();

  // Start server thread
  std::thread t2(&serverthread);
  t2.detach();

  cv::namedWindow("WindowForWaitkey");
  do
  {
    inputchar = cv::waitKey(100);
    if (inputchar == 'q') 
    { 
      serverthreadexit = true; 
    }

    serv.get_cmd(cmds);

    if (cmds.size() > 0)
    {
      for (int i = 0; i < cmds.size(); i++)
      {
        if (cmds.at(i) == "a")
        {
          std::cout << "\nReceived 'a' command";

          // Send an 'a' message
          std::string reply = "Hi there from Server";
          serv.send_string(reply);
        }
        else
        {
          std::string reply = "Got some other message";
          serv.send_string(reply);
        }
      }
    }
  } while (serverthreadexit == false);

  serv.stop();
  
  Sleep(100);
}

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Template Project";
	std::cout << "\n***********************************";
	std::cout << "\n(1) Test serial COM communication";
	std::cout << "\n(2) Show image manipulation";
	std::cout << "\n(3) Show video manipulation";
	std::cout << "\n(4) Test client/server communication";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

int main(int argc, char* argv[])
{
	int cmd = -1;
	do
	{
		print_menu();
		std::cin >> cmd;
		switch (cmd)
		{
		case 1: test_com(); break;
		case 2: do_image(); break;
		case 3: do_video(); break;
		case 4: server(); break;
		}
	} while (cmd != 0);
}
