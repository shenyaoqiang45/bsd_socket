#include "Client.h" 

int Client::connect_server()
{
	_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_clientSocket == -1)
	{
		std::cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << std::endl;
		return -1;
	}

	struct sockaddr_in srvAddr;
	memset( &srvAddr, 0, sizeof( struct sockaddr_in ) );
	srvAddr.sin_addr.s_addr = inet_addr(_server_addr.c_str());
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(_server_port);

	int iRet = connect(_clientSocket, (struct sockaddr*)&srvAddr, sizeof(srvAddr));
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
	close(_clientSocket);
	return 0;
}
