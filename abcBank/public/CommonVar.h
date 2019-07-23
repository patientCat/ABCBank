#pragma once

namespace PUBLIC
{

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
namespace {
// 字段长度
const int PASSWORD_LENGTH = 8;
const int ACCOUNT_ID_LENGTH = 6;
const int ERROR_MESSAGE_LENGTH = 30;
const int ENCRYPTED_PASS_LENGTH = 16;
const int IDENTIFY_ID_LENGTH = 18;
const int LEN_LENGTH = 2;
const int REQUEST_DATE_LENGTH = 10;
const int PAGE_LENGTH = 4;
const int TRANS_DATE_LENGTH = 19;

// 包的长度
const int kRequestHeadLength = 4;
const int kResponseHeadLength = 40;
const int kChangePasswordRequestPackLength = 42;
const int kChangePasswordResponsePackLength = 40;
const int kQueryDayBillRequestPackLength = 18;
}

}// end namespace PUBLIC
