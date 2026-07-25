#pragma once 
#include <string>

// represent a single user 
class Client{
    private:
        int fd;
        std::string nickname;
        std::string username;
        std::string buffer_in;
        std::string buffer_out;
        bool is_registered;// have they completed the registration process
};

// methods to implement:
// send message to client
// receive message from client
// set nickname
//set username