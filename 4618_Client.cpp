////////////////////////////////////////////////////////////////
// ELEX 4618 Client Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated March 29, 2019
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "Client.h"

Client client(4618, "127.0.0.1");

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Client Project";
	std::cout << "\n***********************************";
	std::cout << "\n(1) Send command";
	std::cout << "\n(2) Get image";
	std::cout << "\n(3) Send A";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

void send_command()
{
	std::string str;
	client.tx_str("cmd");

	do
	{
		client.rx_str(str);
		if (str.length() > 0)
		{
			std::cout << "\nClient Rx: " << str;
		}
	} while (str.length() == 0);
}


void send_a()
{
	std::string str;
	client.tx_str("a");

	do
	{
		client.rx_str(str);
		if (str.length() > 0)
		{
			std::cout << "\nClient Rx: " << str;
		}
	} while (str.length() == 0);
}

void get_image()
{
	cv::Mat im;
	int count = 0;

  client.tx_str("im");

	if (client.rx_im(im) == true)
	{
		if (im.empty() == false)
		{
			count++;
			std::cout << "\nImage received: " << count;
			cv::imshow("rx", im);
			cv::waitKey(100);
		}
	}
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
		case 1: send_command(); break;
		case 2: get_image(); break;
		case 3: send_a(); break;
		}
	} while (cmd != 0);
}
