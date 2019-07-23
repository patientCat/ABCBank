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
	// 用户登录
	virtual int userLogin(const string& user, const string& pass) = 0;
	// 开户
	virtual int openAccount(Account& account) = 0;
	// 销户
	virtual int closeAccount(Account& account, double& interest) = 0;
	// 修改密码
	virtual int changePassword(Account& account, const string& newPass) = 0;
	// 存款
	virtual int deposit(Account& account) = 0;
	// 取款
	virtual int withDraw(Account& account) = 0;
	// 转帐
	virtual int transfer(Account& account, const string& otherAccountId) = 0;
	// 查询余额
	virtual int balanceInquiry(Account& account) = 0;
	// 查询某日报表
	virtual int queryDayBill(list<PUBLIC::TransDetail>& result, int page, const string& date) = 0;
	// 根据日期查询明细
	virtual int queryHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& begin, const string& end) = 0;
	// 根据帐号查询明细
	virtual int queryAccountHistoryBill(list<PUBLIC::TransDetail>& result, int page, const string& accountId, const string& begin, const string& end) = 0;
};

}
