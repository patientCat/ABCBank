#pragma once

#include "Transaction.h"

namespace CMD
{

// ���

class Deposit : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}

