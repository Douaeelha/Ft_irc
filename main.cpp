#include <iostream>
// #include "Client.hpp"
// #include "Channel.hpp"
#include <vector>
#include <cctype>
#include <cstdlib>
#include <string>
// #include "Server.hpp"


bool isStrictlyNumeric(const std::string &str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}


int main (int ac, char **av)
{
    if(ac != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    //input edge cases 
    std::string pass = av[2];
    if (pass.empty() || pass.find_first_not_of(" \t\n\r\f\v") == std::string::npos)
    {
        std::cerr << "Password cannot be empty or whitespace only" << std::endl;
        return 1;
    }
    if(!isStrictlyNumeric(av[1]))
    {
        std::cerr << "Port must be a number" << std::endl;
        return 1;
    }
    int port = std::atoi(av[1]);
    if(port < 1024 || port > 49151) // check notes to know why this check 
    {
        std::cerr << "Port number must be between 1024 and 49151" << std::endl;
        return 1;
    }
    return 0;

}