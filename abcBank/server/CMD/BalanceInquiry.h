#pragma once

#include "Transaction.h"

namespace CMD
{

// ��ѯ���

class BalanceInquiry : public Transaction
{
public:
	virtual void execute(BankSession& session);
};

}


