#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include "src/lexer.h"

namespace fs = std::filesystem;

struct Options {
    std::string inputFile;
    std::string outputDir = "output";
    std::string tokensFile = "tokens.txt";
    std::string symbolsFile = "symbol_table.txt";
    std::string errorsFile = "errors.txt";
    bool showHelp = false;
};

void showHelp(const char* programName) {
    std::cout << "用法: " << programName << " <input_file> [选项]\n\n";
    std::cout << "选项:\n";
    std::cout << "  -o, --output-dir <dir>    输出目录（默认: output）\n";
    std::cout << "  --tokens <file>           Token文件名（默认: tokens.txt）\n";
    std::cout << "  --symbols <file>          符号表文件名（默认: symbol_table.txt）\n";
    std::cout << "  --errors <file>           错误文件名（默认: errors.txt）\n";
    std::cout << "  -h, --help                显示此帮助信息\n\n";
    std::cout << "示例:\n";
    std::cout << "  " << programName << " input.c\n";
    std::cout << "  " << programName << " input.c -o custom_output\n";
    std::cout << "  " << programName << " input.c --tokens my_tokens.txt\n";
}

Options parseArguments(int argc, char* argv[]) {
    Options options;
    
    if(argc < 2) {
        options.showHelp = true;
        return options;
    }
    
    for(int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if(arg == "-h" || arg == "--help") {
            options.showHelp = true;
            return options;
        }
        else if(arg == "-o" || arg == "--output-dir") {
            if(i + 1 < argc) {
                options.outputDir = argv[++i];
            } else {
                std::cerr << "错误: " << arg << " 需要一个参数\n";
                options.showHelp = true;
                return options;
            }
        }
        else if(arg == "--tokens") {
            if(i + 1 < argc) {
                options.tokensFile = argv[++i];
            } else {
                std::cerr << "错误: --tokens 需要一个参数\n";
                options.showHelp = true;
                return options;
            }
        }
        else if(arg == "--symbols") {
            if(i + 1 < argc) {
                options.symbolsFile = argv[++i];
            } else {
                std::cerr << "错误: --symbols 需要一个参数\n";
                options.showHelp = true;
                return options;
            }
        }
        else if(arg == "--errors") {
            if(i + 1 < argc) {
                options.errorsFile = argv[++i];
            } else {
                std::cerr << "错误: --errors 需要一个参数\n";
                options.showHelp = true;
                return options;
            }
        }
        else if(arg[0] == '-') {
            std::cerr << "错误: 未知选项 '" << arg << "'\n";
            options.showHelp = true;
            return options;
        }
        else {
            if(options.inputFile.empty()) {
                options.inputFile = arg;
            } else {
                std::cerr << "错误: 多余的参数 '" << arg << "'\n";
                options.showHelp = true;
                return options;
            }
        }
    }
    
    if(options.inputFile.empty() && !options.showHelp) {
        std::cerr << "错误: 未提供输入文件\n";
        options.showHelp = true;
    }
    
    return options;
}

int main(int argc, char* argv[]) {
    Options options = parseArguments(argc, argv);
    
    if(options.showHelp) {
        showHelp(argv[0]);
        return options.inputFile.empty() ? 1 : 0;
    }
    
    if(!fs::exists(options.inputFile)) {
        std::cerr << "错误: 文件 '" << options.inputFile << "' 不存在\n";
        return 1;
    }
    
    std::ifstream inputFile(options.inputFile);
    if(!inputFile) {
        std::cerr << "错误: 无法打开文件 '" << options.inputFile << "'\n";
        return 1;
    }
    
    std::string sourceCode((std::istreambuf_iterator<char>(inputFile)),
                           std::istreambuf_iterator<char>());
    inputFile.close();
    
    try {
        if(!fs::exists(options.outputDir)) {
            fs::create_directories(options.outputDir);
        }
    } catch(const fs::filesystem_error& e) {
        std::cerr << "错误: 无法创建输出目录 '" << options.outputDir << "': " 
                  << e.what() << "\n";
        return 1;
    }
    
    lexer::Lexer lex(sourceCode);
    auto tokens = lex.tokenize();
    
    std::string tokensPath = options.outputDir + "/" + options.tokensFile;
    std::string symbolsPath = options.outputDir + "/" + options.symbolsFile;
    std::string errorsPath = options.outputDir + "/" + options.errorsFile;
    
    try {
        lex.writeTokens(tokensPath);
        lex.writeSymbolTable(symbolsPath);
        lex.writeErrors(errorsPath);
    } catch(const std::exception& e) {
        std::cerr << "错误: 写入输出文件失败: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "词法分析完成\n";
    std::cout << "Token数量: " << tokens.size() << "\n";
    std::cout << "标识符数量: " << lex.getSymbolTable().size() << "\n";
    std::cout << "错误数量: " << lex.getErrors().size() << "\n";
    
    if(lex.hasErrors()) {
        std::cout << "\n发现以下错误:\n";
        for(const auto& error : lex.getErrors()) {
            std::cout << "  " << error.toString() << "\n";
        }
        return 1;
    }
    
    std::cout << "\n输出文件已生成:\n";
    std::cout << "  - " << tokensPath << "\n";
    std::cout << "  - " << symbolsPath << "\n";
    std::cout << "  - " << errorsPath << "\n";
    
    return 0;
}
