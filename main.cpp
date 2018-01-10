#include <iostream>
#include "casioasyncclient.h"

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        std::string host = "127.0.0.1";
        std::string port = "8003";
        CAsioAsyncClient client(io_service, host, port);
        client.startSend();
        io_service.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
