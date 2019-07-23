//c++ program
//design by >^.^<
//@date    2019-07-08 05:05:03
#include "OpenAccountForm.h"
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

using namespace PUBLIC;


BEGIN_EVENT_TABLE(OpenAccountForm, wxPanel)
  EVT_BUTTON(ID_SUBMIT, OpenAccountForm::OnSubmit)
  EVT_BUTTON(ID_RESET, OpenAccountForm::OnReset)
  EVT_BUTTON(ID_CANCEL, OpenAccountForm::OnCancel)
END_EVENT_TABLE()

OpenAccountForm::OpenAccountForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, "OpenAccount"))
  , gridPanel_(new GridPanel(this, 5, 3, 5, 10))
  , buttonPanel_(new ButtonPanel(this))
{

/*
 * set head panel
 */
 setTitlePanel(); 
/*
 * set grid panel
 */
  setGridPanel();
/*
 * set button panel
 */
  setButtonPanel();
/*
 * totalPanel的设置
 */
  setTotalPanel();

  Centre();
}

void OpenAccountForm::setTitlePanel()
{
  titlePanel_->setLevel2();
}
void OpenAccountForm::setTotalPanel()
{
  totalSizer_->Add(titlePanel_, 0);
  totalSizer_->Add(gridPanel_, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, kBottomBorder);

  SetSizer(totalSizer_);
}

void 
OpenAccountForm::setGridPanel()
{
  gridPanel_->addRowGrid("USERNAME", wxT("长度3-10位"));
  gridPanel_->addRowGrid("IDENTIFY ID", wxT("长度18位"));
  gridPanel_->addRowGrid("ACCOUNT PASSWORD", wxT("长度6-8位"), 1);
  gridPanel_->addRowGrid("REPEAT PASSWORD", wxT("同上"), 1);
  gridPanel_->addRowGrid("MONEY", wxT("小数点最多俩位"));
}

void 
OpenAccountForm::setButtonPanel()
{
  buttonPanel_->addThreeButtons();
}

void 
OpenAccountForm::OnSubmit(wxCommandEvent &event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_OPEN_ACCOUNT);
    // 1. 本地验证是否合法
    if(!validateLocal(bs))
      return;
    // 2. server端验证是否有效，因此这里要有一个TcpClient来连接服务器。
    if(!validateNet(bs))
      return;

    // account_id, date 提供给回执单
    // 显示回执单
    assert(bs.getErrorCode() == 0);
    showReceiptForm(bs);
  }
  catch(Exception &e){
    wxLogError(wxT("BankSession %s"), e.what());
  }
}

void 
OpenAccountForm::showReceiptForm(BankSession& bs)
{
  ReceiptForm receipt;
  receipt.addTitle(wxT("开户成功"));
  receipt.addItemText(wxT("开户日期:"), bs.getResponseItem("DATE"));
  receipt.addItemText(wxT("户名:"), bs.getRequestItem("USERNAME"));
  receipt.addItemText(wxT("账号:"), bs.getResponseItem("ACCOUNT_ID"));
  receipt.addItemText(wxT("金额:"), bs.getRequestItem("MONEY"));
  receipt.addBottomButton(ReceiptForm::ButtonType::ButtonOK);
  receipt.ShowModal();
}

void 
OpenAccountForm::OnReset(wxCommandEvent &event)
{
  gridPanel_->resetText();
}

void 
OpenAccountForm::OnCancel(wxCommandEvent &event)
{
  Show(false);
  GetFormManager().getMainMenuForm()->Show(true);
}

bool 
OpenAccountForm::validateLocal(BankSession &session)
{
  // 本地验证
  // 1. 验证用户名
  auto usernameTc = gridPanel_->getTextCtrl("USERNAME"); 
  auto username = usernameTc->GetLineText(0);
  if(!Validator::validateNameAndTip(username))
    return false;
  // 2. 验证身份ID
  auto idTc = gridPanel_->getTextCtrl("IDENTIFY ID"); 
  auto id = idTc->GetLineText(0);
  if(!Validator::validateIdAndTip(id))
    return false;
  // 3. 验证密码
  auto passwordTc = gridPanel_->getTextCtrl("ACCOUNT PASSWORD"); 
  auto password = passwordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(password))
    return false;
  // 4. 验证repeat密码
  auto repeatPasswordTc = gridPanel_->getTextCtrl("REPEAT PASSWORD"); 
  auto repeatPassword = repeatPasswordTc->GetLineText(0);
  if(!Validator::validateRepeatAndTip(password, repeatPassword))
    return false;
  // 5. 验证money
  auto moneyTc = gridPanel_->getTextCtrl("MONEY"); 
  auto money = moneyTc->GetLineText(0);
  if(!Validator::validateMoneyAndTip(money))
    return false;

  // 构造request包
  // 开户包 name + pass + id + money
  session.setRequestItem("USERNAME", username);
  session.setRequestItem("PASSWORD", password);
  session.setRequestItem("ID", id);
  session.setRequestItem("MONEY", money);
  return true;
}

bool 
OpenAccountForm::validateNet(BankSession& bs)
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
