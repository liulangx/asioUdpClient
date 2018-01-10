#include "casioasyncclient.h"

CAsioAsyncClient::CAsioAsyncClient(boost::asio::io_service& io_service, std::string host,std::string port)
    : m_socketSend(io_service),
      m_messageSend(new Message),
      m_t(io_service, boost::posix_time::seconds(0))
{
    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), host, port);
    m_remoteEndpoint = *resolver.resolve(query);
    if(false == m_socketSend.is_open())
        m_socketSend.open(udp::v4());
    initialMessage();
    //startSend();
}

void CAsioAsyncClient::run()
{
    startSend();
}

void CAsioAsyncClient::startSend()
{
    m_socketSend.async_send_to(
                boost::asio::buffer(m_messageSend, sizeof(Message)), m_remoteEndpoint,
                boost::bind(&CAsioAsyncClient::handleSend, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void CAsioAsyncClient::initialMessage()
{
    m_messageSend->frameSyn[0] = 0x18;
    m_messageSend->frameSyn[1] = 0x52;
    m_messageSend->frame = 0;
    m_messageSend->frameType = 0;
    m_messageSend->frameLength = sizeof(Message);
    m_messageSend->mData.imageNumber = 0;
}
float _time = 10;

void CAsioAsyncClient::handleSend(const boost::system::error_code& error,
                                  std::size_t sendSize/*bytes_transferred*/)
{
    m_t.expires_at(m_t.expires_at() + boost::posix_time::milliseconds(1));
    m_t.wait(); // send a message waiting for 1 second;

    if(m_messageSend->mData.imageNumber < 8)
    {
        if(m_messageSend->mData.lineNumber < 2)
        {
            m_messageSend->mData.lineNumber += 1;
        }
        else
        {
            m_messageSend->mData.lineNumber = 0;
            m_messageSend->mData.imageNumber += 1;
        }
    }
    else
    {
        if(m_messageSend->mData.lineNumber < 2)
        {
            m_messageSend->mData.lineNumber += 1;
        }
        else
        {
            m_messageSend->mData.lineNumber = 0;
            m_messageSend->mData.imageNumber = 0;
        }
    }



    if(0 == m_messageSend->mData.lineNumber)
    {
        m_messageSend->frame ++;
        m_messageSend->mData.dataX = _time;
        m_messageSend->mData.dataY = _time * cos(0.02*_time);
        m_messageSend->mData.dataZ = _time * sin(0.02*_time);
        _time += 0.002;
    }
    else if(1 == m_messageSend->mData.lineNumber)
    {
        m_messageSend->frame ++;
        m_messageSend->mData.dataX = _time;
        m_messageSend->mData.dataY = _time * cos(0.02*_time) * 1.2 ;
        m_messageSend->mData.dataZ = _time * sin(0.02*_time) * 1.2;
        _time += 0.002;
    }
    else
    {
        m_messageSend->frame ++;
        m_messageSend->mData.dataX = _time;
        m_messageSend->mData.dataY = _time * cos(0.02*_time) * 1.4 ;
        m_messageSend->mData.dataZ = _time * sin(0.02*_time) * 1.4;
        _time += 0.002;
    }

//    m_messageSend->mData.imageNumber = 0;
//    if(m_messageSend->mData.lineNumber < 3)
//        m_messageSend->mData.lineNumber++;
//    else
//        m_messageSend->mData.lineNumber = 0;

//    m_messageSend->frame ++;
//    m_messageSend->mData.dataX = _time;
//    m_messageSend->mData.dataY = m_messageSend->mData.lineNumber + _time * cos(0.02*_time) * 1.4 ;
//    m_messageSend->mData.dataZ = _time * sin(0.02*_time) * 1.4;
//    _time += 0.002;

    std::cout << m_messageSend->mData.dataX << std::endl;
    m_socketSend.async_send_to(
                boost::asio::buffer(m_messageSend, sizeof(Message)), m_remoteEndpoint,
                boost::bind(&CAsioAsyncClient::handleSend, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

