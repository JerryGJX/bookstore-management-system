//
// Created by JerryGuo on 2021/12/19.
//

#ifndef UNTITLED1_ERROR_HPP
#define UNTITLED1_ERROR_HPP

//class BasicException : public std::exception {
//    const char *message;
//    BasicException(const char *message_):message(message_){};
//    const char *what() { return message; }
//};
//
//// 格式错误
//class SyntaxError : public BasicException {
//
//};
//
//
//// 密码错误
//class IncorrectPassword : public BasicException {
//
//};
//// 用户名已存在
//class DuplicateUsername : public BasicException {};
//// 用户名不存在
//class UserNotExist : public BasicException {};
//// 权限错误
//class PermissionError : public BasicException {};
//
//
//// 没有匹配的图书
//class NoBookMatched : public BasicException {};
//// 未选择图书
//class NoBookSelected : public BasicException {};
//
//
//// 超过历史交易总笔数
//class EntryNumExceeded : public BasicException {};

#include <ostream>
 class Error: public std::exception{
    const string message;
 public:
  Error()=default;
  Error(const string &error_disc):message(error_disc){};
  Error(const Error &x):message(x.message){};
  friend std::ostream &operator<<(std::ostream &os, const Error &error) {
    os << "message: " << error.message;
    return os;
  }
};

#endif //UNTITLED1_ERROR_HPP
