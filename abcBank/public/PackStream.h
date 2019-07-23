#pragma once

#include <string>
#include <vector>
#ifdef WX_STRING
#include "StringUtil.h"
#include <wx/string.h>
#endif


namespace PUBLIC
{

// 用来打包的类，将数据打包为len + message的方式
class PackStream
{
public:
  static const size_t kInitialSize;
  PackStream();

  // 工具，不会打包
  PackStream& operator<<(uint8_t x);
  PackStream& operator<<(uint16_t x);
  PackStream& operator<<(uint32_t x);
  PackStream& operator<<(uint64_t x);

  PackStream& operator<<(int8_t x);
  PackStream& operator<<(int16_t x);
  PackStream& operator<<(int32_t x);
  PackStream& operator<<(int64_t x);

  PackStream& operator<<(const std::string& str);
#ifdef WX_STRING
  PackStream& operator<<(const wxString& str)
  {
    *this << Convert::wxStringToString(str);
  }
#endif

  void writeBytesFrom(const void* data, size_t len);

  void skip(size_t len)
  {
    ensureWritableBytes(len);
    length_ += len;
  }

  void clear()
  {
    length_= 0;
    if(buffer_.size() != kInitialSize)
      buffer_.resize(kInitialSize);
  }
  // 在调用skip后，又希望重新填充跳过的那段数据
  void replace16t(size_t pos, uint16_t len);

  char *data()
  {
    return buffer_.data();
  }
  const char *data() const
  {
    return buffer_.data();
  }
  size_t length() const
  {
    return length_;
  }


private:
  size_t writeableBytes() const
  {
    return buffer_.size() - length();
  }
  // 自动扩充
  void ensureWritableBytes(size_t len)
  {
    if(writeableBytes() < len)
      buffer_.resize(length_ + len);
  }

  void append(const char* data, size_t len);
  void append(const void* data, size_t len);
  std::vector<char> buffer_;
  size_t length_;
};

}// end namespace PUBLIC
