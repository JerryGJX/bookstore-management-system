//
// Created by JerryGuo on 2021/12/19.
//

#ifndef UNTITLED1_LOGGER_HPP
#define UNTITLED1_LOGGER_HPP

#include <iostream>
#include "UserManager.hpp"
#include "BookManager.hpp"

#define LOGLEN 1024

class Finance {
 public:
  double income = 0, expense = 0;

 public:
  Finance();
  Finance(const double &);
  Finance &operator+=(const Finance &);
  friend std::ostream &operator<<(std::ostream &, const Finance &);
};

class Log {
  Char<KEY_SIZE> user_id;
  Char<LOGLEN> log;
 public:
  Log() = default;
  Log(const string &, const string &);
  friend std::ostream &operator<<(std::ostream &, const Log &);
};



class Logger {
  MemoryRiver<Finance, IntWrapper, sizeof(int)> finance_data;
  UnrolledLinkedList<NodeDigit> finance_list;
  MemoryRiver<Node, int, 4> log_data;
  IntWrapper finance_num = 0, log_num = 0;
 public:
  Logger(const string &finance_list_, const string &finance_data_,
         const string &log_data_);
  void ShowFinance(int time = -1);  // 输出财务记录
  void WriteFinance(const double &);
  void ReportMyself();  // 输出员工自己的操作记录
  void ReportFinance();  // 生成财务记录报告
  void ReportWork();  // 生成全体员工工作情况报告
  void ReportEmployee();  // 生成员工工作情况表，记录其操作
  void ShowLog();  // 生成日志，包括谁干了什么，以及财务上每一笔交易
  void WriteLog(const char *command);  // 写入日志文件
};

#endif //UNTITLED1_LOGGER_HPP
