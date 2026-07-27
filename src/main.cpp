#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>


bool isStrictlyNumeric(const std::string &str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}


int main (int ac, char **av)
{
     //Validate argument count
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
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

