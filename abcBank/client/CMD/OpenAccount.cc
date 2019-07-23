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



// ������ name + pass + id + money
void OpenAccount::execute(BankSession& session)
{
	PackStream packStream;

	uint16_t cmd = CMD_OPEN_ACCOUNT;
	packStream<<cmd;

  // ��λ���ȣ���������������峤�Ⱥ�����
	size_t lengthPos = packStream.length();
	packStream.skip(2);

  // ������ username + password,16 + id,18 + money
	auto username = session.getRequestItem("USERNAME");
	packStream<<username;

	auto password = session.getRequestItem("PASSWORD");
  // ���м��ܵ�16�ֽ�
  char encryptedPass[17] = {0};
  Encrypt::ideaEncrypt(encryptedPass, password.c_str(), cmd);
	packStream.writeBytesFrom(encryptedPass, 16);

	auto id = session.getRequestItem("ID");
	packStream.writeBytesFrom(id.data(), 18);

	auto money = session.getRequestItem("MONEY");
	packStream<<money;

	size_t length = packStream.length();
  uint16_t len =static_cast<uint16_t>(length - sizeof(RequestHead));;
  // ���ȼ��㣬��Ҫ���ܵİ�����ȥ��ͷ�ĳ���
	packStream.replace16t(lengthPos, len);

	session.send(packStream.data(), length);	// ���������

  // Ӧ������
  // cmd + len + cnt + seq + error_code + error_msg
  // ����ɹ�������
  // account_id 6���ֽڶ�����date 19���ֽڶ���
  // ʧ�ܣ�����Ϊ0
  try{
    // recv ���Ѿ���cmd + len ���������
    session.recv();	// ����Ӧ���
  }
  catch(Exception& e)
  {
    LOG_INFO << e.what();
  }

  // ������ͷ
  // ���� cnt + seq + error_code + error_msg
	UnpackStream unpackStream((const char*)session.getResponsePack(), session.getResponsePack()->head.len+sizeof(ResponseHead));
	// ����cmd��len
	unpackStream.skip(4);
	uint16_t cnt;
	uint16_t seq;
	int16_t error_code;
	unpackStream >> cnt >> seq >> error_code;

	char error_msg[31];
  if(!error_code) // �ɹ�
  {
    unpackStream.skip(30);
    assert(session.getResponsePack()->head.len + sizeof(ResponseHead) == 65);
    // �������� 
    // account_id 6���ֽڶ�����date 19���ֽڶ���
    char account_id[6+1] = {0};
    unpackStream.readBytesTo(account_id, 6);
    session.setResponseItem("ACCOUNT_ID", account_id);
    char date[19+1] = {0};
    unpackStream.readBytesTo(date, 19);
    session.setResponseItem("DATE", date);
  }
  else{ // ʧ��
    unpackStream.readBytesTo(error_msg, 30);
    assert(session.getResponsePack()->head.len + sizeof(ResponseHead) == 40);
  }

	session.setErrorCode(error_code);
	session.setErrorMsg(error_msg);

}


