#include "UserLogin.h"

#include "muduo/base/Logging.h"
#include "../../public/UnpackStream.h"
#include "../../public/PackStream.h"
#include "../../public/Encrypt.h"
#include "../../public/Endian.h"
#include "../../public/CommonVar.h"
#include "../BankSession.h"
#include "../DAL/BankService.h"

#include <string.h>

using namespace PUBLIC;
using namespace CMD;
using namespace muduo;
using namespace DAL;

// 登陆请求包name + pass
void UserLogin::execute(BankSession& session)
{
  // 将包体放到解包流中
	UnpackStream unpack(session.getRequestPack()->buf, session.getRequestPack()->head.len);
	uint16_t cmd = session.getCmd();
	std::string name;
	unpack >> name;

  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  unpack.readBytesTo(encPass, ENCRYPTED_PASS_LENGTH);
  char pass[PASSWORD_LENGTH+1] = {0};
  Encrypt::ideaDecrypt(pass, encPass, cmd);

	int16_t error_code = 0;
	char error_msg[ERROR_MESSAGE_LENGTH+1] = {0};

  // 解包后，向数据库进行验证
  BankService service;
  int ret = service.userLogin(name, pass);
  if(ret == 0)
  {
    LOG_INFO << "[" << name << "]"<< " login success";
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
  // 包体：空
  // 响应包为固定40字节
  uint16_t len = 0;
  PackStream &pack = session.getFirstPackStream();
	pack << cmd;
  pack << len; 
	uint16_t cnt = 0;
	uint16_t seq = 0;
	pack << cnt << seq << error_code;
	pack.writeBytesFrom(error_msg, 30);
}


