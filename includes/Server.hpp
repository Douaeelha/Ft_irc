#pragma once

#include "Channel.hpp"
#include "Parser.hpp"
#include "Dispatcher.hpp"
#include <iostream>
#include <vector>
#include <map>
#include "Client.hpp"
#include <sys/socket.h> // ! REVIEW FOR MAC
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h> // for close()
#include <string>
#include <stdexcept> // for runtime_error()
#include <cstdlib> // For std::atoi
#include <signal.h> //signals
#include <arpa/inet.h>

class Server
{
    private:
        std::map<std::string, Channel> _channels;
        
        std::map<int, Client> _clients; // Map of client file descriptors to Client objects
        int         _port;
        std::string _password;
        int         _server_fd; // The main listening socket

        std::vector<struct pollfd> _poll_fds;

        void initSocket();

        void acceptNewClient();
        void receiveData(int client_fd);

        // ignal handling
        static bool _isRunning;
        static void signalHandler(int signum);

    public:
        Server(int port, std::string password);
        ~Server();

        void run();

};