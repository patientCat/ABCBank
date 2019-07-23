#include "Withdrawal.h"

#include "../BankSession.h"
#include "../../public/PackStream.h"
#include "../../public/UnpackStream.h"
#include "../../public/Encrypt.h"
#include "../../public/Exception.h"
#include "../../public/CommonVar.h"
#include "../../public/Logging.h"
#include "../UI/GridPanel.h"

using namespace PUBLIC;
using namespace CMD;
using namespace std;

// 取款请求包 account_id 6bytes + pass 16bytes +  money 
void Withdrawal::execute(BankSession& session)
{
	PackStream packStream;

	uint16_t cmd = CMD_WITHDRAW;
	packStream<<cmd;

  // 定位长度，并跳过，计算包体长度后再填
	size_t lengthPos = packStream.length();
	packStream.skip(2);

  // 取款请求包 account_id + pass +  money
  // pack account_id
	auto account_id = session.getRequestItem("ACCOUNT_ID");
  packStream.writeBytesFrom(account_id.data(), ACCOUNT_ID_LENGTH);

  // pack encPass
	auto pass = session.getRequestItem("PASSWORD");
  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  Encrypt::ideaEncrypt(encPass, pass.c_str(), cmd);
  packStream.writeBytesFrom(encPass, ENCRYPTED_PASS_LENGTH);

	auto money = session.getRequestItem("MONEY");
	packStream << money;

	size_t length = packStream.length();
  // 长度计算，需要用总的包来减去包头的长度
	uint16_t len = static_cast<uint16_t>(length - sizeof(RequestHead));
  packStream.replace16t(lengthPos, len);

	session.send(packStream.data(), length);	// 发送请求包

  try{
    // recv 中已经将cmd + len 解析完成了
    session.recv();	// 接收应答包
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
  }

  // 应答包组成
  // cmd + len + cnt + seq + error_code + error_msg
  // 包体
  // name + balance + date 19bytes
	UnpackStream unpackStream((const char*)session.getResponsePack(), session.getResponsePack()->head.len+sizeof(ResponseHead));
	// 跳过cmd、len
	unpackStream.skip(4);
	uint16_t cnt;
	uint16_t seq;
	int16_t error_code;
	unpackStream >> cnt >> seq >> error_code;

	char error_msg[31];
  if(!error_code)
  {
    unpackStream.skip(30); // 跳过errormsg
    string username;
    unpackStream >> username;
    string balance;
    unpackStream >> balance;
    char date[19+1] = {0};
    unpackStream.readBytesTo(date, 19);

    session.setResponseItem("USERNAME", username);
    session.setResponseItem("BALANCE", balance);
    session.setResponseItem("DATE", date);
  }
  else{
    unpackStream.readBytesTo(error_msg, 30);
  }

	session.setErrorCode(error_code);
	session.setErrorMsg(error_msg);
}


