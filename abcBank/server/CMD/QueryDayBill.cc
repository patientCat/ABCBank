#include "QueryDayBill.h"

#include "muduo/base/Logging.h"
#include "../../public/UnpackStream.h"
#include "../../public/PackStream.h"
#include "../../public/StringUtil.h"
#include "../../public/Encrypt.h"
#include "../../public/CommonVar.h"
#include "../../public/TransDetail.h"
#include "../BankSession.h"
#include "../DAL/BankService.h"

#include <string.h>
#include <list>

using namespace PUBLIC;
using namespace CMD;
using namespace muduo;
using namespace DAL;

// 转账请求包 包体
// page 4bytes
// date 10bytes
void QueryDayBill::execute(BankSession& session)
{
  // 将包体放到解包流中
	UnpackStream unpack(session.getRequestPack()->buf, session.getRequestPack()->head.len);
	uint16_t cmd = session.getCmd();
  // 解包page
  uint32_t page;
  unpack >> page;
  // 解包date
  char date[REQUEST_DATE_LENGTH+1] = {0};
  unpack.readBytesTo(date, REQUEST_DATE_LENGTH);
  string dateString(date);

	int16_t error_code = 0;
	char error_msg[ERROR_MESSAGE_LENGTH] = {0};

  // 解包后，向数据库进行验证
  LOG_INFO << page;
  LOG_INFO << dateString;

  BankService service;

  std::list<TransDetail> transDetailList;
  int ret = service.queryDayBill(transDetailList, page, dateString);
  if(ret == 6)
  {
    error_code = 6;
    strncpy(error_msg, "no data found", sizeof(error_msg));
    LOG_INFO << error_msg;
  }
  else if(ret == -1)
  {
    error_code = -1;
    strncpy(error_msg, "database error", sizeof(error_msg));
    LOG_INFO << error_msg;
  }
  else if(ret == 0)
  {
    error_code = 0;
    LOG_INFO << "queryDayBill success";
  }

  // 构造response包
  // 包头：cmd + len + cnt + seq + error_code + err_msg
  // 包体：name, lalance, date 19bytes
  if(!error_code)
  {
    uint16_t cnt = static_cast<uint16_t>(transDetailList.size());
    uint16_t seq = 0;
    for(auto &it : transDetailList)
    {
      PackStream& pack = session.getNextPackStream();
      // 包头 cmd + len + cnt + seq + error_code + msg;
      pack << cmd;
      int lengthPos = pack.length();
      pack.skip(2);
      pack << cnt;
      pack << seq;
      ++seq;
      pack << error_code;
      pack.skip(30);

      // 包体：trans_date 19 + account_id 6 + other_account_id 6 +
      // money + abstract_name + balance + total 4
      pack.writeBytesFrom(it.trans_date.data(), TRANS_DATE_LENGTH); 
      pack.writeBytesFrom(fillZeroBefore(it.account_id, ACCOUNT_ID_LENGTH).data(), ACCOUNT_ID_LENGTH); 
      pack.writeBytesFrom(fillZeroBefore(it.other_account_id, ACCOUNT_ID_LENGTH).data(), ACCOUNT_ID_LENGTH); 
      string money = formatDoubleString(to_string(it.money), 2);
      pack << money;
      pack << it.abstract_name;
      LOG_INFO << "abstract name.size() " << it.abstract_name.size();
      string balance = formatDoubleString(to_string(it.balance), 2);
      pack << balance;
      pack << it.total;

      pack.replace16t(lengthPos, static_cast<uint16_t>(pack.length() - sizeof(ResponseHead)));
      LOG_INFO << "queryDayBill send " << pack.length();
    }
  }
  else{
    uint16_t len=0, cnt=0, seq=0;

    PackStream& pack = session.getFirstPackStream();
    pack << cmd;
    pack << len;
    pack << cnt << seq << error_code;
    pack.writeBytesFrom(error_msg, 30);
    assert(pack.length() == 40);
  }

  // 计算包体长度，然后<<到PackStream中
}



