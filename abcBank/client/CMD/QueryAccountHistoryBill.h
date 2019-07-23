#pragma once

#include "Transaction.h"

namespace CMD
{

class QueryAccountHistoryBill : public Transaction
{
public:
	virtual void execute(BankSession& session) override;
};

}

