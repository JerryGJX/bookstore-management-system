//
// Created by JerryGuo on 2021/12/19.
//

#include "Parser.hpp"

//UserManager &user_manager;
//BookManager &book_manager;
//Logger &logger;

void CommandParser::Run() {
  std::string parser_carrier;
  if (std::getline(std::cin, parser_carrier)) {
    if (parser_carrier.size() > 1024)throw Error("LengthExceeded");
    logger.FormLog(success, "success", parser_carrier, user_manager);
    logger.FormLog(fail, "fail", parser_carrier, user_manager);

    std::istringstream iss(parser_carrier);
    //if (iss.eof())exit(0);
    std::string first;
    iss >> first;
    if (first == "exit" || first == "quit") {
      std::string second;
      if (iss >> second) throw Error("SyntaxError");
      exit(0);
    } else {
      if (mapFunction.find(first) != mapFunction.end()) {
        // PrintAll();
        vector<string> split_parser;
        while (!iss.eof()) {
          string carrier;
          iss >> carrier;
          if (!carrier.empty()) split_parser.push_back(carrier);
        }
        (this->*mapFunction[first])(split_parser);
      } else throw Error("SyntaxError");
    }
  } else exit(0);
}

CommandParser::CommandParser(UserManager &user_manager_, BookManager &book_manager_, Logger &logger_) : user_manager(
    user_manager_), book_manager(book_manager_), logger(logger_) {
  mapFunction.emplace("su", &CommandParser::ParseSu);
  mapFunction.emplace("logout", &CommandParser::ParseLogout);
  mapFunction.emplace("register", &CommandParser::ParseRegister);
  mapFunction.emplace("passwd", &CommandParser::ParsePasswd);
  mapFunction.emplace("useradd", &CommandParser::ParseUseradd);
  mapFunction.emplace("delete", &CommandParser::ParseDelete);
  mapFunction.emplace("show", &CommandParser::ParseShow);
  //mapFunction.emplace("ShowBook", &CommandParser::ParseShowBook);
  mapFunction.emplace("buy", &CommandParser::ParseBuy);
  mapFunction.emplace("select", &CommandParser::ParseSelect);
  mapFunction.emplace("modify", &CommandParser::ParseModify);
  mapFunction.emplace("import", &CommandParser::ParseImport);
  mapFunction.emplace("report", &CommandParser::ParseReport);
  //mapFunction.emplace("ParseShowFinance", &CommandParser::ParseShowFinance);
  mapFunction.emplace("log", &CommandParser::ParseLog);
}

void CommandParser::ParseSu(vector<string> &cmd) {
  if (!CheckPriority(0))throw Error("PermissionError");

  if (cmd.size() != 1 && cmd.size() != 2) throw Error("SyntaxError");
  if (cmd.size() == 1) {
    if (IdCheck(cmd[0]))user_manager.Login(cmd[0]);
    else throw Error("SyntaxError");
  } else if (cmd.size() == 2) {
    if (IdCheck(cmd[0]) && PasswordCheck(cmd[1]))user_manager.Login(cmd[0], cmd[1]);
    else throw Error("SyntaxError");
  }
}

void CommandParser::ParseLogout(vector<string> &cmd) {
  if (!CheckPriority(1))throw Error("PermissionError");

  if (!cmd.empty())throw Error("SyntaxError");
  user_manager.Logout();
}

void CommandParser::ParseRegister(vector<string> &cmd) {
  if (!CheckPriority(0))throw Error("PermissionError");

  if (cmd.size() != 3)throw Error("SyntaxError");
  if (IdCheck(cmd[0]) && PasswordCheck(cmd[1]) && UserNameCheck(cmd[2]))user_manager.Register(cmd[0], cmd[1], cmd[2]);
  else throw Error("SyntaxError");
}

void CommandParser::ParsePasswd(vector<string> &cmd) {//先new后old
  if (!CheckPriority(1))throw Error("PermissionError");

  if (cmd.size() == 3 && IdCheck(cmd[0]) && PasswordCheck(cmd[1]) && PasswordCheck(cmd[2]))
    user_manager.ChangePassword(cmd[0], cmd[2], cmd[1]);
  else if (cmd.size() == 2 && IdCheck(cmd[0]) && PasswordCheck(cmd[1]))user_manager.ChangePassword(cmd[0], cmd[1]);
  else throw Error("SyntaxError");
}

void CommandParser::ParseUseradd(vector<string> &cmd) {
  if (!CheckPriority(3))throw Error("PermissionError");

  if (cmd.size() != 4) throw Error("SyntaxError");
  if (IdCheck(cmd[0]) && PasswordCheck(cmd[1]) && PriorityCheck(cmd[2]) && UserNameCheck(cmd[3])) {
    int int_temp = std::stoi(cmd[2]);
    user_manager.CreateUser(cmd[0], cmd[1], int_temp, cmd[3]);
  } else throw Error("SyntaxError");
}

void CommandParser::ParseDelete(vector<string> &cmd) {
  if (!CheckPriority(7))throw Error("PermissionError");

  if (cmd.size() != 1)throw Error("SyntaxError");
  if (IdCheck(cmd[0]))user_manager.Remove(cmd[0]);
  else throw Error("SyntaxError");
}

void CommandParser::ParseShow(vector<string> &cmd) {
  if (!cmd.empty() && cmd[0] == "finance")ParseShowFinance(cmd);
  else ParseShowBook(cmd);
}

void CommandParser::ParseShowBook(vector<string> &cmd) {
  if (!CheckPriority(1))throw Error("PermissionError");

  if (cmd.empty())book_manager.ShowBook(BookManager::ALL);
  else if (cmd.size() == 1) {
    string type_carrier = GetType(cmd[0], 20, 3);
    string content = GetContent(cmd[0], 20, 3);
    if (content.empty())throw Error("SyntaxError");
    if (type_carrier == "ISBN") {
      if (IsbnCheck(content)) book_manager.ShowBook(BookManager::ISBN, content); else throw Error("SyntaxError");
    } else if (type_carrier == "name") {
      if (BookNameCheck(content))book_manager.ShowBook(BookManager::NAME, content); else throw Error("SyntaxError");
    } else if (type_carrier == "author") {
      if (AuthorCheck(content))book_manager.ShowBook(BookManager::AUTHOR, content); else throw Error("SyntaxError");
    } else if (type_carrier == "keyword") {
      if (KeywordCheck(content) && !count(content.begin(), content.end(), '|')) {
        book_manager.ShowBook(BookManager::KEYWORD, content);
      } else throw Error("SyntaxError");
    } else throw Error("SyntaxError");
  } else throw Error("SyntaxError");
}

void CommandParser::ParseBuy(vector<string> &cmd) {
  if (!CheckPriority(1))throw Error("PermissionError");

  if (cmd.size() != 2) throw Error("SyntaxError");
  if (IsbnCheck(cmd[0]) && QuantityCheck(cmd[1])) {
    int int_temp = std::stoi(cmd[1]);
    book_manager.BuyBook(cmd[0], int_temp, logger);
  } else throw Error("SyntaxError");
}

void CommandParser::ParseSelect(vector<string> &cmd) {
  if (!CheckPriority(3))throw Error("PermissionError");

  if (cmd.size() != 1) throw Error("SyntaxError");
  if (IsbnCheck(cmd[0]))book_manager.SelectBook(cmd[0], user_manager);
  else throw Error("SyntaxError");
}

void CommandParser::ParseModify(vector<string> &cmd) {
  if (!CheckPriority(3))throw Error("PermissionError");

  if (cmd.size() > 5)throw Error("SyntaxError");
  int num = static_cast<int>(cmd.size());
  std::unordered_map<string, string> info_to_change;
  for (int i = 0; i < num; ++i) {
    string type = GetType(cmd[i], 20, 3);
    string content = GetContent(cmd[i], 20, 3);
    if (content.empty())throw Error("SyntaxError");
    if (info_to_change.find(type) != info_to_change.end())throw Error("SyntaxError");
    if (type == "ISBN") {
      if (IsbnCheck(content)) {
        info_to_change.emplace(type, content);
      } else throw Error("SyntaxError");
    } else if (type == "name") {
      if (BookNameCheck(content)) {
        info_to_change.emplace(type, content);
      } else throw Error("SyntaxError");
    } else if (type == "author") {
      if (AuthorCheck(content)) {
        info_to_change.emplace(type, content);
      } else throw Error("SyntaxError");
    } else if (type == "keyword") {
      if (KeywordCheck(content)) {
        if (KeywordRepeatCheck(content, '|')) {
          info_to_change.emplace(type, content);
        } else throw Error("SyntaxError");
      } else throw Error("SyntaxError");
    } else if (type == "price") {
      if (PriceCheck(content)) {
        info_to_change.emplace(type, content);
      } else throw Error("SyntaxError");
    } else throw Error("SyntaxError");
  }
  book_manager.ModifyBook(info_to_change, user_manager);
}

void CommandParser::ParseImport(vector<string> &cmd) {
  if (!CheckPriority(3))throw Error("PermissionError");

  if (cmd.size() != 2)throw Error("SyntaxError");
  if (QuantityCheck(cmd[0]) && TotalCostCheck(cmd[1])) {
    int quantity_ = std::stoi(cmd[0]);
    double total_cost_ = std::stod(cmd[1]);
    book_manager.ImportBook(quantity_, total_cost_, user_manager, logger);
  } else throw Error("SyntaxError");
}

void CommandParser::ParseReport(vector<string> &cmd) {
  if (!CheckPriority(7))throw Error("PermissionError");

  if (cmd.size() != 1)throw Error("SyntaxError");
  if (cmd[0] == "myself")logger.ReportMyself(user_manager);
  else if (cmd[0] == "finance")logger.ReportFinance();
  else if (cmd[0] == "employee")logger.ReportEmployee();
  else throw Error("SyntaxError");
}

void CommandParser::ParseShowFinance(vector<string> &cmd) {
  if (!CheckPriority(7))throw Error("PermissionError");

  if (cmd.size() == 1)logger.ShowFinance(-1);
  else if (cmd.size() == 2 && TimeCheck(cmd[1])) {
    int carrier = std::stoi(cmd[1]);
    logger.ShowFinance(carrier);
  } else throw Error("SyntaxError");
}

void CommandParser::ParseLog(vector<string> &cmd) {
  if (!CheckPriority(7))throw Error("PermissionError");

  if (cmd.empty())logger.ShowLog();
  else throw Error("SyntaxError");
}

bool CommandParser::IdCheck(const string &Id_) {
  if (Id_.size() > 30) return false;
  for (char A: Id_) {
    if (std::isalnum(A) || A == '_') continue;
    else return false;
  }
  return true;
}

bool CommandParser::PasswordCheck(const string &password_) {
  return IdCheck(password_);
}

bool CommandParser::UserNameCheck(const string &UserName_) {
  if (UserName_.size() > 30) return false;
  for (char A: UserName_) { if (!std::isprint(A))return false; }
  return true;
}

bool CommandParser::PriorityCheck(const string &Priority_) {
  if (Priority_ != "1" && Priority_ != "3" && Priority_ != "7")return false;
  return true;
}

bool CommandParser::IsbnCheck(const string &ISBN_) {
  if (ISBN_.size() > MaxOfIsbn)return false;
  for (char A: ISBN_) { if (!std::isprint(A))return false; }
  return true;
}

bool CommandParser::BookNameCheck(const string &BookName_) {
  if (BookName_.size() > MaxOfName)return false;
  for (char A: BookName_) { if (!std::isprint(A) || A == '\"')return false; }
  return true;
}

bool CommandParser::AuthorCheck(const string &author_) {
  return BookNameCheck(author_);
}

bool CommandParser::KeywordCheck(const string &keyword_) {
  return BookNameCheck(keyword_);
}

bool CommandParser::KeywordRepeatCheck(const string &keyword_, const char &flag) {
  std::set<string> x;
  int size = static_cast<int>(keyword_.size());
  int l = 0, r = 0;
  string carrier;
  for (int i = 0; i < size; ++i) {
    l = (!l) ? r : r + 1;
    if (keyword_[i] == flag) {
      r = i;
      carrier = keyword_.substr(l, r - l);
      if (x.find(carrier) == x.end())x.insert(carrier);
      else return false;
      //cout << carrier << endl;
    } else if (i == size - 1) {
      r = i;
      carrier = keyword_.substr(l, r - l + 1);
      if (x.find(carrier) == x.end())x.insert(carrier);
      else return false;
      //cout << carrier << endl;
    }
  }
  return true;
}

bool CommandParser::QuantityCheck(const string &quantity_) {
  if (quantity_.size() > 10 || quantity_[0] == '-')return false;
  for (char A: quantity_) { if (!std::isdigit(A))return false; }
  long long int_temp = std::stol(quantity_);
  if (int_temp > quantity_max)return false;
  return true;
}

bool CommandParser::PriceCheck(const string &price_) {
  if (price_.size() > 13)return false;
  for (char A: price_) { if (!std::isprint(A) && A != '.')return false; }
  return true;
}

bool CommandParser::TotalCostCheck(const string &total_cost_) {
  return PriceCheck(total_cost_);
}

string CommandParser::GetType(const string &cmd, const int &max_len, const int &min_len) {
  int i = 0;
  while (cmd[i] != '=') {
    ++i;
    if (i > max_len)throw Error("SyntaxError");
  }
  if (i < min_len)throw Error("SyntaxError");
  string type_carrier = cmd.substr(1, i - 1);
  return type_carrier;
}

string CommandParser::GetContent(const string &cmd, const int &max_len, const int &min_len) {
  int i = 0;
  string carrier;
  while (cmd[i] != '=') {
    ++i;
    if (i > max_len)throw Error("SyntaxError");
  }
  if (i < min_len)throw Error("SyntaxError");
  if (cmd[i + 1] != '"') {
    carrier = cmd.substr(i + 1, cmd.size() - i - 1);
  } else carrier = cmd.substr(i + 2, cmd.size() - i - 3);
  return carrier;
}

bool CommandParser::TimeCheck(const string &time_) {
  if (time_.size() > 10)return false;
  for (char A: time_) { if (!std::isdigit(A))return false; }
  long long int_temp = std::stol(time_);
  if (int_temp > quantity_max)return false;
  return true;
}

bool CommandParser::CheckPriority(const int &low) {
  if (user_manager.GetNowPriority() < low)return false;
  return true;
}

void CommandParser::PrintAll() {
  std::vector<Book> target_book;
  std::vector<Node> target_index;
  book_manager.data_rank_by_ISBN.GetAll(target_index);
  Book carrier;
  for (auto &i: target_index) {
    book_manager.book_info.Read(carrier, i.value);
    target_book.push_back(carrier);
  }
  if (target_book.empty())cout << endl;
  else {
    std::sort(target_book.begin(), target_book.end(), BookManager::cmp);
    for (auto &i: target_book) cout << i << endl;
  }
}

void CommandParser::WriteLogSuccess() {
  logger.WriteLog(success);
}
void CommandParser::WriteLogFail() {
  logger.WriteLog(fail);
  //cout << fail << endl;
}
