#pragma once

#include "Transaction.h"

namespace CMD
{

// ≤È—Ø”‡∂Ó

class BalanceInquiry : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}


