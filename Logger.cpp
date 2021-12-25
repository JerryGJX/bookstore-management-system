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
double Finance::GetIncome() {
  return income;
}
double Finance::GetExpense() {
  return expense;
}


//.........Log.............

Log::Log(const string &flag_, const string &user_id_, const string &log_, const int &priority_) {
  user_id = user_id_;
  log = log_;
  flag = flag_;
  priority = priority_;
}
std::ostream &operator<<(std::ostream &os, const Log &x) {
  os << x.time_ << "\t" << x.user_id << "\t" << x.flag << "\t" << x.log;
  return os;
}
void Log::GetTime() {
  time_t rawtime;
  struct tm *info;
  char buffer[80];
  time(&rawtime);
  info = localtime(&rawtime);
  strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
  string time = buffer;
  time_ = time;
}
Log::Log(const Log &x) {
  time_ = x.time_;
  flag = x.flag;
  user_id = x.user_id;
  log = x.log;
  priority = x.priority;
}
int Log::ThisPriority() {
  return priority;
}
string Log::ThisId() {
  return user_id;
}
Log &Log::operator=(const Log &x) {
  time_ = x.time_;
  flag = x.flag;
  user_id = x.user_id;
  log = x.log;
  priority = x.priority;
  return *this;
}



//.........Logger................

Logger::Logger(const string &finance_list_, const string &finance_data_,
               const string &log_data_, const string &log_rank_by_userid_)
    : finance_list(finance_list_),
      finance_data(finance_data_),
      log_data(log_data_),
      log_rank_by_userid(log_rank_by_userid_) {}

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

    //cout<<"target_size= "<<target_size<<endl;

//    for (int i = 0; i < num; ++i) {
//      cout<<"#number: "<<target_size - i - 1<<"\t"<<"address: "<<target_index[target_size - i - 1].value<<"\t"<<endl;
//    }

    for (int i = 0; i < num; ++i) {
      finance_data.Read(carrier, target_index[target_size - i - 1].value);

      //cout<<"#i= "<<i<<"\t"<<"income= "<<carrier.income<<"\t"<<"expense= "<<carrier.expense<<"\t"<<endl;

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

void Logger::ReportMyself(UserManager &user_manager) {
  string now_user = user_manager.GetNowId();
  log_data.GetInfo(log_num);
  int num;
  num = static_cast<int>(log_num);

  Log carrier;
  std::vector<Node> target_index;
  log_rank_by_userid.GetAll(target_index);
  //int target_size = static_cast<int>(target_index.size());

  for (int i = 0; i < num; ++i) {
    log_data.Read(carrier, target_index[i].value);
    if (carrier.ThisId() == now_user)cout << carrier << endl;
  }
  cout << endl;
}

void Logger::ReportFinance() {
  finance_data.GetInfo(finance_num);
  cout << "finance_num= \t " << finance_num << endl;
  int num;
  num = static_cast<int>(finance_num);

  Finance carrier;
  std::vector<NodeDigit> target_index;
  finance_list.GetAll(target_index);
  //int target_size = static_cast<int>(target_index.size());
  Finance answer;

  for (int i = 0; i < num; ++i) {
    finance_data.Read(carrier, target_index[i].value);

    cout << "第\t " << i << "笔交易\t" << "income= " << carrier.GetIncome() << "\t" << "expense= " << carrier.GetExpense()
         << "\t" << endl;

    answer += carrier;
  }
  cout << "总计：\t" << "income= " << answer.GetIncome() << "\t" << "expense= " << answer.GetExpense()
       << "\t" << endl;
}

void Logger::ReportEmployee() {
  log_data.GetInfo(log_num);
  int num;
  num = static_cast<int>(log_num);

  Log carrier;
  std::vector<Node> target_index;
  log_rank_by_userid.GetAll(target_index);
  //int target_size = static_cast<int>(target_index.size());

  for (int i = 0; i < num; ++i) {
    log_data.Read(carrier, target_index[i].value);
    if (carrier.ThisPriority() == 3)cout << carrier << endl;
  }
  cout << endl;
}

void Logger::ShowLog() {
  log_data.GetInfo(log_num);
  cout << "log_num= \t " << log_num << endl;
  int num;
  num = static_cast<int>(log_num);
  Log carrier;
  std::vector<Node> target_index;
  log_rank_by_userid.GetAll(target_index);
  //int target_size = static_cast<int>(target_index.size());
  for (int i = 0; i < num; ++i) {
    log_data.Read(carrier, target_index[i].value);
    cout << carrier << endl;
  }
  cout << endl;
}

void Logger::WriteLog(const Log &x) {
  Log carrier(x);
  log_data.GetInfo(log_num);
  log_num++;
  log_data.WriteInfo(log_num);
  int index = log_data.Write(carrier);
  Node node_carrier(carrier.ThisId(), index);
  log_rank_by_userid.Add(node_carrier);


  //cout << x << endl;
}

void Logger::FormLog(Log &receiver, const string &flag, const string &command, UserManager &user_manager_) {
  string now_id = user_manager_.GetNowId();
  if (now_id.empty())now_id = "<guest>";
  Log carrier(flag, now_id, command, user_manager_.GetNowPriority());
  carrier.GetTime();
  receiver = carrier;
}



