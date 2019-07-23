#include "ChangePassword.h"

#include "../BankSession.h"
#include "../../public/PackStream.h"
#include "../../public/UnpackStream.h"
#include "../../public/Encrypt.h"
#include "../../public/Exception.h"
#include "../../public/Logging.h"
#include "../../public/CommonVar.h"
#include "../UI/GridPanel.h"

using namespace PUBLIC;
using namespace CMD;

// 修改密码请求包 account_id 6bytes + pass 16byes + newpass 16bytes
// 修改密码响应包 
void ChangePassword::execute(BankSession& session)
{
	PackStream packStream;

	uint16_t cmd = CMD_CHANGE_PASSWORD;
	packStream << cmd;

  // 定位长度，并跳过，计算包体长度后再填
	size_t lengthPos = packStream.length();
	packStream.skip(LEN_LENGTH);

  // 修改密码请求包 account_id + pass + newpass
  // 修改密码响应包 
	auto account_id = session.getRequestItem("ACCOUNT_ID");
	packStream.writeBytesFrom(account_id.data(), ACCOUNT_ID_LENGTH);

	auto pass = session.getRequestItem("PASSWORD");
  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  Encrypt::ideaEncrypt(encPass, pass.c_str(), cmd);
	packStream.writeBytesFrom(encPass, ENCRYPTED_PASS_LENGTH);

	auto newpass = session.getRequestItem("NEWPASSWORD");
  char encNewPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  Encrypt::ideaEncrypt(encNewPass, newpass.c_str(), cmd);
	packStream.writeBytesFrom(encNewPass, ENCRYPTED_PASS_LENGTH);

	size_t length = packStream.length();
  // 长度计算，需要用总的包来减去包头的长度
  uint16_t len = static_cast<uint16_t>(length - sizeof(RequestHead)); 
	packStream.replace16t(lengthPos, len);
  assert(length == kChangePasswordRequestPackLength);

	session.send(packStream.data(), length);	// 发送请求包

  try{
    // recv 中已经将cmd + len 解析完成了
    session.recv();	// 接收应答包
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
  }
  // 应答包包头组成
  // cmd + len + cnt + seq + error_code
  // + error_msg
	UnpackStream unpackStream((const char*)session.getResponsePack(), session.getResponsePack()->head.len+sizeof(ResponseHead));
  assert(unpackStream.length() == kChangePasswordResponsePackLength);
	// 跳过cmd、len
	unpackStream.skip(4);
	uint16_t cnt;
	uint16_t seq;
	int16_t error_code;
	unpackStream >> cnt >> seq >> error_code;

	char error_msg[ERROR_MESSAGE_LENGTH+1];
	session.setErrorCode(error_code);
  if(!error_code)
  {
    unpackStream.skip(ERROR_MESSAGE_LENGTH);
  }
  else{
    unpackStream.readBytesTo(error_msg, ERROR_MESSAGE_LENGTH);
    session.setErrorMsg(error_msg);
  }
}


