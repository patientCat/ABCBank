//c++ program
//design by >^.^<
//@date    2019-07-19 07:08:34
#include "Encrypt.h"
#include "MD5.h"
#include "Idea.h"

using namespace PUBLIC;

void Encrypt::ideaEncrypt(char*dest, const char*password, uint16_t cmd)
{
  unsigned char ideaKey[16];
  unsigned char buff[2];
  buff[0] = (cmd >> 8) & 0xff;
  buff[1] = cmd & 0xff;
  MD5 md5;
  md5.MD5Make(ideaKey, buff, 2);
  for(int i = 0; i<8; ++i)
  {
    ideaKey[i] = ideaKey[i] ^ ideaKey[i+8];
    ideaKey[i] = ideaKey[i] ^ ((cmd >> (i%2) & 0xff));
    ideaKey[i+8] = ideaKey[i] ^ ideaKey[i+8];
    ideaKey[i+8] = ideaKey[i+8] ^ ((cmd >> (i%2)) & 0xff);
  }
  Idea idea;
  idea.crypt(ideaKey, (const unsigned char*)password, (unsigned char*)dest, 16, true);
}

void Encrypt::ideaDecrypt(char*dest, const char*password, uint16_t cmd)
{
  unsigned char ideaKey[16];
  unsigned char buff[2];
  buff[0] = (cmd >> 8) & 0xff;
  buff[1] = cmd & 0xff;
  MD5 md5;
  md5.MD5Make(ideaKey, buff, 2);
  for(int i = 0; i<8; ++i)
  {
    ideaKey[i] = ideaKey[i] ^ ideaKey[i+8];
    ideaKey[i] = ideaKey[i] ^ ((cmd >> (i%2) & 0xff));
    ideaKey[i+8] = ideaKey[i] ^ ideaKey[i+8];
    ideaKey[i+8] = ideaKey[i+8] ^ ((cmd >> (i%2)) & 0xff);
  }
  Idea idea;
  idea.crypt(ideaKey, (const unsigned char*)password, (unsigned char*)dest, 16, false);
}
