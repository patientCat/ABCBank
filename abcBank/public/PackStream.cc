//c++ program
//design by >^.^<
//@date    2019-07-12 07:54:31
#include "PackStream.h"
#include "Endian.h"
#include <string.h>
#include <assert.h>

using namespace PUBLIC;

const size_t PackStream::kInitialSize = 1024;

PackStream::PackStream()
  : buffer_(1024)
  , length_(0)
{}

PackStream& PackStream::operator<<(uint8_t x)
{
  append(&x, sizeof x);
  return *this;
}

PackStream& PackStream::operator<<(uint16_t x)
{
  uint16_t be16 = Endian::hostToNetwork16(x);
  append(&be16, sizeof be16);
  return *this;
}

PackStream& PackStream::operator<<(uint32_t x)
{
  uint32_t be32 = Endian::hostToNetwork32(x);
  append(&be32, sizeof be32);
  return *this;
}

PackStream& PackStream::operator<<(uint64_t x)
{
  uint64_t be64 = Endian::hostToNetwork64(x);
  append(&be64, sizeof be64);
  return *this;
}

PackStream& PackStream::operator<<(int8_t x)
{
  append(&x, sizeof x);
  return *this;
}

PackStream& PackStream::operator<<(int16_t x)
{
  int16_t be16 = Endian::hostToNetwork16(x);
  append(&be16, sizeof be16);
  return *this;
}

PackStream& PackStream::operator<<(int32_t x)
{
  int32_t be32 = Endian::hostToNetwork32(x);
  append(&be32, sizeof be32);
  return *this;
}

PackStream& PackStream::operator<<(int64_t x)
{
  int64_t be64 = Endian::hostToNetwork64(x);
  append(&be64, sizeof be64);
  return *this;
}

PackStream& PackStream::operator<<(const std::string& str)
{
  uint16_t len = static_cast<uint16_t>(str.size());
  *this << len;
  append(str.data(), len);
  return *this;
}

void
PackStream:: writeBytesFrom(const void* data, size_t len)
{
  append(data, len);
}

void 
PackStream::append(const char* data, size_t len)
{
  ensureWritableBytes(len);
  std::copy(data, data+len, buffer_.data() + length_);
  length_ += len;
}
void 
PackStream::append(const void* data, size_t len)
{
  append(static_cast<const char*>(data), len);
}

void 
PackStream::replace16t(size_t pos, uint16_t len)
{
  assert(sizeof(len) == sizeof(uint16_t));
  len = Endian::hostToNetwork16(len);
  memcpy(data()+pos, &len, sizeof(len));
}
