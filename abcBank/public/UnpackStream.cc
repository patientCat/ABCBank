//c++ program
//design by >^.^<
//@date    2019-07-12 07:12:56
#include "UnpackStream.h"
#include "Endian.h"

#include <string.h>
using namespace PUBLIC;

UnpackStream::UnpackStream()
  : currIndex_(0)
{
}

UnpackStream::UnpackStream(const char *data, size_t len)
  : currIndex_(0)
{
  setBuffer(data, len);
}

void
UnpackStream::setBuffer(const char *data, size_t len)
{
  currIndex_ = 0;
  buffer_.resize(len);
  std::copy(data, data+len, buffer_.begin());
}

void
UnpackStream::readBytesTo(void *data, size_t len)
{
  assert(readableBytes() >= len);
  std::copy(buffer_.begin() + currIndex_, 
      buffer_.begin() + currIndex_ + len,
      static_cast<char*>(data));
  currIndex_ += len;
}

UnpackStream&
UnpackStream::operator>>(uint8_t & x)
{
  assert(readableBytes() >= sizeof(uint8_t));
  x = *peekCurr();
  currIndex_ += sizeof x;
  return *this;
}
UnpackStream&
UnpackStream::operator>>(uint16_t & x)
{
  assert(readableBytes() >= sizeof(uint16_t));
  uint16_t be16 = 0;

  ::memcpy(&be16, peekCurr(), sizeof be16);
  currIndex_ += sizeof be16;
  x = Endian::networkToHost16(be16);
  return *this;
}
UnpackStream&
UnpackStream::operator>>(uint32_t & x)
{
  assert(readableBytes() >= sizeof(uint32_t));
  uint32_t be32 = 0;
  ::memcpy(&be32, peekCurr(), sizeof be32);
  currIndex_ += sizeof be32;

  x = Endian::networkToHost32(be32);
  return *this;
}
UnpackStream&
UnpackStream::operator>>(uint64_t & x)
{
  assert(readableBytes() >= sizeof(uint64_t));
  uint64_t be64 = 0;
  ::memcpy(&be64, peekCurr(), sizeof be64);
  currIndex_ += sizeof be64;

  x = Endian::networkToHost64(be64);
  return *this;
}

UnpackStream&
UnpackStream::operator>>(int8_t & x)
{
  assert(readableBytes() >= sizeof(int8_t));
  x = *peekCurr();
  currIndex_ += sizeof x;
  return *this;
}
UnpackStream&
UnpackStream::operator>>(int16_t & x)
{
  assert(readableBytes() >= sizeof(int16_t));
  int16_t be16 = 0;
  ::memcpy(&be16, peekCurr(), sizeof be16);
  currIndex_ += sizeof be16;

  x = Endian::networkToHost16(be16);
  return *this;
}
UnpackStream&
UnpackStream::operator>>(int32_t & x)
{
  assert(readableBytes() >= sizeof(int32_t));
  int32_t be32 = 0;
  ::memcpy(&be32, peekCurr(), sizeof be32);
  currIndex_ += sizeof be32;

  x = Endian::networkToHost32(be32);
  return *this;
}
UnpackStream&
UnpackStream::operator>>(int64_t & x)
{
  assert(readableBytes() >= sizeof(int64_t));
  int64_t be64 = 0;
  ::memcpy(&be64, peekCurr(), sizeof be64);
  currIndex_ += sizeof be64;

  x = Endian::networkToHost64(be64);
  return *this;
}

UnpackStream&
UnpackStream::operator>>(std::string & str)
{
  assert(readableBytes() >= sizeof(uint16_t));
  uint16_t len;
  *this >> len;
  assert(readableBytes() >= len);
  str.clear();
  str.append(peekCurr(), len);
  currIndex_ += len;

  return *this;
}
