//c++ program
//design by >^.^<
//@date    2019-07-12 04:14:56
#include <iostream>
#include <functional>
#include <string>
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/InetAddress.h"
#include "muduo/base/Logging.h"
#include "muduo/net/Endian.h"
#include "BankSession.h"
#include "../public/UnpackStream.h"

using namespace muduo;
using namespace muduo::net;
using namespace std;
using namespace PUBLIC;


class ABCBankServer
{
public:
  static const int kHeaderLen = 4;
public:
  ABCBankServer(EventLoop *loop, const InetAddress &addr)
    : server_(loop, addr, "abcBank server", TcpServer::Option::kReusePort)
  {
    server_.setThreadNum(4);
    server_.setConnectionCallback(
        std::bind(&ABCBankServer::onConnection, this, _1)
        );
    server_.setMessageCallback(
        std::bind(&ABCBankServer::onMessage, this, _1, _2, _3)
        );
  }

  void onConnection(const TcpConnectionPtr& conn)
  {
    if(conn->connected())
    {
      LOG_INFO << conn->peerAddress().toIpPort() << " is connected";
      //conn->setContext(new BankSession()); 有点问题，了解下boost::any再来
    }
    else{
      LOG_INFO << conn->peerAddress().toIpPort() << " is disconnected";
    }
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buff, Timestamp recv)
  {
    BankSession bs;
    LOG_INFO << "OnMessage";
    while(buff->readableBytes() >= 4) // 只有收到一个完整请求包的报头后
    {
      // 这4个字节数据包是cmd+len字段，偷偷查看len字段
      char* addr = const_cast<char *>(buff->peek());
      uint16_t len = sockets::networkToHost16(reinterpret_cast<RequestHead*>(addr)->len);
      // 根据len字段来计算是否收到一个完整的数据包
      if(buff->readableBytes() >= len + kHeaderLen) // 则进行处理
      {
        cout << "succ" << endl;
        cout << len + kHeaderLen << endl;
        // 收到一个完整的数据包后，先将数据包拷贝到应用层
        //auto *bs = boost::any_cast<BankSession>(conn->getMutableContext());

        bs.setData(buff->peek(), len + kHeaderLen);
        // 然后确认接收
        buff->retrieve(len + kHeaderLen);
        // 然后处理
        bs.process();
        // 发送Response包
        auto &packStreamVec = bs.getPackStreamVec();
        for(auto & apack : packStreamVec)
          conn->send(apack.data(), apack.length());
        //bs.clear();
      }
      else{ // 没有收到一个完整的请求包
        break;
      }
    }
  }

  void start()
  {
    LOG_INFO << "The server is starting on 8888";
    server_.start();
  }

private:
  TcpServer server_;
};

int main()
{
  EventLoop eventLoop;
  InetAddress addr(8888);
  ABCBankServer server(&eventLoop, addr);
  server.start();
  eventLoop.loop();
}
