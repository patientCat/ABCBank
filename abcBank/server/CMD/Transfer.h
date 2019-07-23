#pragma once

#include "Transaction.h"

namespace CMD
{

// ×ªÕÊ

class Transfer : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}


