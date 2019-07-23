#pragma once

#include "Transaction.h"

namespace CMD
{

// ĞŞ¸ÄÃÜÂë

class ChangePassword : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}

