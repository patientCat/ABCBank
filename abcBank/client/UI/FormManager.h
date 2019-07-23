#pragma once


#include "boost/noncopyable.hpp"
#include "../../public/Singleton.h"
#include "MyFrame.h"

class LoginForm;
class MainMenuForm;
class OpenAccountForm;
class DepositForm;
class WithdrawForm;
class DetailStatementForm;
class ChangePasswordForm;
class TransferForm;
class BalanceInquiryForm;
class CloseAccountForm;
class QueryDayBillForm;
class QueryHistoryBillForm;
class QueryAccountHistoryBillForm;

class FormManager
: boost::noncopyable
{
  friend class PUBLIC::Singleton<FormManager>;
  FormManager();
  ~FormManager();
public:

  void start();

  LoginForm * getLoginForm();
  MainMenuForm *getMainMenuForm();
  OpenAccountForm *getOpenAccountForm();
  DepositForm *getDepositForm();
  WithdrawForm *getWithdrawForm();
  BalanceInquiryForm *getBalanceInquiryForm();
  DetailStatementForm *getDetailStatementForm();
  ChangePasswordForm *getChangePasswordForm();
  TransferForm *getTransferForm();
  CloseAccountForm *getCloseAccountForm();
  QueryDayBillForm *getQueryDayBillForm();
  QueryHistoryBillForm *getQueryHistoryBillForm();
  QueryAccountHistoryBillForm *getQueryAccountHistoryBillForm();
  

private:
  MyFrame *myframe_; 
  LoginForm *loginForm_;
  MainMenuForm *mainMenuForm_;
  OpenAccountForm *openAccountForm_;
  DepositForm *depositForm_;
  WithdrawForm *withdrawForm_;
  DetailStatementForm *detailStatementForm_;
  ChangePasswordForm *changePasswordForm_;
  TransferForm *transferForm_;
  BalanceInquiryForm *balanceInquiryForm_;
  CloseAccountForm *closeAccountForm_;
  QueryDayBillForm *queryDayBillForm_;
  QueryHistoryBillForm *queryHistoryBillForm_;
  QueryAccountHistoryBillForm *queryAccountHistoryBillForm_;
};

#define GetFormManager() (PUBLIC::Singleton<FormManager>::getInstance())
