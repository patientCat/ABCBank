//c++ program
//design by >^.^<
//@date    2019-07-08 01:58:31
#include "Constant.h"
#include "DetailStatementForm.h"
#include "MyFrame.h"
#include "MyWxTools.h"
#include "FormManager.h"
#include "QueryDayBillForm.h"
#include "QueryHistoryBillForm.h"
#include "QueryAccountHistoryBillForm.h"
#include "MainMenuForm.h"

#include <wx/statline.h>

namespace {
  const int ID_DAY = 601;
  const int ID_HISTORY = 602;
  const int ID_ACCOUNT_HISTORY = 603;
}

BEGIN_EVENT_TABLE(DetailStatementForm, wxPanel)
  EVT_BUTTON(ID_DAY, DetailStatementForm::OnQueryDayBill)
  EVT_BUTTON(ID_HISTORY, DetailStatementForm::OnQueryHistoryBill)
  EVT_BUTTON(ID_ACCOUNT_HISTORY, DetailStatementForm::OnAccountHistoryBill)
  EVT_BUTTON(ID_CANCEL, DetailStatementForm::OnCancel)
END_EVENT_TABLE()


DetailStatementForm::DetailStatementForm(wxFrame* myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
{
  wxBoxSizer *totalSizer = new wxBoxSizer(wxVERTICAL);


  wxButton * button1 = new wxButton(this, ID_DAY, wxT("View the one day's detail"));
  wxButton * button2 = new wxButton(this, ID_HISTORY, wxT("View the detail in a range"));
  wxButton * button3 = new wxButton(this, ID_ACCOUNT_HISTORY, wxT("Search the account by account id"));

  wxButton * buttonReturn = new wxButton(this, ID_CANCEL, wxT("CANCEL"));


  addHeadline(this, totalSizer, wxT("Detail Statement"));


  totalSizer->Add(button1, 0, wxALIGN_CENTER | wxTOP , 20);
  totalSizer->Add(button2, 0, wxALIGN_CENTER | wxTOP , 20);
  totalSizer->Add(button3, 0, wxALIGN_CENTER | wxTOP , 20);
  totalSizer->Add(new wxPanel(this, -1), 1);
  totalSizer->Add(buttonReturn, 0, 
      wxALIGN_RIGHT | wxALIGN_BOTTOM| wxRIGHT |wxBOTTOM , kBottomBorder);

  SetSizer(totalSizer);

  Centre();
}

void 
DetailStatementForm::OnQueryDayBill(wxCommandEvent& event)
{
  GetFormManager().getQueryDayBillForm()->Show(true);
}

void 
DetailStatementForm::OnQueryHistoryBill(wxCommandEvent& event)
{
  GetFormManager().getQueryHistoryBillForm()->Show(true);
}

void 
DetailStatementForm::OnAccountHistoryBill(wxCommandEvent& event)
{
  GetFormManager().getQueryAccountHistoryBillForm()->Show(true);
}

void 
DetailStatementForm::OnCancel(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}
