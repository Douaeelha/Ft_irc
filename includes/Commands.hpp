#pragma once 
#include "Client.hpp"
#include "Channel.hpp"
#include <vector>
#include <map>
#include <string>

namespace Commands {
    /*---------Registration commands---------*/
    // PASS <password>
    void handlePass(Client &client, const std::vector<std::string> &args,
                     const std::string &serverPassword, const std::string &serverName);
 
    // NICK <nickname>
    void handleNick(Client &client, const std::vector<std::string> &args,
                     const std::map<int, Client*> &clients, std::map<std::string, Channel> &channels,
                     const std::string &serverName);
 
    // USER <username> <hostname> <servername> :<realname>
    void handleUser(Client &client, const std::vector<std::string> &args,
                     const std::string &serverName);
    // RPL_WELCOME ONCE REGISTRATION IS COMPLETE
    void checkRegistrationComplete(Client &client, const std::string &serverName);
 
    
    bool isValidNickname(const std::string &nick);

    /*---------Channel commands---------*/

    // JOIN <#channel> [key]
    // NOTE: single channel + single key per call. RFC allows comma-separated
    // lists of channels/keys for one JOIN line like this JOIN <channel>{,<channel>} [<key>{,<key>}].
    // if the reference client sends this Reda should split on ',' and call this once per pair.
    void handleJoin(Client &client, const std::vector<std::string> &args,
                     std::map<std::string, Channel> &channels, const std::string &serverName);
 
    // PRIVMSG <target>[,<target>...] :<text>
    void handlePrivmsg(Client &client, const std::vector<std::string> &args,
                        std::map<int, Client*> &clients,
                        std::map<std::string, Channel> &channels,
                        const std::string &serverName);
 
    // KICK <#channel> <nick> [:<comment>]
    void handleKick(Client &client, const std::vector<std::string> &args,
                     std::map<std::string, Channel> &channels, const std::string &serverName);
 
    // INVITE <nick> <#channel>
    void handleInvite(Client &client, const std::vector<std::string> &args,
                       std::map<int, Client*> &clients,
                       std::map<std::string, Channel> &channels,
                       const std::string &serverName);
 
    // TOPIC <#channel> [:<topic>]
    void handleTopic(Client &client, const std::vector<std::string> &args,
                      std::map<std::string, Channel> &channels, const std::string &serverName);
 
    // MODE <#channel> [<+/-modes> [<params...>]]
    void handleMode(Client &client, const std::vector<std::string> &args,
                     std::map<int, Client*> &clients,
                     std::map<std::string, Channel> &channels,
                     const std::string &serverName);
    void handleQuit(Client &client, const std::vector<std::string> &args,
                    std::map<std::string, Channel> &channels, const std::string &serverName);
}

