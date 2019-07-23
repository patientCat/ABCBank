#pragma once

#include "IBankService.h"

namespace DAL
{


class BankService : public IBankService
{
public:
	// �û���¼
	int userLogin(const string& user, const string& pass);
	// ����
	int openAccount(Account& account);
	// ����
	int closeAccount(Account& account, double& interest);
	// �޸�����
	int changePassword(Account& account, const string& newPass);
	// ���
	int deposit(Account& account);
	// ȡ��
	int withDraw(Account& account);
	// ת��
	int transfer(Account& account, const string& otherAccountId);
	// ��ѯ���
	int balanceInquiry(Account& account);
	// ��ѯĳ�ձ���
	int queryDayBill(list<PUBLIC::TransDetail>& result, int page, const string& date);
	// �������ڲ�ѯ��ϸ
	int queryHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& begin, const string& end);
	// �����ʺŲ�ѯ��ϸ
	int queryAccountHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& accountId, const string& begin, const string& end);
};

}
