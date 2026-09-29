#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>
#include "includes/Server_bonus.hpp"
#include "includes/Client_bonus.hpp"
#include "includes/Channel_bonus.hpp"


bool isStrictlyNumeric(const std::string &str) {
    int i = 0;
    while(str[i] != '\0') {
        if (!std::isdigit(str[i])) {
            return false;
        }
        i++;
    }
    return true;
}


int main (int ac, char **av)
{
     //Validate argument count
    if (ac != 3)
    {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    //input edge cases 
    std::string password = av[2];
    if (password.empty() || password.find_first_not_of(" \t\n\r\f\v") == std::string::npos)
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
   try
    {
        Server irc(port, password);
        irc.run();
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Server Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

