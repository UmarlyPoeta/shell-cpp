#include "builtins.hpp"
#include "utils.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

RedirectGuard::RedirectGuard(const std::string& output_file) {
    if (!output_file.empty()) {
        this->original_stdout = dup(STDOUT_FILENO);
        int fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

RedirectGuard::~RedirectGuard() {
    if (this->original_stdout != -1) {
        dup2(this->original_stdout, STDOUT_FILENO);
        close(this->original_stdout);
    }
}


bool builtin_exit(Command& cmd) {
    (void)cmd;  
    exit(0);
}

bool type(Command& cmd) {
    if (cmd.args.size() < 2) {
        std::cout << "type: missing argument\n";
        return false;
    }

    const std::string& command = cmd.args[1];
    std::string check_path;

    if (isBuiltin(command)) {
        std::cout << command << " is a shell builtin\n";
    } else if (inPathAndExecutable(command, check_path)) {
        std::cout << command << " is " << check_path << "\n";
    } else {
        std::cout << command << ": not found\n";
    }

    return true;
}

bool echo(Command& cmd) {
    RedirectGuard guard(cmd.stdout_file);

    for (size_t i = 1; i < cmd.args.size(); i++) {
        if (i > 1) std::cout << " ";
        std::cout << cmd.args[i];
    }
    std::cout << "\n";
    return true;
}

bool pwd(Command& cmd) {
    (void)cmd;  
    char cwd[MAX_PATH_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << "\n";
        return true;
    }
    return false;
}

bool cd(Command& cmd) {
    if (cmd.args.size() < 2) {
        return false;
    }

    std::size_t pos = cmd.args[1].find("~");
    if (pos != std::string::npos) {
        const char* home = std::getenv("HOME");
        if (home) {
            cmd.args[1].replace(pos, 1, home);
        }
    }

    if (access(cmd.args[1].c_str(), F_OK) == 0) {
        chdir(cmd.args[1].c_str());
        return true;
    }

    std::cout << "cd: " << cmd.args[1] << ": No such file or directory\n";
    return false;
}

std::unordered_map<std::string, BuiltinFn> builtinMap = {
    {"type", type},
    {"echo", echo},
    {"exit", builtin_exit},
    {"pwd", pwd},
    {"cd", cd}
};

bool checkKey(const std::string& key) {
    return builtinMap.find(key) != builtinMap.end();
}

