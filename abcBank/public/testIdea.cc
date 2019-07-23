//c++ program
//design by >^.^<
//@date    2019-07-19 06:54:06
#include <iostream>
#include "Encrypt.h"

using namespace PUBLIC;
using namespace std;
int main()
{
  char password[6] = "123";
  char enc[17] = {0};
  char newpassword[6] = {0};
  
  uint16_t cmd = 0x01;
  Encrypt::ideaEncrypt(enc, password, cmd);
  cout << enc << endl;
  Encrypt::ideaDecrypt(newpassword, enc, cmd);
  cout << newpassword << endl;
}
