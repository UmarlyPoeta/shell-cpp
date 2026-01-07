#include "utils.hpp"
#include "builtins.hpp"
#include <sstream>
#include <unistd.h>
#include <cstdlib>

const char* PATH() {
    const char* path = std::getenv("PATH");
    return path ? path : "/bin:/usr/bin";
}

std::vector<std::string> getDirs(const char* p) {
    std::vector<std::string> pathDirs;
    std::string dir;
    std::stringstream ss(p);

    while (std::getline(ss, dir, ':')) {
        pathDirs.push_back(dir);
    }

    return pathDirs;
}

bool isBuiltin(const std::string& command) {
    return checkKey(command);
}

bool inPathAndExecutable(const std::string& command, std::string& path) {
    std::vector<std::string> path_dirs = getDirs(PATH());

    for (const auto& dir : path_dirs) {
        std::string check_path = dir + "/" + command;
        if (access(check_path.c_str(), X_OK) == 0) {
            path = check_path;
            return true;
        }
    }

    return false;
}