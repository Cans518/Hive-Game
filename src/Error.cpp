#include "Error.h"
#include <iostream>

// 构造函数实现
Error::Error(const std::string& msg) : message(msg) {}

// 析构函数实现
Error::~Error() = default;

// what() 函数的实现
const char* Error::what() const noexcept {
    return message.c_str();
}

// 确保 printError 函数只在此处定义
void printError(const std::string& msg) {
    std::cout << "\033[31m" << "Error: " << msg << "\033[0m" << std::endl;
}
