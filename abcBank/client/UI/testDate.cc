//c++ program
//design by >^.^<
//@date    2019-07-22 06:52:05
#include <iostream>
#include "Validator.h"

using namespace std;
int main()
{
  wxString str("2019-07-21"); 
  if(Validator::validateDate(str))
    cout << "yes" << endl;
}
