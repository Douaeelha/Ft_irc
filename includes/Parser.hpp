#pragma once 
#include <vector>
#include <string>
class ParsedMessage {
public:
    std::string command;
    std::vector<std::string> params;
    std::string trailing;
};  