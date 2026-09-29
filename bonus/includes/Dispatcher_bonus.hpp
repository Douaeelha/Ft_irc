#pragma once
#include <string>
#include <map>
#include "Client_bonus.hpp"
#include "Channel_bonus.hpp"
#include "Parser_bonus.hpp"

namespace Dispatcher {
    void dispatch(const Parser::Command &cmd, Client &client,
                   std::map<int, Client*> &clients,
                   std::map<std::string, Channel> &channels,
                   const std::string &serverPassword,
                   const std::string &serverName);
}