//
// Created by JerryGuo on 2021/12/12.
//

#ifndef UNTITLED1_MEMORYRIVER_HPP
#define UNTITLED1_MEMORYRIVER_HPP

#include <fstream>
#include <filesystem>

#ifdef MyDebug

#include <iostream>

#endif

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, class InfoType, int InfoLength>
class MemoryRiver {
private:

    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int free_head = 0;
public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {
//        file.open(file_name);
#ifdef MyDebug
        std::filesystem::remove(file_name);
        std::cout << file_name << " deleted" << std::endl;
#endif
        if (!std::filesystem::exists(file_name)) {
//            file.clear();
            file.open(file_name, std::ios::out);
            file.seekp(0, std::ios::beg);
            InfoType info;
            file.write(reinterpret_cast<char *>(&info), InfoLength);
            file.seekp(InfoLength, std::ios::beg);
            file.write(reinterpret_cast<char *>(&free_head), 4);
            T t;
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        } else {
            file.open(file_name);
            file.seekp(InfoLength, std::ios::beg);
            file.read(reinterpret_cast<char *>(&free_head), 4);
        }
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置

    void WriteInfo(const InfoType &info_) {
        file.open(file_name);
        file.seekp(0, std::ios::beg);
        file.write(reinterpret_cast<const char *>(&info_), InfoLength);
        file.close();
    }

    void GetInfo(InfoType &receiver) {
        file.open(file_name);
        file.seekg(0, std::ios::beg);
        file.write(reinterpret_cast<char *>(&receiver), InfoLength);
        file.close();
    }

    int Write(T &t) {
        /* your code here */
        file.open(file_name);
        int pos, next = 0;
        if (free_head) {
            file.seekp(free_head), pos = file.tellp();
            file.read(reinterpret_cast<char *>(&next), 4);
            file.seekg(-4, std::ios::cur);
            free_head = next, next = 0;
        } else {
            file.seekp(0, std::ios::end), pos = file.tellp();
        }
        file.write(reinterpret_cast<char *>(&next), 4);
        file.write(reinterpret_cast<const char *>(&t), sizeofT);
        file.close();
        return pos - InfoLength;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void Update(T &t, const int index) {
        /* your code here */
        file.open(file_name);
        file.seekp(index + sizeof(int) + InfoLength);
        file.write(reinterpret_cast<const char *>(&t), sizeofT);
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void Read(T &t, const int index) {
        /* your code here */
        file.open(file_name);
        file.seekg(index + sizeof(int) + InfoLength);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        /* your code here */
        file.open(file_name);
        file.seekp(index + sizeof(int) + InfoLength);
        file.write(reinterpret_cast<char *>(&free_head), 4);
        free_head = index + InfoLength;
        file.close();
    }
};


#endif //UNTITLED1_MEMORYRIVER_HPP
