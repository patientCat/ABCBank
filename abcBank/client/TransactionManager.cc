//c++ program
//design by >^.^<
//@date    2019-07-15 20:15:38
#include "TransactionManager.h"

#include "CMD/UserLogin.h"
#include "CMD/OpenAccount.h"
#include "CMD/Deposit.h"
#include "CMD/Withdrawal.h"
#include "CMD/Transfer.h"
#include "CMD/BalanceInquiry.h"
#include "CMD/ChangePassword.h"
#include "CMD/QueryDayBill.h"
#include "CMD/QueryHistoryBill.h"
#include "CMD/QueryAccountHistoryBill.h"
#include "CMD/CloseAccount.h"

using namespace CMD;

TransactionManager::TransactionManager()
{
  actionsMap_[CMD_LOGIN] = new UserLogin;
  actionsMap_[CMD_OPEN_ACCOUNT] = new OpenAccount;
  actionsMap_[CMD_DEPOSIT] = new Deposit;
  actionsMap_[CMD_WITHDRAW] = new Withdrawal;
  actionsMap_[CMD_TRANSFER] = new Transfer;
  actionsMap_[CMD_BALANCE_INQUIRY] = new BalanceInquiry;
  actionsMap_[CMD_CHANGE_PASSWORD] = new ChangePassword;
  actionsMap_[CMD_DAY_BILL] = new QueryDayBill;
  actionsMap_[CMD_HISTORY_BILL] = new QueryHistoryBill;
  actionsMap_[CMD_ACCOUNT_HISTORY_BILL] = new QueryAccountHistoryBill;
  actionsMap_[CMD_CLOSE_ACCOUNT] = new CloseAccount;
}

TransactionManager::~TransactionManager()
{
  for(auto value : actionsMap_)
  {
    delete value.second;
  }
}

bool
TransactionManager::doAction(BankSession &session)
{
  auto cmd = session.getCmd();
  if(actionsMap_.count(cmd) != 0)
  {
    actionsMap_[cmd]->execute(session);
    return true;
  }
  return false;
}
