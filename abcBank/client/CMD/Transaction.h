#pragma once

#include "../BankSession.h"

class GridPanel;

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

