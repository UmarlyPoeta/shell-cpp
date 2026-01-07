#pragma once
#include <string>
#include <vector>

struct Command {
    std::string command_name;
    std::vector<std::string> args;
    std::string stdout_file;
    std::string stderr_file;
    bool append_stdout = false;
    bool append_stderr = false;
};

std::vector<std::string> parseInput(const std::string& input);
std::vector<std::string> parseRedirection(const std::vector<std::string> args, std::string& output_file);


Command parseCommand(const std::string& input);
