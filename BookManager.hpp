//
// Created by JerryGuo on 2021/12/19.
//

#ifndef UNTITLED1_BOOKMANAGER_HPP
#define UNTITLED1_BOOKMANAGER_HPP

#include <unordered_map>
#include "Char.hpp"
#include "ULL_new.hpp"
#include "MemoryRiver.hpp"
#include <iostream>
#include "Node.hpp"
#include "Error.hpp"
#include "UserManager.hpp"
#include "Logger.hpp"

#define QUANTITYMAX 2147483647

using std::string;
using std::unordered_map;

const int MaxOfIsbn = 20, MaxOfKeyword = 60, MaxOfName = 60, MaxOfAuthor = 60;

class Book;
class Logger;
class BookManager {
 public:
  MemoryRiver<Book, LongLongWrapper, sizeof(LongLongWrapper)> book_info;
  UnrolledLinkedList<Node> data_rank_by_ISBN;
  UnrolledLinkedList<Node> data_rank_by_name;
  UnrolledLinkedList<Node> data_rank_by_author;
  UnrolledLinkedList<Node> data_rank_by_keyword;
  long long book_num = 0;
  //UserManager &user_manager_;



 public:
  enum ParaType { ISBN, NAME, AUTHOR, KEYWORD, ALL };  // 参数类型
  BookManager(const string &book_info_,
              const string &by_ISBN_,
              const string &by_name_,
              const string &by_author_,
              const string &by_keyword_);

  static bool cmp(const Book &a, const Book &b);

  void ShowBook(ParaType para_type, const string &arg = "");  // 检索图书
  void BuyBook(const string &ISBN, int quantity, Logger &logger_);  // 购买图书
  void SelectBook(const string &ISBN, UserManager &user_manager_);  // 以当前账户选中指定图书
  void ModifyBook(const unordered_map<string, string> &cmd, UserManager &user_manager_);  // 更新选中图书的信息
  //void ModifyBook(ParaType para_type, int price);  // 更新选中图书的信息
  void ImportBook(int quantity, double total_cost, UserManager &user_manager_, Logger &logger_);  // 指定交易总额购入指定数量的选中图书

  static void SplitString(const string &cmd, std::vector<string> &x, const char &flag);

  bool CheckExist(const string &isbn_);

  void GetTargetBook(UnrolledLinkedList<Node> &file, std::vector<Book> &, const string &arg);

  void BookNumAdd(const int &);

  void ModifyIsbn(const string &, UserManager &user_manager_);

  void ModifyName(const string &, UserManager &user_manager_);

  void ModifyAuthor(const string &, UserManager &user_manager_);

  void ModifyKeywords(const string &, UserManager &user_manager_);

  void ModifyPrice(const string &, UserManager &user_manager_);

};

class Book {
  friend std::ostream &operator<<(std::ostream &, const Book &);
 public:
  Char<MaxOfIsbn> ISBN;
  Char<MaxOfName> name;
  Char<MaxOfAuthor> author;
  Char<MaxOfKeyword> keyword;
  long long quantity = 0;
  double price = 0;
  Book() = default;
  Book(const Book &);
  explicit Book(const string &ISBN_,
                const string &name_ = "",
                const string &author_ = "",
                const string &keyword_ = "",
                const int &quantity_ = 0,
                const int &price_ = 0);
  bool operator==(const Book &rhs) const;
  bool operator!=(const Book &rhs) const;

};

#endif //UNTITLED1_BOOKMANAGER_HPP
