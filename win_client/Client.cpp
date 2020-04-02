#include "Client.h" 

#pragma comment(lib, "ws2_32.lib")


int Client::connect_server()
{
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != 0)
	{
		std::cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << std::endl;
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();
		std::cout << "WSADATA version is not correct!" << std::endl;
		return -1;
	}

	_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_clientSocket == INVALID_SOCKET)
	{
		std::cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << std::endl;
		return -1;
	}

	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = inet_addr(_server_addr.c_str());
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(_server_port);

	//连接服务器
	iRet = connect(_clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	return iRet;
}

int Client::StartLongConnect()
{
	if (-1 == connect_server())
	{
		std::cout << "connect_server error." << std::endl;
		return -1;
	}
	_is_connected = true;
	std::thread recv_thread(&Client::ReceServerData, this);
	recv_thread.detach();
	return 0;
}

int Client::ReceServerData()
{
	while (_is_connected)
	{
		memset(_recv_buf, 0, _recv_buf_len);
		int len = recv(_clientSocket, (char*)_recv_buf, _recv_buf_len, 0);
		if (0 == len || -1 == len || len >= _recv_buf_len)
		{
			std::cout << "recv_server_data err" << std::endl;
			return -1;
		}

		std::cout << "recv_server_data len:" << len << std::endl;
		std::cout << _recv_buf << std::endl;
	}

	return 0;
}

int Client::SendData(char* data, int len)
{
	if (!_is_connected)
	{
		return -1;
	}
	if (nullptr == data)
	{
		std::cout << "databuf if null." << std::endl;
		return -1;
	}
    
    std::cout << "SendData len " << len << std::endl;
	return send(_clientSocket, data, len, 0);
}

int Client::GetConnectionStatus()
{
	return _is_connected;
}

int Client::Disconnect()
{
	_is_connected = false;
	closesocket(_clientSocket);
	WSACleanup();
	return 0;
}
