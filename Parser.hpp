//
// Created by JerryGuo on 2021/12/19.
//

#ifndef UNTITLED1_PARSER_HPP
#define UNTITLED1_PARSER_HPP

#include <unordered_map>
#include <functional>
#include <set>
#include <sstream>

#include "BookManager.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include "UserManager.hpp"

const long long quantity_max = 2147483647;

using std::string;
using std::vector;
using std::cin;
using std::cout;

class CommandParser {
 private:
  // 储存与该类关联的 UserManager 和 BookManager
  UserManager &user_manager;
  BookManager &book_manager;
  Logger &logger;
  // 根据指令的第一个单词查找对应的函数，供 run 函数使用
  std::unordered_map<std::string, void (CommandParser::*)(vector<string> &)> mapFunction;

 public:
  void Run();  // 循环读入指令并解析，直到遇到 quit 或 exit
  // 构造 CommandParser，将其与所给的 UserManager，BookManager 和 Logger 关联起来
  CommandParser(UserManager &user_manager_, BookManager &book_manager_, Logger &logger_);

  void ParseSu(vector<string> &cmd);  // 解析 su [User-ID] ([Password])?
  void ParseLogout(vector<string> &cmd);  // 解析 logout
  void ParseRegister(vector<string> &cmd);  // 解析 register [User-ID] [Password] [User-Name]
  void ParsePasswd(vector<string> &cmd);  // 解析 passwd [User-ID] ([Old-Password])? [New-Password]
  void ParseUseradd(vector<string> &cmd);  // 解析 useradd [User-ID] [Password] [Priority] [User-Name]
  void ParseDelete(vector<string> &cmd);  // 解析 delete [User-ID]

  void ParseShow(vector<string> &cmd);  // 解析 show，调用 showBook 或 showFinance

  void ParseShowBook(vector<string> &cmd);

  // 解析 show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
  void ParseBuy(vector<string> &cmd);  // 解析 buy [ISBN] [Quantity]
  void ParseSelect(vector<string> &cmd);  // 解析 select [ISBN]
  void ParseModify(
      vector<string> &cmd);  // 解析 modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
  void ParseImport(vector<string> &cmd);  // 解析 import [Quantity] [Total-Cost]

  void ParseReport(vector<string> &cmd);  // 解析 report myself 或 report finance 或 report employee
  void ParseShowFinance(vector<string> &cmd);  // 解析 show finance ([Time])?
  void ParseLog(vector<string> &cmd);  // 解析 log

  static string GetType(const string &cmd, const int &max_len, const int &min_len);

  static string GetContent(const string &cmd, const int &max_len, const int &min_len);

  static bool IdCheck(const string &Id_);

  static bool PasswordCheck(const string &password_);

  static bool UserNameCheck(const string &UserName_);

  static bool PriorityCheck(const string &Priority_);

  static bool IsbnCheck(const string &ISBN_);

  static bool BookNameCheck(const string &bookName_);

  static bool AuthorCheck(const string &author_);

  static bool KeywordCheck(const string &keyword_);

  static bool KeywordRepeatCheck(const string &keyword_, const char &flag);

  static bool QuantityCheck(const string &quantity_);

  static bool PriceCheck(const string &price_);

  static bool TotalCostCheck(const string &total_cost_);

  static bool TimeCheck(const string &Time_);

  bool CheckPriority(const int &);

};

#endif //UNTITLED1_PARSER_HPP
