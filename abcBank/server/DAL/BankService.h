#pragma once

#include "IBankService.h"

namespace DAL
{


class BankService : public IBankService
{
public:
	// 用户登录
	int userLogin(const string& user, const string& pass);
	// 开户
	int openAccount(Account& account);
	// 销户
	int closeAccount(Account& account, double& interest);
	// 修改密码
	int changePassword(Account& account, const string& newPass);
	// 存款
	int deposit(Account& account);
	// 取款
	int withDraw(Account& account);
	// 转帐
	int transfer(Account& account, const string& otherAccountId);
	// 查询余额
	int balanceInquiry(Account& account);
	// 查询某日报表
	int queryDayBill(list<PUBLIC::TransDetail>& result, int page, const string& date);
	// 根据日期查询明细
	int queryHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& begin, const string& end);
	// 根据帐号查询明细
	int queryAccountHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& accountId, const string& begin, const string& end);
};

}
