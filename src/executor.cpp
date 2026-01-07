#include "executor.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>

void runExternal(const Command& cmd) {
    pid_t pid = fork();

    if (pid == 0) {
        std::vector<char*> cstr_args;
        for (const auto& arg : cmd.args) {
            cstr_args.push_back(const_cast<char*>(arg.c_str()));
        }
        cstr_args.push_back(nullptr);

        if (!cmd.stdout_file.empty()) {
            int fd = open(cmd.stdout_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cstr_args[0], &cstr_args[0]);
        perror("execvp failed");
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork failed");
    }
}
