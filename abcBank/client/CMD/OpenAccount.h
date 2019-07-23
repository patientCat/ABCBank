#pragma once

#include "Transaction.h"


namespace CMD
{

// ¿ª»§
class OpenAccount : public Transaction
{
public:
	virtual void execute(BankSession& session) override;
};

}

