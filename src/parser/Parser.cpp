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
    cmd.params = splitParams(cleaned);

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

std::vector<std::string> Parser::splitParams(const std::string &line) const
{
    std::vector<std::string> params;

    std::istringstream iss(line);
    std::string command;

    if (!(iss >> command))
        return params;

    std::string token;

    while (iss >> token)
    {
        if (token[0] == ':')
        {
            std::string rest;
            std::getline(iss, rest);

            if (!rest.empty() && rest[0] == ' ')
                rest.erase(0, 1);

            if (!token.empty())
                params.push_back(token.substr(1) + rest);

            break;
        }

        params.push_back(token);
    }

    return params;
}