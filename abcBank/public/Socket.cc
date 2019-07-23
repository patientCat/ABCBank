#include "Socket.h"
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

using namespace PUBLIC;

Socket::Socket() 
  : socket_(-1)
{

}

Socket::~Socket()
{
	if (isValid())
	{
		close();
	}
}

void 
Socket::create(bool tcp/* = true*/)
{
	if (tcp)
		socket_ = socket(PF_INET, SOCK_STREAM, 0);
	else
		socket_ = socket(PF_INET, SOCK_DGRAM, 0);
	assert(-1 != socket_);
}

void 
Socket::setReuseAddr(bool flag)
{
	int optval = flag;
	setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
		(const char*)&optval, sizeof optval);
}

bool 
Socket::isValid() const
{
	return socket_ != -1;
}

bool 
Socket::startup()
{
  return true;
}

void 
Socket::cleanup()
{
}

bool 
Socket::bind(const char* ip, unsigned short port)
{
	sockaddr_in addr;
  bzero(&addr, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (ip == NULL)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(ip);


	if (-1 == ::bind(socket_, (sockaddr*)&addr, sizeof(addr)) )
		return false;

	return true;
}

bool 
Socket::listen()
{
	if (-1 == ::listen(socket_, 5))
	{
		return false;
	}
	return true;
}

int 
Socket::accept()
{
	sockaddr_in peerAddr = {0};
	socklen_t addrSize;
	int sock = ::accept(socket_, (sockaddr*)&peerAddr, &addrSize);

	return sock;
}

bool 
Socket::connect(const char* ip, unsigned short port)
{
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;	
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	if (-1 == ::connect(socket_, (sockaddr*)&addr, sizeof(addr)) )
	{
		return false;
	}	
	return true;
}

void 
Socket::close()
{
  // or shutdown?
  ::close(socket_);
}

int 
Socket::send(const char* buf, size_t len)
{
  return ::send(socket_, buf, len, 0);
}

int 
Socket::recv(char* buf, size_t len)
{
  return ::recv(socket_, buf, len, 0);
}

int 
Socket::sendN(const char* buf, size_t len)
{
	int nLeft;
	int nWritten;
	char* p = (char*)buf;
	nLeft = len;

	while (nLeft > 0)
	{
		nWritten = ::send(socket_, p, nLeft, 0);
		if (nWritten < 0)
    {
      if(errno == EAGAIN || errno == EWOULDBLOCK)
        continue;
			return nWritten;
    }

		nLeft -= nWritten;
		p += nWritten;
	}

	return len - nLeft;
}

int 
Socket::recvN(char* buf, size_t len)
{
	int nLeft;
	int nRead;
	char* p = buf;
	nLeft = len;

	while (nLeft > 0)
	{
		nRead = ::recv(socket_, p, nLeft, 0);
		if (nRead < 0)
		{
      if(errno == EAGAIN || errno == EWOULDBLOCK)
        continue;
			return nRead;
		}
		else if (nRead == 0)
			return nRead;

		nLeft -= nRead;
		p += nRead;
	}

	return len - nLeft;
}
