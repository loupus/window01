#pragma once
#include <exception>
#include <string>


class cException //: public std::exception
{
	
protected:
	int line;
	std::string file;
	const std::string errMsg;
	const std::string errDesc;
	mutable std::string whatBuffer;
	const unsigned long errCode;

public:	
	cException(int _line, const char* _file, const char* _msg) noexcept;
	cException(int _line, const char* _file, const char* _msg, unsigned long _errCode, const char* _errDesc) noexcept;
	~cException();
	const char* what() const noexcept;

	//const char* what() const noexcept override;
	//virtual const char* GetType() const noexcept;
	//int GetLine() const noexcept;
	//const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
};

/*
class exception
{
public:
exception() noexcept;
exception(const exception&) noexcept;
exception& operator=(const exception&) noexcept;
virtual ~exception() noexcept;
virtual const char* what() const noexcept;
};

*/
