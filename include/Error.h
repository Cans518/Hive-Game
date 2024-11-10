#pragma once
#include <string>
#include <stdexcept>

class Error : public std::exception {
private:
    std::string message;
public:
    explicit Error(const std::string& msg);
    virtual ~Error();  // 确保析构函数是虚函数

    const char* what() const noexcept override;
};


void printError(const std::string& msg);