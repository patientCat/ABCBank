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

// �޸���������� account_id 6bytes + pass 16byes + newpass 16bytes
// �޸�������Ӧ�� 
void ChangePassword::execute(BankSession& session)
{
	PackStream packStream;

	uint16_t cmd = CMD_CHANGE_PASSWORD;
	packStream << cmd;

  // ��λ���ȣ���������������峤�Ⱥ�����
	size_t lengthPos = packStream.length();
	packStream.skip(LEN_LENGTH);

  // �޸���������� account_id + pass + newpass
  // �޸�������Ӧ�� 
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
  // ���ȼ��㣬��Ҫ���ܵİ�����ȥ��ͷ�ĳ���
  uint16_t len = static_cast<uint16_t>(length - sizeof(RequestHead)); 
	packStream.replace16t(lengthPos, len);
  assert(length == kChangePasswordRequestPackLength);

	session.send(packStream.data(), length);	// ���������

  try{
    // recv ���Ѿ���cmd + len ���������
    session.recv();	// ����Ӧ���
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
  }
  // Ӧ�����ͷ���
  // cmd + len + cnt + seq + error_code
  // + error_msg
	UnpackStream unpackStream((const char*)session.getResponsePack(), session.getResponsePack()->head.len+sizeof(ResponseHead));
  assert(unpackStream.length() == kChangePasswordResponsePackLength);
	// ����cmd��len
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


