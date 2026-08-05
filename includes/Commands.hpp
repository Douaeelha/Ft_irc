#pragma once 
#include "Client.hpp"
#include <vector>
#include <map>
#include <string>

namespace Commands {
    // PASS <password>
    void handlePass(Client &client, const std::vector<std::string> &args,
                     const std::string &serverPassword, const std::string &serverName);
 
    // NICK <nickname>
    void handleNick(Client &client, const std::vector<std::string> &args,
                     const std::map<int, Client*> &clients, const std::string &serverName);
 
    // USER <username> <hostname> <servername> :<realname>
    void handleUser(Client &client, const std::vector<std::string> &args,
                     const std::string &serverName);
 
    void checkRegistrationComplete(Client &client, const std::string &serverName);
 
    
    bool isValidNickname(const std::string &nick);
}
 