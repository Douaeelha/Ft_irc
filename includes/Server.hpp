#pragma once

#include <iostream>
#include <vector>
#include <sys/socket.h> // ! REVIEW FOR MAC
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h> // for close()
#include <string>
#include <stdexcept> // for runtime_error()
#include <cstdlib> // For std::atoi

class Server
{
private:
    int         _port;
    std::string _password;
    int         _server_fd; // The main listening socket

    std::vector<struct pollfd> _poll_fds;

    void initSocket();

    void acceptNewClient();
    void receiveData(int client_fd);

public:
    Server(int port, std::string password);
    ~Server();

    void run();

};