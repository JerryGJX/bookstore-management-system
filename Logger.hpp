//
// Created by JerryGuo on 2021/12/19.
//

#ifndef UNTITLED1_LOGGER_HPP
#define UNTITLED1_LOGGER_HPP

#include "UserManager.hpp"
#include "BookManager.hpp"

class Logger {
    UserManager &user_manager;
    BookManager &book_manager;
public:
    Logger(UserManager &user_manager_, BookManager &book_manager_);  // 与所给的 UserManager 和 BookManager 关联
    void ShowFinance(int time = -1);  // 输出财务记录
    void ReportMyself();  // 输出员工自己的操作记录
    void ReportFinance();  // 生成财务记录报告
    void ReportWork();  // 生成全体员工工作情况报告
    void ReportEmployee();  // 生成员工工作情况表，记录其操作
    void ShowLog();  // 生成日志，包括谁干了什么，以及财务上每一笔交易
    void Log(const char *command);  // 写入日志文件
};

#endif //UNTITLED1_LOGGER_HPP
