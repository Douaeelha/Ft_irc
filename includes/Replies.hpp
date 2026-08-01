#pragma once
#include <string>

namespace Replies {
    const std::string RPL_WELCOME = "001"; // Welcome message
    const std::string ERR_NONICKNAMEGIVEN = "431";// No nickname given
    const std::string ERR_ERRONEUSNICKNAME = "432"; // invalid nickname(client used spaces or control characters)
    const std::string ERR_NICKNAMEINUSE = "433";// nickname is already in use
    const std::string ERR_NEEDMOREPARAMS = "461";// Not enough parameters
    const std::string ERR_ALREADYREGISTRED = "462";// You may not reregister
    const std::string ERR_PASSWDMISMATCH = "464";// Password mismatch



std::string build(const std::string &serverName, const std::string &code, const std::string &params, const std::string &message);
}