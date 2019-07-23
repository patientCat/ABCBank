#pragma once

#include <wx/wx.h>


class GridPanel;
class ButtonPanel;
class TitlePanel;
class BankSession;

class BalanceInquiryForm
: public wxPanel
{
public :
  BalanceInquiryForm(wxFrame* myframe);

  void OnSubmit(wxCommandEvent& event);
  void OnReset(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);

private:
  bool validateLocal(BankSession&);
  bool validateNet(BankSession&);
  void showReceiptForm(BankSession&);

  void setTitlePanel();
  void setTotalPanel();
  void setGridPanel();
  void setButtonPanel();

private:
  wxBoxSizer *totalSizer_;
  TitlePanel *titlePanel_;
  GridPanel *gridPanel_;
  ButtonPanel *buttonPanel_;


  DECLARE_EVENT_TABLE()
};
