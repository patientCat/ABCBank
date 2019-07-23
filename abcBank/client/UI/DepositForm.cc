//c++ program
//design by >^.^<
//@date    2019-07-08 06:46:31
#include "DepositForm.h"
#include "Constant.h"
#include "MyWxTools.h"
#include "GridPanel.h"
#include "MyFrame.h"
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

BEGIN_EVENT_TABLE(DepositForm, wxPanel)
  EVT_BUTTON(ID_SUBMIT, DepositForm::OnSubmit)
  EVT_BUTTON(ID_RESET, DepositForm::OnReset)
  EVT_BUTTON(ID_CANCEL, DepositForm::OnCancel)
END_EVENT_TABLE()

DepositForm::DepositForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, "Deposit"))
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
DepositForm::setTitlePanel()
{
  titlePanel_->setLevel2();
}


void
DepositForm::setTotalPanel()
{
  totalSizer_->Add(titlePanel_, 0);
  totalSizer_->Add(gridPanel_, 0,  wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM, kBottomBorder);
  SetSizer(totalSizer_);
}

void 
DepositForm::setGridPanel()
{
  gridPanel_->addRowGrid("ACCOUNT ID", wxT("长度6位，数字"));
  gridPanel_->addRowGrid("MONEY", wxT("小数字俩位"));

}


void 
DepositForm::setButtonPanel()
{
  buttonPanel_->addThreeButtons();
}

void 
DepositForm::OnSubmit(wxCommandEvent &event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_DEPOSIT);
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
DepositForm::showReceiptForm(BankSession& bs)
{
  ReceiptForm receipt;
  receipt.addTitle(wxT("存款成功"));
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
DepositForm::OnReset(wxCommandEvent &event)
{
  gridPanel_->resetText();
}

void 
DepositForm::OnCancel(wxCommandEvent &event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}

bool 
DepositForm::validateLocal(BankSession& bs)
{
  // 本地验证
  // 1. 验证用户名
  auto accountIdTc = gridPanel_->getTextCtrl("ACCOUNT ID"); 
  auto accountId = accountIdTc->GetLineText(0);
  if(!Validator::validateAccountIdAndTip(accountId))
    return false;
  // 2. 验证money
  auto moneyTc = gridPanel_->getTextCtrl("MONEY"); 
  auto money = moneyTc->GetLineText(0);
  if(!Validator::validateMoneyAndTip(money))
    return false;

  // 构造request包
  // 存款包 account_id + money 
  bs.setRequestItem("ACCOUNT_ID", accountId);
  bs.setRequestItem("MONEY", money);
  return true;
}

bool 
DepositForm::validateNet(BankSession &bs)
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
