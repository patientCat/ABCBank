#pragma once

#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include "../public/Socket.h"
#include "../public/TransDetail.h"
#include "wx/string.h"
#include "boost/noncopyable.hpp"

#ifndef WX_STRING
#define WX_STRING
#endif


#define CMD_LOGIN					0x01
#define CMD_OPEN_ACCOUNT			0x02
#define CMD_DEPOSIT					0x03
#define CMD_WITHDRAW				0x04
#define CMD_TRANSFER				0x05
#define CMD_BALANCE_INQUIRY			0x06
#define CMD_CHANGE_PASSWORD			0x07
#define CMD_DAY_BILL				0x08
#define CMD_HISTORY_BILL			0x09
#define CMD_ACCOUNT_HISTORY_BILL	0x0A
#define CMD_CLOSE_ACCOUNT			0x0B

struct RequestHead
{
	uint16_t cmd;
	uint16_t len;
};

struct ResponseHead
{
	uint16_t cmd;
	uint16_t len;
	uint16_t cnt;
	uint16_t seq;
	uint16_t error_code;
	char error_msg[30];
};

struct RequestPack
{
	RequestHead head;
	char buf[1];
};

struct ResponsePack
{
	ResponseHead head;
	char buf[1];
};

// 主要完成俩个任务
// 1. 构造Request包，然后发送
// 2. 接收，然后解析Response包，然后做出对应的action
class BankSession  
: boost::noncopyable
{
public:
	BankSession();
	~BankSession();

  // 这里是为了让CMD层和UI层进行交互
  // 发送时，UI层设置表项，CMD层通过这里获得然后发送
  // 感觉这里的东西确实应该全部设置为wxString
	void setRequestItem(const wxString& k, const wxString& v);
	const wxString& getRequestItem(const wxString& k);
	void setResponseItem(const wxString& k, const wxString& v);
	const wxString& getResponseItem(const wxString& k);
  // 接收时，CMD层收集数据发送给UI层，UI层进行反馈


	void send(const char* buf, size_t len);
	void recv();
	void clear();

	void setErrorCode(int16_t errorCode)
	{
		errorCode_ = errorCode;
	}

	int16_t getErrorCode() const
	{
		return errorCode_;
	}

	void setErrorMsg(const std::string& errorMsg)
	{
		errorMsg_ = errorMsg;
	}

	const std::string& getErrorMsgString() const
	{
		return errorMsg_;
	}
  const char* getErrorMsg() const
  {
    return errorMsg_.c_str();
  }

	uint16_t getCmd() const
	{
		return cmd_;
	}

	void setCmd(uint16_t cmd)
	{
		cmd_ = cmd;
	}

	ResponsePack* getResponsePack() const { return responsePack_; }

  std::shared_ptr<std::vector<PUBLIC::TransDetail>> getTransDetailVecPtr()
  {
    return transDetailVecPtr;
  }
private:
	std::unique_ptr<PUBLIC::Socket> socket_;
	char buffer_[2048];				// 接收缓冲区
	ResponsePack* responsePack_;

/*
 * 需要的请求数据
 */
	uint16_t cmd_;					// 请求命令
  std::map<wxString,wxString> request_;	// 请求数据

/*
 * 需要的应答数据
 */
  std::map<wxString,wxString> response_;	// 应答数据
	int16_t errorCode_;
  std::string errorMsg_;

  std::shared_ptr<std::vector<PUBLIC::TransDetail>> transDetailVecPtr;
};

