//c++ program
//design by >^.^<
//@date    2019-07-07 06:20:59
#include "LoginForm.h"
#include "Constant.h" 
#include "MyWxTools.h"
#include "MyFrame.h"
#include "FormManager.h"
#include "TitlePanel.h"
#include "GridPanel.h"
#include "ButtonPanel.h"
#include "Validator.h"
#include "MainMenuForm.h"
#include "../../public/StringUtil.h"
#include "../../public/PackStream.h"
#include "../../public/Socket.h"
#include "../../public/Exception.h"

#include "../BankSession.h"
#include "../TransactionManager.h"

#include <wx/app.h>
#include <iostream>

using namespace PUBLIC;

BEGIN_EVENT_TABLE(LoginForm, wxPanel)
  EVT_BUTTON(ID_LOGIN, LoginForm::OnLogin)
  EVT_BUTTON(wxID_EXIT, LoginForm::OnExit)
END_EVENT_TABLE()

LoginForm::LoginForm(wxFrame *myframe)
  : wxPanel(myframe, -1, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
  , titlePanel_(new TitlePanel(this, "Login To The Bank System")) 
  , gridPanel_(new GridPanel(this, 2, 3, 5, 30))
  , buttonPanel_(new ButtonPanel(this))
{
  titlePanel_->setLevel1(); 

  gridPanel_->addRowGrid("USERNAME", wxT("长度3-10位，字母或数字"));
  gridPanel_->addRowGrid("PASSWORD", wxT("长度6-8位"), 1);

  buttonPanel_->addButton(ID_LOGIN, wxT("LOGIN"));
  buttonPanel_->addButton(wxID_EXIT, wxT("EXIT"));

  totalSizer_->Add(titlePanel_, 0, wxALIGN_CENTER);
  totalSizer_->Add(gridPanel_, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, kTitlePos);
  totalSizer_->Add(new wxPanel(this, -1), 1);
  totalSizer_->Add(buttonPanel_, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, kBottomBorder);

  SetSizer(totalSizer_);

  Centre();
}

void
LoginForm::OnLogin(wxCommandEvent& event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_LOGIN);
    // 1. 本地验证是否合法
    if(!validateLocal(bs))
      return;
    // 2. server端验证是否有效，因此这里要有一个TcpClient来连接服务器。
    if(!validateNet(bs))
      return;

    
    Show(false);
    GetFormManager().getMainMenuForm()->Show(true);
  }
  catch(Exception &e){
    wxLogError(wxT("BankSession %s"), wxString::FromUTF8(e.what()));
  }
}

void
LoginForm::OnExit(wxCommandEvent& event)
{
  event.Skip();
}

bool 
LoginForm::validateLocal(BankSession &bs)
{
  // 验证用户名
  auto usernameTc = gridPanel_->getTextCtrl("USERNAME"); 
  auto username = usernameTc->GetLineText(0);
  if(!Validator::validateNameAndTip(username))
    return false;
  // 验证密码
  auto passwordTc = gridPanel_->getTextCtrl("PASSWORD"); 
  auto password = passwordTc->GetLineText(0);
  if(!Validator::validatePasswordAndTip(password))
    return false;

  // 构造Request包，并将包发送出去
  bs.setRequestItem("USERNAME", username);
  bs.setRequestItem("PASSWORD", password);
  return true;
}
bool 
LoginForm::validateNet(BankSession &bs)
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
