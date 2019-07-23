//c++ program
//design by >^.^<
//@date    2019-07-12 18:50:33
#include "TransactionManager.h"

#include "./CMD/UserLogin.h"
#include "./CMD/OpenAccount.h"
#include "./CMD/Deposit.h"
#include "./CMD/Withdrawal.h"
#include "./CMD/Transfer.h"
#include "./CMD/BalanceInquiry.h"
#include "./CMD/ChangePassword.h"
#include "./CMD/QueryDayBill.h"
#include "./CMD/QueryHistoryBill.h"
#include "./CMD/QueryAccountHistoryBill.h"
#include "./CMD/CloseAccount.h"

#include "muduo/base/Logging.h"
#include "muduo/base/Exception.h"

// 构造函数是私有的，只允许在友元Singleton中被构造
// 构造函数中初始化map
#include <iostream>
using namespace std;
using namespace CMD;
using namespace muduo;
using namespace muduo::net;

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
  //actionsMap_[CMD_HISTORY_BILL] = new QueryHistoryBill;
  //actionsMap_[CMD_ACCOUNT_HISTORY_BILL] = new QueryAccountHistoryBill;
  //actionsMap_[CMD_CLOSE_ACCOUNT] = new CloseAccount;
}

TransactionManager::~TransactionManager()
{
  for(auto elem : actionsMap_)
  {
    delete elem.second;
  }
}

bool 
TransactionManager::doAction(BankSession& session)
{
  uint16_t cmd = session.getCmd();
  // todo 做出根据cmd打印对应命令的日志
  switch(cmd)
  {
    case CMD_LOGIN :
      LOG_INFO << "CMD_LOGIN"; break;
    case CMD_OPEN_ACCOUNT :
      LOG_INFO << "CMD_OPEN_ACCOUNT"; break;
    case CMD_DEPOSIT :
      LOG_INFO << "CMD_DEPOSIT"; break;
    case CMD_WITHDRAW :
      LOG_INFO << "CMD_WITHDRAW"; break;
    case CMD_TRANSFER :
      LOG_INFO << "CMD_TRANSFER"; break;
    case CMD_BALANCE_INQUIRY :
      LOG_INFO << "CMD_BALANCE_INQUIRY"; break;
    case CMD_CHANGE_PASSWORD :
      LOG_INFO << "CMD_CHANGE_PASSWORD"; break;
    case CMD_DAY_BILL :
      LOG_INFO << "CMD_DAY_BILL"; break;
    case CMD_HISTORY_BILL :
      LOG_INFO << "CMD_HISTORY_BILL"; break;
    case CMD_ACCOUNT_HISTORY_BILL :
      LOG_INFO << "CMD_ACCOUNT_HISTORY_BILL"; break;
    case CMD_CLOSE_ACCOUNT :
      LOG_INFO << "CMD_CLOSE_ACCOUNT"; break;
    default:
      LOG_INFO << "unkown cmd";
  }
  if(actionsMap_.count(cmd) != 0)
  {
    try{
      actionsMap_[cmd]->execute(session);
    }
    catch(Exception& e){
      LOG_INFO << e.what();
    }
    return true;
  }
  return false;
}
