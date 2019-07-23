#pragma once

#include <wx/wx.h>

class TitlePanel;
class GridPanel;
class ButtonPanel;

class QueryHistoryBillForm
: public wxPanel
{
public:
  QueryHistoryBillForm(wxPanel *panel);

  void OnQuery(wxCommandEvent&);
  void OnCancel(wxCommandEvent&);

private:
  bool validateLocal();
  bool validateNet();

private:
  wxBoxSizer *totalSizer_;
  TitlePanel *titlePanel_;
  GridPanel * gridPanel_;
  ButtonPanel *buttonPanel_;

  DECLARE_EVENT_TABLE()
};
