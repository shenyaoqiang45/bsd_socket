#include <iostream>
#include <string.h>
#include <thread>
#include <chrono>
#include "Server.h"


int main(int argc, char *argv[])
{
    Server tran(8066, 1024);
	
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
