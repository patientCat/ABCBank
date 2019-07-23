#pragma  once
#include <wx/wx.h>

class ButtonPanel;
class GridPanel;
class TitlePanel;
class BankSession;

class OpenAccountForm
: public wxPanel
{
public:
  OpenAccountForm(wxFrame *frame);

  void OnSubmit(wxCommandEvent& event);
  void OnReset(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);

private:
  void setTitlePanel();
  void setGridPanel();
  void setButtonPanel();
  void setTotalPanel();
  void showReceiptForm(BankSession&);

  bool validateLocal(BankSession&);
  bool validateNet(BankSession&);

private:
  wxBoxSizer *totalSizer_;
  TitlePanel *titlePanel_;
  GridPanel *gridPanel_;
  ButtonPanel *buttonPanel_;

  DECLARE_EVENT_TABLE()
};
