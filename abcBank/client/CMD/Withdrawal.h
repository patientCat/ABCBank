#pragma once

#include "Transaction.h"

namespace CMD
{

// È¡¿î

class Withdrawal : public Transaction
{
public:
	virtual void execute(BankSession& session) override;
};

}

