//c++ program
//design by >^.^<
//@date    2019-07-16 06:32:04
#include "QueryDayBill.h"

#include "../BankSession.h"
#include "../../public/PackStream.h"
#include "../../public/UnpackStream.h"
#include "../../public/CommonVar.h"
#include "../../public/TransDetail.h"
#include "../UI/GridPanel.h"

using namespace PUBLIC;
using namespace CMD;
using namespace std;

// 请求包 page 4bytes + date 10bytes
//
void QueryDayBill::execute(BankSession& session)
{
	PackStream packStream;

	uint16_t cmd = CMD_DAY_BILL;
	packStream << cmd;

  // 定位长度，并跳过，计算包体长度后再填
	size_t lengthPos = packStream.length();
	packStream.skip(2);

  // 请求包 page + date
	auto page = session.getRequestItem("PAGE");
  uint32_t page32= stoi(string(page.c_str()));
  packStream << page32;

	auto date = session.getRequestItem("DATE");
	packStream.writeBytesFrom(date.data(), REQUEST_DATE_LENGTH);

	size_t length = packStream.length();
  // 长度计算，需要用总的包来减去包头的长度
	packStream.replace16t(lengthPos, static_cast<uint16_t>(length - sizeof(RequestHead)));

  assert(length == kQueryDayBillRequestPackLength);
	session.send(packStream.data(), length);	// 发送请求包

  // 接收处理,
	session.recv();	// 接收应答包
	UnpackStream unpackStream((const char*)session.getResponsePack(), session.getResponsePack()->head.len+sizeof(ResponseHead));
	// 跳过cmd、len
	unpackStream.skip(4);
	uint16_t cnt;
	uint16_t seq;
	int16_t error_code;
	unpackStream >> cnt >> seq >> error_code;

	char error_msg[31];
  // 应答包 trans_date 19 + account_id 6 + other_account_id 6
  //  + money + abstract_name + balance + total 4
  if(!error_code)
  {
    unpackStream.skip(30);

    char trans_date[19+1] = {0};
    unpackStream.readBytesTo(trans_date, 19);
    char account_id[6+1] = {0};
    unpackStream.readBytesTo(account_id, 6);
    char other_account_id[6+1] = {0};
    unpackStream.readBytesTo(other_account_id, 6);
    string money, abstract_name, balance;
    unpackStream >> money >> abstract_name >> balance;
    int total;
    unpackStream >> total;
    auto transDetailVecPtr = session.getTransDetailVecPtr();
    transDetailVecPtr->resize(cnt);
    transDetailVecPtr->at(seq).trans_date = trans_date;
    transDetailVecPtr->at(seq).account_id = account_id;
    transDetailVecPtr->at(seq).other_account_id = other_account_id;
    transDetailVecPtr->at(seq).money = stod(money);
    transDetailVecPtr->at(seq).balance = stod(balance);
    transDetailVecPtr->at(seq).abstract_name = abstract_name;
    transDetailVecPtr->at(seq).total = total;
    transDetailVecPtr->at(seq).trans_id = to_string(seq);
    for(int i = 0; i < cnt -1; ++i)
    {
      money.clear();
      abstract_name.clear();
      balance.clear();

      session.recv();
      UnpackStream unpack2((const char*)session.getResponsePack(), session.getResponsePack()->head.len+sizeof(ResponseHead));
      unpack2.skip(6); // skip cmd + len + cnt
      unpack2 >> seq;
      unpack2.skip(2); // skip error_code
      unpack2.skip(30); // skip error_msg
      unpack2.readBytesTo(trans_date, 19);
      unpack2.readBytesTo(account_id, 6);
      unpack2.readBytesTo(other_account_id, 6);
      unpack2 >> money >> abstract_name >> balance;
      unpack2 >> total;
      transDetailVecPtr->at(seq).trans_date = trans_date;
      transDetailVecPtr->at(seq).account_id = account_id;
      transDetailVecPtr->at(seq).other_account_id = other_account_id;
      transDetailVecPtr->at(seq).money = stod(money);
      transDetailVecPtr->at(seq).balance = stod(balance);
      transDetailVecPtr->at(seq).abstract_name = abstract_name;
      transDetailVecPtr->at(seq).trans_id = to_string(seq);
      transDetailVecPtr->at(seq).total = total;
    }
  }
  else{
    unpackStream.readBytesTo(error_msg, 30);
    session.setErrorMsg(error_msg);
  }
	session.setErrorCode(error_code);
}


