#pragma once
#include <string>

namespace Replies {
    //replies 
    const std::string RPL_WELCOME = "001"; // Welcome message
    const std::string RPL_YOURHOST = "002"; 
    const std::string RPL_CREATED = "003";
    const std::string RPL_MYINFO = "004"; 
    const std::string RPL_CHANNELMODEIS = "324";// <channel> <mode> <mode params>// sent by server to report channel settings 
    const std::string RPL_NOTOPIC = "331";// <channel> :No topic is set 
    const std::string RPL_TOPIC = "332";// <channel> :<topic> // sent by server to report the topic of a channel
    const std::string RPL_INVITING = "341";// <nick> <channel> // sent by server to indicate that a user has been invited to a channel
    const std::string RPL_NAMREPLY = "353";// <channel> :[[@|+]<nick> [[@|+]<nick> [...]]] // sent by server to list the nicknames of users in a channel
    const std::string RPL_ENDOFNAMES = "366";// <channel> :End of /NAMES list // sent by server to indicate the end of a /NAMES list

    //errors
    const std::string ERR_NOSUCHNICK = "401";// <nickname> :No such nick/channel
    const std::string ERR_NOSUCHCHANNEL = "403";// <channel> :No such channel
    const std::string ERR_CANNOTSENDTOCHAN = "404";// <channel> :Cannot send to channel
    const std::string ERR_NORECIPIENT = "411";// <command> :No recipient given (for PRIVMSG/NOTICE)
    const std::string ERR_UNKNOWNCOMMAND = "421";// <command> :Unknown command
    const std::string ERR_NOTEXTTOSEND = "412";// :No text to
    const std::string ERR_NONICKNAMEGIVEN = "431";// No nickname given
    const std::string ERR_ERRONEUSNICKNAME = "432"; // invalid nickname(client used spaces or control characters)
    const std::string ERR_NICKNAMEINUSE = "433";// nickname is already in use
    const std::string ERR_USERNOTINCHANNEL = "441";// <nick> <channel> :They aren't on that channel
    const std::string ERR_NOTONCHANNEL     = "442";// <channel> :You're not on that channel
    const std::string ERR_USERONCHANNEL    = "443";// <user> <channel> :is already on channel
    const std::string ERR_NOTREGISTERED = "451";// :You have not registered
    const std::string ERR_NEEDMOREPARAMS = "461";// Not enough parameters
    const std::string ERR_ALREADYREGISTRED = "462";// You may not reregister
    const std::string ERR_PASSWDMISMATCH = "464";// Password mismatch
    const std::string ERR_CHANNELISFULL    = "471";// <channel> :Cannot join channel (+l)
    const std::string ERR_UNKNOWNMODE      = "472";// <char> :is unknown mode char to me for <channel>
    const std::string ERR_INVITEONLYCHAN   = "473";// <channel> :Cannot join channel (+i)
    const std::string ERR_BADCHANNELKEY    = "475";// <channel> :Bad channel key
    const std::string ERR_CHANOPRIVSNEEDED = "482";// <channel> :You're not channel operator

    
    std::string build(const std::string &serverName, const std::string &code, const std::string &params, const std::string &message);
}