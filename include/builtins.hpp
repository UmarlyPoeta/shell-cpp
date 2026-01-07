#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <fcntl.h>
#include "parser.hpp"

class RedirectGuard {
    int original_stdout = -1;
public:
    RedirectGuard(const std::string& output_file);
    ~RedirectGuard();
};


constexpr std::size_t MAX_PATH_LENGTH = 4096;

using BuiltinFn = std::function<bool(Command& cmd)>;

extern std::unordered_map<std::string, BuiltinFn> builtinMap;

bool builtin_exit(Command& cmd);
bool type(Command& cmd);
bool echo(Command& cmd);
bool pwd(Command& cmd);
bool cd(Command& cmd);