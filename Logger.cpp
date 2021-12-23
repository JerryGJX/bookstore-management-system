//
// Created by JerryGuo on 2021/12/19.
//

#include "Logger.hpp"
Logger::Logger(UserManager &user_manager_, BookManager &book_manager_):book_manager(book_manager_),user_manager(user_manager_) {}
void Logger::ShowFinance(int time) {
cout<<"ShowFinance"<<endl;
}
void Logger::ReportMyself() {
  cout<<"ReportMyself"<<endl;
}
void Logger::ReportFinance() {
  cout<<"ReportFinance"<<endl;
}
void Logger::ReportWork() {
  cout<<"ReportWork"<<endl;
}
void Logger::ReportEmployee() {
  cout<<"ReportEmployee"<<endl;
}
void Logger::ShowLog() {
  cout<<"ShowLog"<<endl;
}
void Logger::Log(const char *command) {
  cout<<"Log"<<endl;
}
