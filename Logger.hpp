//
// Created by JerryGuo on 2021/12/19.
//

#ifndef UNTITLED1_LOGGER_HPP
#define UNTITLED1_LOGGER_HPP

#include <iostream>
#include <time.h>
#include "UserManager.hpp"
#include "BookManager.hpp"

#define LOGLEN 1024

class Finance {
 private:
  double income = 0, expense = 0;

 public:
  Finance();
  Finance(const double &);
  Finance &operator+=(const Finance &);
  friend std::ostream &operator<<(std::ostream &, const Finance &);
  double GetIncome();
  double GetExpense();
};

class Log {
  int priority = 0;
  Char<80> time_;
  Char<20> flag;
  Char<KEY_SIZE> user_id;
  Char<LOGLEN> log;
 public:
  Log() = default;
  Log(const string &flag_, const string &user_id_, const string &log_, const int &priority_);
  Log(const Log &);
  Log &operator=(const Log &rhs);
  friend std::ostream &operator<<(std::ostream &, const Log &);
  void GetTime();
  int ThisPriority();
  string ThisId();
};

class Logger {
  MemoryRiver<Finance, IntWrapper, sizeof(int)> finance_data;
  UnrolledLinkedList<NodeDigit> finance_list;

  MemoryRiver<Log, IntWrapper, sizeof(int)> log_data;
  UnrolledLinkedList<Node> log_rank_by_userid;
  IntWrapper finance_num = 0, log_num = 0;
 public:
  Logger(const string &finance_list_, const string &finance_data_,
         const string &log_data_, const string &log_rank_by_userid);
  void ShowFinance(int time = -1);  // 输出财务记录
  void WriteFinance(const double &);
  void ReportMyself(UserManager &);  // 输出员工自己的操作记录
  void ReportFinance();  // 生成财务记录报告
  //void ReportWork();  // 生成全体员工工作情况报告
  void ReportEmployee();  // 生成员工工作情况表，记录其操作
  void ShowLog();  // 生成日志，包括谁干了什么，以及财务上每一笔交易
  void WriteLog(const Log &x);  // 写入日志文件
  void FormLog(Log &, const string &flag, const string &command, UserManager &user_manager_);
};

#endif //UNTITLED1_LOGGER_HPP
