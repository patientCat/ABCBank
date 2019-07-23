#include "ChangePassword.h"

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
#include <iostream>
using namespace std;

using namespace PUBLIC;
using namespace CMD;
using namespace muduo;
using namespace DAL;

// 取款请求包 包体
// account_id 6bytes
// pass 16bytes
// newPass 16bytes
void ChangePassword::execute(BankSession& session)
{
  // 将包体放到解包流中
	UnpackStream unpack(session.getRequestPack()->buf, session.getRequestPack()->head.len);
	uint16_t cmd = session.getCmd();
  // 解包account_id
  char account_id[ACCOUNT_ID_LENGTH+1] = {0};
  unpack.readBytesTo(account_id, ACCOUNT_ID_LENGTH);
  // 解包pass
  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  char pass[PASSWORD_LENGTH+1] = {0};
  unpack.readBytesTo(encPass, ENCRYPTED_PASS_LENGTH);
  Encrypt::ideaDecrypt(pass, encPass, cmd); 
  // 解包newPass
  char encNewPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  char newPass[PASSWORD_LENGTH+1] = {0};
  unpack.readBytesTo(encNewPass, ENCRYPTED_PASS_LENGTH);
  Encrypt::ideaDecrypt(newPass, encNewPass, cmd); 


  // 解包后，向数据库进行验证

  BankService service;
  Account account;
  account.account_id = stoi(account_id);
  account.pass = pass;

	int16_t error_code = 0;
	char error_msg[ERROR_MESSAGE_LENGTH+1] = {0};
  int ret = service.changePassword(account, newPass);
  if(ret == 0)
  {
    error_code = 0;
    LOG_INFO << "[" << account_id << "]"<< " change password success";
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

  // 构造response包
  // 包头：cmd + len + cnt + seq + error_code + err_msg
  // 包体：空
  PackStream &pack = session.getFirstPackStream();
	pack << cmd;
  size_t lengthPos = pack.length();
  assert(lengthPos == 2);
  pack.skip(2);
	uint16_t cnt = 0;
	uint16_t seq = 0;
	pack << cnt << seq << error_code;
	pack.writeBytesFrom(error_msg, ERROR_MESSAGE_LENGTH);

  // 计算包体长度，然后<<到PackStream中
  uint16_t len = pack.length() - sizeof(ResponseHead);
  pack.replace16t(lengthPos, len);
  assert(pack.length() == kChangePasswordResponsePackLength);
}



