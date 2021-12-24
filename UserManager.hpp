//
// Created by JerryGuo on 2021/12/19.
//

#ifndef UNTITLED1_USERMANAGER_HPP
#define UNTITLED1_USERMANAGER_HPP

#include <vector>
#include <iostream>

#include "Char.hpp"
#include "ULL_new.hpp"
#include "Node.hpp"
#include "Error.hpp"

#define OWNERPRIORITY 7
#define STAFFPRIORITY 3
#define CUSTOMERPRIORITY 1

using std::cout;
using std::endl;

using std::string;

class UserManager;
class User {
  friend class UserManager;
 private:
  Char<KEY_SIZE> password;
 public:
  Char<KEY_SIZE> user_ID;
  Char<KEY_SIZE> user_name;
  int priority = 0;  // 权限，可以取 7 或 3 或 1
  User() = default;
  User(const string &user_id,
       const string &password,
       int priority,
       const string &user_name);
};

class UserManager {
 private:
  UnrolledLinkedList<Node> user_data_list;//id to position in memory_river
  MemoryRiver<User, int, 0> user_database;
  std::vector<std::pair<User, int>> user_stack;  // 用户栈，储存登录的用户和他所选的图书的 offset
  //bool log_flag = false;
 public:
  UserManager(const string &user_data_list_, const string &user_database_);
  void Login(const string &user_id, const string &password_ = "");  // 登录用户
  void Logout();  // 退出登录
  void ChangePassword(const string &user_id, const string &new_password, const string &old_password = "");  // 修改密码
  void CreateUser(const string &user_id, const string &password, const int &priority, const string &user_name);  // 创建用户
  void Register(const string &user_id, const string &password, const string &user_name);  // 注册账户
  void Remove(const string &user_id);  // 删除账户

  bool FindId(const string &user_id_, int &index_);
  int GetNowPriority();
  string GetNowId();
  int GetNowIndex();
  void ChangeNowIndex(const int &index_);
};

#endif //UNTITLED1_USERMANAGER_HPP
