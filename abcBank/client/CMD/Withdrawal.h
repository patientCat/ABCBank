#pragma once

#include "Transaction.h"

namespace CMD
{

// ȡ��

class Withdrawal : public Transaction
{
public:
	virtual void execute(BankSession& session) override;
};

}

