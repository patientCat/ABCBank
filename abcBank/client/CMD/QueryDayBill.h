#pragma once

#include "Transaction.h"

namespace CMD
{

class QueryDayBill : public Transaction
{
public:
  virtual void execute(BankSession& session) override;
};

}

