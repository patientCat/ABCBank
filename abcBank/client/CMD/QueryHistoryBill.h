#pragma once

#include "Transaction.h"

namespace CMD
{

class QueryHistoryBill : public Transaction
{
public:
  virtual void execute(BankSession& session) override;
};

}

