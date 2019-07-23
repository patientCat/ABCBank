#include "StringUtil.h"

#include <sstream>
using namespace PUBLIC;
using namespace std;

int Convert::stringToInt(const string& str)
{
	stringstream ss;
	ss<<str;
	int x;
	ss>>x;

	return x;
}

string Convert::intToString(int x)
{
	stringstream ss;
	ss<<x;
	string str;
	ss>>str;

	return str;
}

double Convert::stringToDouble(const string& str)
{
	stringstream ss;
	ss<<str;
	double x;
	ss>>x;

	return x;
}

string Convert::doubleToString(double x)
{
	stringstream ss;
	ss<<x;
	string str;
	ss>>str;

	return str;
}

#ifdef WX_STRING
std::string 
Convert::wxStringToString(const wxString& wxstr)
{
  string ret(wxstr.data(), wxstr.size());
  return ret;
}
#endif
