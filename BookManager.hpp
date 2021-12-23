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

using std::string;
using std::unordered_map;

const int MaxOfIsbn = 20, MaxOfKeyword = 60, MaxOfName = 60, MaxOfAuthor = 60;

class Book;

class BookManager {
 private:
  MemoryRiver<Book, int, sizeof(int)> book_info;
  UnrolledLinkedList<Node> data_rank_by_ISBN;
  UnrolledLinkedList<Node> data_rank_by_name;
  UnrolledLinkedList<Node> data_rank_by_author;
  UnrolledLinkedList<Node> data_rank_by_keyword;
  int book_num = 0;
  UserManager &user_manager_;
 public:
  enum ParaType { ISBN, NAME, AUTHOR, KEYWORD, ALL };  // 参数类型
  BookManager(const string &book_info_,
              const string &by_ISBN_,
              const string &by_name_,
              const string &by_author_,
              const string &by_keyword_,
              UserManager &user_manager);

  static bool cmp(const Book& a, const Book& b);

  void ShowBook(ParaType para_type, const string &arg = "");  // 检索图书
  void BuyBook(const string &ISBN, int quantity);  // 购买图书
  void SelectBook(const string &ISBN);  // 以当前账户选中指定图书
  void ModifyBook(const unordered_map<string, string> &cmd);  // 更新选中图书的信息
  //void ModifyBook(ParaType para_type, int price);  // 更新选中图书的信息
  void ImportBook(int quantity, double total_cost);  // 指定交易总额购入指定数量的选中图书

  static void SplitString(const string &cmd, std::vector<string> &x, const char &flag);

  bool CheckExist(const string &isbn_);

  void GetTargetBook(UnrolledLinkedList<Node> &file, std::vector<Book> &, const string &arg);

  bool CheckPriority(const int &);

  void BookNumAdd(const int &);

  void ModifyIsbn(const string &);

  void ModifyName(const string &);

  void ModifyAuthor(const string &);

  void ModifyKeywords(const string &);

  void ModifyPrice(const string &);

};



class Book {
  friend std::ostream &operator<<(std::ostream &, const Book &);
 public:
  Char<MaxOfIsbn> ISBN;
  Char<MaxOfName> name;
  Char<MaxOfAuthor> author;
  Char<MaxOfKeyword> keyword;
  int quantity = 0;
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
