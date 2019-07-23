//c++ program
//design by >^.^<
//@date    2019-07-08 06:46:31
#include "CloseAccountForm.h"
#include "Constant.h"
#include "MyWxTools.h"
#include "MyFrame.h"
#include "FormManager.h"
#include "TitlePanel.h"
#include "GridPanel.h"
#include "ButtonPanel.h"
#include "Validator.h"
#include "MainMenuForm.h"
#include "../BankSession.h"

BEGIN_EVENT_TABLE(CloseAccountForm, wxPanel)
  EVT_BUTTON(ID_SUBMIT, CloseAccountForm::OnSubmit)
  EVT_BUTTON(ID_RESET, CloseAccountForm::OnReset)
  EVT_BUTTON(ID_CANCEL, CloseAccountForm::OnCancel)
END_EVENT_TABLE()

CloseAccountForm::CloseAccountForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, wxT("CloseAccount")))
  , gridPanel_(new GridPanel(this, 2, 3, 5, 15))
  , buttonPanel_(new ButtonPanel(this))
{
  setTitlePanel();
  setGridPanel();
  setButtonPanel();
  setTotalPanel();

  Centre();
}

void 
CloseAccountForm::setTitlePanel()
{
  titlePanel_->setLevel2();
}


void
CloseAccountForm::setTotalPanel()
{
  totalSizer_->Add(titlePanel_, 0);
  totalSizer_->Add(gridPanel_, 0,  wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM, kBottomBorder);
  SetSizer(totalSizer_);
}

void 
CloseAccountForm::setGridPanel()
{
  gridPanel_->addRowGrid(wxT("ACCOUNT ID"), wxT("长度6位，数字"));
  gridPanel_->addRowGrid(wxT("ACCOUNT PASSWORD"), wxT("长度6-8位"));

}


void 
CloseAccountForm::setButtonPanel()
{
  buttonPanel_->addThreeButtons();
}

void 
CloseAccountForm::OnSubmit(wxCommandEvent &event)
{
  BankSession bs;
  if(!validateLocal(bs))
    return;
  if(!validateNet(bs))
    return;
}

void 
CloseAccountForm::OnReset(wxCommandEvent &event)
{
  gridPanel_->resetText();
}

void 
CloseAccountForm::OnCancel(wxCommandEvent &event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}

bool 
CloseAccountForm::validateLocal(BankSession &bs)
{
  gridPanel_->addRowGrid(wxT("ACCOUNT ID"), wxT("长度6位，数字"));
  gridPanel_->addRowGrid(wxT("ACCOUNT PASSWORD"), wxT("长度6-8位"));
  // 本地验证
  // 1. 验证Account Id
  auto accountIdTc = gridPanel_->getTextCtrl(wxT("ACCOUNT ID")); 
  auto accountId = accountIdTc->GetLineText(0);
  if(!Validator::validateAccountIdAndTip(accountId))
    return false;
  // 2. 验证密码
  auto passwordTc = gridPanel_->getTextCtrl(wxT("ACCOUNT PASSWORD")); 
  auto password = passwordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(password))
    return false;
  return true;
}

bool 
CloseAccountForm::validateNet(BankSession &bs)
{
  return true;
}
