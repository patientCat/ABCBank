#pragma once

#include "Transaction.h"

namespace CMD
{

// ����
class CloseAccount : public Transaction
{
public:
	virtual void execute(BankSession& session) override;
};

}

