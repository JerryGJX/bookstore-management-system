//
// Created by JerryGuo on 2021/12/19.
//

#include "Logger.hpp"

//...........Finance............
Finance::Finance() {
  income = 0;
  expense = 0;
}
Finance::Finance(const double &digit) {
  if (digit > 0) {
    income = digit;
    expense = 0;
  } else {
    expense = -digit;
    income = 0;
  };
}
Finance &Finance::operator+=(const Finance &x) {
  income += x.income, expense += x.expense;
  return *this;
}
std::ostream &operator<<(std::ostream &os, const Finance &x) {

  os << std::fixed << std::setprecision(2) << "+ " << x.income << " - "
     << x.expense;
  return os;
}


//.........Log.............

Log::Log(const string &id_, const string &log_) {
  user_id = id_;
  log = log_;
}
std::ostream &operator<<(std::ostream &os, const Log &x) {
  return os;
}


//.........Logger................

Logger::Logger(const string &finance_list_, const string &finance_data_,
               const string &log_data_)
    : finance_list(finance_list_), finance_data(finance_data_), log_data(log_data_) {}

void Logger::ShowFinance(int time) {
  if (!time)cout << endl;
  else {
    finance_data.GetInfo(finance_num);
    //cout<<"finance_num= "<<finance_num<<endl;
    if (time > finance_num)throw Error("EntryNumExceeded");
    int num;
    num = (time == -1) ? static_cast<int>(finance_num) : time;
//    if(time==-1){
//      cout<<num<<endl;
//      cout<<"#-1"<<endl;
//      throw Error("#-1");
//    }
    Finance carrier;
    std::vector<NodeDigit> target_index;
    finance_list.GetAll(target_index);
    int target_size = static_cast<int>(target_index.size());
    Finance answer;
    for (int i = 0; i < num; ++i) {
      finance_data.Read(carrier, target_index[target_size - i - 1].value);
      answer += carrier;
    }
    cout << answer << endl;
  }
//cout<<"ShowFinance"<<endl;
}

void Logger::WriteFinance(const double &digit) {
  finance_data.GetInfo(finance_num);
  finance_num++;
  finance_data.WriteInfo(finance_num);
  Finance carrier(digit);
  int index = finance_data.Write(carrier);
  NodeDigit node_carrier(std::to_string(finance_num), index);
  finance_list.Add(node_carrier);
}

void Logger::ReportMyself() {
  cout << "ReportMyself" << endl;
}
void Logger::ReportFinance() {
  cout << "ReportFinance" << endl;
}
void Logger::ReportWork() {
  cout << "ReportWork" << endl;
}
void Logger::ReportEmployee() {
  cout << "ReportEmployee" << endl;
}
void Logger::ShowLog() {
  cout << "ShowLog" << endl;
}
void Logger::WriteLog(const char *command) {
  cout << "WriteLog" << endl;
}



