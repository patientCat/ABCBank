#pragma once

#include "Transaction.h"

namespace CMD
{

// ��Ա��¼
class UserLogin : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}


