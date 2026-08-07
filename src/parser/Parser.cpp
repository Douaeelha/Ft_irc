#include "Parser.hpp"
#include <sstream>

Parser::Parser() {}

Parser::~Parser() {}

Parser::Command Parser::parse(const std::string &line) const
{
    Parser::Command cmd;

    std::string cleaned = trimTrailingCRLF(line);
    if (cleaned.empty())
        return cmd;

    std::istringstream iss(cleaned);
    std::string firstToken;

    if (!(iss >> firstToken))
        return cmd;

    cmd.name = firstToken;
    
    std::string token;
    while (iss >> token)
    {
        if (token[0] == ':')
        {
            std::string rest;
            std::getline(iss, rest);

            if (!rest.empty() && rest[0] == ' ')
                rest.erase(0, 1);

            cmd.params.push_back(token.substr(1) + rest);
            break;
        }

        cmd.params.push_back(token);
    }

    return cmd;
}

std::string Parser::trimTrailingCRLF(const std::string &line) const
{
    std::string result = line;

    if (!result.empty() && result.back() == '\n')
        result.pop_back();

    if (!result.empty() && result.back() == '\r')
        result.pop_back();

    return result;
}

