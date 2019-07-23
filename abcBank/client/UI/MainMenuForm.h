#pragma once
#include <wx/wx.h>

class MainMenuForm
: public wxPanel
{
public:
  MainMenuForm(wxFrame *);

  void OnOpenAccount(wxCommandEvent& event);
  void OnDepositMoney(wxCommandEvent& event);
  void OnWithdrawMoney(wxCommandEvent& event);
  void OnTransferAccount(wxCommandEvent& event);
  void OnBalanceInquiry(wxCommandEvent& event);
  void OnChangePassword(wxCommandEvent& event);
  void OnDetailedStatement(wxCommandEvent& event);
  void OnCloseAccount(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);

private:
  DECLARE_EVENT_TABLE()
};
