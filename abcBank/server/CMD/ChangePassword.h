#pragma once

#include "Transaction.h"

namespace CMD
{

// �޸�����

class ChangePassword : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}

