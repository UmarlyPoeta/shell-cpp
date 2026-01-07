#include "parser.hpp"
#include <sstream>

std::vector<std::string> parseInput(const std::string& input) {
    bool is_single_quote = false;
    bool is_double_quote = false;
    std::vector<std::string> args;
    std::string argument;

    for (size_t i = 0; i < input.size(); i++) {

        if (input[i] == '\\' && !is_single_quote && !is_double_quote && i + 1 < input.size()) {
            i++;
            argument.push_back(input[i]);
            continue;
        }

        else if (input[i] == '\\' && is_double_quote && !is_single_quote && i + 1 < input.size()) {
            char next = input[i + 1];
            if (next == '"' || next == '\\' || next == '$' || next == '`') {
                i++;
                argument.push_back(input[i]);
                continue;
            }
            argument.push_back(input[i]);
            continue;
        }

        else if (input[i] == '"' && !is_single_quote) {
            is_double_quote = !is_double_quote;
        } else if (input[i] == '\'' && !is_double_quote) {
            is_single_quote = !is_single_quote;
        }

        else if (input[i] == ' ' && !is_double_quote && !is_single_quote) {
            if (!argument.empty()) {
                args.push_back(argument);
                argument.clear();
            }
        } else {
            argument.push_back(input[i]);
        }
    }

    if (!argument.empty()) {
        args.push_back(argument);
    }

    return args;
}

std::vector<std::string> parseRedirection(const std::vector<std::string> args, std::string& output_file)
{
    std::vector<std::string> new_args;

    for (size_t i = 0; i < args.size(); i++ ){
        if (args[i] == ">" || args[i] == "1>") {
            output_file = args[i + 1];
            break;
        }
        else {
            new_args.push_back(args[i]);
        }
    }



    return new_args;
}

Command parseCommand(const std::string& input) {
    Command cmd;
    std::string file_output_from_func;
    std::vector<std::string> args_with_redirection = parseInput(input);
    cmd.args = parseRedirection(args_with_redirection, file_output_from_func);
    cmd.stdout_file = file_output_from_func;
    cmd.command_name = cmd.args[0];


    return cmd;
}
