#include "Server.h"
#include<iostream>
#include <chrono>
#include <unistd.h>
#include <string.h>
using namespace std;

int Server::init()
{
    host_socket_ = socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == host_socket_)
    {
        std::cout << "init err: create socket err. host_socket == -1" << std::endl;
        return -1;
    }


    memset(&host_addr_, 0, sizeof(host_addr_));
    host_addr_.sin_family = AF_INET;
    host_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    host_addr_.sin_port = htons(port_);

    if(-1 == bind(host_socket_, (struct sockaddr*) &host_addr_, sizeof(host_addr_)))
    {
        std::cout << "bind err." << std::endl;
        return -1;
    }

    if( -1 == listen(host_socket_, 5))
    {
        std::cout << " listen err." << std::endl;
        return -1;
    }

    std::thread t(&Server::listen_thread_proc, this);

    listen_thread_ = std::move(t);
    listen_thread_.detach();

    clnt_socket_ = -1;
    return 0;
}

int Server::listen_thread_proc()
{
	std::cout << "begin to accept  client." << std::endl;
    while(!clear_up_)
    {
        clnt_addr_size_ = sizeof(client_addr_);
        clnt_socket_ = accept(host_socket_, (struct sockaddr*)&client_addr_, &clnt_addr_size_);
        if(-1 == clnt_socket_)
        {
            std::cout << "accept err." << std::endl;
            continue;
        }
        std::cout << "client connected," << inet_ntoa(client_addr_.sin_addr) << ":" << ntohs(client_addr_.sin_port)<< " client Id:"<< clnt_socket_ << std::endl;
        std::thread t(&Server::work_thread_proc, this);
        work_thread_ = std::move(t);
        work_thread_.detach();
    }
    return 0;
}

int Server::work_thread_proc()
{
    while( -1 != clnt_socket_ )
    {
        int len = recv(clnt_socket_, _recv_buf, _recv_buf_len, 0);
        if (-1 == len || 0 == len || len >= _recv_buf_len)
        {
            std::cout << "client disconnected. client Id:" << clnt_socket_ << std::endl;
            close(clnt_socket_);
            clnt_socket_ = -1;
			continue;
            //return -1;
        }

        std::cout << "client_socket:" << clnt_socket_  << " recv_client_data len:" << len << std::endl;
        cout << "msg value:" << _recv_buf << endl;
		// TODO send to ddr
        char message[100] = "msg from Server!";
        int send_len = send(clnt_socket_, message, strlen(message), 0);
        if (0 == send_len)
        {
            std::cout << "send data err." << std::endl;
            close(clnt_socket_);
            clnt_socket_ = -1;
            return -1;
        }
    }
    return 0;
}

int Server::heartbeat()
{
    if( -1 == clnt_socket_)
    {
        return -1;
    }
    char message[64];
    sprintf(message, "HB");
    return send(clnt_socket_, message, strlen(message), 0);
}

void Server::clear()
{
    clear_up_ = true;
	if (-1 != clnt_socket_)
	{
		close(clnt_socket_);
	}
	if (-1 != host_socket_)
	{
		close(host_socket_);
	}
}
