#include "Commands.hpp"
#include "Replies.hpp"
#include <cctype>

static bool isSpecialNickChar(char c) {
    return c == '[' || c == ']' || c == '\\' || c == '`' ||
           c == '_' || c == '^' || c == '{' || c == '|' || c == '}';
}

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
    // channel list, which lives in Server, so that broadcast belongs in the
    // command dispatcher, not here.
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