#include "OpenAccount.h"

#include "../BankSession.h"
#include "../../public/PackStream.h"
#include "../../public/UnpackStream.h"
#include "../../public/Exception.h"
#include "../../public/Logging.h"
#include "../../public/Encrypt.h"
#include "../UI/GridPanel.h"

using namespace PUBLIC;
using namespace CMD;



// 开户包 name + pass + id + money
void OpenAccount::execute(BankSession& session)
{
	PackStream packStream;

	uint16_t cmd = CMD_OPEN_ACCOUNT;
	packStream<<cmd;

  // 定位长度，并跳过，计算包体长度后再填
	size_t lengthPos = packStream.length();
	packStream.skip(2);

  // 开户包 username + password,16 + id,18 + money
	auto username = session.getRequestItem("USERNAME");
	packStream<<username;

	auto password = session.getRequestItem("PASSWORD");
  // 进行加密到16字节
  char encryptedPass[17] = {0};
  Encrypt::ideaEncrypt(encryptedPass, password.c_str(), cmd);
	packStream.writeBytesFrom(encryptedPass, 16);

	auto id = session.getRequestItem("ID");
	packStream.writeBytesFrom(id.data(), 18);

	auto money = session.getRequestItem("MONEY");
	packStream<<money;

	size_t length = packStream.length();
  uint16_t len =static_cast<uint16_t>(length - sizeof(RequestHead));;
  // 长度计算，需要用总的包来减去包头的长度
	packStream.replace16t(lengthPos, len);

	session.send(packStream.data(), length);	// 发送请求包

  // 应答包组成
  // cmd + len + cnt + seq + error_code + error_msg
  // 如果成功，包体
  // account_id 6个字节定长，date 19个字节定长
  // 失败，包体为0
  try{
    // recv 中已经将cmd + len 解析完成了
    session.recv();	// 接收应答包
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
  }

  // 解析包头
  // 解析 cnt + seq + error_code + error_msg
	UnpackStream unpackStream((const char*)session.getResponsePack(), session.getResponsePack()->head.len+sizeof(ResponseHead));
	// 跳过cmd、len
	unpackStream.skip(4);
	uint16_t cnt;
	uint16_t seq;
	int16_t error_code;
	unpackStream >> cnt >> seq >> error_code;

	char error_msg[31];
  if(!error_code) // 成功
  {
    unpackStream.skip(30);
    assert(session.getResponsePack()->head.len + sizeof(ResponseHead) == 65);
    // 解析包体 
    // account_id 6个字节定长，date 19个字节定长
    char account_id[6+1] = {0};
    unpackStream.readBytesTo(account_id, 6);
    session.setResponseItem("ACCOUNT_ID", account_id);
    char date[19+1] = {0};
    unpackStream.readBytesTo(date, 19);
    session.setResponseItem("DATE", date);
  }
  else{ // 失败
    unpackStream.readBytesTo(error_msg, 30);
    assert(session.getResponsePack()->head.len + sizeof(ResponseHead) == 40);
  }

	session.setErrorCode(error_code);
	session.setErrorMsg(error_msg);

}


