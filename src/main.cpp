#include "parser.hpp"
#include "executor.hpp"
#include "builtins.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::string input;
    std::string path_to_file;

    while (true) {
        std::cout << "$ ";
        std::getline(std::cin, input);

        auto cmd = parseCommand(input);

        if (cmd.args.empty()) {
            continue;
        }

        if (checkKey(cmd.args[0])) {
            builtinMap[cmd.args[0]](cmd);
        } else if (inPathAndExecutable(cmd.args[0], path_to_file)) {
            runExternal(cmd);
        } else {
            std::cout << cmd.args[0] << ": command not found\n";
        }
    }

    return 0;
}
