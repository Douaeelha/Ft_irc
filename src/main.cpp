#include "Server.hpp"
#include <iostream>
#include <cstdlib> // For std::atoi

int main(int argc, char **argv)
{
    //Validate argument count
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    // Parse arguments
    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    // Basic port validation (avoid well-known system ports under 1024)
    if (port < 1024 || port > 65535)
    {
        std::cerr << "Error: Port must be between 1024 and 65535." << std::endl;
        return 1;
    }

    // Run that shit
    try
    {
        Server irc(port, password);
        // irc.run();
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Server Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}