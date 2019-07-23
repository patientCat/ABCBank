#include "OpenAccount.h"

#include "muduo/base/Logging.h"
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

// 开户请求包 name + pass + id + money
// name 不定长
// pass 16字节
// id 18字节
// money 不定长
void OpenAccount::execute(BankSession& session)
{
  // 将包体放到解包流中
	UnpackStream unpack(session.getRequestPack()->buf, session.getRequestPack()->head.len);
	uint16_t cmd = session.getCmd();
  // 解包name
	std::string name;
  unpack >> name;
  // 解包pass
  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  unpack.readBytesTo(encPass, ENCRYPTED_PASS_LENGTH);
  char pass[PASSWORD_LENGTH+1] = {0};
  Encrypt::ideaDecrypt(pass, encPass, cmd);

  // 解包id
  char id[IDENTIFY_ID_LENGTH+1] = {0};
  unpack.readBytesTo(id, IDENTIFY_ID_LENGTH);
  // 解包money
  string money;
  unpack >> money;

	int16_t error_code = 0;
	char error_msg[ERROR_MESSAGE_LENGTH] = {0};

  // 解包后，向数据库进行验证
  BankService service;
  Account account;
  account.id = id;
  account.name = name;
  account.pass = pass;
  account.balance = Convert::stringToDouble(money);
  int ret = service.openAccount(account);
  if(ret == 0)
  {
    error_code = 0;
    LOG_INFO << "[" << name << "]"<< " open account success";
  }
  else if(ret == 1)
  {
    error_code = 1;
    strncpy(error_msg, "username or password error", sizeof(error_msg));
    LOG_INFO << error_msg;
  }
  else if(ret == -1)
  {
    error_code = -1;
    strncpy(error_msg, "database error", sizeof(error_msg));
    LOG_INFO << error_msg;
  }

  // 构造response包
  // 包头：cmd + len + cnt + seq + error_code + err_msg
  // 包体：account_id-6 date-19
  // 响应包为固定40字节
  PackStream &pack = session.getFirstPackStream();
	pack << cmd;
  size_t lengthPos = pack.length();
  pack.skip(2);
	uint16_t cnt = 0;
	uint16_t seq = 0;
	pack << cnt << seq << error_code;
	pack.writeBytesFrom(error_msg, 30);

  // 包体 
  // account_id 6个字节定长
  // date 19个字节定长 yyyy-mm-dd hh:mm:ss
  char account_id[6+1] = {0};
  snprintf(account_id, sizeof(account_id), "%06d", account.account_id);
  pack.writeBytesFrom(account_id, 6);
  char date[19+1] = {0};
  snprintf(date, sizeof(date), "%s", account.op_date.c_str());
  pack.writeBytesFrom(date, 19);

  assert(pack.length() == (40 +6 +19));

  // 计算包体长度，然后<<到PackStream中
  uint16_t len = pack.length() - sizeof(ResponseHead);
  pack.replace16t(lengthPos, len);
}



