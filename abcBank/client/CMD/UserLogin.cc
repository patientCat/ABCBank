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

  // ��λ���ȣ���������������峤�Ⱥ�����
	size_t lengthPos = packStream.length();
	packStream.skip(2);

// ��½�� username + password
	auto username = session.getRequestItem("USERNAME");
	packStream << username;

	auto password = session.getRequestItem("PASSWORD");
  char encPass[ENCRYPTED_PASS_LENGTH+1] = {0};
  Encrypt::ideaEncrypt(encPass, password.c_str(), cmd);

	packStream.writeBytesFrom(encPass, ENCRYPTED_PASS_LENGTH);

	size_t length = packStream.length();
  // ���ȼ��㣬��Ҫ���ܵİ�����ȥ��ͷ�ĳ���
  uint16_t len = static_cast<uint16_t>(length - sizeof(RequestHead));
	packStream.replace16t(lengthPos, len);

	session.send(packStream.data(), length);	// ���������

  try{
    session.recv();	// ����Ӧ���
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
  }
  // Ӧ������
  // cmd + len + cnt + seq + error_code
  // + error_msg
	UnpackStream unpackStream(
      (const char*)session.getResponsePack(), 
      session.getResponsePack()->head.len+sizeof(ResponseHead) /*���峤��+��ͷ����*/
      ); 
	// ����cmd��len
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


