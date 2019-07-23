//c++ program
//design by >^.^<
//@date    2019-07-07 06:47:32
#include "FormManager.h"

#include "MainMenuForm.h"
#include "BalanceInquiryForm.h"
#include "CloseAccountForm.h"
#include "ChangePasswordForm.h"
#include "DepositForm.h"
#include "DetailStatementForm.h"
#include "LoginForm.h"
#include "OpenAccountForm.h"
#include "TransferForm.h"
#include "WithdrawForm.h"
#include "QueryDayBillForm.h"
#include "QueryHistoryBillForm.h"
#include "QueryAccountHistoryBillForm.h"

#include "MyFrame.h"

FormManager::FormManager()
{
  myframe_ = new MyFrame(wxT("abcBank")); 

  loginForm_ = new LoginForm(myframe_);
  mainMenuForm_ = new MainMenuForm(myframe_);
  openAccountForm_ = new OpenAccountForm(myframe_);
  depositForm_ = new DepositForm(myframe_);
  withdrawForm_ = new WithdrawForm(myframe_);
  changePasswordForm_ = new ChangePasswordForm(myframe_);
  transferForm_ = new TransferForm(myframe_);
  balanceInquiryForm_ = new BalanceInquiryForm(myframe_);
  closeAccountForm_ = new CloseAccountForm(myframe_);

  detailStatementForm_ = new DetailStatementForm(myframe_);
  queryDayBillForm_ = new QueryDayBillForm();
  queryHistoryBillForm_ = new QueryHistoryBillForm(detailStatementForm_);
  queryAccountHistoryBillForm_ = new QueryAccountHistoryBillForm(detailStatementForm_);
}
FormManager::~FormManager()
{
  queryDayBillForm_->Destroy();
}
void
FormManager::start()
{
  loginForm_->Show(true);

  mainMenuForm_->Show(false);
  openAccountForm_->Show(false);
  depositForm_->Show(false);
  withdrawForm_->Show(false);
  detailStatementForm_->Show(false);
  changePasswordForm_->Show(false);
  transferForm_->Show(false);
  balanceInquiryForm_->Show(false);
  closeAccountForm_->Show(false);
  queryDayBillForm_->Show(false);
  queryHistoryBillForm_->Show(false);
  queryAccountHistoryBillForm_->Show(false);

  myframe_->Show(true);
}

LoginForm * FormManager::getLoginForm()
{
  return loginForm_;
}
MainMenuForm * FormManager::getMainMenuForm()
{
  return mainMenuForm_;
}
OpenAccountForm * FormManager::getOpenAccountForm()
{
  return openAccountForm_;
}
DepositForm * FormManager::getDepositForm()
{
  return depositForm_;
}
WithdrawForm * FormManager::getWithdrawForm()
{
  return withdrawForm_;
}
BalanceInquiryForm * FormManager::getBalanceInquiryForm()
{
  return balanceInquiryForm_;
}
DetailStatementForm * FormManager::getDetailStatementForm()
{
  return detailStatementForm_;
}
ChangePasswordForm * FormManager::getChangePasswordForm()
{
  return changePasswordForm_;
}
TransferForm * FormManager::getTransferForm()
{
  return transferForm_;
}
CloseAccountForm * FormManager::getCloseAccountForm()
{
  return closeAccountForm_;
}
QueryDayBillForm * FormManager::getQueryDayBillForm()
{
  return queryDayBillForm_;
}
QueryHistoryBillForm * FormManager::getQueryHistoryBillForm()
{
  return queryHistoryBillForm_;
}
QueryAccountHistoryBillForm * FormManager::getQueryAccountHistoryBillForm()
{
  return queryAccountHistoryBillForm_;
}
