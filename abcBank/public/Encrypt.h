#pragma once

#include <inttypes.h>

namespace PUBLIC{
class Encrypt
{
public:
  static void ideaEncrypt(char*dest, const char*password, uint16_t buff);
  static void ideaDecrypt(char*dest, const char*password, uint16_t buff);
};

}
