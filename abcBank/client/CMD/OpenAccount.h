#pragma once

#include "Transaction.h"


namespace CMD
{

// ����
class OpenAccount : public Transaction
{
public:
	virtual void execute(BankSession& session) override;
};

}

