# C语言子集词法分析器

一个用C++实现的C语言子集词法分析器，能够识别标识符、整数、关键字、运算符、分界符和注释，并将源代码转换为二元式序列（token流）。

## 项目简介

本项目实现了一个完整的词法分析器，采用基于有限自动机（DFA）的设计方法，通过状态转换实现token识别。系统采用模块化设计，包括词法分析器核心、token定义、符号表管理和错误处理等模块。

### 主要特性

- ✅ 识别C语言子集的所有关键字（void、int、float、double、if、else、for、do、while、return）
- ✅ 识别标识符（最大长度32字符）
- ✅ 识别无符号整数常量
- ✅ 识别单字符和双字符运算符（+、-、*、/、++、--、+=、-=、*=、/=、==、!=、<=、>=、<<、>>、&&、||等）
- ✅ 识别分界符（;、,、(、)、{、}）
- ✅ 处理单行注释（//）和多行注释（/* */）
- ✅ 完善的错误检测和报告机制
- ✅ 生成标准化的二元式序列输出
- ✅ 自动维护符号表

## 项目结构

```
lexical-analyzer/
├── CMakeLists.txt          # CMake配置文件
├── README.md               # 本文件
├── main.cpp                # 主程序入口
├── src/                    # 源代码目录
│   ├── token_types.h       # Token类型定义
│   ├── token_types.cpp     # Token类实现
│   ├── lexer.h             # 词法分析器接口
│   ├── lexer.cpp           # 词法分析器实现
│   ├── symbol_table.h      # 符号表接口
│   └── symbol_table.cpp    # 符号表实现
├── examples/               # 测试用例目录
│   ├── test_case_1_basic_tokens.c
│   ├── test_case_2_compound_operators.c
│   ├── test_case_3_comments.c
│   ├── test_case_4_logical_operators.c
│   ├── test_case_5_illegal_characters.c
│   ├── test_case_6_long_identifier.c
│   ├── test_case_7_boundary_identifier.c
│   └── test_case_8_complex_nested.c
├── output/                 # 默认输出目录
└── report/                 # 实验报告目录
```

## 编译要求

- CMake 3.10 或更高版本
- C++17 兼容的编译器（GCC 7+, Clang 6+, MSVC 2017+）

## 编译步骤

```bash
mkdir build
cd build
cmake ..
make
```

编译成功后，可执行文件 `lexer` 将生成在 `build` 目录中。

## 使用方法

### 基本用法

```bash
# 从build目录运行
./lexer <input_file>

# 示例
./lexer ../examples/test_case_1_basic_tokens.c
```

### 命令行选项

```
用法: lexer <input_file> [选项]

选项:
  -o, --output-dir <dir>    输出目录（默认: output）
  --tokens <file>           Token文件名（默认: tokens.txt）
  --symbols <file>          符号表文件名（默认: symbol_table.txt）
  --errors <file>           错误文件名（默认: errors.txt）
  -h, --help                显示帮助信息

示例:
  ./lexer input.c
  ./lexer input.c -o custom_output
  ./lexer input.c --tokens my_tokens.txt
```

### 查看帮助信息

```bash
./lexer -h
```

## 使用示例

### 示例1：基本token识别

输入文件 `test.c`:
```c
int main() {
    int x = 10;
    return 0;
}
```

运行命令:
```bash
./lexer test.c
```

输出文件 `output/tokens.txt`:
```
(2, int)
(21, main)
(62, ()
(63, ))
(64, {)
(2, int)
(21, x)
(34, =)
(22, 10)
(60, ;)
(10, return)
(22, 0)
(60, ;)
(65, })
(99, EOF)
```

输出文件 `output/symbol_table.txt`:
```
ID  | 标识符名
----|----------
0   | main
1   | x
```

### 示例2：复合运算符

输入文件:
```c
int a = 5;
a++;
a += 10;
```

Token序列将正确识别 `++` 和 `+=` 等复合运算符。

### 示例3：注释处理

输入文件:
```c
// 这是单行注释
int x = 5; /* 这是多行
注释 */ int y = 10;
```

注释将被跳过，不会生成token。

## 输出文件

词法分析器会生成三个输出文件：

1. **tokens.txt**: Token序列，每行一个二元式 `(类别码, 属性值)`
2. **symbol_table.txt**: 符号表，包含所有标识符及其ID
3. **errors.txt**: 错误日志，包含所有词法错误或"无错误"消息

## 类别码分配方案

### 保留字 (1-10)
- 1: void
- 2: int
- 3: float
- 4: double
- 5: if
- 6: else
- 7: for
- 8: do
- 9: while
- 10: return

### 标识符和常量 (21-22)
- 21: 标识符 (IDENTIFIER)
- 22: 整数常量 (INTEGER)

### 运算符 (30-51)
- 30: +
- 31: -
- 32: *
- 33: /
- 34: =
- 35: <
- 36: >
- 37: !
- 38: ++
- 39: --
- 40: +=
- 41: -=
- 42: *=
- 43: /=
- 44: ==
- 45: !=
- 46: <=
- 47: >=
- 48: <<
- 49: >>
- 50: &&
- 51: ||

### 分界符 (60-65)
- 60: ;
- 61: ,
- 62: (
- 63: )
- 64: {
- 65: }

### 特殊
- 99: 文件结束 (EOF)
- -1: 错误 (ERROR)

## 错误处理

词法分析器能够检测并报告以下类型的错误：

1. **非法字符**: 不属于C语言子集的字符
2. **标识符过长**: 超过32个字符的标识符
3. **注释未闭合**: 多行注释缺少结束标记 `*/`

错误格式:
```
错误: [行号:列号] 错误描述
```

示例:
```
错误: [3:15] 非法字符 '@'
错误: [5:8] 标识符 'very_long_identifier_name_that_exceeds_thirty_two_characters' 长度超过32个字符
```

错误检测采用错误恢复策略，记录错误后继续分析后续代码。

## 运行测试

### 测试所有用例

项目包含多个测试用例文件在 `examples/` 目录中：

```bash
# 从build目录运行
cd build

# 测试基本token识别
./lexer ../examples/test_case_1_basic_tokens.c

# 测试复合运算符
./lexer ../examples/test_case_2_compound_operators.c

# 测试注释处理
./lexer ../examples/test_case_3_comments.c

# 测试逻辑运算符
./lexer ../examples/test_case_4_logical_operators.c

# 测试错误检测（非法字符）
./lexer ../examples/test_case_5_illegal_characters.c

# 测试标识符长度限制
./lexer ../examples/test_case_6_long_identifier.c

# 测试边界情况（32字符标识符）
./lexer ../examples/test_case_7_boundary_identifier.c

# 测试复杂嵌套代码
./lexer ../examples/test_case_8_complex_nested.c
```

### 批量测试脚本

可以创建一个简单的脚本来运行所有测试用例：

```bash
#!/bin/bash
cd build
for testfile in ../examples/test_case_*.c; do
    echo "Testing: $testfile"
    ./lexer "$testfile" -o "output_$(basename $testfile .c)"
    echo "---"
done
```

## 技术实现

### 核心算法

1. **主扫描循环**: 遍历源代码字符流，根据当前字符类型调用相应的识别方法
2. **标识符识别**: 使用DFA识别以字母开头的字符序列，并区分关键字和标识符
3. **整数识别**: 识别连续的数字字符并转换为整数值
4. **运算符识别**: 使用向前查看技术识别单字符和双字符运算符
5. **注释处理**: 跳过单行注释和多行注释，检测未闭合的注释
6. **错误恢复**: 记录错误后继续分析，不中断整个过程

### 数据结构

- **Token类**: 表示词法单元，包含类型、值、行号、列号
- **TokenType枚举**: 定义所有token类型和类别码
- **SymbolTable类**: 管理标识符，提供插入和查询功能
- **LexicalError类**: 表示词法错误，包含错误消息和位置信息

### C++17特性使用

- `enum class` 提供类型安全的枚举
- `std::string_view` 优化字符串传递（可选）
- `std::optional` 处理可选返回值
- RAII原则管理资源（文件句柄、内存）

### 标准库容器

- `std::vector` 用于Token和错误列表
- `std::unordered_map` 用于符号表和关键字映射
- `std::string` 用于字符串处理

### 编码规范

- 使用命名空间 `lexer` 避免名称冲突
- 遵循现代C++最佳实践
- 清晰的接口和实现分离
- 完善的错误处理机制

## 开发者信息

23331147 苏长皓 

aka.苏心贤

Personal Site: https://fcsu.dev



## 参考资料

- 《编译原理》（龙书）- Alfred V. Aho等著
- 《现代编译原理》- Andrew W. Appel著
- C语言标准文档
