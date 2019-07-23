#pragma once

#include "BankSession.h"
#include "./CMD/Transaction.h"
#include "../public/Singleton.h"

#include "boost/noncopyable.hpp"
#include <map>
#include <memory> 
// 一个单例且用来管理命令的基类，使用map存放几个命令的解析类
class TransactionManager
: boost::noncopyable
{
  friend class PUBLIC::Singleton<TransactionManager>;
public:
  bool doAction(BankSession &);

private:
  std::map<uint16_t, CMD::Transaction*> actionsMap_;
  TransactionManager();
  ~TransactionManager();

};
