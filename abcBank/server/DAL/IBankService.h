#pragma once

#include "Account.h"

#include <list>
using namespace std;

namespace PUBLIC {
  struct TransDetail;
}
namespace DAL
{

class IBankService
{
public:
	// �û���¼
	virtual int userLogin(const string& user, const string& pass) = 0;
	// ����
	virtual int openAccount(Account& account) = 0;
	// ����
	virtual int closeAccount(Account& account, double& interest) = 0;
	// �޸�����
	virtual int changePassword(Account& account, const string& newPass) = 0;
	// ���
	virtual int deposit(Account& account) = 0;
	// ȡ��
	virtual int withDraw(Account& account) = 0;
	// ת��
	virtual int transfer(Account& account, const string& otherAccountId) = 0;
	// ��ѯ���
	virtual int balanceInquiry(Account& account) = 0;
	// ��ѯĳ�ձ���
	virtual int queryDayBill(list<PUBLIC::TransDetail>& result, int page, const string& date) = 0;
	// �������ڲ�ѯ��ϸ
	virtual int queryHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& begin, const string& end) = 0;
	// �����ʺŲ�ѯ��ϸ
	virtual int queryAccountHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& accountId, const string& begin, const string& end) = 0;
};

}
