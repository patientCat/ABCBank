#pragma once

#include "../public/Singleton.h"
#include "BankSession.h"
#include "CMD/Transaction.h"
#include <boost/noncopyable.hpp>

class TransactionManager
: boost::noncopyable
{
  friend class PUBLIC::Singleton<TransactionManager>;
public:
  bool doAction(BankSession& session);

private:
  TransactionManager();
  ~TransactionManager();

private:
  std::map<uint16_t, CMD::Transaction*> actionsMap_;
};

#define GetTransactionManager() (PUBLIC::Singleton<TransactionManager>::getInstance())
