#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

#include <WinSock2.h>
#include <string.h>
#include <time.h>
#include <thread>

typedef SOCKET SOCKET_FD;

class Server
{
public:
    explicit Server(int port, int rec_buf_len) : port_(port),  _recv_buf_len(rec_buf_len){
        _recv_buf = new unsigned char[_recv_buf_len];
        init();
    }
    void clear();
private:
    int init(); // 初始化
    int listen_thread_proc(); // 监听线线程，用于接收新的连接
    int work_thread_proc(); // 工作线程，用于接收已有连接数据
    int heartbeat(); // 长连接心跳
private:
    int port_; // 绑定端口
    struct sockaddr_in host_addr_;
    struct sockaddr_in client_addr_; // 客户端地址
    SOCKET_FD host_socket_; // 监听socket
    SOCKET_FD clnt_socket_; //
    clock_t last_connected_; // 最后心跳时间，用于检测是否断链
    std::thread work_thread_;
    std::thread listen_thread_;
    bool clear_up_ = false;
    unsigned char* _recv_buf;
    int _recv_buf_len;
};

#endif // ServerSERVER_H_INCLUDED
