#include "Withdrawal.h"

#include "muduo/base/Logging.h"
#include "muduo/base/Exception.h"
#include "../../public/UnpackStream.h"
#include "../../public/PackStream.h"
#include "../../public/StringUtil.h"
#include "../../public/Encrypt.h"
#include "../../public/CommonVar.h"
#include "../BankSession.h"
#include "../DAL/BankService.h"

#include <string.h>

using namespace PUBLIC;
using namespace CMD;
using namespace muduo;
using namespace DAL;

// 取款请求包 包体
// account_id 6bytes
// pass 16bytes
// money
void Withdrawal::execute(BankSession& session)
{
  // 将包体放到解包流中
	UnpackStream unpack(session.getRequestPack()->buf, session.getRequestPack()->head.len);
	uint16_t cmd = session.getCmd();
  // 解包account_id
  char account_id[ACCOUNT_ID_LENGTH+1] = {0};
  unpack.readBytesTo(account_id, 6);
  // 解包pass
  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  char pass[PASSWORD_LENGTH+1] = {0};
  unpack.readBytesTo(encPass, ENCRYPTED_PASS_LENGTH);
  Encrypt::ideaDecrypt(pass, encPass, cmd); 
  // 解包money
  string money;
  unpack >> money;

	int16_t error_code = 0;
	char error_msg[ERROR_MESSAGE_LENGTH+1] = {0};

  // 解包后，向数据库进行验证

  BankService service;
  Account account;
  account.account_id = stoi(account_id);
  account.pass = pass;
  account.money = stod(money);

  LOG_INFO << account.account_id;
  LOG_INFO << account.pass;
  LOG_INFO << account.money;

  int ret = service.withDraw(account);
  if(ret == 0)
  {
    error_code = 0;
    LOG_INFO << "[" << account_id << "]"<< " withdraw success";
  }
  else if(ret == -1)
  {
    error_code = -1;
    strncpy(error_msg, "database error", sizeof(error_msg));
    LOG_INFO << error_msg;
  }
  else if(ret == 2)
  {
    error_code = 2;
    strncpy(error_msg, "account doesn't exit", sizeof(error_msg));
    LOG_INFO << account_id << " "<< error_msg;
  }
  else if(ret == 3)
  {
    error_code = 3;
    strncpy(error_msg, "password error", sizeof(error_msg));
    LOG_INFO << account_id << " "<< error_msg;
  }
  else if(ret == 4)
  {
    error_code = 4;
    strncpy(error_msg, "not enough balance", sizeof(error_msg));
    LOG_INFO << account_id << " "<< error_msg;
  }

  // 构造response包
  // 包头：cmd + len + cnt + seq + error_code + err_msg
  // 包体：name, lalance, date 19bytes
  PackStream &pack = session.getFirstPackStream();
	pack << cmd;
  size_t lengthPos = pack.length();
  pack.skip(2);
	uint16_t cnt = 0;
	uint16_t seq = 0;
	pack << cnt << seq << error_code;
	pack.writeBytesFrom(error_msg, 30);

  if(!error_code)
  {
    // 包体：name, balance, date 19bytes
    pack << account.name;
    // balance 需要进行格式化处理
    char balance[50] = {0};
    snprintf(balance, sizeof(balance), "%-30.2f", account.balance);
    pack << string{balance};
    pack.writeBytesFrom(account.op_date.data(), 19);
  }

  // 计算包体长度，然后<<到PackStream中
  uint16_t len = pack.length() - sizeof(ResponseHead);
  pack.replace16t(lengthPos, len);
}



