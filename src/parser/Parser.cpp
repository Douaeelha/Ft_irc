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

    if (!result.empty() && result[result.size() - 1] == '\n')
        result.erase(result.size() - 1);

    if (!result.empty() && result[result.size() - 1] == '\r')
        result.erase(result.size() - 1);

    return result;
}

/*---------TEST---------*/

// #include <iostream>
// #include <string>
// #include "Parser.hpp"

// static void testParse(const std::string &input)
// {
//     Parser parser;
//     Parser::Command cmd = parser.parse(input);

//     std::cout << "INPUT: " << input << "\n";
//     std::cout << "COMMAND: " << cmd.name << "\n";
//     std::cout << "PARAMS:";
//     for (size_t i = 0; i < cmd.params.size(); ++i)
//         std::cout << " [" << cmd.params[i] << "]";
//     std::cout << "\n\n";
// }

// int main()
// {
//     // testParse("PASS secret\r\n");
//     // testParse("NICK test\r\n");
//     testParse("PRIVMSG #chan :hello world\r\n");
//     // testParse("JOIN #room\r\n");
//     return 0;
// }