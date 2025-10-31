cmake_minimum_required(VERSION 3.10)
project(LexicalAnalyzer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 收集所有源文件
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

# 收集所有头文件
file(GLOB_RECURSE HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# 只有在源文件存在时才创建可执行文件
if(SOURCES)
    # 可执行文件
    add_executable(lexer ${SOURCES} ${HEADERS})

    # 包含目录
    target_include_directories(lexer PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # 编译选项
    if(MSVC)
        target_compile_options(lexer PRIVATE /W4 /WX-)
    else()
        target_compile_options(lexer PRIVATE -Wall -Wextra -pedantic)
    endif()
else()
    message(WARNING "No source files found. Please add source files to build the project.")
endif()
