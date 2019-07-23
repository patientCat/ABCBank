//c++ program
//design by >^.^<
//@date    2019-07-12 05:58:53
#include "BankSession.h"
#include "TransactionManager.h"
#include "muduo/base/Logging.h"

using namespace PUBLIC;
using namespace muduo;

void
BankSession::process()
{
  // 首先要根据不同的数据包进行处理，主要是根据cmd字段
  parse();
  buildRequest();
}

void 
BankSession::parse()
{
  if(!Singleton<TransactionManager>::getInstance().doAction(*this))
  {
    LOG_INFO << "请求包解析失败";
  }
}

void 
BankSession::buildRequest()
{
}
