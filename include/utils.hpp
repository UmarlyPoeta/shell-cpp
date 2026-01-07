#pragma once
#include <string>
#include <vector>

const char* PATH();

std::vector<std::string> getDirs(const char* p);

bool isBuiltin(const std::string& command);

bool inPathAndExecutable(const std::string& command, std::string& path);

bool checkKey(const std::string& key);
