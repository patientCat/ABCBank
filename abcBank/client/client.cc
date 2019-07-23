//c++ program
//design by >^.^<
//@date    2019-07-12 20:39:15

#include <wx/wx.h>
#include "../public/Singleton.h"
#include "./UI/FormManager.h"

using namespace PUBLIC;
class Client
: public wxApp
{
public:
  bool virtual OnInit()
  {
    // 屏蔽几个信号
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);

    PUBLIC::Singleton<FormManager>::getInstance().start();    

    return true;
  }
};

wxIMPLEMENT_APP(Client);

