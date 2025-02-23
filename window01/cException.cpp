#include "cException.hpp"
#include <iostream>
#include <sstream>




cException::cException(int _line, const char* _file, const char* _msg) noexcept
    :cException(_line,_file,_msg, 0,"")
{
   
}

cException::cException(int _line, const char* _file, const char* _msg, unsigned long _errCode, const char* _errDesc) noexcept
    :line(_line), file(_file), errMsg(_msg), errCode(_errCode),errDesc(_errDesc)
{
}

cException::~cException()
{
}

const char* cException::what() const noexcept
{
    std::ostringstream oss;
    oss << "[ErrCode]: " << errCode << " [ErrMsg]: " << errMsg << " [ErrDesc]: " << errDesc << std::endl
        << "[File]: " << file << std::endl << "[Line]: " << line;   
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

//const char* cException::what() const noexcept
//{
//    std::ostringstream oss;
//    oss << GetType() <<  " " << msg << std::endl << GetOriginString();
//    whatBuffer = oss.str();
//    return whatBuffer.c_str();
//}
//
//const char* cException::GetType() const noexcept
//{
//    return "cException";
//}
//
//int cException::GetLine() const noexcept
//{
//    return line;
//}
//
//const std::string& cException::GetFile() const noexcept
//{
//    return file;
//}
//
std::string cException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file << std::endl << "[Line] " << line;
    return oss.str();
}
