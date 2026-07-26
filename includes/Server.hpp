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
    int _port;
    int _server_fd;
    std::vector<struct pollfd> _poll_fds;

    // Private helper methods
    void initSocket();
    void acceptNewClient();
    void receiveData(int client_fd);

public:
    Server(int port);
    ~Server();

    void run(); // The main infinite poll() loop
};