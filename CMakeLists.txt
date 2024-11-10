cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(test)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)

# 查找 src 目录下的所有源文件，并将列表存储在 SRC_FILES 变量中
file(GLOB SRC_FILES "src/*.cpp")

#输出信息
message(STATUS "SRC_FILES: ${SRC_FILES}")

# 添加可执行文件
add_executable(MyExecutable ${SRC_FILES})

# 设置库文件输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 如果需要编译成库，可以使用以下命令
# add_library(MyLibrary STATIC ${SRC_FILES})
# 或者
# add_library(MyLibrary SHARED ${SRC_FILES})

# 链接库（如果有）
# target_link_libraries(MyExecutable MyLibrary)