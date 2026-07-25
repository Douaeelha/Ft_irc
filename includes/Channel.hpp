#pragma once
#include <string>
#include <vector>
#include "Client.hpp"

class Channel{
    private:
        std::string name;
        std::string topic;
        std::vector<Client*> users;
        std::vector<Client*> operators;
        bool invite_only;
};

/// methods to implement:
// add user to channel
// remove user from channel
//broadcast message to all users in channel
// is user in channel