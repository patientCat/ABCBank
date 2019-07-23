#pragma once

#include <memory>
#include <string.h>
#include "../public/PackStream.h"
#include "muduo/net/Endian.h"
#include <cstdio>
#include "boost/noncopyable.hpp"
using namespace std;

#define CMD_LOGIN 0x01
#define CMD_OPEN_ACCOUNT 0x02
#define CMD_DEPOSIT 0x03
#define CMD_WITHDRAW 0x04
#define CMD_TRANSFER 0x05
#define CMD_BALANCE_INQUIRY 0x06
#define CMD_CHANGE_PASSWORD 0x07
#define CMD_DAY_BILL 0x08
#define CMD_HISTORY_BILL 0x09
#define CMD_ACCOUNT_HISTORY_BILL 0X0A
#define CMD_CLOSE_ACCOUNT 0X0B

// 请求头4个字节
struct RequestHead
{
  uint16_t cmd;
  uint16_t len;
};

// 响应头40个字节
struct ResponseHead
{
  u_int16_t cmd;
  u_int16_t len;
  u_int16_t cnt;
  u_int16_t seq;
  u_int16_t error_code;
  char error_msg[30];
};

// 请求包，不定长
struct RequestPack
{
  RequestHead head;
  char buf[1];
};

// 响应包，不定长
struct ResponsePack
{
  ResponseHead head;
  char buf[1];
};

// 抽象一个会话类，用来对客户端响应
class BankSession
: boost::noncopyable
{
public:
  using PackStreamVec = std::vector<PUBLIC::PackStream>;
public:
  BankSession()
    : packStreamVec_(1, PUBLIC::PackStream())
    , currPackStreamIndex_(-1)
  {
    requestPack_ = (RequestPack*)(buffer_);
  }

  uint16_t getCmd() const { return muduo::net::sockets::networkToHost16(getRequestPack()->head.cmd);  }
  RequestPack *getRequestPack() const { return requestPack_;  }

  void setData(const char *head, int len)
  {
    memcpy(buffer_, head, len);
  }
  char* data()
  {
    return buffer_;
  }

  // 在将PackStream中的数据发送完毕后，调用clear来清空数据
  // 用来防止PackStream过度膨胀
  // 一般用在长连接中
  void clear()
  {
    for(auto &elem : packStreamVec_)
      elem.clear();
  }

  void process();

  PUBLIC::PackStream& getFirstPackStream()
  {
    if(packStreamVec_.empty())
    {
      packStreamVec_.emplace_back(PUBLIC::PackStream{});
      currPackStreamIndex_ = 0;
    }
    return packStreamVec_[0];
  }
  PUBLIC::PackStream& getNextPackStream()
  {
    if(currPackStreamIndex_+1 == packStreamVec_.size())
    {
      packStreamVec_.emplace_back(PUBLIC::PackStream());
    }
    return packStreamVec_[++currPackStreamIndex_];
  }
  PackStreamVec& getPackStreamVec()
  {
    return packStreamVec_;
  }
private:
  void parse();
  void buildRequest();

private:
  int len_;
  // 接收缓冲区
  char buffer_[2048]; //ToDo kBufferLen;
  RequestPack *requestPack_; // 指向buffer，主要用来方便编程
  PackStreamVec packStreamVec_;
  int currPackStreamIndex_;
};
