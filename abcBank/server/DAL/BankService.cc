#include "BankService.h"

#include "muduo/base/Logging.h"
#include "muduo/base/Exception.h"
#include "muduo/base/Timestamp.h"
#include "MysqlDB.h"
#include "Account.h"
#include "../../public/TransDetail.h"

#include <sstream>
#include <stdio.h>
#include <string.h>

using namespace DAL;
using namespace muduo;
using namespace PUBLIC;

int BankService::userLogin(const string& user, const string& pass)
{
  // ������
  // -1 ���ݿ����
  // 1  �û������������
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    char buff[1024] = {0};
    snprintf(buff, sizeof(buff), 
        "select teller_name from teller where teller_name='%s' and teller_pass='%s';",
        user.c_str(), pass.c_str());
    MysqlRecordset res;
    res = db.querySQL(buff);
    if(res.getRows() < 1)
      return 1;
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
    return -1;
  }
	return 0;
}


// ����
int BankService::openAccount(Account& account)
{
  // ������
  // -1 ���ݿ����
  // 1  �û������������
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    // ��������
    db.startTransaction();

    char buff[1024] = {0};
    // ���˻��������Ϣ
    snprintf(buff, sizeof(buff), 
        "insert into bank_account values(null, '%s', '%s', '%s', '%10.2f', now(), 0);",
        account.name.c_str(), account.pass.c_str(), account.id.c_str(), account.balance);

    auto ret = db.execSQL(buff);

    account.account_id = static_cast<int>(db.getInsertId());

    bzero(buff, sizeof(buff));
    // ���ױ������Ϣ
    snprintf(buff, sizeof(buff), 
        "insert into trans values(null, '%d', null, 1, '%10.2f', '%10.2f', now());",
        account.account_id, account.balance, account.balance);
    ret = db.execSQL(buff);
    db.commit();

    //
    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "select open_date from bank_account where account_id='%d';",
        account.account_id);
    MysqlRecordset rs = db.querySQL(buff);
    account.op_date = rs.getItem(0, "open_date");
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
    // ���ʧ��Ҫ�ع�����
    db.rollback();
    return -1;
  }
	return 0;
	
}


// ���
int BankService::deposit(Account& account)
{
  // ������
  // -1 ���ݿ����
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    // ��������
    db.startTransaction();

    char buff[1024] = {0};
    // �Ȳ鿴�˻������
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and flag=0 for update;",
        account.account_id);
    MysqlRecordset rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �˻�������
      return 2;

    // �õ��˻����
    double balance = stod(rs.getItem(0, "balance"));

    bzero(buff, sizeof(buff));
    // �����˻����=money + ���
    snprintf(buff, sizeof(buff), 
        "update bank_account set balance=balance+%10.2f where account_id=%d;",
        account.money, account.account_id);
    db.execSQL(buff);

    bzero(buff, sizeof(buff));
    // �������׼�¼��2�������ȡmoney����ʱ���Ϊmoney + balance
    snprintf(buff, sizeof(buff), 
        "insert into trans values(null, %d, null, 2, %10.2f, %10.2f, now());",
        account.account_id, account.money, balance + account.money);
    db.execSQL(buff);
    int trans_id = static_cast<int>(db.getInsertId());
    db.commit();

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "select a.name, b.trans_date from bank_account a, (select account_id, trans_date from trans where trans_id=%d) b where a.account_id=b.account_id;", 
        trans_id);
    rs.clear();
    rs = db.querySQL(buff);
    account.name = rs.getItem(0, "name");
    account.op_date = rs.getItem(0, "trans_date");
    account.balance = account.money + balance;
  }
  catch(Exception& e)
  {
    // ���ʧ��Ҫ�ع�����
    db.rollback();
    LOG_INFO << e.what();
    return -1;
  }
	return 0;
}

// ȡ��
int BankService::withDraw(Account& account)
{
  // ������
  // -1 ���ݿ����
  // 2 �˻�������
  // 3 �������
  // 4 ����
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    // ��������
    db.startTransaction();

    char buff[1024] = {0};
    // �Ȳ鿴�˻��Ƿ����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and flag=0;",
        account.account_id);
    MysqlRecordset rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �˻�������
      return 2;

    bzero(buff, sizeof(buff));
    // �����˻�+����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and pass=%s for update;",
        account.account_id, account.pass.c_str());
    rs.clear();
    rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �������
      return 3;

    double balance = stod(rs.getItem(0, "balance"));

    if(account.money > balance) // ����
      return 4;

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "update bank_account set balance=balance-%10.2f where account_id=%d;",
        account.money, account.account_id);
    db.execSQL(buff);

    bzero(buff, sizeof(buff));
    // �������׼�¼��3����ȡ���ȡmoney����ʱ���Ϊmoney + balance
    snprintf(buff, sizeof(buff), 
        "insert into trans values(null, %d, null, 3, %10.2f, %10.2f, now());",
        account.account_id, account.money, balance - account.money);
    db.execSQL(buff);
    int trans_id = static_cast<int>(db.getInsertId());
    db.commit();

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "select a.name, b.trans_date from bank_account a, (select account_id, trans_date from trans where trans_id=%d) b where a.account_id=b.account_id;", 
        trans_id);
    rs.clear();
    rs = db.querySQL(buff);
    account.name = rs.getItem(0, "name");
    account.op_date = rs.getItem(0, "trans_date");
    account.balance = balance - account.money;
  }
  catch(Exception& e)
  {
    // ���ʧ��Ҫ�ع�����
    db.rollback();
    LOG_INFO << e.what();
    return -1;
  }
	return 0;
}
// ת��
int BankService::transfer(Account& account, const string& otherAccountId)
{
  // ������
  // -1 ���ݿ����
  // 2 �˻�������
  // 3 �������
  // 4 ����
  int peerId = stoi(otherAccountId);
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    // ��������
    db.startTransaction();

    char buff[1024] = {0};
    // �Ȳ鿴�˻��Ƿ����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and flag=0;",
        account.account_id);
    MysqlRecordset rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �˻�������
      return 2;

    bzero(buff, sizeof(buff));
    // �����˻�+����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and pass=%s for update;",
        account.account_id, account.pass.c_str());
    rs.clear();
    rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �������
      return 3;

    double balance = stod(rs.getItem(0, "balance"));

    if(account.money > balance) // ����
      return 4;

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and flag=0 for update;",
        peerId);
    rs.clear();
    rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �Է��˻�������
      return 5;

    double peerBalance = stod(rs.getItem(0, "balance"));

    // �����˻�����money
    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "update bank_account set balance=balance-%f where account_id=%d;",
        account.money, account.account_id);
    db.execSQL(buff);
    // �Զ��˻�����money
    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "update bank_account set balance=balance+%f where account_id=%d;",
        peerBalance, peerId);
    db.execSQL(buff);

    bzero(buff, sizeof(buff));
    // �������׼�¼��5����ת��account_id->peerid  ��ʱ���Ϊmoney + balance
    snprintf(buff, sizeof(buff), 
        "insert into trans values(null, %d, %d, 5, %10.2f, %10.2f, now());",
        account.account_id, peerId, account.money, balance - account.money);
    db.execSQL(buff);

    int trans_id = static_cast<int>(db.getInsertId());

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "select a.name, b.trans_date from bank_account a, (select account_id, trans_date from trans where trans_id=%d) b where a.account_id=b.account_id;", 
        trans_id);
    rs.clear();
    rs = db.querySQL(buff);

    bzero(buff, sizeof(buff));
    // �������׼�¼��4����'��'ת��peerId->accountId  ��ʱ���Ϊmoney + balance
    snprintf(buff, sizeof(buff), 
        "insert into trans values(null, %d, %d, 4, %10.2f, %10.2f, now());",
         peerId, account.account_id, account.money, balance - account.money);
    db.execSQL(buff);

    db.commit();


    account.balance = balance - account.money;
    account.name = rs.getItem(0, "name");
    account.op_date = rs.getItem(0, "trans_date");
  }
  catch(Exception& e)
  {
    // ���ʧ��Ҫ�ع�����
    db.rollback();
    LOG_INFO << e.what();
    return -1;
  }
	return 0;
}
// ��ѯ���
int BankService::balanceInquiry(Account& account)
{
  // ������
  // -1 ���ݿ����
  // 2 �˻�������
  // 3 �������
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    char buff[1024] = {0};
    // �Ȳ鿴�˻��Ƿ����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and flag=0;",
        account.account_id);
    MysqlRecordset rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �˻�������
      return 2;

    bzero(buff, sizeof(buff));
    // �����˻�+����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and pass=%s for update;",
        account.account_id, account.pass.c_str());
    rs.clear();
    rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �������
      return 3;

    double balance = stod(rs.getItem(0, "balance"));

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), "select name, now() query_date from bank_account where account_id=%d;",
        account.account_id);
    rs.clear();
    rs = db.querySQL(buff);
    account.name = rs.getItem(0, "name");
    account.op_date = rs.getItem(0, "query_date");
    account.balance = balance;
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
    return -1;
  }
	return 0;
}

// �޸�����
int BankService::changePassword(Account& account, const string& newPass)
{
  // ������
  // -1 ���ݿ����
  // 2 �˻�������
  // 3 �������
  // 4 ����
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    char buff[1024] = {0};
    // �Ȳ鿴�˻��Ƿ����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and flag=0;",
        account.account_id);
    MysqlRecordset rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �˻�������
      return 2;

    bzero(buff, sizeof(buff));
    // �����˻�+����
    snprintf(buff, sizeof(buff), 
        "select balance from bank_account where account_id=%d and pass=%s for update;",
        account.account_id, account.pass.c_str());
    rs.clear();
    rs = db.querySQL(buff);
    if(rs.getRows() < 1) // �������
      return 3;

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), "update bank_account set pass='%d' where account_id=%d;",
        stoi(newPass), account.account_id);
    db.execSQL(buff);
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
    return -1;
  }
	return 0;
}

// ��ѯĳ�ձ���
int BankService::queryDayBill(list<TransDetail>& result, int page, const string& date)
{
  // ������
  // -1 ���ݿ����
  // 2 �˻�������
  MysqlDB db;
  try{
    db.open("localhost", "root", "123", "bank");

    char buff[1024] = {0};
    // �Ȳ鿴�˻��Ƿ����
    snprintf(buff, sizeof(buff), 
        "select count(*) as total from  \
        (select * from trans where date_format(trans_date,'%%Y-%%m-%%d')='%s')\
        as a, abstract b where a.abstract_id=b.abstract_id;",
        date.c_str());
    MysqlRecordset rs = db.querySQL(buff);
    
    int total = stoi(rs.getItem(0, "total"));
    LOG_INFO << "total = " << total;

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), 
        "select a.account_id, a.other_account_id, b.name, a.money, a.balance, a.trans_date from\
        (select * from trans where date_format(trans_date, '%%Y-%%m-%%d')='%s') \
        a, abstract b where a.abstract_id = b.abstract_id order by a.trans_date limit %d, 15;",
        date.c_str(), page*15);
    rs.clear();
    rs = db.querySQL(buff);
    if(rs.getRows() < 1) // no data found
      return 6;
    LOG_INFO << "rows " << rs.getRows();

    for(unsigned int i=0; i<rs.getRows(); ++i)
    {
      TransDetail td;
      td.account_id = rs.getItem(i, "account_id");
      td.other_account_id = rs.getItem(i, "other_account_id");
      td.abstract_name = rs.getItem(i, "name");
      td.money = stod(rs.getItem(i, "money"));
      td.balance = stod(rs.getItem(i, "balance"));
      td.trans_date = rs.getItem(i, "trans_date");
      td.total = total;
      result.push_back(td);
    }
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
    return -1;
  }
	return 0;
}
// �������ڲ�ѯ��ϸ
int BankService::queryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end)
{
	
	return 0;
}
// �����ʺŲ�ѯ��ϸ
int BankService::queryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end)
{
	
	return 0;
}

// ����
int BankService::closeAccount(Account& account, double& interest)
{
	
	return 0;

}
