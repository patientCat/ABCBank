//c++ program
//design by >^.^<
//@date    2019-07-08 06:46:31
#include "WithdrawForm.h"
#include "Constant.h"
#include "MyWxTools.h"
#include "MyFrame.h"
#include "FormManager.h"
#include "TitlePanel.h"
#include "GridPanel.h"
#include "ButtonPanel.h"
#include "MainMenuForm.h"
#include "Validator.h"
#include "ReceiptForm.h"

#include "../TransactionManager.h"
#include "../BankSession.h"
#include "../../public/Exception.h"

using namespace PUBLIC;
BEGIN_EVENT_TABLE(WithdrawForm, wxPanel)
  EVT_BUTTON(ID_SUBMIT, WithdrawForm::OnSubmit)
  EVT_BUTTON(ID_RESET, WithdrawForm::OnReset)
  EVT_BUTTON(ID_CANCEL, WithdrawForm::OnCancel)
END_EVENT_TABLE()

WithdrawForm::WithdrawForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, wxT("Withdraw")))
  , gridPanel_(new GridPanel(this, 3, 3, 5, 15))
  , buttonPanel_(new ButtonPanel(this))
{
  setTitlePanel();
  setGridPanel();
  setButtonPanel();
  setTotalPanel();

  Centre();
}


void 
WithdrawForm::setTitlePanel()
{
  titlePanel_->setLevel2();
}


void
WithdrawForm::setTotalPanel()
{
  totalSizer_->Add(titlePanel_, 0);
  totalSizer_->Add(gridPanel_, 0,  wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM, kBottomBorder);
  SetSizer(totalSizer_);
}

void 
WithdrawForm::setGridPanel()
{
  gridPanel_->addRowGrid(wxT("ACCOUNT ID"), wxT("长度6位，数字"));
  gridPanel_->addRowGrid(wxT("ACCOUNT PASSWORD"), wxT("长度6-8位"));
  gridPanel_->addRowGrid(wxT("MONEY"), wxT("小数字俩位"));

}

void 
WithdrawForm::setButtonPanel()
{
  buttonPanel_->addThreeButtons();
}

void 
WithdrawForm::OnSubmit(wxCommandEvent &event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_WITHDRAW);
    // 1. 本地验证是否合法
    if(!validateLocal(bs))
      return;
    // 2. server端验证是否有效，因此这里要有一个TcpClient来连接服务器。
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
WithdrawForm::showReceiptForm(BankSession& bs)
{
  ReceiptForm receipt;
  receipt.addTitle(wxT("取款成功"));
  receipt.addItemText(wxT("交易日期:"), bs.getResponseItem("DATE"));
  receipt.addItemText(wxT("户    名:"), bs.getResponseItem("USERNAME"));
  receipt.addItemText(wxT("账    号:"), bs.getRequestItem("ACCOUNT_ID"));
  receipt.addItemText(wxT("交易金额:"), bs.getRequestItem("MONEY"));
  receipt.addItemText(wxT("摘    要:"), wxT("取款"));
  receipt.addItemText(wxT("余    额:"), bs.getResponseItem("BALANCE"));
  receipt.addBottomButton(ReceiptForm::ButtonType::ButtonOK);
  receipt.ShowModal();
}
void 
WithdrawForm::OnReset(wxCommandEvent &event)
{
  gridPanel_->resetText();
}

void 
WithdrawForm::OnCancel(wxCommandEvent &event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}

bool 
WithdrawForm::validateLocal(BankSession& bs)
{
  auto accountIdTc = gridPanel_->getTextCtrl(wxT("ACCOUNT ID")); 
  auto accountId = accountIdTc->GetLineText(0);
  if(!Validator::validateAccountIdAndTip(accountId))
    return false;

  auto accountPasswordTc = gridPanel_->getTextCtrl(wxT("ACCOUNT PASSWORD")); 
  auto accountPassword = accountPasswordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(accountPassword))
    return false;

  auto moneyTc = gridPanel_->getTextCtrl(wxT("MONEY")); 
  auto money = moneyTc->GetLineText(0);
  if(!Validator::validateMoneyAndTip(money))
    return false;

  // 构造Request包
  // 取款请求包 account_id + pass +  money
  bs.setRequestItem("ACCOUNT_ID", accountId);
  bs.setRequestItem("PASSWORD", accountPassword);
  bs.setRequestItem("MONEY", money);
  return true;
}
bool 
WithdrawForm::validateNet(BankSession& bs)
{
  GetTransactionManager().doAction(bs);
  return true;
}
