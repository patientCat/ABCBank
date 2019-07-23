#pragma once

#include "StringUtil.h"
#include <exception>
#include <string>
#include <wx/string.h>

namespace PUBLIC
{

class Exception : public std::exception
{
public:
	explicit Exception(const std::string& message)
		: message_(message)
	{
		FillStackTrace();
	}


	virtual ~Exception() throw()
	{

	}

	virtual const char* what() const throw();
	const char* StackTrace() const throw();

private:
	void FillStackTrace();

  std::string message_;
  std::string stackTrace_;
};

//
//class SocketException : public Exception
//{
//public:
//	explicit SocketException(const char* message)
//		: Exception(message)
//	{
//
//	}
//	explicit SocketException(const wxString& message)
//		: Exception(message)
//	{
//
//	}
//	virtual ~SocketException() throw()
//	{
//
//	}
//};
//
//
//class SocketInputException : public SocketException
//{
//public:
//	explicit SocketInputException(const char* message)
//		: SocketException(message)
//	{
//
//	}
//	explicit SocketInputException(const wxString& message)
//		: SocketException(message)
//	{
//
//	}
//	virtual ~SocketInputException() throw()
//	{
//
//	}
//};
//
//
//class SocketOutputException : public SocketException
//{
//public:
//	explicit SocketOutputException(const char* message)
//		: SocketException(message)
//	{
//
//	}
//	explicit SocketOutputException(const wxString& message)
//		: SocketException(message)
//	{
//
//	}
//	virtual ~SocketOutputException() throw()
//	{
//
//	}
//};
//
//class DBException : public Exception
//{
//public:
//	explicit DBException(const char* message)
//		: Exception(message)
//	{
//
//	}
//	explicit DBException(const wxString& message)
//		: Exception(message)
//	{
//
//	}
//	virtual ~DBException() throw()
//	{
//
//	}
//};

}

