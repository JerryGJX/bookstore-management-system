//
// Created by JerryGuo on 2021/12/19.
//

#include "BookManager.hpp"

using std::cout;
using std::endl;

BookManager::BookManager(const string &book_info_,
                         const string &by_ISBN_,
                         const string &by_name_,
                         const string &by_author_,
                         const string &by_keyword_)
    : book_info(book_info_),
      data_rank_by_ISBN(by_ISBN_),
      data_rank_by_name(by_name_),
      data_rank_by_author(by_author_),
      data_rank_by_keyword(by_keyword_) {}

bool BookManager::cmp(const Book &a, const Book &b) {
  return a.ISBN < b.ISBN;
}

void BookManager::ShowBook(BookManager::ParaType para_type, const string &arg) {
  std::vector<Book> target_book;
  switch (para_type) {
    case ISBN:GetTargetBook(data_rank_by_ISBN, target_book, arg);
      break;
    case NAME:GetTargetBook(data_rank_by_name, target_book, arg);
      break;
    case AUTHOR:GetTargetBook(data_rank_by_author, target_book, arg);
      break;
    case KEYWORD:GetTargetBook(data_rank_by_keyword, target_book, arg);
      break;
    case ALL:std::vector<Node> target_index;
      data_rank_by_ISBN.GetAll(target_index);
      Book carrier;
      for (auto &i: target_index) {
        book_info.Read(carrier, i.value);
        target_book.push_back(carrier);
      }
      break;
  }
  if (target_book.empty())cout << endl;
  else {
    std::sort(target_book.begin(), target_book.end(), cmp);
    for (auto &i: target_book) cout << i << endl;
  }
}

void BookManager::BuyBook(const string &ISBN_, int quantity_, Logger &logger_) {
  std::vector<int> target_index;
  data_rank_by_ISBN.Query(ISBN_, target_index);
  if (target_index.empty())throw Error("NoBookMatched");
  Book carrier;
  book_info.Read(carrier, target_index[0]);
  if (carrier.quantity < quantity_)throw Error("NoBookMatched");
  carrier.quantity -= quantity_;
  double cost = quantity_ * carrier.price;
  logger_.WriteFinance(cost);
  cout << std::fixed << std::setprecision(2) << cost << endl;
  book_info.Update(carrier, target_index[0]);

}

void BookManager::SelectBook(const string &ISBN_, UserManager &user_manager_) {
  std::vector<int> target_index;
  data_rank_by_ISBN.Query(ISBN_, target_index);
  int index;
  if (target_index.empty()) {
    Book new_book(ISBN_);
    index = book_info.Write(new_book);
    Node new_one(ISBN_, index);
    data_rank_by_ISBN.Add(new_one);
    BookNumAdd(1);
  } else index = target_index[0];
//  int test=user_manager_.GetNowIndex();
//  cout<<test<<endl;
  user_manager_.ChangeNowIndex(index);
}
void BookManager::ModifyBook(const unordered_map<string, string> &cmd, UserManager &user_manager_) {
  if (!user_manager_.GetNowIndex())throw Error("NoBookSelected");
  for (const auto &i: cmd) {
    if (i.first == "ISBN")ModifyIsbn(i.second, user_manager_);
    else if (i.first == "name")ModifyName(i.second, user_manager_);
    else if (i.first == "author")ModifyAuthor(i.second, user_manager_);
    else if (i.first == "keyword")ModifyKeywords(i.second, user_manager_);
    else if (i.first == "price")ModifyPrice(i.second, user_manager_);
  }
}

void BookManager::ImportBook(int quantity, double total_cost, UserManager &user_manager_, Logger &logger_) {
  int index = user_manager_.GetNowIndex();
  if(!index)throw Error("NoBookSelected");
  Book now_book;
  book_info.Read(now_book, index);
  now_book.quantity += quantity;
  book_info.Update(now_book, index);
  logger_.WriteFinance(-total_cost);
}

void BookManager::ModifyIsbn(const string &isbn_, UserManager &user_manager_) {
  if (CheckExist(isbn_))throw Error("ISBNExist");
  int index = user_manager_.GetNowIndex();
  Book now_book;
  book_info.Read(now_book, index);
  Node old_node(now_book.ISBN, index);
  now_book.ISBN = isbn_;
  book_info.Update(now_book, index);
  Node new_node(isbn_, index);
  data_rank_by_ISBN.Del(old_node);
  data_rank_by_ISBN.Add(new_node);
}
void BookManager::ModifyName(const string &name_, UserManager &user_manager_) {
  int index = user_manager_.GetNowIndex();
  Book now_book;
  book_info.Read(now_book, index);
  Node old_node(now_book.name, index);
  now_book.name = name_;
  book_info.Update(now_book, index);
  Node new_node(name_, index);
  data_rank_by_name.Del(old_node);
  data_rank_by_name.Add(new_node);
}
void BookManager::ModifyAuthor(const string &author_, UserManager &user_manager_) {
  int index = user_manager_.GetNowIndex();
  Book now_book;
  book_info.Read(now_book, index);
  Node old_node(now_book.author, index);
  now_book.author = author_;
  book_info.Update(now_book, index);
  Node new_node(author_, index);
  data_rank_by_author.Del(old_node);
  data_rank_by_author.Add(new_node);
}
void BookManager::ModifyKeywords(const string &keywords_, UserManager &user_manager_) {
  int index = user_manager_.GetNowIndex();
  Book now_book;
  book_info.Read(now_book, index);
  string old_keyword = now_book.keyword;
  std::vector<string> old_keyword_list;
  SplitString(old_keyword, old_keyword_list, '|');
  for (auto &i: old_keyword_list) {
    Node carrier(i, index);
    data_rank_by_keyword.Del(carrier);
  }

  now_book.keyword = keywords_;
  book_info.Update(now_book, index);

  std::vector<string> new_keyword_list;
  SplitString(keywords_, new_keyword_list, '|');
  for (auto &i: new_keyword_list) {
    Node carrier(i, index);
    data_rank_by_keyword.Add(carrier);
  }
}
void BookManager::ModifyPrice(const string &price_, UserManager &user_manager_) {
  double price = std::stod(price_);
  int index = user_manager_.GetNowIndex();
  Book now_book;
  book_info.Read(now_book, index);
  now_book.price = price;
  book_info.Update(now_book, index);
}

void BookManager::SplitString(const string &cmd, std::vector<string> &x, const char &flag) {
  int size = static_cast<int>(cmd.size());
  int l = 0, r = 0;
  string carrier;
  for (int i = 0; i < size; ++i) {
    l = (!l) ? r : r + 1;
    if (cmd[i] == flag || i == size) {
      r = i;
      carrier = cmd.substr(l, r - l);
      x.push_back(carrier);
    }
  }
}

void BookManager::GetTargetBook(UnrolledLinkedList<Node> &file, std::vector<Book> &receiver, const string &arg) {
  std::vector<int> target_location;
  Book carrier;
  file.Query(arg, target_location);
  int i = 0;
  while (i < target_location.size()) {
    book_info.Read(carrier, target_location[i]);
    receiver.push_back(carrier);
    ++i;
  }
}

void BookManager::BookNumAdd(const int &x) {
  book_info.GetInfo(book_num);
  book_num += x;
  book_info.WriteInfo(book_num);
}
bool BookManager::CheckExist(const string &isbn_) {
  std::vector<Book> target_book;
  GetTargetBook(data_rank_by_ISBN, target_book, isbn_);
  return !target_book.empty();
}

//..........Book...........
std::ostream &operator<<(std::ostream &os, const Book &rhs) {
  os << rhs.ISBN << '\t' << rhs.name << '\t' << rhs.author << '\t'
     << rhs.keyword << '\t' << std::fixed << std::setprecision(2) << rhs.price
     << '\t' << rhs.quantity;
  return os;
}
Book::Book(const string &ISBN_,
           const string &name_,
           const string &author_,
           const string &keyword_,
           const int &quantity_,
           const int &price_) : ISBN(ISBN_), name(name_), author(author_), keyword(keyword_) {
  quantity = quantity_;
  price = price_;
}

Book::Book(const Book &x) {
  ISBN = x.ISBN;
  name = x.name;
  author = x.author;
  keyword = x.keyword;
  quantity = x.quantity;
  price = x.price;
}

bool Book::operator==(const Book &rhs) const {
  return ISBN == rhs.ISBN &&
      name == rhs.name &&
      author == rhs.author &&
      keyword == rhs.keyword &&
      quantity == rhs.quantity &&
      price == rhs.price;
}
bool Book::operator!=(const Book &rhs) const {
  return !(rhs == *this);
}


