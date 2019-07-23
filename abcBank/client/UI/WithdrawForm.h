#pragma once

#include <wx/wx.h>

class TitlePanel;
class GridPanel;
class ButtonPanel;
class BankSession;

class WithdrawForm
: public wxPanel
{
public:
  WithdrawForm(wxFrame *);

  void OnSubmit(wxCommandEvent& event);
  void OnReset(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);

private:
  bool validateLocal(BankSession&);
  bool validateNet(BankSession&);
  void showReceiptForm(BankSession&);

  void setTotalPanel();
  void setTitlePanel();
  void setGridPanel();
  void setButtonPanel();
private:
  wxBoxSizer *totalSizer_;
  TitlePanel *titlePanel_;
  GridPanel *gridPanel_;
  ButtonPanel *buttonPanel_;

  DECLARE_EVENT_TABLE()
};
