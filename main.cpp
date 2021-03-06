//
// Created by JerryGuo on 2021/12/22.
//

#include <iostream>
#include "Char.hpp"
#include "Parser.hpp"

int main() {
  UserManager user_manager("user_data_list","user_database");
  BookManager book_manager("all_book_data", "by_isbn", "by_name", "by_author", "by_keyword");
  Logger logger("finance_list","finance_data","log_data","log_rank_by_userid");
  CommandParser command_parser(user_manager, book_manager, logger);
  //cout.precision(2);

  while(true){
    try{
      command_parser.Run();
      command_parser.WriteLogSuccess();
    } catch (Error &get_error) {
      command_parser.WriteLogFail();
      cout <<"Invalid"<<"\n";
    }
  }
}