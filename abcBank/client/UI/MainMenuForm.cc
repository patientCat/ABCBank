//c++ program
//design by >^.^<
//@date    2019-07-08 01:58:31
#include "Constant.h"
#include "MainMenuForm.h"
#include "FormManager.h"
#include "MyWxTools.h"
#include "MyFrame.h"

#include "BalanceInquiryForm.h"
#include "CloseAccountForm.h"
#include "ChangePasswordForm.h"
#include "DepositForm.h"
#include "DetailStatementForm.h"
#include "LoginForm.h"
#include "OpenAccountForm.h"
#include "TransferForm.h"
#include "WithdrawForm.h"

#include <wx/statline.h>

namespace {
const int ID_OPENACCOUNT = 501;
const int ID_DEPOSITMONEY = 502;
const int ID_WITHDRAWMONEY = 503;
const int ID_TRANSFERACCOUNT = 504;
const int ID_BALANCEINQUIRY = 505;
const int ID_CHANGEPASSWORD = 506;
const int ID_DETAILSTATEMENT = 507;
const int ID_CLOSEACCOUNT = 508;
const int ID_BACK = 509;
};

BEGIN_EVENT_TABLE(MainMenuForm, wxPanel)
  EVT_BUTTON(ID_OPENACCOUNT, MainMenuForm::OnOpenAccount)
  EVT_BUTTON(ID_DEPOSITMONEY, MainMenuForm::OnDepositMoney)
  EVT_BUTTON(ID_WITHDRAWMONEY, MainMenuForm::OnWithdrawMoney)
  EVT_BUTTON(ID_TRANSFERACCOUNT, MainMenuForm::OnTransferAccount)
  EVT_BUTTON(ID_BALANCEINQUIRY, MainMenuForm::OnBalanceInquiry)
  EVT_BUTTON(ID_CHANGEPASSWORD, MainMenuForm::OnChangePassword)
  EVT_BUTTON(ID_DETAILSTATEMENT, MainMenuForm::OnDetailedStatement)
  EVT_BUTTON(ID_CLOSEACCOUNT, MainMenuForm::OnCloseAccount)
  EVT_BUTTON(ID_BACK, MainMenuForm::OnExit)
END_EVENT_TABLE()
MainMenuForm::MainMenuForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
{
  wxBoxSizer *totalSizer = new wxBoxSizer(wxVERTICAL);

  wxStaticText * headLine = new wxStaticText(this, -1, wxT("Main Menu List"));

  wxPanel *panel = new wxPanel(this, -1);
  wxButton * button1 = new wxButton(panel, ID_OPENACCOUNT, wxT("1. Open Account"));
  wxButton * button2 = new wxButton(panel, ID_DEPOSITMONEY, wxT("2. Deposit Money"));
  wxButton * button3 = new wxButton(panel, ID_WITHDRAWMONEY, wxT("3. Withdraw Money"));
  wxButton * button4 = new wxButton(panel, ID_TRANSFERACCOUNT, wxT("4. Transfer Of Account"));
  wxButton * button5 = new wxButton(panel, ID_BALANCEINQUIRY, wxT("5. Balance Inquiry"));
  wxButton * button6 = new wxButton(panel, ID_CHANGEPASSWORD, wxT("6. Change Password"));
  wxButton * button7 = new wxButton(panel, ID_DETAILSTATEMENT, wxT("7. Detailed Statement"));
  wxButton * button8 = new wxButton(panel, ID_CLOSEACCOUNT, wxT("8. Close Account"));
  wxButton * button0 = new wxButton(panel, ID_BACK, wxT("0. Back"));

  wxStaticLine *line = new wxStaticLine(this, -1, wxDefaultPosition, wxSize(200, -1), wxLI_HORIZONTAL);
  totalSizer->Add(headLine, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);
  totalSizer->Add(line, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);


  wxGridSizer *gridSizer = new wxGridSizer(5, 2, 5, 8);
  gridSizer->Add(button1, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button2, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button3, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button4, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button5, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button6, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button7, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button8, 0, wxALIGN_LEFT | wxLEFT , 50);
  gridSizer->Add(button0, 0, wxALIGN_LEFT | wxLEFT , 50);

  panel->SetSizer(gridSizer);

  totalSizer->Add(panel, 0, wxTOP, 20);
  SetSizer(totalSizer);

  Centre();
}


void 
MainMenuForm::OnOpenAccount(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getOpenAccountForm()->Show(true);
}
void
MainMenuForm::OnDepositMoney(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getDepositForm()->Show(true);
}
void
MainMenuForm::OnWithdrawMoney(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getWithdrawForm()->Show(true);
}
void
MainMenuForm::OnTransferAccount(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getTransferForm()->Show(true);
}
void
MainMenuForm::OnBalanceInquiry(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getBalanceInquiryForm()->Show(true);
}
void
MainMenuForm::OnChangePassword(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getChangePasswordForm()->Show(true);
}
void
MainMenuForm::OnDetailedStatement(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getDetailStatementForm()->Show(true);
}
void
MainMenuForm::OnCloseAccount(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getCloseAccountForm()->Show(true);
}
void
MainMenuForm::OnExit(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getLoginForm()->Show(true);
}
