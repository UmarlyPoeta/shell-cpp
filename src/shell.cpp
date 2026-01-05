#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <unordered_map>
#include <functional>
#include <sys/wait.h>


void runExternal(std::vector<std::string>& args);
bool isBuiltin(std::string command);
bool inPathAndExcetueable(std::string command, std::string& path);
const char* PATH();
bool type(std::vector<std::string>& args);
bool echo(std::vector<std::string>& args);
bool builtin_exit(std::vector<std::string>& args);

void runExternal(std::vector<std::string>& args) {
  pid_t pid = fork();

  if (pid == 0) {
    std::vector<char*> cstr_args;
    for (auto& arg : args) {
      cstr_args.push_back(&arg[0]);
    }
    cstr_args.push_back(nullptr);

    execvp(cstr_args[0], &cstr_args[0]);
    perror("execvp failed");
    exit(1);
  } else if (pid > 0){
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork failed");
  }
}

// ########################### builtin ######################
using BuiltinFn = std::function<bool(std::vector<std::string>&)>;



bool builtin_exit(std::vector<std::string>& args) {
  exit(0);
}

bool type(std::vector<std::string>& args)
{
  if (args.size() < 2) {
    std::cout << "type: missing argument\n";
    return false;
  }

  std::string croped_command = args[1];
  std::string check_path;
  if (isBuiltin(croped_command)){
    std::cout << croped_command << " is a shell builtin\n";
  }
  else if (inPathAndExcetueable(croped_command, check_path)){ 
    std::cout << croped_command << " is " << check_path << "\n";
  }
  else {
    std::cout << croped_command << ": not found\n";
  }

  return true;
};

bool echo(std::vector<std::string>& args)
{
  for (size_t i = 1; i < args.size(); i++) {
    if (i > 1) std::cout << " ";
    std::cout << args[i];
  }
  std::cout << "\n";
  return true;
};

bool pwd(std::vector<std::string>& args) {
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    std::cout << cwd << "\n";
    return true;
  }
  return false;
}

bool cd(std::vector<std::string>& args) {
  if (args.size() < 2) {return false;}

  std::size_t pos = args[1].find("~");

  if (pos != std::string::npos) {
    args[1].replace(pos, 1, std::getenv("HOME"));
  }

  if (access(args[1].c_str(), F_OK) == 0) {
    chdir(args[1].c_str());
    return true;
  }
  std::cout << "cd: " << args[1] << ": No such file or directory\n";
  return false;
}

std::unordered_map<std::string, BuiltinFn> builtinMap = {
  {"type", type},
  {"echo", echo},
  {"exit", builtin_exit},
  {"pwd", pwd},
  {"cd", cd}
};


bool checkKey(std::string key) {
  return !(builtinMap.find(key) == builtinMap.end());
}


// ###################################################


// ################### helper func #####################
std::vector<std::string> getDirs(const char* p) {
  std::vector<std::string> pathDirs;
  std::string dir;
  std::stringstream ss(p);

  while (std::getline(ss, dir, ':')) {
    pathDirs.push_back(dir);
  }

  return pathDirs;
}

bool isBuiltin(std::string command) {
  return checkKey(command);
}

bool inPathAndExcetueable(std::string command, std::string& path) {
  std::vector<std::string> path_dirs = getDirs(PATH());


  for (auto dir : path_dirs) {
      std::string check_path = dir + "/" + command;
      if (access(check_path.c_str(), X_OK) == 0){ 
        path = check_path;
        return true;
      }
  }

  return false;
}

const char* PATH() {
  return std::getenv("PATH");
}

std::vector<std::string> parseInput(std::string input) {
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
    else if (input[i] == '"' && !is_single_quote){
      is_double_quote = !is_double_quote;
    } else if (input[i] == '\'' && !is_double_quote){
      is_single_quote = !is_single_quote;
    } else if (input[i] == ' ' && !is_double_quote && !is_single_quote) {
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

// #####################################################

// main
int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  std::string input;
  std::string line;
  std::vector<std::string> arguments;
  std::string path_to_file;

  while (true) {
  std::cout << "$ ";
  std::getline(std::cin, input);

  arguments = parseInput(input);

  if (arguments.empty()) {
    continue;
  }

  if (checkKey(arguments[0])) {
    builtinMap[arguments[0]](arguments);
  }
  else if (inPathAndExcetueable(arguments[0], path_to_file)){
    runExternal(arguments);
  }
  else {
  std::cout << arguments[0] << ": command not found" << std::endl;
  }
  }

  return 0;
}
