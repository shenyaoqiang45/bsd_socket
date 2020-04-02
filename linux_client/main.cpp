#include "Client.h"
 
using namespace std;

int main()
{
    string ip_addr = "10.10.10.73";
	int tcp_port = 8066;
    int rec_buf_len = 1024;
	
	Client* _client = new Client(ip_addr, tcp_port, rec_buf_len);
	if(!_client)
		cout << "client init failed" << endl;

	int ret = _client->StartLongConnect();
	if (-1 == ret)
	{
		cout << "connect server err." << endl;
	}

	if (1 != _client->GetConnectionStatus())
	{
		cout << "connnect fail!" << endl;
		return -1;
	}
	
	cout << "connect success!" << endl;

    int count = 0;
	char msg[100] = "msg from client!";
	while(1)
	{
		int ret = _client->SendData(msg, strlen(msg));
		if (ret == -1)
		{
			cout << "SendData fail!" << endl;

		}
		count++;
        std::this_thread::sleep_for(std::chrono::seconds(5));
		if(100 ==  count)
			break;
	}
	_client->Disconnect();
	return 0;
}