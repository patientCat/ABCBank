//c++ program
//design by >^.^<
//@date    2019-07-08 06:46:31
#include "BalanceInquiryForm.h"
#include "Constant.h"
#include "MyFrame.h"
#include "MyWxTools.h"
#include "FormManager.h"
#include "TitlePanel.h"
#include "GridPanel.h"
#include "ButtonPanel.h"
#include "Validator.h"
#include "MainMenuForm.h"
#include "ReceiptForm.h"
#include "../TransactionManager.h"
#include "../BankSession.h"
#include "../../public/Exception.h"

using namespace PUBLIC;
BEGIN_EVENT_TABLE(BalanceInquiryForm, wxPanel)
  EVT_BUTTON(ID_SUBMIT, BalanceInquiryForm::OnSubmit)
  EVT_BUTTON(ID_RESET, BalanceInquiryForm::OnReset)
  EVT_BUTTON(ID_CANCEL, BalanceInquiryForm::OnCancel)
END_EVENT_TABLE()

BalanceInquiryForm::BalanceInquiryForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, wxT("BalanceInquiry")))
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
BalanceInquiryForm::setTitlePanel()
{
  titlePanel_->setLevel2();
}


void
BalanceInquiryForm::setTotalPanel()
{
  totalSizer_->Add(titlePanel_, 0);
  totalSizer_->Add(gridPanel_, 0,  wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM, kBottomBorder);
  SetSizer(totalSizer_);
}

void 
BalanceInquiryForm::setGridPanel()
{
  gridPanel_->addRowGrid(wxT("ACCOUNT ID"), wxT("长度6位，数字"));
  gridPanel_->addRowGrid(wxT("ACCOUNT PASSWORD"), wxT("长度6-8位"), 1);
}


void 
BalanceInquiryForm::setButtonPanel()
{
  buttonPanel_->addThreeButtons();
}

void 
BalanceInquiryForm::OnSubmit(wxCommandEvent &event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_BALANCE_INQUIRY);
    // 1. 本地验证是否合法
    if(!validateLocal(bs))
      return;
    // 2. 网络验证
    if(!validateNet(bs))
      return;

    assert(bs.getErrorCode() == 0);
    showReceiptForm(bs);
  }
  catch(Exception &e){
    wxLogError(wxT("BankSession %s"), e.what());
  }
}
void
BalanceInquiryForm::showReceiptForm(BankSession& bs)
{
  ReceiptForm receipt;
  receipt.addTitle(wxT("取款成功"));
  receipt.addItemText(wxT("交易日期:"), bs.getResponseItem("DATE"));
  receipt.addItemText(wxT("户    名:"), bs.getResponseItem("USERNAME"));
  receipt.addItemText(wxT("账    号:"), bs.getRequestItem("ACCOUNT_ID"));
  receipt.addItemText(wxT("余    额:"), bs.getResponseItem("BALANCE"));
  receipt.addBottomButton(ReceiptForm::ButtonType::ButtonOK);
  receipt.ShowModal();
}

void 
BalanceInquiryForm::OnReset(wxCommandEvent &event)
{
  gridPanel_->resetText();
}

void 
BalanceInquiryForm::OnCancel(wxCommandEvent &event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}

bool 
BalanceInquiryForm::validateLocal(BankSession &session)
{
  // 本地验证
  // 1. 验证ACCOUNT ID
  auto accountIdTc = gridPanel_->getTextCtrl(wxT("ACCOUNT ID")); 
  auto accountId = accountIdTc->GetLineText(0);
  if(!Validator::validateAccountIdAndTip(accountId))
    return false;
  // 2. 验证密码
  auto passwordTc = gridPanel_->getTextCtrl(wxT("ACCOUNT PASSWORD")); 
  auto password = passwordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(password))
    return false;
  // 构造Request包
  // 余额查询请求包 account_id + pass
  session.setRequestItem("ACCOUNT_ID", accountId);
  session.setRequestItem("PASSWORD", password);

  return true;
}

bool 
BalanceInquiryForm::validateNet(BankSession &bs)
{
  GetTransactionManager().doAction(bs);
  // 根据返回的错误码和错误msg反馈
  if(bs.getErrorCode() != 0)
  {
    wxLogError(wxT("error: %s"), wxString::FromUTF8(bs.getErrorMsg()));
    return false;
  }
  return true;
}
