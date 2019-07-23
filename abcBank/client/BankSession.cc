#include "BankSession.h"
#include "../public/CommonVar.h"
#include "../public/Endian.h"
#include "../public/Exception.h"
#include "../public/PackStream.h"
#include "../public/StringUtil.h"
#include "../public/Logging.h"
#include "../public/TransDetail.h"

#include <wx/wx.h>

using namespace std;
using namespace PUBLIC;

BankSession::BankSession() 
  : socket_(new Socket)
  , errorCode_(0)
  , transDetailVecPtr(new vector<TransDetail>())
{
	socket_->create();

	if(!socket_->connect(SERVER_IP, SERVER_PORT))
    throw Exception("连接失败");

	responsePack_ = (ResponsePack*)buffer_;
}

BankSession::~BankSession()
{
}

void BankSession::send(const char* buf, size_t len)
{
	socket_->sendN(buf, len);
}

void BankSession::recv()
{
  LOG_INFO << "recv";
	int ret;
	ret = socket_->recvN(buffer_, sizeof(ResponseHead));
	if (ret == 0)
		throw Exception("服务器断开");
	else if (ret != sizeof(ResponseHead))
		throw Exception("数据接收错误");

	uint16_t cmd = Endian::networkToHost16(responsePack_->head.cmd);
	uint16_t len = Endian::networkToHost16(responsePack_->head.len);

	responsePack_->head.cmd = cmd;
	responsePack_->head.len = len;
  LOG_INFO << "recv " << len << "bytes";

	if (len == 0)
		return;

	ret = socket_->recvN(responsePack_->buf, len);
	if (ret == 0)
		throw Exception("服务器断开");
	else if (ret != len)
		throw Exception("数据接收错误");

	
}

void BankSession::clear()
{
	request_.clear();
	response_.clear();
	errorCode_ = 0;
}

void 
BankSession::setRequestItem(const wxString& k, const wxString& v)
{
	request_[k] = v;
}

const wxString& 
BankSession::getRequestItem(const wxString& k)
{
  assert(request_.count(k));
	return request_[k];
}

void 
BankSession::setResponseItem(const wxString& k, const wxString& v)
{
  response_[k] = v;
}
const wxString& 
BankSession::getResponseItem(const wxString& k)
{
  assert(response_.count(k));
  return response_[k];
}
