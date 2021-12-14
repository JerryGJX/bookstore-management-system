//
// Created by JerryGuo on 2021/12/11.
//

#ifndef UNTITLED1_CHAR_HPP
#define UNTITLED1_CHAR_HPP

#include <string>
#include <cstring>
#include <iostream>

template<int maxLength = 65>
struct Char {
    // +1 是因为要存结尾 '\0'
    char content[maxLength]{};
    // 默认为空字符串
    Char() = default;

    Char(const std::string &s) { strcpy(content, s.c_str()); };

    Char(const char *cstr) { strcpy(content, cstr); };

    operator std::string() const { return str(); };

    std::string str() const { return std::string(content); };

    // 赋值操作
    Char &operator=(const Char<maxLength> &that) {
        if(this==&that)return *this;
        strcpy(content, that.content);
        return *this;
    };

    bool operator<(const Char<maxLength> &that) const { return strcmp(content, that.content) < 0; };

    bool operator>(const Char<maxLength> &that) const { return strcmp(content, that.content) > 0; };

    bool operator<=(const Char<maxLength> &that) const { return strcmp(content, that.content) <= 0; };

    bool operator>=(const Char<maxLength> &that) const { return strcmp(content, that.content) >= 0; };

    bool operator==(const Char<maxLength> &that) const { return strcmp(content, that.content) == 0; };

    bool operator!=(const Char<maxLength> &that) const { return strcmp(content, that.content) != 0; };

    friend std::ostream &operator<<(std::ostream &os,const Char<maxLength> &obj){
        for(int i=0;i<maxLength;i++){
            os<<obj.content[i];
        }
//        os << obj.content;
        return os;
    }
};

#endif //UNTITLED1_CHAR_HPP
