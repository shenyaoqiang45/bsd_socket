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
    int init(); // ��ʼ��
    int listen_thread_proc(); // �������̣߳����ڽ����µ�����
    int work_thread_proc(); // �����̣߳����ڽ���������������
    int heartbeat(); // ����������
private:
    int port_; // �󶨶˿�
    struct sockaddr_in host_addr_;
    struct sockaddr_in client_addr_; // �ͻ��˵�ַ
    SOCKET_FD host_socket_; // ����socket
    SOCKET_FD clnt_socket_; //
    clock_t last_connected_; // �������ʱ�䣬���ڼ���Ƿ����
    std::thread work_thread_;
    std::thread listen_thread_;
    bool clear_up_ = false;
    unsigned char* _recv_buf;
    int _recv_buf_len;
};

#endif // ServerSERVER_H_INCLUDED
