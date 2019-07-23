//c++ program
//design by >^.^<
//@date    2019-07-23 02:51:46
#include <iostream>
#include "StringUtil.h"

using namespace PUBLIC;
using namespace std;

int main()
{
  string number = "1.33333";
  cout << formatDoubleString(number, 2) << endl;
  string num = "1";
  cout << fillZeroBefore(num, 6) << endl;
}
