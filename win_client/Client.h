#pragma once
#include <winsock2.h>
#include <string>
#include <iostream>
#include <thread>
#include <fstream> 

class Client
{
public:
	explicit Client(std::string server, int port, int rec_buf_len) : _server_addr(server), _server_port(port), 	_recv_buf_len(rec_buf_len){
		_is_connected = false;
		_recv_buf = new unsigned char[_recv_buf_len];
	}
	int SendData(char* buf, int len);
	int GetConnectionStatus();
	int Disconnect();
	int StartLongConnect();
	int ReceServerData();

private:
	int connect_server();
private:
	bool _is_connected;
	unsigned char* _recv_buf;
	int _recv_buf_len;
	std::string _server_addr;
	int _server_port;
	SOCKET _clientSocket;
};

