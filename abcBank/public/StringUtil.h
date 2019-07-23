#pragma once

#include <string>
#ifdef WX_STRING
#include <wx/string.h>
#endif

namespace PUBLIC
{

class Convert
{
public:
	static int stringToInt(const std::string& str);
	static std::string intToString(int x);
	static double stringToDouble(const std::string& str);
	static std::string doubleToString(double x);

#ifdef WX_STRING
  static std::string wxStringToString(const wxString& wxstr);
#endif
};

inline 
std::string fillZeroBefore(const std::string& str, int n)
{
  std::string nZero(n-str.size(), '0');
  nZero.append(str);
  return nZero;
}

inline
std::string formatDoubleString(const std::string &num, int numAfterDot)
{
  std::string ret;
  int dotPos = num.find(".");
  if(dotPos == std::string::npos)
  {
    ret = num;
  }
  else{
    ret = num.substr(0, dotPos+numAfterDot+1);
  }
  return ret;
}
}
