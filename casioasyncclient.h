#ifndef CASIOASYNCCLIENT_H
#define CASIOASYNCCLIENT_H
#include<string>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>

using boost::asio::ip::udp;

struct MData
{
    u_short imageNumber;
    u_short lineNumber;
    float   dataX;
    float   dataY;
    float   dataZ;
};
struct Message {
    u_char frameSyn[2];//= 0x5218;
    u_short frameLength;
    u_char frameType;
    u_short messageCount;
    u_char checkByte;
    u_int frame;
    MData mData;
};

class CAsioAsyncClient
{
public:
    CAsioAsyncClient(boost::asio::io_service &io_service, std::__cxx11::string host, std::__cxx11::string port);
    virtual void run();
    void startSend();
protected:
    void initialMessage();
    virtual void handleSend(const boost::system::error_code &error, std::size_t sendSize);


private:
    udp::socket m_socketSend;
    //boost::asio::io_service m_io;
    //udp::socket m_socketRecv;
    udp::endpoint m_remoteEndpoint;
    Message* m_messageSend;
    u_int m_lastFrame;
    boost::asio::deadline_timer m_t;
};

#endif // CASIOASYNCCLIENT_H
