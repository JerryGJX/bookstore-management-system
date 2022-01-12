# 书店管理系统开发文档

作者：林田川

代码风格：[Google 开源项目风格指南](https://zh-google-styleguide.readthedocs.io/en/latest/)

## 文件结构

- `main.cpp` 入口点
- `src/`
  - `command_parser.h` 指令解析
  - `command_parser.cpp`
  - `user_manager.h` 用户管理
  - `user_manager.cpp`
  - `book_manager.h` 图书管理
  - `book_manager.cpp`
  - `logger.h` 日志系统
  - `logger.cpp`
  - `exceptions.h` 异常处理
  - `exceptions.cpp`
  - `basic_file_io.h` 基础文件读写类
  - `basic_file_io.cpp`
  - `unrolled_linked_list.h` 块状链表
  - `unrolled_linked_list.cpp`
- `data/`
  - `books.dat` 储存的书本信息
  - `ISBN.index` 按 ISBN 升序储存对应书本的在 `books.dat` 中的位置
  - `bookname.index` 按书名升序储存上述内容
  - `author.index` 按作者升序储存上述内容
  - `keyword.index` 按 keyword 升序储存上述内容
  - `users.dat` 储存用户信息
  - `finance.dat` 储存收支情况
  - `log.dat` 储存操作日志

## 类的接口

### 指令解析

用 `CommandParser` 类来解析指令。

`run` 函数表示运行书店管理系统，即循环读入指令并解析，然后调用相应的函数。

`run` 函数只匹配指令的第一个单词（假设为 `abc`），然后调用相应的函数 `ParseAbc()`。`ParseAbc()` 只会判断指令在格式上是否合法，若非法则抛出 `SyntexError` 异常，合法则调用相对应的类的成员函数。

```cpp
class CommandParser {
 private:
  // 储存与该类关联的 UserManager 和 BookManager
  UserManager &user_manager;
  BookManager &book_manager;
  Logger &logger;
  // 根据指令的第一个单词查找对应的函数，供 run 函数使用
  std::unordered_map<std::string, std::function<int(const char*)>> mapFunction;

 public:
  void Run();  // 循环读入指令并解析，直到遇到 quit 或 exit
  // 构造 CommandParser，将其与所给的 UserManager，BookManager 和 Logger 关联起来
  CommandParser(UserManager &user_manager_, BookManager &book_manager_, Logger &logger_);

  void ParseSu(vector<string> &cmd);  // 解析 su [User-ID] ([Password])?
  void ParseLogout(vector<string> &cmd);  // 解析 logout
  void ParseRegister(vector<string> &cmd);  // 解析 register [User-ID] [Password] [User-Name]
  void ParsePasswd(vector<string> &cmd);  // 解析 passwd [User-ID] ([Old-Password])? [New-Password]
  void ParseUseradd(vector<string> &cmd);  // 解析 useradd [User-ID] [Password] [Priority] [User-Name]
  void ParseDelete(vector<string> &cmd);  // 解析 delete [User-ID]

  void ParseShow(vector<string> &cmd);  // 解析 show，调用 showBook 或 showFinance

  void ParseShowBook(vector<string> &cmd);  // 解析 show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
  void ParseBuy(vector<string> &cmd);  // 解析 buy [ISBN] [Quantity]
  void ParseSelect(vector<string> &cmd);  // 解析 select [ISBN]
  void ParseModify(vector<string> &cmd);  // 解析 modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
  void ParseImport(vector<string> &cmd);  // 解析 import [Quantity] [Total-Cost]

  void ParseReport(vector<string> &cmd);  // 解析 report myself 或 report finance 或 report employee
  void ParseShowFinance(vector<string> &cmd);  // 解析 show finance ([Time])?
  void ParseLog(vector<string> &cmd);  // 解析 log
};
```

### 字符串类

包装了 `char` 数组，重载运算符以支持比较。


```cpp
template <const int size = 64>
class Char {
  char content[size + 1];  // 多开一位用来放 '\0'

 public:
  Char();
  Char(const std::string &s);
  Char(const char *cstr);
  operator std::string() const;
  std::string str() const;

  Char &operator=(const Char &that);
  friend bool operator<(const Char<size> &a, const Char<size> &b);
  friend bool operator==(const Char<size> &a, const Char<size> &b);
  friend bool operator>(const Char<size> &a, const Char<size> &b);
  friend bool operator<=(const Char<size> &a, const Char<size> &b);
  friend bool operator>=(const Char<size> &a, const Char<size> &b);
  friend bool operator!=(const Char<size> &a, const Char<size> &b);
  friend std::istream &operator>>(std::istream &is, Char<size> &s);
  friend std::ostream &operator<<(std::ostream &os, const Char<size> &s);
};
```

### 用户管理

`UserManager` 需要通过 `UnrolledLinkedList` 读写 `users.dat`。

```cpp
class UserManager;
class User {
  friend class UserManager;

 private:
  Char<30> password;

 public:
  Char<30> user_ID;
  Char<30> user_name;
  int priority = 0;  // 权限，可以取 7 或 3 或 1
};

class UserManager {
 private:
    UnrolledLinkedList<Node> user_data_list;//id to position in memory_river
  MemoryRiver<User, int, 0> user_database;
  std::vector<std::pair<User, int>> user_stack;  // 用户栈，储存登录的用户和他所选的图书的 offset

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
  bool LoginFlag(const string&user_id_);
};
```

### 图书管理

图书类

```cpp
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
```

图书管理类

图书信息存放于 `book.dat`，无序。检索图书时需要的索引存放于 `*.index`。

```cpp
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
```

### 日志系统

日志格式 `[time] [user-id] (success|fail) [command]`。

其中 `[time]` 格式为 `%Y-%m-%d %H:%M:%S`，例如 `2021-12-07 20:05:25`；若当前没有已登陆的用户，则 `[user-id]` 为 `<guest>`；`(success|fail)` 表示该指令是否正常执行；`[command]` 为输入的指令。

对于 `import` 和 `modify` 指令，如果当前有被选中的书，以 `[command] [ISBN] [args]` 格式输出，例如：

```plain
2021-09-01 08:30:00 root success select 978-7-115-42935-3
2021-09-01 08:30:01 root success import 978-7-115-42935-3 33 233.33
2021-09-01 08:30:05 root success modify 978-7-115-42935-3 -name="C++ Program Design" -author="why, yyu" -price=49.8
```

```cpp
class Finance {
 private:
  double income = 0, expense = 0;

 public:
  Finance();
  Finance(const double &);
  Finance &operator+=(const Finance &);
  friend std::ostream &operator<<(std::ostream &, const Finance &);
  double GetIncome();
  double GetExpense();
};

class Log {
  int priority = 0;
  Char<80> time_;
  Char<20> flag;
  Char<KEY_SIZE> user_id;
  Char<LOGLEN> log;
 public:
  Log() = default;
  Log(const string &flag_, const string &user_id_, const string &log_, const int &priority_);
  Log(const Log &);
  Log &operator=(const Log &rhs);
  friend std::ostream &operator<<(std::ostream &, const Log &);
  void GetTime();
  int ThisPriority();
  string ThisId();
};

class Logger {
  MemoryRiver<Finance, LongLongWrapper, sizeof(LongLongWrapper)> finance_data;
  UnrolledLinkedList<NodeDigit> finance_list;

  MemoryRiver<Log, LongLongWrapper, sizeof(LongLongWrapper)> log_data;
  UnrolledLinkedList<Node> log_rank_by_userid;
  LongLongWrapper finance_num = 0, log_num = 0;
 public:
  Logger(const string &finance_list_, const string &finance_data_,
         const string &log_data_, const string &log_rank_by_userid);
  void ShowFinance(int time = -1);  // 输出财务记录
  void WriteFinance(const double &);
  void ReportMyself(UserManager &);  // 输出员工自己的操作记录
  void ReportFinance();  // 生成财务记录报告
  //void ReportWork();  // 生成全体员工工作情况报告
  void ReportEmployee();  // 生成员工工作情况表，记录其操作
  void ShowLog();  // 生成日志，包括谁干了什么，以及财务上每一笔交易
  void WriteLog(const Log &x);  // 写入日志文件
  void FormLog(Log &, const string &flag, const string &command, UserManager &user_manager_);
};

```

### 基础文件读写

类似于 Memory River 的包含空间回收的文件读写类。用于存储 `books.dat` 中的图书信息。也用于写入 `finance.dat` 和 `log.dat`。

```cpp
template<class T, class InfoType, int InfoLength>
class MemoryRiver {
 private:

  fstream file;
  string file_name;
  int sizeofT = sizeof(T);
  int free_head = 0;
 public:
  MemoryRiver() = default;

  MemoryRiver(const string &file_name);

  //在文件合适位置写入类对象t，并返回写入的位置索引index
  //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  //位置索引index可以取为对象写入的起始位置

  void WriteInfo(const InfoType &info_);
    
  void GetInfo(InfoType &receiver) ;
    
  int ReturnInfoLength() ;

  int Write(T &t) ;

  //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void Update(T &t, const int index) ;

  //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void Read(T &t, const int index) ;

  //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
  void Delete(int index);
};
```

### 块状链表

有序地存放数据。用于读写 `*.index` 和 `users.dat`。

```cpp
template<class NodeType>
class UnrolledLinkedList {
 private:
  MemoryRiver<Block<NodeType>, BlockGallery<NodeType>, 		     sizeof(BlockGallery<NodeType>)> block_list;
  BlockGallery<NodeType> block_info;

  void DeleteBlock(const int &);

 public:
  UnrolledLinkedList();

  explicit UnrolledLinkedList(const std::string &);

  void GetInfo();

  void PutInfo();

  bool Add(const NodeType &);

  bool Del(const NodeType &);

  bool Query(const std::string &, std::vector<int> &);

  void GetAll(std::vector<NodeType> &find_list_);
};
```

### 异常处理

#### 用户管理异常

```cpp
class Error: public std::exception{
    const string message;
 public:
  Error()=default;
  Error(const string &error_disc);
  Error(const Error &x);
  friend std::ostream &operator<<(std::ostream &os, const Error &error) ；
};
```

