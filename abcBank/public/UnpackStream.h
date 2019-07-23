#pragma once

#include <string>
#include <vector>
#include <cassert>

// 这是一个解包类，可以利用>>算子，解包len+message的简单包类型
// OutStream是对应的打包类

namespace PUBLIC
{

class UnpackStream
{
public:
  UnpackStream();
  UnpackStream(const char* data, size_t len);
  
  // ToDo 可以设置长度是多少字节表示的。1, 2，4，8只能是这几个数

  void setBuffer(const char* data, size_t len);

  UnpackStream & operator>>(std::string& str);

  // 输出确切的多少字节
  UnpackStream & operator>>(uint8_t & x);
  UnpackStream & operator>>(uint16_t & x);
  UnpackStream & operator>>(uint32_t & x);
  UnpackStream & operator>>(uint64_t & x);

  UnpackStream & operator>>(int8_t & x);
  UnpackStream & operator>>(int16_t & x);
  UnpackStream & operator>>(int32_t & x);
  UnpackStream & operator>>(int64_t & x);


  void skip(size_t len)
  {
    assert(readableBytes() >= len);
    currIndex_ += len;
  }

  void readBytesTo(void *data, size_t len);
  size_t length()
  {
    return readableBytes();
  }
private:
  size_t readableBytes() const
  {
     return buffer_.size() - currIndex_;
  }

  const char* peekCurr() const
  {
    return buffer_.data() + currIndex_;
  }

  size_t currIndex_;
  std::vector<char> buffer_;
};

}// end namespace PUBLIC

