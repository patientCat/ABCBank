#pragma once

#include "Transaction.h"

namespace CMD
{

// ¹ñÔ±µÇÂ¼
class UserLogin : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}


