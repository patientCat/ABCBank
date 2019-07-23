#include "UserLogin.h"

#include "../BankSession.h"
#include "../../public/PackStream.h"
#include "../../public/UnpackStream.h"
#include "../../public/Exception.h"
#include "../../public/Logging.h"
#include "../../public/Endian.h"
#include "../../public/Encrypt.h"
#include "../../public/CommonVar.h"
#include "../UI/GridPanel.h"

#include <iostream>
using namespace std;
using namespace PUBLIC;
using namespace CMD;

void UserLogin::execute(BankSession& session)
{
	PackStream packStream;

	uint16_t cmd = CMD_LOGIN;
	packStream << cmd;

  // 定位长度，并跳过，计算包体长度后再填
	size_t lengthPos = packStream.length();
	packStream.skip(2);

// 登陆包 username + password
	auto username = session.getRequestItem("USERNAME");
	packStream << username;

	auto password = session.getRequestItem("PASSWORD");
  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  Encrypt::ideaEncrypt(encPass, password.c_str(), cmd);

	packStream.writeBytesFrom(encPass, ENCRYPTED_PASS_LENGTH);

	size_t length = packStream.length();
  // 长度计算，需要用总的包来减去包头的长度
  uint16_t len = static_cast<uint16_t>(length - sizeof(RequestHead));
	packStream.replace16t(lengthPos, len);

	session.send(packStream.data(), length);	// 发送请求包

  try{
    session.recv();	// 接收应答包
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
  }
  // 应答包组成
  // cmd + len + cnt + seq + error_code
  // + error_msg
	UnpackStream unpackStream(
      (const char*)session.getResponsePack(), 
      session.getResponsePack()->head.len+sizeof(ResponseHead) /*包体长度+包头长度*/
      ); 
	// 跳过cmd、len
	unpackStream.skip(4);
	uint16_t cnt;
	uint16_t seq;
	int16_t error_code;
	unpackStream >> cnt >> seq >> error_code;

	char error_msg[31] = {0};
	unpackStream.readBytesTo(error_msg, 30);

	session.setErrorCode(error_code);
	session.setErrorMsg(error_msg);
}


