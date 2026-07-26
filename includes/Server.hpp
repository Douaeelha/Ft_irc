#pragma once

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

class Server
{
private:
    int         _port;
    std::string _password;
    int         _server_fd; // The main listening socket

    void initSocket();

public:
    Server(int port, std::string password);
    ~Server();

};