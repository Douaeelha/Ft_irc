#include "Commands.hpp"
#include "Replies.hpp"
#include <cctype>
#include <sstream>
#include <utility>

/*---------helper functions-------------*/

static bool isSpecialNickChar(char c) {
    return c == '[' || c == ']' || c == '\\' || c == '`' ||
           c == '_' || c == '^' || c == '{' || c == '|' || c == '}';
}

static std::string prefixFor(const Client &c) {
    return c.getNickname() + "!" + c.getUsername() + "@" + c.getHostname();
}

static Client *findClientByNick(const std::map<int, Client*> &clients, const std::string &nick) {
    std::map<int, Client*>::const_iterator it = clients.begin();
    for (; it != clients.end(); ++it) {
        if (it->second->getNickname() == nick)
            return it->second;
    }
    return NULL;
}
 
static std::string toString(size_t value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
 
static bool parseSizeT(const std::string &s, size_t &out) {
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    }
    std::istringstream iss(s);
    iss >> out;
    return !iss.fail();
}

static std::vector<std::string> splitCommaSeparated(const std::string &str) {
    std::vector<std::string> result;
    std::string current;

    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (*it == ',') {
            result.push_back(current);
            current.clear();
        } else {
            current += *it;
        }
    }

    result.push_back(current);
    return result;
}

void Commands::handleJoinCommand(Client &client,
                                 const std::vector<std::string> &args,
                                 std::map<std::string, Channel> &channels,
                                 const std::string &serverName) {
    if (args.empty()) {
        handleJoin(client, args, channels, serverName);
        return;
    }

    std::vector<std::string> channelNames = splitCommaSeparated(args[0]);
    std::vector<std::string> keys;

    if (args.size() > 1)
        keys = splitCommaSeparated(args[1]);

    for (size_t i = 0; i < channelNames.size(); ++i) {
        std::vector<std::string> joinArgs;
        joinArgs.push_back(channelNames[i]);

        if (i < keys.size())
            joinArgs.push_back(keys[i]);

        handleJoin(client, joinArgs, channels, serverName);
    }
}

/*---------Registration---------*/

// RFC1459-ish nickname validation (letter/special first char, then
    // letters/digits/special/'-', max 9 chars).

bool Commands::isValidNickname(const std::string &nick) {
    const size_t MAX_NICK_LEN = 9;

    if (nick.empty() || nick.size() > MAX_NICK_LEN)
        return false;

    char first = nick[0];
    if (!std::isalpha(static_cast<unsigned char>(first)) && !isSpecialNickChar(first))
        return false;

    for (size_t i = 1; i < nick.size(); ++i) {
        char c = nick[i];
        bool ok = std::isalnum(static_cast<unsigned char>(c)) || c == '-' || isSpecialNickChar(c);
        if (!ok)
            return false;
    }
    return true;
}

void Commands::handlePass(Client &client, const std::vector<std::string> &args,
                           const std::string &serverPassword, const std::string &serverName) {
    std::string target = client.getNickname().empty() ? "*" : client.getNickname();

    if (client.getIsRegistered()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_ALREADYREGISTRED,
                                                  target, "You may not reregister"));
        return;
    }
    if (args.empty()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                  target + " PASS", "Not enough parameters"));
        return;
    }
    if (args[0] != serverPassword) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_PASSWDMISMATCH,
                                                  target, "Password incorrect"));
        return;
    }
    client.setHadPassword(true);
}
// here something need to be added 
//channel map to args so i can check in what channels 
// current clinet is active in and broadcats the nick change 
// also needed for quit disconnect cleanup 
void Commands::handleNick(Client &client, const std::vector<std::string> &args,
                           const std::map<int, Client*> &clients, const std::string &serverName) {
    std::string target = client.getNickname().empty() ? "*" : client.getNickname();

    if (args.empty() || args[0].empty()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NONICKNAMEGIVEN,
                                                  target, "No nickname given"));
        return;
    }

    const std::string &newNick = args[0];

    if (!isValidNickname(newNick)) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_ERRONEUSNICKNAME,
                                                  target + " " + newNick, "Erroneous nickname"));
        return;
    }

    std::map<int, Client*>::const_iterator it = clients.begin();
    for (; it != clients.end(); ++it) {
        if (it->second->getFd() != client.getFd() && it->second->getNickname() == newNick) {
            client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NICKNAMEINUSE,
                                                      target + " " + newNick, "Nickname is already in use"));
            return;
        }
    }

    client.setNickname(newNick);
    client.setHasNickname(true);
    // NB: if the client was already registered, changing nickname should also
    // broadcast a "NICK" message to every channel they're in -- that needs the
    // channel list, which lives in Server, so it's not handled here.
}

void Commands::handleUser(Client &client, const std::vector<std::string> &args,
                           const std::string &serverName) {
    std::string target = client.getNickname().empty() ? "*" : client.getNickname();

    if (client.getIsRegistered()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_ALREADYREGISTRED,
                                                  target, "You may not reregister"));
        return;
    }
    // USER <username> <hostname> <servername> :<realname>
    if (args.size() < 4) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                  target + " USER", "Not enough parameters"));
        return;
    }

    client.setUsername(args[0]);
    client.setRealname(args[3]);
    client.setHasUsername(true);
}

// Call after any of the three above: flips _is_registered and sends
    // RPL_WELCOME once PASS+NICK+USER have all succeeded.
void Commands::checkRegistrationComplete(Client &client, const std::string &serverName) {
    if (client.getIsRegistered())
        return;
    if (client.getHadPassword() && client.getHasNickname() && client.getHasUsername()) {
        client.setIsRegistered(true);
        std::string welcome = "Welcome to the Internet Relay Network " + client.getNickname();
        client.appendToBufferOut(Replies::build(serverName, Replies::RPL_WELCOME,
                                                  client.getNickname(), welcome));
    }
}

// ---------------- channel commands ----------------

void Commands::handleJoin(Client &client, const std::vector<std::string> &args,
                           std::map<std::string, Channel> &channels, const std::string &serverName) {
    std::string target = client.getNickname();

    if (args.empty()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                  target + " JOIN", "Not enough parameters"));
        return;
    }
    const std::string &chanName = args[0];
    std::string key = args.size() > 1 ? args[1] : "";

    if (chanName.empty() || chanName[0] != '#') {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHCHANNEL,
                                                  target + " " + chanName, "No such channel"));
        return;
    }

    std::map<std::string, Channel>::iterator it = channels.find(chanName);
    bool isNewChannel = (it == channels.end());
    if (isNewChannel) {
        channels.insert(std::pair<std::string, Channel>(chanName, Channel(chanName)));
        it = channels.find(chanName);
    }
    Channel &channel = it->second;

    if (channel.isMember(client.getFd()))
        return; // already in, nothing to do

    if (!isNewChannel) {
        if (channel.isInviteOnly() && !channel.isInvited(client.getFd())) {
            client.appendToBufferOut(Replies::build(serverName, Replies::ERR_INVITEONLYCHAN,
                                                      target + " " + chanName, "Cannot join channel (+i)"));
            return;
        }
        if (channel.hasKey() && channel.getKey() != key) {
            client.appendToBufferOut(Replies::build(serverName, Replies::ERR_BADCHANNELKEY,
                                                      target + " " + chanName, "Cannot join channel (+k)"));
            return;
        }
        if (channel.hasUserLimit() && channel.getMemberCount() >= channel.getUserLimit()) {
            client.appendToBufferOut(Replies::build(serverName, Replies::ERR_CHANNELISFULL,
                                                      target + " " + chanName, "Cannot join channel (+l)"));
            return;
        }
    }

    channel.addMember(&client);
    channel.removeInvite(client.getFd());
    if (isNewChannel)
        channel.addOperator(client.getFd()); // founder is operator

    std::string joinMsg = ":" + prefixFor(client) + " JOIN :" + chanName + "\r\n";
    channel.broadcast(joinMsg);

    if (channel.getTopic().empty())
        client.appendToBufferOut(Replies::build(serverName, Replies::RPL_NOTOPIC,
                                                  target + " " + chanName, "No topic is set"));
    else
        client.appendToBufferOut(Replies::build(serverName, Replies::RPL_TOPIC,
                                                  target + " " + chanName, channel.getTopic()));

    std::string names;
    const std::map<int, Client*> &members = channel.getMembers();
    std::map<int, Client*>::const_iterator mit = members.begin();
    for (; mit != members.end(); ++mit) {
        if (!names.empty())
            names += " ";
        if (channel.isOperator(mit->first))
            names += "@";
        names += mit->second->getNickname();
    }
    client.appendToBufferOut(Replies::build(serverName, Replies::RPL_NAMREPLY,
                                              target + " = " + chanName, names));
    client.appendToBufferOut(Replies::build(serverName, Replies::RPL_ENDOFNAMES,
                                              target + " " + chanName, "End of /NAMES list"));
}

void Commands::handlePrivmsg(Client &client, const std::vector<std::string> &args,
                              std::map<int, Client*> &clients,
                              std::map<std::string, Channel> &channels,
                              const std::string &serverName) {
    std::string target = client.getNickname();

    if (args.empty()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NORECIPIENT,
                                                  target, "No recipient given (PRIVMSG)"));
        return;
    }
    if (args.size() < 2 || args[1].empty()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOTEXTTOSEND,
                                                  target, "No text to send"));
        return;
    }

    const std::string &recipients = args[0];
    const std::string &text = args[1];
    std::string prefix = ":" + prefixFor(client) + " PRIVMSG ";

    size_t pos = 0;
    for (;;) {
        size_t comma = recipients.find(',', pos);
        std::string dest = (comma == std::string::npos)
            ? recipients.substr(pos)
            : recipients.substr(pos, comma - pos);

        if (!dest.empty()) {
            if (dest[0] == '#') {
                std::map<std::string, Channel>::iterator it = channels.find(dest);
                if (it == channels.end()) {
                    client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHCHANNEL,
                                                              target + " " + dest, "No such channel"));
                } else if (!it->second.isMember(client.getFd())) {
                    client.appendToBufferOut(Replies::build(serverName, Replies::ERR_CANNOTSENDTOCHAN,
                                                              target + " " + dest, "Cannot send to channel"));
                } else {
                    it->second.broadcast(prefix + dest + " :" + text + "\r\n", client.getFd());
                }
            } else {
                Client *dst = findClientByNick(clients, dest);
                if (!dst) {
                    client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHNICK,
                                                              target + " " + dest, "No such nick/channel"));
                } else {
                    dst->appendToBufferOut(prefix + dest + " :" + text + "\r\n");
                }
            }
        }

        if (comma == std::string::npos)
            break;
        pos = comma + 1;
    }
}

void Commands::handleKick(Client &client, const std::vector<std::string> &args,
                           std::map<std::string, Channel> &channels, const std::string &serverName) {
    std::string target = client.getNickname();

    if (args.size() < 2) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                  target + " KICK", "Not enough parameters"));
        return;
    }
    const std::string &chanName = args[0];
    const std::string &targetNick = args[1];
    std::string comment = args.size() > 2 ? args[2] : client.getNickname();

    std::map<std::string, Channel>::iterator it = channels.find(chanName);
    if (it == channels.end()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHCHANNEL,
                                                  target + " " + chanName, "No such channel"));
        return;
    }
    Channel &channel = it->second;

    if (!channel.isMember(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOTONCHANNEL,
                                                  target + " " + chanName, "You're not on that channel"));
        return;
    }
    if (!channel.isOperator(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_CHANOPRIVSNEEDED,
                                                  target + " " + chanName, "You're not channel operator"));
        return;
    }

    int targetFd = -1;
    const std::map<int, Client*> &members = channel.getMembers();
    std::map<int, Client*>::const_iterator mit = members.begin();
    for (; mit != members.end(); ++mit) {
        if (mit->second->getNickname() == targetNick) {
            targetFd = mit->first;
            break;
        }
    }
    if (targetFd == -1) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_USERNOTINCHANNEL,
                                                  target + " " + targetNick + " " + chanName,
                                                  "They aren't on that channel"));
        return;
    }

    // send to everyone incl. the kicked member (still in _members at this point),
    // *then* remove them
    std::string kickMsg = ":" + prefixFor(client) + " KICK " + chanName + " " + targetNick +
                           " :" + comment + "\r\n";
    channel.broadcast(kickMsg);
    channel.removeMember(targetFd);
}

void Commands::handleInvite(Client &client, const std::vector<std::string> &args,
                             std::map<int, Client*> &clients,
                             std::map<std::string, Channel> &channels,
                             const std::string &serverName) {
    std::string target = client.getNickname();

    if (args.size() < 2) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                  target + " INVITE", "Not enough parameters"));
        return;
    }
    const std::string &targetNick = args[0];
    const std::string &chanName = args[1];

    Client *dst = findClientByNick(clients, targetNick);
    if (!dst) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHNICK,
                                                  target + " " + targetNick, "No such nick/channel"));
        return;
    }

    std::map<std::string, Channel>::iterator it = channels.find(chanName);
    if (it == channels.end()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHCHANNEL,
                                                  target + " " + chanName, "No such channel"));
        return;
    }
    Channel &channel = it->second;

    if (!channel.isMember(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOTONCHANNEL,
                                                  target + " " + chanName, "You're not on that channel"));
        return;
    }
    if (!channel.isOperator(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_CHANOPRIVSNEEDED,
                                                  target + " " + chanName, "You're not channel operator"));
        return;
    }
    if (channel.isMember(dst->getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_USERONCHANNEL,
                                                  target + " " + targetNick + " " + chanName,
                                                  "is already on channel"));
        return;
    }

    channel.inviteClient(dst->getFd());
    client.appendToBufferOut(Replies::build(serverName, Replies::RPL_INVITING,
                                              target + " " + targetNick, chanName));
    dst->appendToBufferOut(":" + prefixFor(client) + " INVITE " + targetNick + " :" + chanName + "\r\n");
}

void Commands::handleTopic(Client &client, const std::vector<std::string> &args,
                            std::map<std::string, Channel> &channels, const std::string &serverName) {
    std::string target = client.getNickname();

    if (args.empty()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                  target + " TOPIC", "Not enough parameters"));
        return;
    }
    const std::string &chanName = args[0];

    std::map<std::string, Channel>::iterator it = channels.find(chanName);
    if (it == channels.end()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHCHANNEL,
                                                  target + " " + chanName, "No such channel"));
        return;
    }
    Channel &channel = it->second;

    if (!channel.isMember(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOTONCHANNEL,
                                                  target + " " + chanName, "You're not on that channel"));
        return;
    }

    if (args.size() < 2) {
        if (channel.getTopic().empty())
            client.appendToBufferOut(Replies::build(serverName, Replies::RPL_NOTOPIC,
                                                      target + " " + chanName, "No topic is set"));
        else
            client.appendToBufferOut(Replies::build(serverName, Replies::RPL_TOPIC,
                                                      target + " " + chanName, channel.getTopic()));
        return;
    }

    if (channel.isTopicRestricted() && !channel.isOperator(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_CHANOPRIVSNEEDED,
                                                  target + " " + chanName, "You're not channel operator"));
        return;
    }

    channel.setTopic(args[1]);
    std::string topicMsg = ":" + prefixFor(client) + " TOPIC " + chanName + " :" + args[1] + "\r\n";
    channel.broadcast(topicMsg);
}

void Commands::handleMode(Client &client, const std::vector<std::string> &args,
                           std::map<int, Client*> &clients,
                           std::map<std::string, Channel> &channels,
                           const std::string &serverName) {
    std::string target = client.getNickname();

    if (args.empty()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                  target + " MODE", "Not enough parameters"));
        return;
    }
    const std::string &chanName = args[0];

    std::map<std::string, Channel>::iterator it = channels.find(chanName);
    if (it == channels.end()) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOSUCHCHANNEL,
                                                  target + " " + chanName, "No such channel"));
        return;
    }
    Channel &channel = it->second;

    if (args.size() < 2) {
        // query current modes, no change requested
        std::string modes = "+";
        std::string params;
        if (channel.isInviteOnly()) modes += "i";
        if (channel.isTopicRestricted()) modes += "t";
        if (channel.hasKey()) { modes += "k"; params += " " + channel.getKey(); }
        if (channel.hasUserLimit()) { modes += "l"; params += " " + toString(channel.getUserLimit()); }
        client.appendToBufferOut(Replies::build(serverName, Replies::RPL_CHANNELMODEIS,
                                                  target + " " + chanName, modes + params));
        return;
    }

    if (!channel.isMember(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOTONCHANNEL,
                                                  target + " " + chanName, "You're not on that channel"));
        return;
    }
    if (!channel.isOperator(client.getFd())) {
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_CHANOPRIVSNEEDED,
                                                  target + " " + chanName, "You're not channel operator"));
        return;
    }

    const std::string &modeString = args[1];
    size_t paramIndex = 2;
    char sign = '+';
    std::string appliedModes;
    std::string appliedParams;

    for (size_t i = 0; i < modeString.size(); ++i) {
        char c = modeString[i];

        if (c == '+' || c == '-') {
            sign = c;
            continue;
        }

        if (c == 'i') {
            channel.setInviteOnly(sign == '+');
            appliedModes += sign;
            appliedModes += c;
        } else if (c == 't') {
            channel.setTopicRestricted(sign == '+');
            appliedModes += sign;
            appliedModes += c;
        } else if (c == 'k') {
            if (sign == '+') {
                if (paramIndex >= args.size()) {
                    client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                              target + " MODE", "Not enough parameters"));
                    continue;
                }
                channel.setKey(args[paramIndex]);
                appliedModes += sign;
                appliedModes += c;
                appliedParams += " " + args[paramIndex];
                paramIndex++;
            } else {
                channel.removeKey();
                appliedModes += sign;
                appliedModes += c;
            }
        } else if (c == 'l') {
            if (sign == '+') {
                if (paramIndex >= args.size()) {
                    client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                              target + " MODE", "Not enough parameters"));
                    continue;
                }
                size_t limit;
                if (parseSizeT(args[paramIndex], limit)) {
                    channel.setUserLimit(limit);
                    appliedModes += sign;
                    appliedModes += c;
                    appliedParams += " " + args[paramIndex];
                }
                // invalid numeric value: silently ignored, param still consumed
                paramIndex++;
            } else {
                channel.removeUserLimit();
                appliedModes += sign;
                appliedModes += c;
            }
        } else if (c == 'o') {
            if (paramIndex >= args.size()) {
                client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NEEDMOREPARAMS,
                                                          target + " MODE", "Not enough parameters"));
                continue;
            }
            Client *dst = findClientByNick(clients, args[paramIndex]);
            if (!dst || !channel.isMember(dst->getFd())) {
                client.appendToBufferOut(Replies::build(serverName, Replies::ERR_USERNOTINCHANNEL,
                                                          target + " " + args[paramIndex] + " " + chanName,
                                                          "They aren't on that channel"));
                paramIndex++;
                continue;
            }
            if (sign == '+')
                channel.addOperator(dst->getFd());
            else
                channel.removeOperator(dst->getFd());
            appliedModes += sign;
            appliedModes += c;
            appliedParams += " " + args[paramIndex];
            paramIndex++;
        } else {
            std::string unknown(1, c);
            client.appendToBufferOut(Replies::build(serverName, Replies::ERR_UNKNOWNMODE,
                                                      target + " " + unknown, "is unknown mode char to me"));
        }
    }
    // here if user uses MODE +l with garbage param , in a refrence irc server 
    //the param gets consumed but the user doesn't see anything in the output, so i did the same here i just consumed 
    // it silently 

    if (!appliedModes.empty()) {
        std::string modeMsg = ":" + prefixFor(client) + " MODE " + chanName + " " +
                               appliedModes + appliedParams + "\r\n";
        channel.broadcast(modeMsg);
    }
}

void Commands::handleQuit(Client &client, const std::vector<std::string> &args,
                           std::map<std::string, Channel> &channels, const std::string &serverName) {
    (void)serverName; // no reply is sent to the quitting client itself here;
                       // Server closes their fd right after this call anyway

    std::string reason = args.empty() ? "Client Quit" : args[0];
    std::string quitMsg = ":" + prefixFor(client) + " QUIT :" + reason + "\r\n";

    std::map<std::string, Channel>::iterator it = channels.begin();
    for (; it != channels.end(); ++it) {
        if (it->second.isMember(client.getFd())) {
            it->second.broadcast(quitMsg, client.getFd()); // everyone but the client leaving
            it->second.removeMember(client.getFd());
        }
    }
}



