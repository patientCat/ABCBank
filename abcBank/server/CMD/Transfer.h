#pragma once

#include "Transaction.h"

namespace CMD
{

// ת��

class Transfer : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}


