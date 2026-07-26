#include "Server.hpp"
#include <unistd.h> // for close()
#include <sting>
#include <sys/socket.h>
#include <stdexcept> // for runtime_error()

// Constructor
Server::Server(int port, std::string password) : _port(port), _password(password), _server_fd(-1)
{
    initSocket();
}

// Destructor
Server::~Server()
{
    // Clean up the main socket if it was successfully opened
    if (_server_fd != -1)
        close(_server_fd);
}

void    Server::initSocket()
{
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_server_fd < 0)
        throw std::runtime_error("Error: Failed to create socket");


    //multiplexing: one waiter does all , doesnt not wait on a table if there not ready 
    fcntl(_server_fd, F_SETFL, O_NONBLOCK);
    /*
    this line does:
       F_SETFL: Tells the operating system, "I want to SET the FiLe status flags."
       O_NONBLOCK: The specific flag that turns off the default blocking behavior.
    */
}