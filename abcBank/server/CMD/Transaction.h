#pragma once
#include "../BankSession.h"

namespace CMD
{

// �������
class Transaction
{
public:
	virtual void execute(BankSession& session) = 0;
	virtual ~Transaction() {};
};

}

