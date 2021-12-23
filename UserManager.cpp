//
// Created by JerryGuo on 2021/12/19.
//

#include "UserManager.hpp"


////.......IdIndex...........
//
//IdIndex::IdIndex(const int &value_, const string &Id_) : key(Id_) {
//  value = value_;
//}
//bool IdIndex::operator<(const IdIndex &x) const {
//  return key < x.key;
//}
//bool IdIndex::operator==(const IdIndex &x) const {
//  return key == x.key;
//}
//bool IdIndex::operator!=(const IdIndex &x) const {
//  return key != x.key;
//}

//.........User..........
User::User(const string &user_id_,
           const string &password_,
           const int priority_,
           const string &user_name_) : user_ID(user_id_), password(password_), user_name(user_name_) {
  priority = priority_;
}

//.......UserManager........

UserManager::UserManager(const string &user_data_list_, const string &user_database_)
    : user_data_list(user_data_list_), user_database(user_database_) {
  int index;
  if (!FindId("root", index)) {
    User owner("root", "sjtu", OWNERPRIORITY, "root");
    index = user_database.Write(owner);
    Node carrier("root", index);
    user_data_list.Add(carrier);
  }
}

void UserManager::Login(const string &user_id, const string &password_) {
  int index;
  if (!FindId(user_id, index))throw Error("UserNotExist");
  User to_log_user;
  user_database.Read(to_log_user, index);
  if ((!password_.empty()) && (to_log_user.password != password_))throw Error("IncorrectPassword");//检测密码
  if (password_.empty() && to_log_user.priority < GetNowPriority())throw Error("IncorrectPassword");
  std::pair<User, int> carrier(to_log_user, NULL);
  user_stack.push_back(carrier);
}

void UserManager::Logout() {
  if (user_stack.empty())throw Error("NoUser");
  user_stack.pop_back();
}

void UserManager::ChangePassword(const string &user_id, const string &new_password, const string &old_password) {
  int index;
  if (!FindId(user_id, index))throw Error("UserNotExist");
  User to_log_user;
  user_database.Read(to_log_user, index);
  if ((!old_password.empty()) && (to_log_user.password != old_password))throw Error("IncorrectPassword");
  if (GetNowPriority() != OWNERPRIORITY && old_password.empty())throw Error("IncorrectPassword");
  to_log_user.password = new_password;
  user_database.Update(to_log_user, index);
}

void UserManager::CreateUser(const string &user_id,
                             const string &password,
                             const int &priority,
                             const string &user_name) {
  int index;
  if (priority >= GetNowPriority() || GetNowPriority() < STAFFPRIORITY)throw Error("PermissionError");
  if (FindId(user_id, index))throw Error("DuplicateUsername");
  User new_user(user_id, password, priority, user_name);
  index = user_database.Write(new_user);
  Node new_node(user_id, index);
  user_data_list.Add(new_node);
}

void UserManager::Register(const string &user_id, const string &password, const string &user_name) {
  int index;
  if (FindId(user_id, index))throw Error("DuplicateUsername");
  User new_user(user_id, password, 1, user_name);
  index = user_database.Write(new_user);
  Node new_node(user_id, index);
  user_data_list.Add(new_node);
}

void UserManager::Remove(const string &user_id) {
  int index;
  if (!FindId(user_id, index))throw Error("UserNotExist");
  if (GetNowId().empty() || GetNowId() == user_id) throw Error("PermissionError");
  user_database.Delete(index);
  Node carrier(user_id, index);
  user_data_list.Del(carrier);
}
bool UserManager::FindId(const string &user_id_, int &index_) {
  std::vector<int> result;
  user_data_list.Query(user_id_, result);
  if (result.empty())return false;
  index_ = result[0];
  return true;
}
int UserManager::GetNowPriority() {
  int now_priority = (user_stack.empty()) ? 0 : user_stack.back().first.priority;
  return now_priority;
}
string UserManager::GetNowId() {
  string now_id = (user_stack.empty()) ? "" : user_stack.back().first.user_ID;
  return now_id;
}
void UserManager::ChangeNowIndex(const int &index_) {
  user_stack.back().second = index_;
}
int UserManager::GetNowIndex() {
  if (user_stack.empty())return 0;
  return user_stack.back().second;
}


