#pragma once

#include <string>

namespace PUBLIC
{

// ½»Ò×Ã÷Ï¸
struct TransDetail
{
  std::string trans_id;
  std::string account_id;
  std::string other_account_id;
  std::string abstract_name;
	double money;
	double balance;
  std::string trans_date;
	unsigned int total;
};

}

