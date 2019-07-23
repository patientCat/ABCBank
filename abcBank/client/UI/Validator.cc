//c++ program
//design by >^.^<
//@date    2019-07-10 07:14:04
#include "Validator.h"


#include <cctype>

bool  // 用户名长度3-10位， 字母
Validator:: validateName(const wxString& str) 
{
  for(int i = 0; i < str.size(); ++i)
    if(!isalpha(str[i]))
      return false;
  return str.size() >= 3 && str.size() <= 10;
}
bool  // 密码6-8位
Validator:: validatePassword(const wxString& str)
{
  return str.size() >= 6 && str.size() <= 8;
}
bool  // 身份证18位 必须是数字
Validator:: validateId(const wxString& str)
{
  for(int i = 0; i < str.size(); ++i)
    if(!isdigit(str[i]))
      return false;

  // TODO 验证有效的身份证
  return str.size() == 18;
}
bool // 数字账号6位
Validator:: validateAccountId(const wxString& str)
{
  for(int i = 0; i < str.size(); ++i)
    if(!isdigit(str[i]))
      return false;
  return str.length() == 6;
}
bool // 存款金额必须大于0， 且小数点后俩为
Validator:: validateMoney(const wxString& str)
{
  // 0123456
  //"100.00"
  if(str.empty())
    return false;
  int dotPos = str.find(wxT(".")); 
  if(dotPos != wxString::npos) // 有小数
  {
    for(int i = 0; i < str.size(); ++i)
      if(!isdigit(str[i]) && i != dotPos)
        return false;
    return str.size() - dotPos <= 3;
  } 
  else{ // 没有小数
    for(int i = 0; i < str.size(); ++i)
      if(!isdigit(str[i]))
        return false;
    return true;
  }
}
bool  // 日期不超过10位
Validator:: validateDate(const wxString& str)
{
  for(int i = 0; i < str.size(); ++i)
    // 不是数字或者不是-就算false
    if(str[i] == '-' || isdigit(str[i]))
      ;
    else
      return false;
  return str.size() == 10;
}


bool 
Validator:: validateNameAndTip(const wxString& str)
{
  if(!Validator::validateName(str))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        wxT("用户名3-10位，只能字母"),wxT("Error Username"),  wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  return true;
}

bool 
Validator:: validatePasswordAndTip(const wxString& str)
{
  if(!Validator::validatePassword(str))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        wxT("密码6-8位"),wxT("Error Password"),  wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  return true;
}
bool 
Validator:: validateIdAndTip(const wxString& str)
{
  if(!Validator::validateId(str))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        wxT("身份证id为18位"),wxT("Error Id"),  wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  return true;
}
bool 
Validator:: validateAccountIdAndTip(const wxString& str)
{
  if(!Validator::validateAccountId(str))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        wxT("账户id为6位，纯数字"),wxT("Error AccountId"),  wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  return true;
}
bool 
Validator:: validateMoneyAndTip(const wxString& str)
{
  if(!Validator::validateMoney(str))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        wxT("存款金额必须大于0，最多小数点后俩位"),wxT("Error Money"),  wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  return true;
}
bool 
Validator:: validateDateAndTip(const wxString& str)
{
  if(!Validator::validateDate(str))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        wxT("日期必须有效"),wxT("Error Date"),  wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  return true;
}
bool 
Validator:: validateRepeatAndTip(const wxString& str1, const wxString& str2)
{
  if(str1 != str2)
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        wxT("密码和上面不同"),wxT("Error RepeatPassword"),  wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  return true;
}
