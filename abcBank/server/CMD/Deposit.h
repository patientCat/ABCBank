#pragma once

#include "Transaction.h"

namespace CMD
{

// ´æ¿î

class Deposit : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}

