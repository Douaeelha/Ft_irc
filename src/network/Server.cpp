#include "Server.hpp"

Server::Server(int port) : _port(port), _server_fd(-1)
{
    initSocket();
}

Server::~Server()
{
    if (_server_fd != -1)
        close(_server_fd);
    // Loop through and close all client FDs later
}

void Server::initSocket()
{
    // 1. Call socket()
    // 2. Use setsockopt() to allow port reuse
    // 3. Use fcntl() to set O_NONBLOCK
    // 4. Configure sockaddr_in struct
    // 5. Call bind()
    // 6. Call listen()
    // 7. Add _server_fd to _poll_fds
}

void Server::run()
{
    std::cout << "Server starting on port " << _port << "..." << std::endl;
    
    while (true)
    {
        // 1. Call poll() on _poll_fds vector
        // 2. Loop through _poll_fds to check for revents (POLLIN, POLLOUT)
        // 3. If event is on _server_fd -> acceptNewClient()
        // 4. If event is on client_fd -> receiveData(client_fd)
    }
}