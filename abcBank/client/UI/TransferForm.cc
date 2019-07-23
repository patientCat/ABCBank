//c++ program
//design by >^.^<
//@date    2019-07-08 06:46:31
#include "TransferForm.h"
#include "Constant.h"
#include "MyWxTools.h"
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
#include "../../public/Logging.h"

using namespace PUBLIC;

BEGIN_EVENT_TABLE(TransferForm, wxPanel)
  EVT_BUTTON(ID_SUBMIT, TransferForm::OnSubmit)
  EVT_BUTTON(ID_RESET, TransferForm::OnReset)
  EVT_BUTTON(ID_CANCEL, TransferForm::OnCancel)
END_EVENT_TABLE()

TransferForm::TransferForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, "Transfer"))
  , gridPanel_(new GridPanel(this, 4, 3, 5, 15))
  , buttonPanel_(new ButtonPanel(this))
{
  setTitlePanel();
  setGridPanel();
  setButtonPanel();
  setTotalPanel();

  Centre();
}

void 
TransferForm::setTitlePanel()
{
  titlePanel_->setLevel2();
}


void
TransferForm::setTotalPanel()
{
  totalSizer_->Add(titlePanel_, 0);
  totalSizer_->Add(gridPanel_, 0,  wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM, kBottomBorder);
  SetSizer(totalSizer_);
}

void 
TransferForm::setGridPanel()
{
  gridPanel_->addRowGrid("ACCOUNT ID", wxT("长度6位，数字"));
  gridPanel_->addRowGrid("ACCOUNT PASSWORD", wxT("长度6-8位"), 1);
  gridPanel_->addRowGrid("MONEY", wxT("小数字俩位"));
  gridPanel_->addRowGrid("TO ACCOUNT ID", wxT("长度6位，数字"));

}


void 
TransferForm::setButtonPanel()
{
  buttonPanel_->addThreeButtons();
}

void 
TransferForm::OnSubmit(wxCommandEvent &event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_TRANSFER);
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
TransferForm::showReceiptForm(BankSession& bs)
{
  ReceiptForm receipt;
  receipt.addTitle(wxT("转账成功"));
  receipt.addItemText(wxT("交易日期:"), bs.getResponseItem("DATE"));
  receipt.addItemText(wxT("户    名:"), bs.getResponseItem("USERNAME"));
  receipt.addItemText(wxT("账    号:"), bs.getRequestItem("ACCOUNT_ID"));
  receipt.addItemText(wxT("对方账号:"), bs.getRequestItem("OTHER_ACCOUNT_ID"));
  receipt.addItemText(wxT("交易金额:"), bs.getRequestItem("MONEY"));
  receipt.addItemText(wxT("摘    要:"), wxT("转账"));
  receipt.addItemText(wxT("余    额:"), bs.getResponseItem("BALANCE"));
  receipt.addBottomButton(ReceiptForm::ButtonType::ButtonOK);
  receipt.ShowModal();
}

void 
TransferForm::OnReset(wxCommandEvent &event)
{
  gridPanel_->resetText();
}

void 
TransferForm::OnCancel(wxCommandEvent &event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}

bool 
TransferForm::validateLocal(BankSession& bs)
{
  // 本地验证
  // 1. 验证account id
  auto accountIdTc = gridPanel_->getTextCtrl("ACCOUNT ID"); 
  auto accountId = accountIdTc->GetLineText(0);
  if(!Validator::validateAccountIdAndTip(accountId))
    return false;
  // 2. 验证密码
  auto passwordTc = gridPanel_->getTextCtrl("ACCOUNT PASSWORD"); 
  auto password = passwordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(password))
    return false;
  // 3. 验证money
  auto moneyTc = gridPanel_->getTextCtrl("MONEY"); 
  auto money = moneyTc->GetLineText(0);
  LOG_INFO << 1 << " " << money.size();
  if(!Validator::validateMoneyAndTip(money))
    return false;
  // 4. 验证身份ID
  auto idTc = gridPanel_->getTextCtrl("TO ACCOUNT ID"); 
  auto id = idTc->GetLineText(0);
  if(!Validator::validateAccountIdAndTip(id))
    return false;

  // 构造Request包
  // 转账请求包 account_id + pass + money + other_account_id
  bs.setRequestItem("ACCOUNT_ID", accountId);
  bs.setRequestItem("PASSWORD", password);
  bs.setRequestItem("MONEY", money);
  bs.setRequestItem("OTHER_ACCOUNT_ID", id);
  return true;
}

bool 
TransferForm::validateNet(BankSession& bs)
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
