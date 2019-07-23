//c++ program
//design by >^.^<
//@date    2019-07-08 06:46:31
#include "ChangePasswordForm.h"
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
#include "../BankSession.h"
#include "../TransactionManager.h"
#include "../../public/Exception.h"

using namespace PUBLIC;

BEGIN_EVENT_TABLE(ChangePasswordForm, wxPanel)
  EVT_BUTTON(ID_SUBMIT, ChangePasswordForm::OnSubmit)
  EVT_BUTTON(ID_RESET, ChangePasswordForm::OnReset)
  EVT_BUTTON(ID_CANCEL, ChangePasswordForm::OnCancel)
END_EVENT_TABLE()

ChangePasswordForm::ChangePasswordForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, wxT("ChangePassword")))
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
ChangePasswordForm::setTitlePanel()
{
  titlePanel_->setLevel2();
}


void
ChangePasswordForm::setTotalPanel()
{
  totalSizer_->Add(titlePanel_, 0);
  totalSizer_->Add(gridPanel_, 0,  wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM, kBottomBorder);
  SetSizer(totalSizer_);
}

void 
ChangePasswordForm::setGridPanel()
{
  gridPanel_->addRowGrid(wxT("ACCOUNT ID"), wxT("长度6位，数字"));
  gridPanel_->addRowGrid(wxT("ACCOUNT PASSWORD"), wxT("长度6-8位"), 1);
  gridPanel_->addRowGrid(wxT("NEW PASSWORD"), wxT("同上"), 1);
  gridPanel_->addRowGrid(wxT("REPEAT PASSWORD"), wxT("同上"), 1);

}


void 
ChangePasswordForm::setButtonPanel()
{
  buttonPanel_->addThreeButtons();
}

void 
ChangePasswordForm::OnSubmit(wxCommandEvent &event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_CHANGE_PASSWORD);
    // 1. 本地验证是否合法
    if(!validateLocal(bs))
      return;
    // 2. server端验证是否有效，因此这里要有一个TcpClient来连接服务器。
    if(!validateNet(bs))
      return;

    assert(bs.getErrorCode() == 0);
    showReceiptForm();
  }
  catch(Exception &e){
    wxLogError(wxT("BankSession %s"), e.what());
  }
}
void 
ChangePasswordForm::showReceiptForm()
{
  ReceiptForm receipt;
  receipt.addTitle("ChangePassword");
  receipt.addItemText("Message:", wxT("密码变更成功"));
  receipt.addBottomButton();
  receipt.ShowModal();
}

void 
ChangePasswordForm::OnReset(wxCommandEvent &event)
{
  gridPanel_->resetText();
}

void 
ChangePasswordForm::OnCancel(wxCommandEvent &event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}

bool 
ChangePasswordForm::validateLocal(BankSession& session)
{
  // 本地验证
  // 1. 验证Account id
  auto accountIdTc = gridPanel_->getTextCtrl(wxT("ACCOUNT ID")); 
  auto accountId = accountIdTc->GetLineText(0);
  if(!Validator::validateAccountIdAndTip(accountId))
    return false;
  // 2. 验证密码
  auto passwordTc = gridPanel_->getTextCtrl(wxT("ACCOUNT PASSWORD")); 
  auto password = passwordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(password))
    return false;
  // 3. 验证New密码
  auto newPasswordTc = gridPanel_->getTextCtrl(wxT("NEW PASSWORD")); 
  auto newPassowrd = newPasswordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(password))
    return false;
  // 4. 验证repeat密码
  auto repeatPasswordTc = gridPanel_->getTextCtrl(wxT("REPEAT PASSWORD")); 
  auto repeatPassword = repeatPasswordTc->GetLineText(0);
  if(!Validator::validateRepeatAndTip(newPassowrd, repeatPassword))
    return false;

  // 构造Request包
  // 修改密码请求包 account_id + pass + newpass
  session.setRequestItem("ACCOUNT_ID", accountId);
  session.setRequestItem("PASSWORD", password);
  session.setRequestItem("NEWPASSWORD", newPassowrd);
  return true;
}

bool 
ChangePasswordForm::validateNet(BankSession &bs)
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
