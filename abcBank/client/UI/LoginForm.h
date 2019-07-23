#pragma  once

#include <wx/wx.h>


class TitlePanel;
class GridPanel;
class ButtonPanel;
class BankSession;

class LoginForm
: public wxPanel
{
public:
  LoginForm(wxFrame *myframe);

  void OnLogin(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);

private:

  bool validateLocal(BankSession&);
  bool validateNet(BankSession&);

private:
  wxBoxSizer * totalSizer_;
  TitlePanel *titlePanel_;
  GridPanel *gridPanel_;
  ButtonPanel *buttonPanel_;

  DECLARE_EVENT_TABLE()
};
