////////////////////////////////////////////////////////////////
// ELEX 4618 Client Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated April 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "Client.h"

std::string server_ip = "127.0.0.1";
int server_port = 4618;

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Client Project";
	std::cout << "\n***********************************";
	std::cout << "\n(1) Send command 1";
	std::cout << "\n(2) Send command 2";
	std::cout << "\n(3) Send get image command";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

void send_command(CClient &client, std::string cmd)
{
	std::string str;

	client.tx_str(cmd);
	std::cout << "\nClient Tx: " << cmd;

	if (cmd == "im")
	{
		cv::Mat im;
		if (client.rx_im(im) == true)
		{
			if (im.empty() == false)
			{
				std::cout << "\nClient Rx: Image received";
				cv::imshow("rx", im);
				cv::waitKey(10);
			}
		}
		else
		{
			// No response, disconnect and reconnect
			client.close_socket();
			client.connect_socket(server_ip, server_port);
		}
	}
	else
	{
		if (client.rx_str(str) == true)
		{
			std::cout << "\nClient Rx: " << str;
		}
		else
		{
			// No response, disconnect and reconnect
			client.close_socket();
			client.connect_socket(server_ip, server_port);
		}
	}
}

int main(int argc, char* argv[])
{
	CClient client;
	int cmd = -1;

	client.connect_socket(server_ip, server_port);

	do
	{
		print_menu();
		std::cin >> cmd;

		switch (cmd)
		{
		case 1: send_command(client, "G 0"); break;
		case 2: send_command(client, "G 1"); break;
		case 3: send_command(client, "im"); break;
		}
	} while (cmd != 0);
}
