#pragma once

#include "boost/noncopyable.hpp"

namespace PUBLIC
{

class Socket
: boost::noncopyable
{
public:
  Socket();
  ~Socket();
  void create(bool tcp = true);

  static bool startup();
  static void cleanup();

  bool isValid() const;

  bool bind(const char* ip, unsigned short port);
  bool listen();
  int accept();
  bool connect(const char* ip, unsigned short port);
  void close();
  int send(const char* buf, size_t len);
  int recv(char* buf, size_t len);
  int sendN(const char* buf, size_t len);
  int recvN(char* buf, size_t len);

  void setReuseAddr(bool flag = 1);


private:
  int socket_;
};

}

