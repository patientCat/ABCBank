#pragma once
#include <wx/wx.h>

class DetailStatementForm
: public wxPanel
{
public:
  DetailStatementForm(wxFrame* myframe);

  void OnQueryDayBill(wxCommandEvent& event);
  void OnQueryHistoryBill(wxCommandEvent& event);
  void OnAccountHistoryBill(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);

private:
  DECLARE_EVENT_TABLE()
};
